#include <stdio.h>

#include <QCoreApplication>
#include <QSettings>

#include "httpserver.h"

#define DEFAULT_PORT 8080

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    printf("Reading up the settings...\n");
    QSettings sett("settings.txt", QSettings::IniFormat);

    printf("Starting up the server...\n");
    HttpServer((unsigned short) sett.value("PORT", DEFAULT_PORT).toUInt());

    return a.exec();
}
