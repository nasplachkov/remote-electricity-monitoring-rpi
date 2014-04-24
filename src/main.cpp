#include <QCoreApplication>
#include <QSettings>

#include "pst05.h"
#include "pst05store.h"

#include <signal.h>

QCoreApplication *app = 0;
PST05Store *store = 0;

void signalHandler(int sig)
{
    qDebug() << "Received signal: " << sig;

    if (store)
    {
        store->disconnect();
    }
    else
    {
        if (app) app->quit();
        else exit(0);
    }
}

int main(int argc, char **argv)
{
    QCoreApplication a(argc, argv);
    app = &a;

    qDebug() << "Reading up the settings file...";
    QSettings settings("settings.ini", QSettings::IniFormat);

    qDebug() << "Intializing serial port...";
    PST05 pst05(&settings);

    qDebug() << "Intializing data store...";
    PST05Store pst05store(&a, &settings, &pst05);
    store = &pst05store;

    // Handle some POSIX signals
    struct sigaction sig;
    sig.sa_flags = SA_SIGINFO;
    sig.sa_handler = &signalHandler;

    sigaction(SIGINT, &sig, NULL);
    sigaction(SIGHUP, &sig, NULL);
    sigaction(SIGTERM, &sig, NULL);

    return a.exec();
}
