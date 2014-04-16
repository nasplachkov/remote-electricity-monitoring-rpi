#include "pst05.h"

PST05::PST05()
{
    qDebug() << "Reading up the device's serial id...\n";
    QFile procInfo("/proc/cpuinfo");
    if (!procInfo.open(QFile::ReadOnly))
    {
        qDebug() << "Failed to read \"/proc/cpuinfo\"";
        exit(2);
    }

    deviceId = DEFAULT_DEVICE_ID;
    foreach (QByteArray line, procInfo.readAll().split('\n')) {
        QList<QByteArray> pair = line.split(':');
        if (pair[0] == "Serial")
        {
            deviceId = pair[1];
        }
    }
    qDebug() << "Device ID is: " << deviceId;

    procInfo.close();
}
