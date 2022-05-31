#pragma once

#include "RacesTableModel.h"
#include "RunnersTableModel.h"
#include "ColumnIndexSortFilterProxyModel.h"
#include "activerunnersform.h"

#include <QDataWidgetMapper>
#include <QMainWindow>
#include <RacetimingInterface/IRacetimingInterface.h>
#include <TagReaders/ITagReader.h>
#include <array>
#include <memory>


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
    void racetimingInterface_racesUpdated();
    void racetimingInterface_runnersUpdated();

    void on_raceConnectionsConnectButton_clicked();

    void on_getRunnersPushButton_clicked();

    void on_connectRFID1ConnectPushButton_clicked();

    void on_connectRFID2ConnectPushButton_clicked();

    void on_writeTagPreviousPushButton_clicked();

    void on_writeTagNextPushButton_clicked();

    void on_writeTagWritePushButton_clicked();

    void on_startRacePushButton_toggled(bool checked);

    void on_connectTestPushButton_clicked();

    void on_connectLocalPushButton_clicked();

private:
    void tagDetected(int readerIndex, std::string_view epc);
    void updateWriteTagsLabels();

    void runnerLapStart(int runnerID);
    void runnerFinished(int runnerID);

    Ui::MainWindow *ui;

    std::array<std::unique_ptr<TagReaders::ITagReaderWriter>, 2> m_tagReaders;

    std::unique_ptr<RacetimingInterface::IRacetimingInterface> m_racetimingInterface;

    RacetimingInterface::RacesTable m_races;
    RacetimingInterface::RunnersTable m_runners;
    ColumnIndexSortFilterProxyModel m_numberSummarySortFilterProxyModel;
    ColumnIndexSortFilterProxyModel m_raceProgressSortFilterProxyModel;
    ColumnIndexSortFilterProxyModel m_racesSortFilterProxyModel;
    ColumnIndexSortFilterProxyModel m_attachTagSortFilterProxyModel;

    RacesTableModel m_racesTableModel;
    RunnersTableModel m_runnersTableModel;

    ActiveRunnersForm m_activeRunnersForm;
    QDataWidgetMapper m_tagWriterDataWidgetMapper;

    int m_writeTagsIndex = 0;

    bool m_raceStarted = false;
};
