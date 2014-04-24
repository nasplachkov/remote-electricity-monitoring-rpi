#ifndef PST05DATA_H
#define PST05DATA_H

#include <QJsonObject>
#include <QDateTime>

/**
 * @brief The PST05Data class
 *
 * Only PST05 class will be able to create a new empty object
 */
class PST05Data
{
    PST05Data(bool empty);

public:
    friend class PST05;

    PST05Data(const PST05Data &object);

    bool isEmpty();
    QJsonObject toJSON();

    double U1, U2, U3;
    double I1, I2, I3;
    double P, Q;
    double F;

    QByteArray deviceId;

private:
    bool empty;
};

#endif // PST05DATA_H
