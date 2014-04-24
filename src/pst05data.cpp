#include "pst05data.h"

PST05Data::PST05Data(bool empty)
{
    this->empty = empty;
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
    obj["U1"] = U1;
    obj["U2"] = U2;
    obj["U3"] = U3;
    obj["I1"] = I1;
    obj["I2"] = I2;
    obj["I3"] = I3;
    obj["P"] = P;
    obj["Q"] = Q;
    obj["F"] = F;

    obj["id"] = QString(deviceId);
    obj["date"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    return obj;
}
