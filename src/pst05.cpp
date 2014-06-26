#include "pst05.h"

PST05::PST05(QSettings *settings)
{
    this->settings = settings;

    qDebug() << "Reading up the device's serial id...";
    QFile procInfo("/proc/cpuinfo");
    if (!procInfo.open(QFile::ReadOnly))
    {
        qDebug() << "Failed to read \"/proc/cpuinfo\"";
        exit(2);
    }

    mDeviceId = DEFAULT_DEVICE_ID;
    foreach (QByteArray line, procInfo.readAll().split('\n')) {
        QList<QByteArray> pair = line.split(':');
        if (pair[0].trimmed() == "Serial")
        {
            mDeviceId = pair[1].trimmed();
        }
    }
    mDeviceId = "raspberry";
    qDebug() << "Device ID is: " << mDeviceId;

    procInfo.close();

    QList<QSerialPortInfo> ports =  QSerialPortInfo::availablePorts();
    foreach (const QSerialPortInfo &info, ports) {
        qDebug() << info.portName();
        qDebug() << info.isBusy();
        serial = new QSerialPort(info);
        break;
    }

    //serial = new QSerialPort(settings->value("portName", "ttyS0").toString());

    if (serial->open(QSerialPort::ReadWrite))
    {
        serial->setBaudRate((QSerialPort::BaudRate) settings->value("baudRate", QSerialPort::Baud9600).toInt());
        serial->setDataBits((QSerialPort::DataBits) settings->value("dataBits", QSerialPort::Data8).toInt());
        serial->setFlowControl((QSerialPort::FlowControl) settings->value("flowControl", QSerialPort::NoFlowControl).toInt());
        serial->setParity((QSerialPort::Parity) settings->value("parity", QSerialPort::NoParity).toInt());
        serial->setStopBits((QSerialPort::StopBits) settings->value("stopBits", QSerialPort::OneStop).toInt());

        qDebug() << serial->baudRate();
        qDebug() << serial->dataBits();
        qDebug() << serial->flowControl();
        qDebug() << serial->parity();
        qDebug() << serial->stopBits();
    }
}

PST05::~PST05()
{
    if (serial)
    {
        if (serial->isOpen()) serial->close();
        serial->deleteLater();
    }
}

void PST05::writeTestDataToSerial(uint cbytes)
{
    QByteArray data;
    qsrand(QTime::currentTime().msec());
    for (uint i = 0; i < cbytes; i++)
    {
        data.append((char) (qrand() % 256));
    }
    data[2] = 49;   // data bytes count...
    serial->write(data);
    serial->flush();
}

// Possibly make it asynchronously some day
PST05Data PST05::queryDevice()
{
    char res;
    char queryAddress[] = {(char) 0xFA};
    char queryCommand[] = {(char) 0x82};

    if (!serial->isOpen()) return PST05Data(true);

    // address
    serial->write(queryAddress, 1);
    serial->flush();
    if (!serial->waitForReadyRead(30000))
    {
        // writeTestDataToSerial(54);
        goto rs232read;
    }
    serial->read(&res, 1);
    if (res != queryAddress[0])
    {
        // writeTestDataToSerial(54);
        goto rs232read;
    }

    // command
    serial->write(queryCommand, 1);
    serial->flush();
    if (!serial->waitForReadyRead(30000))
    {
        // writeTestDataToSerial(54);
        goto rs232read;
    }
    serial->read(&res, 1);
    if (res != queryCommand[0])
    {
        // writeTestDataToSerial(54);
        goto rs232read;
    }

rs232read:
    QByteArray data;
    uint bytesRemaining = 5, i = 0;

    // Wait 2 seconds for data at MOST
    while (serial->waitForReadyRead(60000))
    {
        QByteArray read = serial->read(bytesRemaining);
        data += read;
        bytesRemaining -= read.size();

        if (i++ == 0 && data.size() > 3)
        {
            // The address or the command returned is wrong (first 2 bits are 0)
            if (data[0] != (queryAddress[0] & (char) 63)) break;
            if (data[1] != (queryCommand[0] & (char) 63)) break;

            // The third byte contains the number of bytes that will be available after the first 4
            bytesRemaining += data[2];
        }
        qDebug() << QString::number(bytesRemaining);
        if (!bytesRemaining) break;
    }

    // The data wasn't received OR incorrect checksum (not added)
    if (bytesRemaining > 0 || !checkCRC(data))
    {
        return PST05Data(true);
    }

    // We got it all
    data = data.right(data.size() - 5);

    PST05Data pstdata(false);

    pstdata.U1 = (((float)data[0] * 128 + (float)data[1]) / 10 ) * 220 / 57.7;
    pstdata.U2 = (((float)data[2] * 128 + (float)data[3]) / 10 ) * 220 / 57.7 ;
    pstdata.U3 = (((float)data[4] * 128 + (float)data[5]) / 10 ) * 220 / 57.7;

    pstdata.I1 = ((float)data[6] * 128 + (float)data[7]) / 1000;
    pstdata.I2 = ((float)data[8] * 128 + (float)data[9]) / 1000;
    pstdata.I3 = ((float)data[10] * 128 + (float)data[11]) / 1000;

    pstdata.P = ((float)data[12] * 128 + (float)data[13]) / 10;
    if (pstdata.P > 64*128) pstdata.P -= 128*128;

    pstdata.Q = ((float)data[14] * 128 + (float)data[15]) / 10;
    if (pstdata.Q > 64*128) pstdata.Q -= 128*128;

    pstdata.F = ((float)data[16] * 128 + (float)data[17]) / 100;

    return pstdata;
}

QByteArray PST05::deviceId()
{
    return mDeviceId;
}

bool PST05::checkCRC(const QByteArray &data) {
    unsigned short calcCRC = 0;

    for (int i = 5; i < data.length(); i++)
    {
        calcCRC += data[i];
    }

    QByteArray crc((const char *) calcCRC, 2);
    return data.mid(3, 2) == crc;
}
