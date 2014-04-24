#include "pst05store.h"

PST05Store::PST05Store(QSettings *settings, PST05Query *iQuery) :
    piServer(0), ticks(0), connected(false)
{
    this->settings = settings;
    this->iQuery = iQuery;
    masterServerAddress = settings->value("masterServer", MASTER_SERVER_ADDRESS).toString();
    deviceQueryInterval = settings->value("regularInterval", REGULARQUERY_INTERVAL_DEFAULT).toUInt();
    postInterval = settings->value("postInterval", POST_INTERVAL_DEFAULT).toUInt();
    port = settings->value("port", PORT_DEFAULT).toUInt();

    dataFile = new QFile("/tmp/pst05_average_data");
    if (dataFile->open(QFile::ReadOnly))
    {
        QList<QByteArray> data = dataFile->readAll().split('\n');
        foreach (QByteArray datum, data) {
            QJsonDocument json = QJsonDocument::fromJson(datum);
            averageData.append(json.object());
        }

        dataFile->close();
    }

    deviceQueryTimer = new QTimer(this);
    connect(deviceQueryTimer, SIGNAL(timeout()), this, SLOT(deviceQueryTimeout()));

    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(connectResponse(QNetworkReply*)));

    connectTimer = new QTimer(this);
    connect(connectTimer, SIGNAL(timeout()), this, SLOT(connectTimeout()));
    connectTimer->start(CONNECT_INTERVAL);

    qDebug() << "Data store is initialized!";
}

PST05Store::~PST05Store()
{
    if (deviceQueryTimer) deviceQueryTimer->deleteLater();
    if (manager) manager->deleteLater();
    if (piServer) piServer->deleteLater();
    if (dataFile)
    {
        if (dataFile->isOpen()) dataFile->close();
        dataFile->deleteLater();
    }
}

void PST05Store::connectTimeout()
{
    QJsonObject obj;
    obj["id"] = QString(iQuery->deviceId());
    obj["port"] = QString("%1").arg(port);
    obj["date"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    QJsonDocument document;
    document.setObject(obj);

    QByteArray data = document.toJson();

    QNetworkRequest req;
    req.setUrl(QUrl(QString("%1%2").arg(masterServerAddress, "/connect")));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setHeader(QNetworkRequest::ContentLengthHeader, data.length());

    qDebug() << "Trying to connect to the master server...";
    manager->post(req, data);
}

void PST05Store::deviceQueryTimeout()
{
    deviceQueries.append(iQuery->queryDevice());

    if (++ticks == postInterval / deviceQueryInterval)
    {
        // Calculate the average and add it to the POST list
        PST05Data average(deviceQueries[0]);

        for (int i = 1; i < deviceQueries.length(); i++)
        {
            average.U1 += deviceQueries[i].U1;
            average.U2 += deviceQueries[i].U2;
            average.U3 += deviceQueries[i].U3;
            average.I1 += deviceQueries[i].I1;
            average.I2 += deviceQueries[i].I2;
            average.I3 += deviceQueries[i].I3;
            average.P += deviceQueries[i].P;
            average.Q += deviceQueries[i].Q;
            average.F += deviceQueries[i].F;
        }
        average.U1 /= deviceQueries.length();
        average.U2 /= deviceQueries.length();
        average.U3 /= deviceQueries.length();
        average.I1 /= deviceQueries.length();
        average.I2 /= deviceQueries.length();
        average.I3 /= deviceQueries.length();
        average.P /= deviceQueries.length();
        average.Q /= deviceQueries.length();
        average.F /= deviceQueries.length();

        // Put the new average in the average list
        averageData.append(average.toJSON());

        // Clear the used up list
        deviceQueries.clear();

        ticks = 0;

        // Make a POST to save data
        QJsonArray arr;
        for (int i = 0; i < averageData.length(); i++)
        {
            arr.append(averageData[i]);
        }

        QJsonObject obj;
        obj["deviceId"] = QString(iQuery->deviceId());
        obj["queries"] = arr;

        QJsonDocument json;
        json.setObject(obj);
        QByteArray data = json.toJson();

        QNetworkRequest req;
        req.setUrl(QUrl(QString("%1%2").arg(masterServerAddress, "/store")));
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        req.setHeader(QNetworkRequest::ContentLengthHeader, data.length());

        qDebug() << "Trying to save the average data for the past - " << averageData.length() * (postInterval / 60000) << " minutes";
        manager->post(req, data);
    }
}

void PST05Store::disconnect()
{
    if (connected)
    {
        saveAverageDataLocally();

        // Notify master server
        QJsonObject obj;
        obj["deviceId"] = QString(iQuery->deviceId());

        QJsonDocument json;
        json.setObject(obj);
        QByteArray data = json.toJson();

        QNetworkRequest req;
        req.setUrl(QUrl(QString("%1%2").arg(masterServerAddress, "/store")));
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        req.setHeader(QNetworkRequest::ContentLengthHeader, data.length());

        connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(disconnectResponse(QNetworkReply*)));
        manager->post(req, data);
    }
    else
    {
        disconnectResponse(0);
    }
}

void PST05Store::connectResponse(QNetworkReply *reply)
{
    if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == QHttpResponse::STATUS_OK)
    {
        qDebug() << "Connection to the master server was successful. The device has been registered!";
        connectTimer->stop();

        qDebug() << "Starting up the server...";
        piServer = new HttpServer(settings, iQuery);
        connected = true;

        // Reconnect the manager
        connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(dataSaveResponse(QNetworkReply*)));

        deviceQueryTimer->start(deviceQueryInterval);
    }
    else
    {
        qDebug() << "Connection to the master server was UNsuccessful!";
    }
    reply->deleteLater();
}

void PST05Store::dataSaveResponse(QNetworkReply *reply)
{
    if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == QHttpResponse::STATUS_CREATED)
    {
        averageData.clear();

        // Clear the file also
        if (dataFile->open(QFile::WriteOnly | QFile::Truncate))
        {
            dataFile->close();
        }
    }
    else
    {
        saveAverageDataLocally();
    }
    reply->deleteLater();
}

void PST05Store::disconnectResponse(QNetworkReply *reply)
{
    // Stop the application w/e the response
    if (reply) reply->deleteLater();

    qDebug() << "Shutting down...";
    QCoreApplication::quit();
}

bool PST05Store::saveAverageDataLocally()
{
    bool res;
    if ((res = dataFile->open(QFile::WriteOnly | QFile::Truncate)))
    {
        foreach (QJsonObject obj, averageData) {
            QJsonDocument json; json.setObject(obj);
            dataFile->write(json.toJson() + '\n');
        }
        dataFile->close();
    }

    return res;
}
