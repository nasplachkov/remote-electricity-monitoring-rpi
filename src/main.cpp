#include <QCoreApplication>
#include <QSettings>

#include "httpserver.h"

#include <signal.h>

void signalHandler(int sig)
{
    qDebug() << "Received: " << sig;

    // TODO: "/disconnect" the store from the master server and probably shutdown the application

    // TODO: Remove this
    exit(0);
}

int main(int argc, char **argv)
{
    QCoreApplication a(argc, argv);

    qDebug() << "Reading up the settings file...\n";
    QSettings settings("settings.ini", QSettings::IniFormat);

    qDebug() << "Starting up the server...\n";
    HttpServer piHttpServer(&settings);

    // Handle some POSIX signals
    struct sigaction sig;
    sig.sa_flags = SA_SIGINFO;
    sig.sa_handler = &signalHandler;

    sigaction(SIGINT, &sig, NULL);
    sigaction(SIGHUP, &sig, NULL);
    sigaction(SIGTERM, &sig, NULL);

    return a.exec();
}
