#ifndef M6ERFIDREADER_H
#define M6ERFIDREADER_H

#include "IRFIDReader.h"
#include <QSerialPort>
#include <QTimer>

namespace RFIDReaders {
class M6ERFIDReader : public IRFIDReader
{
public:
    explicit M6ERFIDReader(const std::string& portName);

    void start() override;
    void stop() override;

    void setTagDetectedCallback(const TagDetectedCallbackFunc& callback) override;
    void setConnectedCallback(const ConnectedCallbackFunc& callback) override;
    void setDisconnectedCallback(const DisconnectedCallbackFunc& callback) override;

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
#endif // M6ERFIDREADER_H
