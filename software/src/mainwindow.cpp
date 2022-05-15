#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QtNetwork/QNetworkRequest>
#include <QListView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->runnersTableView->setModel(&m_raceTimingInterface.GetRacesTable());
    ui->availableRacesComboBox->setModel(&m_raceTimingInterface.GetRacesTable());
    ui->availableRacesComboBox->setView(new QTableView());
    ui->tableView->setModel(&m_raceTimingInterface.GetRunnersTable());
    ui->customEventRunnerIDComboBox->setModel(&m_raceTimingInterface.GetRunnersTable());
    ui->customEventRunnerIDComboBox->setView(new QTableView());
    ui->customEventTIDComboBox->setModel(&m_raceTimingInterface.GetRunnersTable());
    ui->customEventTIDComboBox->setView(new QTableView());

    connect(&m_raceTimingInterface, &RaceTiming::RaceTimingInterface::gotRaces, this, &MainWindow::on_raceTimingInterface_gotRaces);
    connect(&m_raceTimingInterface, &RaceTiming::RaceTimingInterface::gotRunners, this, &MainWindow::on_raceTimingInterface_gotRunners);

    m_rfidDevices.push_back(QSerialPort());
    m_rfidDevices.push_back(QSerialPort());
    m_rfidReaders.push_back(M6ERFIDReader(m_rfidDevices[0]));
    m_rfidReaders.push_back(M6ERFIDReader(m_rfidDevices[1]));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_raceTimingInterface_gotRaces()
{
}

void MainWindow::on_raceTimingInterface_gotRunners()
{
}

void MainWindow::on_raceConnectionsConnectButton_clicked()
{
    m_raceTimingInterface.setApiKey(ui->apiKeyLineEdit->text());
    m_raceTimingInterface.setUrl(ui->websiteLineEdit->text());
    m_raceTimingInterface.getRaces();
}


void MainWindow::on_getRunnersPushButton_clicked()
{
    const auto id = m_raceTimingInterface.GetRacesTable().item(ui->availableRacesComboBox->currentIndex())->text().toInt();
    m_raceTimingInterface.getRunners(id);
    ui->currentRaceLabel->setText("Current race: " + ui->availableRacesComboBox->currentText());
}


void MainWindow::on_customEventInsertPushButton_clicked()
{
    const int numberID = m_raceTimingInterface.GetRunnersTable().item(ui->customEventRunnerIDComboBox->currentIndex(), 5)->text().toInt();
    m_raceTimingInterface.sendEvent(
                numberID,
                ui->customEventNowCheckBox->isChecked() ? QDateTime::currentDateTime() : ui->customEventTimestampDateTimeEdit->dateTime(),
                ui->customEventEventTypeComboBox->currentText() == "Start/lap" ? RaceTiming::EventType::LapStart : RaceTiming::EventType::Finish);
}


void MainWindow::on_customEventTriggerPushButton_clicked()
{
    int column = 5;
    const auto tidString = ui->customEventTIDComboBox->currentText();
    const auto tids = m_raceTimingInterface.GetRunnersTable().findItems(tidString, Qt::MatchFixedString, column);
    if (!tids.empty()) {
        const auto tid = tids[0];
        const auto numberID = m_raceTimingInterface.GetRunnersTable().item(tid->row(), 2)->text().toInt();

        m_raceTimingInterface.sendEvent(
                    numberID,
                    ui->customEventNowCheckBox->isChecked() ? QDateTime::currentDateTime() : ui->customEventTimestampDateTimeEdit->dateTime(),
                    ui->customEventEventTypeComboBox->currentText() == "Start/lap" ? RaceTiming::EventType::LapStart : RaceTiming::EventType::Finish);
    }
}


void MainWindow::on_connectRFID1ConnectPushButton_clicked()
{
    m_rfidDevices[0].setPortName(ui->connectRFID1ConnectionComboBox->getPort());
    m_rfidDevices[0].open(QIODevice::ReadOnly);
    m_rfidDevices[0].setBaudRate(9600);
    m_rfidDevices[0].setParity(QSerialPort::NoParity);
    m_rfidDevices[0].setDataBits(QSerialPort::Data8);
    m_rfidDevices[0].setStopBits(QSerialPort::OneStop);
}

