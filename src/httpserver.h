#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QJsonDocument>

#include <qhttpserver.h>
#include <qhttprequest.h>
#include <qhttpresponse.h>

#include "pst05query.h"
#include "pst05data.h"

class HttpServer : public QObject
{
    Q_OBJECT
public:
    explicit HttpServer(unsigned short port, PST05Query *iQuery = 0);
    ~HttpServer();

private slots:
    void router(QHttpRequest *req, QHttpResponse *res);

private:
    void manageSensorData(QHttpRequest *req, QHttpResponse *res);

    QHttpServer *server;
    PST05Query *iQuery;
    QString deviceId;
};

#endif // HTTPSERVER_H
