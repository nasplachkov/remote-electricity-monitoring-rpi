#ifndef PST05STORE_H
#define PST05STORE_H

#include <QSettings>
#include <QList>
#include <QNetworkAccessManager>
#include <QTimer>

#include "pst05query.h"
#include "pst05data.h"

#define POST_INTERVAL_DEFAULT 15 * 60 * 1000    // 15 minutes
#define REGULAR_INTERVAL_DEFAULT 15 * 1000      // 15 seconds

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
    void timerPostToServer();
    void timerRegularQuery();

private:
    QTimer *post;
    QTimer *regular;

    QNetworkAccessManager *manager;
    PST05Query *iQuery;
    QList<QJsonObject> postQueries;
    QList<PST05Data> regularQueries;
};

#endif // PST05STORE_H
