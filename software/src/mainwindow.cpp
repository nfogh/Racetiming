#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QtNetwork/QNetworkRequest>
#include <QListView>
#include <QMessageBox>

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

    m_rfidDevices.emplace_back(std::make_shared<QSerialPort>());
    m_rfidDevices.emplace_back(std::make_shared<QSerialPort>());
    m_rfidReaders.emplace_back(std::make_shared<M6ERFIDReader>(m_rfidDevices[0].get()));
    m_rfidReaders.emplace_back(std::make_shared<M6ERFIDReader>(m_rfidDevices[1].get()));

    connect(m_rfidReaders[0].get(), &M6ERFIDReader::rfidDetected, this, &MainWindow::on_RFID1Reader_rfidDetected);
    connect(m_rfidReaders[1].get(), &M6ERFIDReader::rfidDetected, this, &MainWindow::on_RFID2Reader_rfidDetected);
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
    if (!m_rfidDevices[0]->isOpen()) {
        m_rfidDevices[0]->setPortName(ui->connectRFID1ConnectionComboBox->getPort());
        if (!m_rfidDevices[0]->open(QIODevice::ReadOnly)) {
            QMessageBox::warning(this, "Warning", "Unable to open port " + ui->connectRFID1ConnectionComboBox->currentText() + ". " + m_rfidDevices[0]->errorString());
            return;
        }
        m_rfidDevices[0]->setBaudRate(9600);
        m_rfidDevices[0]->setParity(QSerialPort::NoParity);
        m_rfidDevices[0]->setDataBits(QSerialPort::Data8);
        m_rfidDevices[0]->setStopBits(QSerialPort::OneStop);
        m_rfidDevices[0]->flush();
        m_rfidReaders[0]->connect();
        ui->connectRFID1ConnectPushButton->setText("Close");
    } else {
        m_rfidReaders[0]->disconnect();
        m_rfidDevices[0]->close();
        ui->connectRFID1ConnectPushButton->setText("Open");
    }
}

void MainWindow::on_connectRFID2ConnectPushButton_clicked()
{
    if (!m_rfidDevices[1]->isOpen()) {
        m_rfidDevices[1]->setPortName(ui->connectRFID2ConnectionComboBox->getPort());
        if (!m_rfidDevices[1]->open(QIODevice::ReadOnly)) {
            QMessageBox::warning(this, "Warning", "Unable to open port " + ui->connectRFID2ConnectionComboBox->currentText() + ". " + m_rfidDevices[1]->errorString());
            return;
        }
        m_rfidDevices[1]->setBaudRate(9600);
        m_rfidDevices[1]->setParity(QSerialPort::NoParity);
        m_rfidDevices[1]->setDataBits(QSerialPort::Data8);
        m_rfidDevices[1]->setStopBits(QSerialPort::OneStop);
        m_rfidDevices[1]->flush();
        m_rfidReaders[1]->connect();
        ui->connectRFID2ConnectPushButton->setText("Close");
    } else {
        m_rfidReaders[1]->disconnect();
        m_rfidDevices[1]->close();
        ui->connectRFID2ConnectPushButton->setText("Open");
    }
}

void MainWindow::on_RFID1Reader_rfidDetected(const QString& tid)
{
    rfidDetected(0, tid);
}

void MainWindow::on_RFID2Reader_rfidDetected(const QString& tid)
{
    rfidDetected(1, tid);
}

void MainWindow::rfidDetected(int readerIndex, const QString &tid)
{
    int column = 5;
    const auto tidString = tid;
    const auto tids = m_raceTimingInterface.GetRunnersTable().findItems(tidString, Qt::MatchFixedString, column);
    if (!tids.empty()) {
        const auto tid = tids[0];
        const auto numberID = m_raceTimingInterface.GetRunnersTable().item(tid->row(), 2)->text().toInt();
        const auto name = m_raceTimingInterface.GetRunnersTable().item(tid->row(), 0)->text();
        const auto surname = m_raceTimingInterface.GetRunnersTable().item(tid->row(), 4)->text();

        RaceTiming::EventType event;
        if (readerIndex == 0) {
            event = ui->connectRFID1FunctionComboBox->currentText() == "Start/Lap" ? RaceTiming::EventType::LapStart : RaceTiming::EventType::Finish;
        } else if (readerIndex == 1){
            event = ui->connectRFID2FunctionComboBox->currentText() == "Start/Lap" ? RaceTiming::EventType::LapStart : RaceTiming::EventType::Finish;
        } else {
            return;
        }

        ui->eventsPlainTextEdit->moveCursor(QTextCursor::End);
        ui->eventsPlainTextEdit->insertPlainText(name + " " + surname + " - number id " + QString::number(numberID) + " recorded " + (event == RaceTiming::EventType::LapStart ? "Start/Lap" : "Finish") + " event\n");
        ui->eventsPlainTextEdit->moveCursor(QTextCursor::End);

        m_raceTimingInterface.sendEvent(
                    numberID,
                    QDateTime::currentDateTime(),
                    event
                    );
    }
}
