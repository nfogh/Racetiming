#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QtNetwork/QNetworkRequest>
#include <QListView>
#include <QMessageBox>
#include <TagReaders/TagReaders.h>
#include <RacetimingInterface/RacetimingInterface.h>
#include <time.h>
#include <QSettings>
#include "PushButtonDelegate.h"

struct tm Now()
{
    const auto t = std::time(0);
    struct tm time;
#ifdef WIN32
    ::localtime_s(&time, &t);
#else
    ::localtime_s(&t, &time);
#endif
    return time;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_activeRunnersForm.show();

    m_raceProgressSortFilterProxyModel.setSourceModel(&m_runnersTableModel);
    m_raceProgressSortFilterProxyModel.setAcceptedColumns({RunnersTableModel::number_col, RunnersTableModel::name_col, RunnersTableModel::surname_col, RunnersTableModel::lapButton_col, RunnersTableModel::finishButton_col});
    ui->runnersTableView->setModel(&m_raceProgressSortFilterProxyModel);
    ui->runnersTableView->setAlternatingRowColors(true);
    ui->runnersTableView->setSortingEnabled(true);
    ui->runnersTableView->sortByColumn(0, Qt::SortOrder::AscendingOrder);

    m_racesSortFilterProxyModel.setSourceModel(&m_racesTableModel);
    m_racesSortFilterProxyModel.setAcceptedColumns({RacesTableModel::id_col, RacesTableModel::name_col});
    ui->availableRacesComboBox->setModel(&m_racesSortFilterProxyModel);
    ui->availableRacesComboBox->setView(new QTableView());

    m_numberSummarySortFilterProxyModel.setSourceModel(&m_runnersTableModel);
    m_numberSummarySortFilterProxyModel.setAcceptedColumns({RunnersTableModel::number_col, RunnersTableModel::name_col, RunnersTableModel::surname_col});

    ui->tableView->setModel(&m_numberSummarySortFilterProxyModel);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->sortByColumn(0, Qt::SortOrder::AscendingOrder);

    ui->customEventRunnerIDComboBox->setModel(&m_runnersTableModel);
    ui->customEventRunnerIDComboBox->setView(new QTableView());
    ui->customEventTIDComboBox->setModel(&m_runnersTableModel);
    ui->customEventTIDComboBox->setView(new QTableView());

    m_tagWriterDataWidgetMapper.setModel(&m_runnersTableModel);
    m_tagWriterDataWidgetMapper.addMapping(ui->writeTagNameLabel, RunnersTableModel::name_col, "text");
    m_tagWriterDataWidgetMapper.addMapping(ui->writeTagEPCLabel, RunnersTableModel::tag_col, "text");
    m_tagWriterDataWidgetMapper.addMapping(ui->writeTagNumberLabel, RunnersTableModel::number_col, "text");

    m_attachTagSortFilterProxyModel.setSourceModel(&m_runnersTableModel);
    m_attachTagSortFilterProxyModel.setAcceptedColumns({RunnersTableModel::number_col, RunnersTableModel::name_col, RunnersTableModel::surname_col, RunnersTableModel::tag_col, RunnersTableModel::attachTag_col});
    ui->writeTagsTableView->setModel(&m_attachTagSortFilterProxyModel);
    ui->writeTagsTableView->setAlternatingRowColors(true);
    ui->writeTagsTableView->setSortingEnabled(true);
    ui->writeTagsTableView->sortByColumn(0, Qt::SortOrder::AscendingOrder);

    QSettings settings;
    ui->apiKeyLineEdit->setText(settings.value("state/apikey", "Please enter the API key to your web service").toString());
    ui->endpointLineEdit->setText(settings.value("state/endpoint", "http://openracetiming.net").toString());
}

MainWindow::~MainWindow()
{
    QSettings settings;
    settings.setValue("state/apikey", ui->apiKeyLineEdit->text());
    settings.setValue("state/endpoint", ui->endpointLineEdit->text());
    delete ui;
}

void MainWindow::on_racetimingInterface_racesUpdated()
{
    m_racesTableModel.setData(m_races);
}

void MainWindow::updateWriteTagsLabels()
{
    ui->writeTagProgressBar->setMaximum(m_runners.size() - 1);
    ui->writeTagProgressBar->setValue(m_writeTagsIndex);
}

