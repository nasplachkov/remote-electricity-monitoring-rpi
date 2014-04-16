#ifndef PST05_H
#define PST05_H

#include <QSerialPort>

class PST05 : public QObject
{
    Q_OBJECT
public:
    explicit PST05();

signals:

public slots:

private:
    QSerialPort *serial;

};

#endif // PST05_H
