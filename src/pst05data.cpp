#include "pst05data.h"

PST05Data::PST05Data(bool empty)
{
    this->empty = empty;
    U1 = 0;
    U2 = 0;
    U3 = 0;
    I1 = 0;
    I2 = 0;
    I3 = 0;
    P = 0;
    Q = 0;
    F = 0;
}

PST05Data::PST05Data(const PST05Data &object)
{
    empty = object.empty;
    U1 = object.U1;
    U2 = object.U2;
    U3 = object.U3;
    I1 = object.I1;
    I2 = object.I2;
    I3 = object.I3;
    P = object.P;
    Q = object.Q;
    F = object.F;
}

bool PST05Data::isEmpty()
{
    return empty;
}

QJsonObject PST05Data::toJSON()
{
    QJsonObject obj;
    obj["u1"] = U1;
    obj["u2"] = U2;
    obj["u3"] = U3;
    obj["i1"] = I1;
    obj["i2"] = I2;
    obj["i3"] = I3;
    obj["p"] = P;
    obj["q"] = Q;
    obj["f"] = F;

    obj["date"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    return obj;
}