void MainWindow::runnerLapStart(const int runnerID)
{
    m_racetimingInterface->sendEvent(m_runners[runnerID].numberid, Now(), RacetimingInterface::EventType::LapStart);
    m_activeRunnersForm.runnerStart(m_runners[runnerID].name + " " + m_runners[runnerID].surname);
}

void MainWindow::runnerFinished(int runnerID)
{
    m_racetimingInterface->sendEvent(m_runners[runnerID].numberid, Now(), RacetimingInterface::EventType::Finish);
    m_activeRunnersForm.runnerFinish(m_runners[runnerID].name + " " + m_runners[runnerID].surname);
}

void MainWindow::on_racetimingInterface_runnersUpdated()
{
    m_runnersTableModel.setData(m_runners);
    m_tagWriterDataWidgetMapper.toFirst();
    m_writeTagsIndex = 0;
    updateWriteTagsLabels();
    for (int row = 0; row < m_runnersTableModel.rowCount(); row++) {
        const auto lapButton = new QPushButton("Lap");
        connect(lapButton, &QPushButton::clicked, this, [this, row](){
            qDebug() << "Lap pushed for " << row;
            runnerLapStart(row);
        });
        ui->runnersTableView->setIndexWidget(m_raceProgressSortFilterProxyModel.index(row, 3), lapButton);

        const auto finishButton = new QPushButton("Finish");
        connect(finishButton, &QPushButton::clicked, this, [this, row](){
            qDebug() << "Finished pushed for " << row;
            runnerFinished(row);
        });
        ui->runnersTableView->setIndexWidget(m_raceProgressSortFilterProxyModel.index(row, 4), finishButton);

        const auto attachTagButton = new QPushButton("Attach");
        connect(attachTagButton, &QPushButton::clicked, this, [this, row](){
            qDebug() << "Attaching tag for " << m_runners[row].numberid;
            m_racetimingInterface->attachTag(m_runners[row].numberid, ui->writeTagCurrentTagLabel->text().toStdString());
            m_racetimingInterface->requestRunners(m_races[ui->availableRacesComboBox->currentIndex()].id);
        });
        ui->writeTagsTableView->setIndexWidget(m_attachTagSortFilterProxyModel.index(row, 4), attachTagButton);
    }
}

void MainWindow::on_raceConnectionsConnectButton_clicked()
{
    /*m_racetimingInterface = RacetimingInterface::CreateWebRacetimingInterface(
                ui->endpointLineEdit->text().toStdString(),
                ui->apiKeyLineEdit->text().toStdString()
                ); */
    m_racetimingInterface = RacetimingInterface::CreateTestRacetimingInterface();
    m_racetimingInterface->setRacesTableUpdatedCallback([this](const auto& races) { m_races = races; on_racetimingInterface_racesUpdated(); });
    m_racetimingInterface->setRunnersTableUpdatedCallback([this](const auto& runners) { m_runners = runners; on_racetimingInterface_runnersUpdated(); });

    m_racetimingInterface->requestRaces();
}

void MainWindow::on_getRunnersPushButton_clicked()
{
    const auto raceID = m_races[ui->availableRacesComboBox->currentIndex()].id;
    m_racetimingInterface->requestRunners(raceID);
    ui->currentRaceLabel->setText("Current race: " + ui->availableRacesComboBox->currentText());
}


void MainWindow::on_customEventInsertPushButton_clicked()
{
    /*const int numberID = m_raceTimingInterface.GetRunnersTable().item(ui->customEventRunnerIDComboBox->currentIndex(), 5)->text().toInt();
    m_racetimingInterface->sendEvent(
                numberID,
                ui->customEventNowCheckBox->isChecked() ? QDateTime::currentDateTime() : ui->customEventTimestampDateTimeEdit->dateTime(),
                ui->customEventEventTypeComboBox->currentText() == "Start/lap" ? RaceTiming::EventType::LapStart : RaceTiming::EventType::Finish);
                */
}

void MainWindow::on_customEventTriggerPushButton_clicked()
{
    /*
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
    */
}

