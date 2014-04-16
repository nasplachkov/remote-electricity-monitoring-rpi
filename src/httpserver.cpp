#include "httpserver.h"

HttpServer::HttpServer(unsigned short port, PST05Query *iQuery)
{
    server = new QHttpServer(this);
    connect(server, SIGNAL(newRequest(QHttpRequest*,QHttpResponse*)), this, SLOT(router(QHttpRequest*,QHttpResponse*)));

    if (server->listen(port))
    {
        printf("Listening on port %d\n", port);
    }
    else
    {
        printf("Could not listen on that port...");
        exit(1);
    }

    this->iQuery = iQuery;
    deviceId = "TODO: GET DEVICE ID";
}

HttpServer::~HttpServer()
{
    if (server) server->deleteLater();
}

void HttpServer::router(QHttpRequest *req, QHttpResponse *res)
{
    if (req->path() == "/pst05")
    {
        manageSensorData(req, res);
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

        QJsonObject data = pst05data.toJSON();
        data["id"] = deviceId;
        data["date"] = QString("TODO: PUT ISO DATE");

        QJsonDocument json(data);

        res->writeHead(QHttpResponse::STATUS_OK);
        res->write(json.toBinaryData());
        res->end();
    }
    else
    {
        res->writeHead(QHttpResponse::STATUS_METHOD_NOT_ALLOWED);
        res->end();
    }
}
