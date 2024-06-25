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
#include <QFileDialog>
#include "PushButtonDelegate.h"

struct tm Now()
{
    const auto t = std::time(0);
    struct tm time;
#ifdef WIN32
    ::localtime_s(&time, &t);
#else
    ::localtime_r(&t, &time);
#endif
    return time;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
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

    m_tagWriterDataWidgetMapper.setModel(&m_runnersTableModel);
    m_tagWriterDataWidgetMapper.addMapping(ui->writeTagNameLabel, RunnersTableModel::name_col, "text");
    ui->writeTagTagComboBox->setModel(&m_runnersTableModel);
    ui->writeTagTagComboBox->setModelColumn(RunnersTableModel::tag_col);

    // m_tagWriterDataWidgetMapper.addMapping(ui->writeTagTagComboBox, RunnersTableModel::tag_col, "items");
    m_tagWriterDataWidgetMapper.addMapping(ui->writeTagNumberLabel, RunnersTableModel::number_col, "text");

    m_attachTagSortFilterProxyModel.setSourceModel(&m_runnersTableModel);
    m_attachTagSortFilterProxyModel.setAcceptedColumns({RunnersTableModel::number_col, RunnersTableModel::name_col, RunnersTableModel::surname_col, RunnersTableModel::tag_col, RunnersTableModel::attachTag_col});
    ui->writeTagsTableView->setModel(&m_attachTagSortFilterProxyModel);
    ui->writeTagsTableView->setAlternatingRowColors(true);
    ui->writeTagsTableView->setSortingEnabled(false);
    // ui->writeTagsTableView->sortByColumn(0, Qt::SortOrder::AscendingOrder);

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

void MainWindow::racetimingInterface_racesUpdated()
{
    m_racesTableModel.setData(m_races);
}

void MainWindow::updateWriteTagsLabels()
{
    ui->writeTagProgressBar->setMaximum(m_runners.size() - 1);
    ui->writeTagProgressBar->setValue(m_writeTagsIndex);
    ui->writeTagTagComboBox->clear();
}

void MainWindow::appendToEvents(const QString &str)
{
    auto cursor = ui->eventsPlainTextEdit->textCursor();
    ui->eventsPlainTextEdit->moveCursor(QTextCursor::End);
    ui->eventsPlainTextEdit->insertPlainText(str);
    ui->eventsPlainTextEdit->setTextCursor(cursor);
    ui->statusbar->showMessage(str, 10000);
}

void MainWindow::runnerLapStart(const int runnerIndex)
{
    const auto runnerName = QString::fromStdString(m_runners[runnerIndex].name) + " " + QString::fromStdString(m_runners[runnerIndex].surname);
    if (m_runnerFinished.count(runnerIndex) > 0)
    {
        appendToEvents(runnerName + " registered a start/lap, but is already finished.. Ignoring.\n");
        return; // Do not lap or start if runner has finished
    }

    if (m_latestEvent.count(runnerIndex) > 0)
    {
        const auto secsElapsed = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - m_latestEvent.at(runnerIndex));
        if (secsElapsed < std::chrono::seconds(ui->minimumTimeBetweenEventsSpinBox->value()))
        {
            appendToEvents(runnerName + " had an event too quickly (" + QString::number(secsElapsed.count()) + "s) after the other.. Ignoring.\n");
            return; // Event happened too quick after the latest
        }
    }

    m_latestEvent[runnerIndex] = std::chrono::steady_clock::now();

    m_racetimingInterface->sendEvent(m_runners[runnerIndex].numberid, Now(), RacetimingInterface::EventType::LapStart);
    m_activeRunnersForm.runnerStart(m_runners[runnerIndex].name + " " + m_runners[runnerIndex].surname);
    appendToEvents(runnerName + " - number id " + QString::number(m_runners[runnerIndex].numberid) + " recorded a start/lap event\n");
}

