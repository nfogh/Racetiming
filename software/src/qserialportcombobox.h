#ifndef QSERIALPORTCOMBOBOX_H
#define QSERIALPORTCOMBOBOX_H

#include <QComboBox>

class QSerialPortComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit QSerialPortComboBox(QWidget *parent = 0);
    void rescan();
    QString getPort();

signals:

public slots:

};

#endif // QSERIALPORTCOMBOBOX_H
