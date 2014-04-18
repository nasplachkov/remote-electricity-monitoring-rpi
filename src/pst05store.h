#ifndef PST05STORE_H
#define PST05STORE_H

#include <QSettings>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonArray>

#include "../qhttpserver/qhttpresponse.h"
#include "pst05query.h"
#include "pst05data.h"

#define MASTER_SERVER_ADDRESS "http://master.server.com"
#define PORT_DEFAULT 8080                               // The same as in httpserver class
#define CONNECT_INTERVAL 60000                          // Try to connect each minute
#define POST_INTERVAL_DEFAULT 15 * 60 * 1000            // 15 minutes
#define REGULARQUERY_INTERVAL_DEFAULT 15 * 1000         // 15 seconds

/**
 * @brief The PST05Store class
 *
 * TODO: Query the RPi's attached pst05 sensor for data on a regular time interval (settings file) and
 * send it to the master server OR if there is no connection - store it locally (preferably persistent and not on ram)
 */
class PST05Store : public QObject
{
    Q_OBJECT
public:
    PST05Store(QSettings *settings, PST05Query *iQuery);
    ~PST05Store();

private slots:
    void connectTimeout();
    void deviceQueryTimeout();

    void connectResponse(QNetworkReply *reply);
    void dataSaveResponse(QNetworkReply *reply);

private:
    QSettings *settings;

    QTimer *connectTimer;
    QTimer *deviceQueryTimer;

    QNetworkAccessManager *manager;
    PST05Query *iQuery;
    QList<QJsonObject> averageData;
    QList<PST05Data> deviceQueries;

    QString masterServerAddress;
    uint ticks;
    uint deviceQueryInterval;
    uint postInterval;
    uint port;
};

#endif // PST05STORE_H
