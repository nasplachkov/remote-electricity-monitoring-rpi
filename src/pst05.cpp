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
        if (pair[0] == "Serial")
        {
            mDeviceId = pair[1];
        }
    }
    qDebug() << "Device ID is: " << mDeviceId;

    procInfo.close();

    serial = new QSerialPort(settings->value("portName", "ttyS0").toString());

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

PST05Data PST05::queryDevice()
{
    char queryCommand[] = {(char) 0xFA, (char) 0x82};
    if (serial->write(queryCommand) == -1)
    {
        qDebug() << "Could not write the query bytes into the device, hence no response will be returned :-(";
        return PST05Data(true);
    }
    else
    {
        QByteArray data;
        uint bytesRemaining = 4, i = 0;

        // Wait 2 seconds for data at MOST
        while (serial->waitForReadyRead(2000) && bytesRemaining > 0)
        {
            data += serial->read(bytesRemaining);

            if (i++ == 0 && data.size() > 3)
            {
                // The third byte contains the number of bytes that will be available after the first 4
                bytesRemaining += data[2];
            }
        }

        // The data wasn't received OR incorrect checksum
        if (bytesRemaining > 0)
        {
            return PST05Data(true);
        }

        // We got it all, so let's parse it!
        data = data.right(data.size() - 4);
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
}

QByteArray PST05::deviceId()
{
    return mDeviceId;
}
