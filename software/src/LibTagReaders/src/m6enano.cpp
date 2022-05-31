#include "m6enano.h"
#include <thread>
#include <QDebug>
#include <QCoreApplication>

namespace TagReaders {
M6ENano::M6ENano(const std::string_view portName)
    : m_serialPort(QString::fromStdString(std::string(portName)))
{
    if (!m_serialPort.open(QIODevice::ReadWrite))
        throw std::runtime_error("Unable to open serial port " + std::string(portName) + ": " + m_serialPort.errorString().toStdString());

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
    m_serialPort.write("start");
}

void M6ENano::start()
{
    m_serialPort.write("start");
    qDebug() << "Sent: start";
}

void M6ENano::stop()
{
    m_serialPort.write("stop");
    qDebug() << "Sent: stop";
}

void M6ENano::writeTag(std::string_view tag)
{
    stop();
    QCoreApplication::processEvents();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    QCoreApplication::processEvents();
    m_serialPort.write(QString::fromStdString("setid," + std::string(tag)).toLatin1());
    QCoreApplication::processEvents();
    qDebug() << QString("Sent: setid,") + QString::fromStdString(std::string(tag));
    QCoreApplication::processEvents();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    QCoreApplication::processEvents();
    start();
    QCoreApplication::processEvents();
}

void M6ENano::setTagDetectedCallback(const TagDetectedCallbackFunc &callback)
{
    m_tagDetectedCallback = callback;
}

void M6ENano::setConnectedCallback(const ConnectedCallbackFunc &callback)
{
    m_connectedCallback = callback;
}

void M6ENano::setDisconnectedCallback(const DisconnectedCallbackFunc &callback)
{
    m_disconnectedCallback = callback;
}

void M6ENano::on_serialPort_readyRead()
{
    while (m_serialPort.canReadLine()) {
        const auto line = QString(m_serialPort.readLine()).trimmed();
        qDebug() << "Received: " << line;
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
