#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "racetiminginterface.h"
#include <RFIDReaders/IRFIDReader.h>

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

private:
    void rfidDetected(int readerIndex, const QString& tid);

    Ui::MainWindow *ui;

    std::array<std::unique_ptr<RFIDReaders::IRFIDReader>, 2> m_rfidReaders;

    RaceTiming::RaceTimingInterface m_raceTimingInterface;
};
#endif // MAINWINDOW_H
