#include <QCoreApplication>
#include <QSettings>

#include "httpserver.h"

int main(int argc, char **argv)
{
    QCoreApplication a(argc, argv);

    qDebug() << "Reading up the settings file...\n";
    QSettings settings("settings.ini", QSettings::IniFormat);

    qDebug() << "Starting up the server...\n";
    HttpServer piHttpServer(&settings);

    return a.exec();
}
