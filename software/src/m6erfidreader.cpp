#include "m6erfidreader.h"

M6ERFIDReader::M6ERFIDReader(QIODevice* ioDevice, QObject *parent)
    : QObject{parent},
      m_ioDevice(ioDevice)
{
}

void M6ERFIDReader::on_ioDevice_readyRead()
{
    while (m_ioDevice->canReadLine()) {
        const auto line = QString(m_ioDevice->readLine()).trimmed();
        const auto tokens = line.split(",");
        if (tokens.size() == 2)
            if (tokens[0] == "TAGID")
                emit rfidDetected(tokens[1]);
    }
}

void M6ERFIDReader::connect()
{
    QObject::connect(m_ioDevice, &QIODevice::readyRead, this, &M6ERFIDReader::on_ioDevice_readyRead);
}

void M6ERFIDReader::disconnct()
{
    QObject::disconnect(m_ioDevice, &QIODevice::readyRead, this, &M6ERFIDReader::on_ioDevice_readyRead);
}
