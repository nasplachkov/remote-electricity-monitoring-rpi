#ifndef PST05_H
#define PST05_H

#include <QSettings>
#include <QFile>
#include <QSerialPort>
#include <QDebug>

#include "pst05query.h"

#define DEFAULT_DEVICE_ID "0x00000000"

class PST05 : public QObject, public PST05Query
{
    Q_OBJECT
public:
    explicit PST05(QSettings *settings);
    ~PST05();

    /**
     * Must avoid the asynchronous nature
     */
    PST05Data queryDevice();
    QByteArray deviceId();

signals:

public slots:

private:
    QSettings *settings;
    QSerialPort *serial;
    QByteArray mDeviceId;
};

#endif // PST05_H
