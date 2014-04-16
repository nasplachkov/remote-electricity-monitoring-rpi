#include "pst05store.h"

PST05Store::PST05Store(QSettings *settings, PST05Query *iQuery) :
    ticks(0)
{
    this->iQuery = iQuery;
    regularQueryInterval = settings->value("regularInterval", REGULARQUERY_INTERVAL_DEFAULT).toUInt();
    postInterval = settings->value("postInterval", POST_INTERVAL_DEFAULT).toUInt();

    regularQuery = new QTimer(this);
    connect(regularQuery, SIGNAL(timeout()), this, SLOT(timerRegularQuery()));
    regularQuery->start(regularQueryInterval);
}

PST05Store::~PST05Store()
{
    if (regularQuery) regularQuery->deleteLater();
    if (manager) manager->deleteLater();
}

/*void PST05Store::timerPostToServer()
{
    QJsonObject obj;
    if (iQuery)
    {
        obj = iQuery->queryDevice().toJSON();
    }

    if (!obj.empty()) postQueries.append(obj);

    if (!postQueries.empty())
    {
        // Send all queries to the master server

        // TODO: If successful => clean up the list
    }
}*/

void PST05Store::timerRegularQuery()
{
    // TODO: Query the sensor for data and store it

    if (++ticks == postInterval / regularQueryInterval)
    {
        // Calculate the average and add it to the POST list
        // POST the data in that list
        ticks = 0;
    }
}
