#pragma once

#include "ITagReader.h"
#include <QSerialPort>
#include <QTimer>

namespace TagReaders {
class M6ENano : public ITagReaderWriter
{
public:
    explicit M6ENano(std::string_view portName);

    void start() override;
    void stop() override;

    void writeTag(std::string_view tag) override;

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