void MainWindow::runnerFinished(const int runnerIndex, const bool manual)
{
    const auto runnerName = QString::fromStdString(m_runners[runnerIndex].name) + " " + QString::fromStdString(m_runners[runnerIndex].surname);
    if (m_latestEvent.count(runnerIndex) > 0)
    {
        const auto secsElapsed = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - m_latestEvent.at(runnerIndex));
        if (secsElapsed < std::chrono::seconds(ui->minimumTimeBetweenEventsSpinBox->value()))
        {
            appendToEvents(runnerName + " had an event too quickly (" + QString::number(secsElapsed.count()) + "s) after the other.. Ignoring.\n");
            return; // Event happened too quick after the latest
        }
    }
    else
    {
        appendToEvents(runnerName + " registered a finish event, but has not yet started.. Ignored.\n");
        return; // Event happened too quick after the latest
    }
    m_runnerFinished[runnerIndex] = true;
    m_latestEvent[runnerIndex] = std::chrono::steady_clock::now();

    if (!ui->onlyManualFinishCheckBox->isChecked() || manual)
    {
        m_racetimingInterface->sendEvent(m_runners[runnerIndex].numberid, Now(), RacetimingInterface::EventType::Finish);
    }

    m_activeRunnersForm.runnerFinish(m_runners[runnerIndex].name + " " + m_runners[runnerIndex].surname);

    appendToEvents(runnerName + " - number id " + QString::number(m_runners[runnerIndex].numberid) + " recorded a finish event\n");
}

void MainWindow::racetimingInterface_runnersUpdated()
{
    m_runnersTableModel.setData(m_runners);
    m_tagWriterDataWidgetMapper.toFirst();
    m_writeTagsIndex = 0;
    updateWriteTagsLabels();
    for (int row = 0; row < m_runnersTableModel.rowCount(); row++)
    {
        const auto lapButton = new QPushButton("Lap");
        connect(lapButton, &QPushButton::clicked, this, [this, row]()
                {
            qDebug() << "Lap pushed for " << row;
            runnerLapStart(row); });
        ui->runnersTableView->setIndexWidget(m_raceProgressSortFilterProxyModel.index(row, 3), lapButton);

        const auto finishButton = new QPushButton("Finish");
        connect(finishButton, &QPushButton::clicked, this, [this, row]()
                {
            qDebug() << "Finished pushed for " << row;
            runnerFinished(row, true); });
        ui->runnersTableView->setIndexWidget(m_raceProgressSortFilterProxyModel.index(row, 4), finishButton);

        const auto sourceRow = m_attachTagSortFilterProxyModel.mapToSource(m_attachTagSortFilterProxyModel.index(row, 0)).row();

        const auto attachTagButton = new QPushButton("Attach");
        connect(attachTagButton, &QPushButton::clicked, this, [this, row]()
                {
            qDebug() << "Attaching tag for row " << row << " number " << m_runners[row].number << " numberid " << m_runners[row].numberid;
            m_racetimingInterface->attachTag(m_runners[row].numberid, ui->writeTagCurrentTagLabel->text().toStdString());
            m_racetimingInterface->requestRunners(m_races[ui->availableRacesComboBox->currentIndex()].id); });
        ui->writeTagsTableView->setIndexWidget(m_attachTagSortFilterProxyModel.index(row, 4), attachTagButton);
    }
}

void MainWindow::on_raceConnectionsConnectButton_clicked()
{
    m_racetimingInterface = RacetimingInterface::CreateWebRacetimingInterface(
        ui->endpointLineEdit->text().toStdString(),
        ui->apiKeyLineEdit->text().toStdString(),
        [this](const auto &races)
        { m_races = races; racetimingInterface_racesUpdated(); },
        [this](const auto &runners)
        { m_runners = runners; racetimingInterface_runnersUpdated(); });

    m_racetimingInterface->requestRaces();
}

void MainWindow::on_getRunnersPushButton_clicked()
{
    const auto raceID = m_races[ui->availableRacesComboBox->currentIndex()].id;
    m_racetimingInterface->requestRunners(raceID);
    ui->currentRaceLabel->setText("Current race: " + ui->availableRacesComboBox->currentText());
}

void MainWindow::on_connectRFID1ConnectPushButton_clicked()
{
    if (!m_tagReaders[0])
    {
        m_tagReaders[0] = TagReaders::CreateM6EReader(ui->connectRFID1ConnectionComboBox->getPort().toStdString());
        m_tagReaders[0]->setTagDetectedCallback([this](const auto &tag)
                                                { tagDetected(0, tag); });
        m_tagReaders[0]->setConnectedCallback([this]
                                              {
            ui->connectRFID1StatusLabel->setText("Connected");
            ui->connectRFID1StatusLabel->setStyleSheet("color:green"); });
        m_tagReaders[0]->setDisconnectedCallback([this]
                                                 {
            ui->connectRFID1StatusLabel->setText("Disconnected");
            ui->connectRFID1StatusLabel->setStyleSheet("color : red"); });
        ui->connectRFID1ConnectPushButton->setText("Close");
    }
    else
    {
        m_tagReaders[0].reset();
        ui->connectRFID1ConnectPushButton->setText("Open");
    }
}

