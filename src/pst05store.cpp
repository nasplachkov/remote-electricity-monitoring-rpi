#include "pst05store.h"

PST05Store::PST05Store(QSettings *settings, PST05Query *iQuery)
{
    this->iQuery = iQuery;

    post = new QTimer(this);
    connect(post, SIGNAL(timeout()), this, SLOT(timerPostToServer()));
    post->start(settings->value("postInterval", POST_INTERVAL_DEFAULT).toUInt());

    regular = new QTimer(this);
    connect(regular, SIGNAL(timeout()), this, SLOT(timerRegularQuery()));
    regular->start(settings->value("regularInterval", REGULAR_INTERVAL_DEFAULT).toUInt());
}

PST05Store::~PST05Store()
{
    if (post) post->deleteLater();
    if (regular) regular->deleteLater();
    if (manager) manager->deleteLater();
}

void PST05Store::timerPostToServer()
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
}

void PST05Store::timerRegularQuery()
{
    // TODO: Query the sensor for data, store it, and at some point calculate the average and add it to the POST list
}
