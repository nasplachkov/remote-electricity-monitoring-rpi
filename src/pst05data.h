#ifndef PST05DATA_H
#define PST05DATA_H

#include <QJsonObject>

#include "pst05.h"

class PST05Data
{
    // Only PST05 class will be able to create a new empty object
    PST05Data(bool empty);

public:
    friend class PST05;

    PST05Data(const PST05Data &object);

    bool isEmpty();
    QJsonObject toJSON();

    double U1, U2, U3;
    double I;
    double F;
    // Add other

private:
    bool empty;
};

#endif // PST05DATA_H
