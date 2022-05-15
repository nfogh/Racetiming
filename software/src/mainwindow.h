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

private:
    Ui::MainWindow *ui;

    QVector<M6ERFIDReader> m_rfidReaders;
    QVector<QSerialPort> m_rfidDevices;

    RaceTiming::RaceTimingInterface m_raceTimingInterface;

};
#endif // MAINWINDOW_H
