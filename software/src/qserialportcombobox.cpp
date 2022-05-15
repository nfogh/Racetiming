#include "qserialportcombobox.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>

QSerialPortComboBox::QSerialPortComboBox(QWidget *parent) :
    QComboBox(parent)
{
    rescan();
}

QString QSerialPortComboBox::getPort()
{
    if (currentText().isEmpty())
        return currentText();

    QString port = currentText().split(" - ").at(0);

    return port;
}

void QSerialPortComboBox::rescan()
{
    clear();
    foreach (QSerialPortInfo info, QSerialPortInfo::availablePorts()) {
#ifdef Q_OS_WIN32
        if (!info.description().isEmpty())
            addItem(info.portName() + " - " + info.description());
        else
            addItem(info.portName());
#else
        if (!info.description().isEmpty())
            addItem(info.systemLocation() + " - " + info.description());
        else
            addItem(info.systemLocation());
#endif
    }
}
