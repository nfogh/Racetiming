#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "racetiminginterface.h"
#include "m6erfidreader.h"
#include <QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_raceTimingInterface_gotRaces();
    void on_raceTimingInterface_gotRunners();

    void on_raceConnectionsConnectButton_clicked();

    void on_getRunnersPushButton_clicked();

    void on_customEventInsertPushButton_clicked();

    void on_customEventTriggerPushButton_clicked();

    void on_connectRFID1ConnectPushButton_clicked();

    void on_connectRFID2ConnectPushButton_clicked();

    void on_RFID1Reader_rfidDetected(const QString& tid);
    void on_RFID2Reader_rfidDetected(const QString& tid);

private:
    void rfidDetected(int readerIndex, const QString& tid);

    Ui::MainWindow *ui;

    QVector<std::shared_ptr<M6ERFIDReader>> m_rfidReaders;
    QVector<std::shared_ptr<QSerialPort>> m_rfidDevices;

    RaceTiming::RaceTimingInterface m_raceTimingInterface;

};
#endif // MAINWINDOW_H