void MainWindow::on_connectRFID2ConnectPushButton_clicked()
{
    if (!m_tagReaders[1])
    {
        m_tagReaders[1] = TagReaders::CreateM6EReader(ui->connectRFID2ConnectionComboBox->getPort().toStdString());
        m_tagReaders[1]->setTagDetectedCallback([this](const auto &tag)
                                                { tagDetected(1, tag); });
        m_tagReaders[1]->setConnectedCallback([this]
                                              {
            ui->connectRFID2StatusLabel->setText("Connected");
            ui->connectRFID2StatusLabel->setStyleSheet("color:green"); });
        m_tagReaders[1]->setDisconnectedCallback([this]
                                                 {
            ui->connectRFID2StatusLabel->setText("Disconnected");
            ui->connectRFID2StatusLabel->setStyleSheet("color : red"); });
        ui->connectRFID2ConnectPushButton->setText("Close");
    }
    else
    {
        m_tagReaders[1].reset();
        ui->connectRFID2ConnectPushButton->setText("Open");
    }
}

void MainWindow::tagDetected(int readerIndex, const std::string_view tag)
{
    qDebug() << "Reader " << readerIndex << " detected tag " << tag;
    ui->writeTagCurrentTagLabel->setText(QString::fromStdString(static_cast<const std::string>(tag)));

    qDebug() << "Reader " << readerIndex << " tag " << QString::fromStdString(static_cast<const std::string>(tag));

    if (ui->logEventsToFileGroupBox->isChecked())
    {
        QFile f(ui->logEventsToFilePathLineEdit->text());
        if (f.open(QIODevice::WriteOnly | QIODevice::Append))
        {
            QTextStream s(&f);
            s << "Reader " << readerIndex << " tag " << QString::fromStdString(std::string(tag));
        }
    }

    const auto runner = std::find_if(m_runners.cbegin(), m_runners.cend(), [&tag](const auto &runner)
                                     { return std::find(runner.tags.cbegin(), runner.tags.cend(), tag) != runner.tags.cend(); });

    if (runner != m_runners.cend())
    {
        const auto runnerIndex = std::distance(m_runners.cbegin(), runner);
        m_runnersTableModel.setSelectedRowIndex(runnerIndex);

        qDebug() << "  Matching runner " << runnerIndex << ": " << runner->name << " " << runner->surname;

        // Do not put event if race has not started
        if (!m_raceStarted)
            return;

        RacetimingInterface::EventType event;
        if (readerIndex == 0)
        {
            event = ui->connectRFID1FunctionComboBox->currentText() == "Start/Lap" ? RacetimingInterface::EventType::LapStart : RacetimingInterface::EventType::Finish;
        }
        else if (readerIndex == 1)
        {
            event = ui->connectRFID2FunctionComboBox->currentText() == "Start/Lap" ? RacetimingInterface::EventType::LapStart : RacetimingInterface::EventType::Finish;
        }
        else
        {
            return;
        }

        if (event == RacetimingInterface::EventType::LapStart)
            runnerLapStart(runnerIndex);
        else
            runnerFinished(runnerIndex);
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
    m_tagReaders[0]->writeTag(ui->writeTagTagComboBox->currentText().toStdString());
}

void MainWindow::on_startRacePushButton_toggled(const bool checked)
{
    m_raceStarted = checked;
    ui->startRacePushButton->setText(checked ? "Pause race" : "Start race");
}

void MainWindow::on_connectTestPushButton_clicked()
{
    m_racetimingInterface = RacetimingInterface::CreateTestRacetimingInterface(
        [this](const auto &races)
        { m_races = races; racetimingInterface_racesUpdated(); },
        [this](const auto &runners)
        { m_runners = runners; racetimingInterface_runnersUpdated(); });
    m_racetimingInterface->requestRaces();
}

void MainWindow::on_connectLocalPushButton_clicked()
{
    m_racetimingInterface = RacetimingInterface::CreateLocalRacetimingInterface(
        ui->connectLocalFilePathLineEdit->text().toStdString(),
        [this](const auto &races)
        { m_races = races; racetimingInterface_racesUpdated(); },
        [this](const auto &runners)
        { m_runners = runners; racetimingInterface_runnersUpdated(); });
    m_racetimingInterface->requestRaces();
}

void MainWindow::on_logEventsToFileBrowsePathToolButton_triggered(QAction *arg1)
{
    const auto path = QFileDialog::getSaveFileName(this, "Select file", ui->logEventsToFilePathLineEdit->text(), "Text files (*.txt);; All Files (*.*)");
    if (!path.isNull())
        ui->logEventsToFilePathLineEdit->setText(path);
}
