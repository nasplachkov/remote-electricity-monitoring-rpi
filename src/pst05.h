#ifndef PST05_H
#define PST05_H

#include <QFile>
#include <QSerialPort>
#include <QDebug>

#define DEFAULT_DEVICE_ID "0x00000000"

class PST05 : public QObject
{
    Q_OBJECT
public:
    explicit PST05();

signals:

public slots:

private:
    QSerialPort *serial;
    QByteArray deviceId;
};

#endif // PST05_H
