#include "httpserver.h"

HttpServer::HttpServer(QSettings *settings, PST05Query *iQuery)
{
    server = new QHttpServer(this);
    connect(server, SIGNAL(newRequest(QHttpRequest*,QHttpResponse*)), this, SLOT(router(QHttpRequest*,QHttpResponse*)));

    if (server->listen(settings->value("port", PORT_DEFAULT).toUInt()))
    {
        qDebug() << "Listening on port " << settings->value("port", PORT_DEFAULT).toUInt();
    }
    else
    {
        qDebug() << "Could not listen on that port...";
        exit(1);
    }

    this->iQuery = iQuery;
}

HttpServer::~HttpServer()
{
    if (server) server->deleteLater();
}

void HttpServer::router(QHttpRequest *req, QHttpResponse *res)
{
    qDebug() << req->method() << " - " << req->path();

    if (req->path() == "/pst05")
    {
        manageSensorData(req, res);
    }
    else if (req->path() == "/ping")
    {
        res->writeHead(QHttpResponse::STATUS_OK);
        res->end();
    }
    else
    {
        res->writeHead(QHttpResponse::STATUS_NOT_FOUND);
        res->end();
    }
}

void HttpServer::manageSensorData(QHttpRequest *req, QHttpResponse *res)
{
    if (req->methodString() == "HTTP_GET")
    {
        if (!iQuery)
        {
noData:
            res->writeHead(QHttpResponse::STATUS_NO_CONTENT);
            res->end();
            return;
        }

        PST05Data pst05data = iQuery->queryDevice();
        if (pst05data.isEmpty()) goto noData;

        QJsonDocument json(pst05data.toJSON());
        QByteArray data = json.toJson();

        res->setHeader("Content-Type", "application/json");
        res->setHeader("Content-Length", QString("%1").arg(data.length()));
        res->writeHead(QHttpResponse::STATUS_OK);
        res->write(data);
        res->end();
    }
    else
    {
        res->writeHead(QHttpResponse::STATUS_METHOD_NOT_ALLOWED);
        res->end();
    }
}
