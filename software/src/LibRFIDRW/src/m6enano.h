#pragma once

#include "IRFIDRW.h"
#include <QSerialPort>
#include <QTimer>

namespace RFIDRW {
class M6ENano : public IRFIDRW
{
public:
    explicit M6ENano(const std::string& portName);

    void start() override;
    void stop() override;

    void writeEPC(const std::string &epc) override;

    void setTagDetectedCallback(const TagDetectedCallbackFunc& callback) override;
    void setConnectedCallback(const ConnectedCallbackFunc& callback) override;
    void setDisconnectedCallback(const DisconnectedCallbackFunc& callback) override;
    ~M6ENano() override = default;

private:
    TagDetectedCallbackFunc m_tagDetectedCallback;
    ConnectedCallbackFunc m_connectedCallback;
    DisconnectedCallbackFunc m_disconnectedCallback;

    void on_serialPort_readyRead();

    bool m_connected = false;

    QSerialPort m_serialPort;
    QTimer m_watchdogTimer;
};
}
