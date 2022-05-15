#ifndef M6ERFIDREADER_H
#define M6ERFIDREADER_H

#include <QObject>
#include <QIODevice>

class M6ERFIDReader : public QObject
{
    Q_OBJECT
public:
    explicit M6ERFIDReader(QIODevice* ioDevice, QObject *parent = nullptr);

    M6ERFIDReader(M6ERFIDReader&& other)
    {
        std::swap(m_ioDevice, other.m_ioDevice);
    }

    M6ERFIDReader& operator=(M6ERFIDReader&& other)
    {
        if (this != &other)
            std::swap(m_ioDevice, other.m_ioDevice);

        return *this;
    }

    void connect();
    void disconnct();

private slots:
    void on_ioDevice_readyRead();

signals:
    void rfidDetected(const QString& tid);

private:
    QIODevice* m_ioDevice = nullptr;
};

#endif // M6ERFIDREADER_H