void MainWindow::on_connectRFID1ConnectPushButton_clicked()
{
    if (!m_tagReaders[0]) {
        m_tagReaders[0] = TagReaders::CreateM6EReader(ui->connectRFID1ConnectionComboBox->getPort().toStdString());
        m_tagReaders[0]->setTagDetectedCallback([this](const auto& epc) { tagDetected(0, epc); });
        m_tagReaders[0]->setConnectedCallback([this] {
            ui->connectRFID1StatusLabel->setText("Connected");
            ui->connectRFID1StatusLabel->setStyleSheet("color:green");});
        m_tagReaders[0]->setDisconnectedCallback([this]{
            ui->connectRFID1StatusLabel->setText("Disconnected");
            ui->connectRFID1StatusLabel->setStyleSheet("color : red");
        });
        ui->connectRFID1ConnectPushButton->setText("Close");
    } else {
        m_tagReaders[0].reset();
        ui->connectRFID1ConnectPushButton->setText("Open");
    }
}

void MainWindow::on_connectRFID2ConnectPushButton_clicked()
{
    if (!m_tagReaders[1]) {
        m_tagReaders[1] = TagReaders::CreateM6EReader(ui->connectRFID2ConnectionComboBox->getPort().toStdString());
        m_tagReaders[1]->setTagDetectedCallback([this](const auto& epc) { tagDetected(0, epc); });
        m_tagReaders[1]->setConnectedCallback([this] {
            ui->connectRFID2StatusLabel->setText("Connected");
            ui->connectRFID2StatusLabel->setStyleSheet("color:green");});
        m_tagReaders[1]->setDisconnectedCallback([this]{
            ui->connectRFID2StatusLabel->setText("Disconnected");
            ui->connectRFID2StatusLabel->setStyleSheet("color : red");
        });
        ui->connectRFID2ConnectPushButton->setText("Close");
    } else {
        m_tagReaders[1].reset();
        ui->connectRFID2ConnectPushButton->setText("Open");
    }
}

void MainWindow::tagDetected(int readerIndex, const std::string_view tag)
{
    ui->writeTagCurrentTagLabel->setText(QString::fromStdString(static_cast<const std::string>(tag)));

    if (!m_raceStarted)
        return;

    const auto runner = std::find_if(m_runners.cbegin(), m_runners.cend(), [&tag](const auto& runner) {
        return std::find(runner.tags.cbegin(), runner.tags.cend(), tag) != runner.tags.cend();
    });
    if (runner != m_runners.cend()) {

        RacetimingInterface::EventType event;
        if (readerIndex == 0) {
            event = ui->connectRFID1FunctionComboBox->currentText() == "Start/Lap" ? RacetimingInterface::EventType::LapStart : RacetimingInterface::EventType::Finish;
        } else if (readerIndex == 1){
            event = ui->connectRFID2FunctionComboBox->currentText() == "Start/Lap" ? RacetimingInterface::EventType::LapStart : RacetimingInterface::EventType::Finish;
        } else {
            return;
        }

        ui->eventsPlainTextEdit->moveCursor(QTextCursor::End);
        ui->eventsPlainTextEdit->insertPlainText(QString::fromStdString(runner->name) + " " + QString::fromStdString(runner->surname) + " - number id " + QString::number(runner->numberid) + " recorded " + (event == RacetimingInterface::EventType::LapStart ? "Start/Lap" : "Finish") + " event\n");
        ui->eventsPlainTextEdit->moveCursor(QTextCursor::End);

        m_racetimingInterface->sendEvent(
                    runner->numberid,
                    Now(),
                    event
                    );
    }
}

void MainWindow::on_writeTagPreviousPushButton_clicked()
{
    m_tagWriterDataWidgetMapper.toPrevious();
    updateWriteTagsLabels();
}


void MainWindow::on_writeTagNextPushButton_clicked()
{
    m_tagWriterDataWidgetMapper.toNext();
    updateWriteTagsLabels();
}


void MainWindow::on_writeTagWritePushButton_clicked()
{
//    m_tagReaders[0]->writeEPC(ui->writeTagEPCLabel->text().toStdString());
    on_writeTagNextPushButton_clicked();
}


void MainWindow::on_startRacePushButton_toggled(const bool checked)
{
    m_raceStarted = checked;
    ui->startRacePushButton->setText(checked ? "Pause race" : "Start race");
}

