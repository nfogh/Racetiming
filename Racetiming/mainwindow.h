#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QTimer>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QToolButton>
#include <QSerialPort>
#include <vector>

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
    void updateTime();

    void on_addParticipantButton_clicked();

    void deleteParticipant();
    void participantLapped(int index = -1);
    void participantFinished(int index = -1);
    void participantNameChanged(const QString& newName);
    void participantIDChanged(const QString& newID);

    void on_actionSave_participants_triggered();

    void on_actionSave_times_to_CSV_triggered();

    void on_actionLoad_participants_triggered();

    void on_actionAutotimer_triggered();

    void serialPort_readyRead();

private:
    QVector<int> m_teamIndices;

    Ui::MainWindow *ui;

    QLabel* m_idLabel;
    QLabel* m_nameLabel;
    std::vector<QLabel*> m_timeLabels;
    QLabel* m_totalTimeLabel;

    struct ParticipantData {
        QString id;
        QString name;
        bool active;
        std::vector<QTime> times;
    };

    std::vector<ParticipantData> m_participants;

    struct ParticipantWidgets {
        QToolButton* toolButton;
        QLineEdit* idLineEdit;
        QLineEdit* nameLineEdit;
        QPushButton* lapPushButton;
        QPushButton* finishedPushButton;
        std::vector<QLineEdit*> times;
        QLineEdit* totalTime;
    };

    std::vector<ParticipantWidgets> m_participantWidgets;


    struct ActiveRunnerWidgets {
        QLabel* m_nameLabel;
        QLabel* m_timeLabel;
    };

    std::vector<ActiveRunnerWidgets> m_activeRunnerWidgets;
    QDialog* m_activeRunnersDialog;

    QSerialPort m_serialPort;

    QTimer m_timer;

    void addParticipant(const QString& id, const QString& name);

    void updateParticipantList();

    void saveParticipants(const QString& path);
    void loadParticipants(const QString& path);

    void saveParticipantTimes(const QString& path);
};
#endif // MAINWINDOW_H
