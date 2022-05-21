#include "m6erfidreader.h"
namespace RFIDReaders {
M6ERFIDReader::M6ERFIDReader(const std::string& portName)
    : m_serialPort(QString::fromStdString(portName))
{
    if (!m_serialPort.open(QIODevice::ReadWrite))
        throw std::runtime_error("Unable to open serial port " + portName + ": " + m_serialPort.errorString().toStdString());

    m_serialPort.setBaudRate(9600);
    m_serialPort.setParity(QSerialPort::NoParity);
    m_serialPort.setDataBits(QSerialPort::Data8);
    m_serialPort.setStopBits(QSerialPort::OneStop);
    m_serialPort.flush();
    QObject::connect(&m_serialPort, &QSerialPort::readyRead, [this]{on_serialPort_readyRead(); });

    m_watchdogTimer.setInterval(2000);
    QObject::connect(&m_watchdogTimer, &QTimer::timeout, [this]{
        if (m_connected && m_disconnectedCallback)
            m_disconnectedCallback();
        m_connected = false;
    });
    m_watchdogTimer.start();
}

void M6ERFIDReader::start()
{
    m_serialPort.write("start");
}

void M6ERFIDReader::stop()
{
    m_serialPort.write("stop");
}

void M6ERFIDReader::setTagDetectedCallback(const TagDetectedCallbackFunc &callback)
{
    m_tagDetectedCallback = callback;
}

void M6ERFIDReader::setConnectedCallback(const ConnectedCallbackFunc &callback)
{
    m_connectedCallback = callback;
}

void M6ERFIDReader::setDisconnectedCallback(const DisconnectedCallbackFunc &callback)
{
    m_disconnectedCallback = callback;
}

void M6ERFIDReader::on_serialPort_readyRead()
{
    while (m_serialPort.canReadLine()) {
        const auto line = QString(m_serialPort.readLine()).trimmed();
        const auto tokens = line.split(",");
        if (tokens.size() == 2) {
            if (tokens[0] == "TAGID") {
                if (m_tagDetectedCallback)
                    m_tagDetectedCallback(tokens[1].toStdString());
            }
        } else if (tokens.size() == 1) {
            if (tokens[0] == "KEEPALIVE") {
                if (!m_connected && m_connectedCallback)
                    m_connectedCallback();

                m_connected = true;
                // Restart the watchdog timer
                m_watchdogTimer.start();
            }
        }
    }
}
}
