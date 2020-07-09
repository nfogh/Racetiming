#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_timer.setInterval(10);
    m_timer.start();
    connect(&m_timer, &QTimer::timeout, this, &MainWindow::updateTime);

    m_nameLabel = new QLabel("Name");
    m_idLabel = new QLabel("ID");
    m_timeLabels.push_back(new QLabel("Start time"));
    for (int i = 1; i < 10; i++) {
        m_timeLabels.push_back(new QLabel("Lap " + QString::number(i)));
    }
    m_totalTimeLabel = new QLabel("Total time");
}

void MainWindow::updateTime()
{
    const auto time = QTime::currentTime();
    ui->masterTimeLabel->setText(time.toString("HH:mm:ss"));

    const QString timeString = time.toString("HH:mm:ss.zzz");

    for (unsigned int participantIndex = 0; participantIndex < m_participants.size(); participantIndex++) {
        auto& participant = m_participants[participantIndex];
        if (participant.active) {
            participant.times.back() = time;
            m_participantWidgets[participantIndex].times.back()->setText(timeString);

            auto totalTime = participant.times.front().msecsTo(time);
            int mins = totalTime / 60 / 1000;
            int secs = (totalTime / 1000) % 60;
            int msecs = totalTime % 1000;

            QString str = QString("%1:%2.%3").arg(mins, 2, 10, QChar('0')).arg(secs, 2, 10, QChar('0')).arg(msecs, 3, 10, QChar('0'));

            m_participantWidgets[participantIndex].totalTime->setText(str);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addParticipant(const QString& id, const QString& name)
{
    m_participants.push_back({id, name, false, {QTime()}});

    int participantIndex = static_cast<int>(m_participants.size() - 1);

    QToolButton* deleteButton = new QToolButton();
    deleteButton->setText("x");
    deleteButton->setProperty("participant", participantIndex);
    connect(deleteButton, &QToolButton::clicked, this, &MainWindow::deleteParticipant);

    QLineEdit* idLineEdit = new QLineEdit(m_participants[participantIndex].id);
    idLineEdit->setProperty("participant", participantIndex);
    connect(idLineEdit, &QLineEdit::textChanged, this, &MainWindow::participantIDChanged);

    QLineEdit* nameLineEdit = new QLineEdit(m_participants[participantIndex].name);
    nameLineEdit->setProperty("participant", participantIndex);
    connect(nameLineEdit, &QLineEdit::textChanged, this, &MainWindow::participantNameChanged);

    QPushButton* lapPushButton = new QPushButton();
    lapPushButton->setText("Lap");
    lapPushButton->setProperty("participant", participantIndex);
    connect(lapPushButton, &QPushButton::clicked, this, &MainWindow::participantLapped);

    QPushButton* finishedPushButton = new QPushButton();
    finishedPushButton->setText("Finish");
    finishedPushButton->setProperty("participant", participantIndex);
    connect(finishedPushButton, &QPushButton::clicked, this, &MainWindow::participantFinished);

    std::vector<QLineEdit*> times;
    times.push_back(new QLineEdit());

    QLineEdit* totalTimeLineEdit = new QLineEdit();
    totalTimeLineEdit->setProperty("participant", participantIndex);

    m_participantWidgets.push_back({deleteButton, idLineEdit, nameLineEdit, lapPushButton, finishedPushButton, times, totalTimeLineEdit});
    updateParticipantList();
}

static int GetParticipantIndex(const QObject& object)
{
    return object.property("participant").toInt();
}

void MainWindow::participantLapped()
{
    const auto time = QTime::currentTime();
    const int participantIndex = GetParticipantIndex(*QObject::sender());
    if (m_participants[participantIndex].times.size() == 1)
        m_participants[participantIndex].times.back() = time;

    m_participants[participantIndex].times.push_back(time);
    m_participants[participantIndex].active = true;

    if (m_participantWidgets[participantIndex].times.size() == 1)
        m_participantWidgets[participantIndex].times.back()->setText(time.toString("HH:mm:ss.zzz"));

    m_participantWidgets[participantIndex].times.push_back(new QLineEdit());
    updateParticipantList();
}

void MainWindow::participantFinished()
{
    const int participantIndex = GetParticipantIndex(*QObject::sender());
    m_participants[participantIndex].active = false;
    updateParticipantList();
}

void MainWindow::on_addParticipantButton_clicked()
{
    QString id = QString::number(m_participants.size());
    addParticipant(id, "Participant " + id);
}

void MainWindow::deleteParticipant()
{
    const int participantIndex = GetParticipantIndex(*QObject::sender());
    m_participants.erase(m_participants.begin() + participantIndex);
    auto& participant = m_participantWidgets[participantIndex];
    m_participantWidgets.erase(m_participantWidgets.begin() + participantIndex);
    updateParticipantList();
}

void MainWindow::participantNameChanged(const QString& newName)
{
    const int participantIndex = GetParticipantIndex(*QObject::sender());

    m_participants[participantIndex].name = newName;
}

void MainWindow::participantIDChanged(const QString &newID)
{
    const int participantIndex = GetParticipantIndex(*QObject::sender());

    m_participants[participantIndex].id = newID;
}

void MainWindow::updateParticipantList()
{
    QGridLayout* layout = new QGridLayout;

    for (unsigned int participantIndex = 0; participantIndex < m_participantWidgets.size(); participantIndex++) {
        int column = 0;
        const auto& widgets = m_participantWidgets[participantIndex];
        layout->addWidget(widgets.toolButton, participantIndex + 1, column++);
        layout->addWidget(widgets.idLineEdit, participantIndex + 1, column++);
        layout->addWidget(widgets.nameLineEdit, participantIndex + 1, column++);
        layout->addWidget(widgets.lapPushButton, participantIndex + 1, column++);
        layout->addWidget(widgets.finishedPushButton, participantIndex + 1, column++);
        layout->addWidget(widgets.totalTime, participantIndex + 1, column++);
        for (unsigned int timeIndex = 0; timeIndex < widgets.times.size(); timeIndex++)
            layout->addWidget(widgets.times[timeIndex], participantIndex + 1, column++);

        widgets.lapPushButton->setText(m_participants[participantIndex].times.size() == 1 ? "Start" : "Lap");

        if (m_participants[participantIndex].active) {
            widgets.nameLineEdit->setStyleSheet("font: bold;");
            widgets.lapPushButton->setStyleSheet("font: bold;");
            widgets.finishedPushButton->setStyleSheet("font: bold;");
            for (unsigned int timeIndex = 0; timeIndex < widgets.times.size(); timeIndex++)
                widgets.times[timeIndex]->setStyleSheet("font: bold;");
            widgets.totalTime->setStyleSheet("font: bold;");
        } else {
            widgets.nameLineEdit->setStyleSheet("");
            widgets.lapPushButton->setStyleSheet("");
            widgets.finishedPushButton->setStyleSheet("");
            for (unsigned int timeIndex = 0; timeIndex < widgets.times.size(); timeIndex++)
                widgets.times[timeIndex]->setStyleSheet("");
            widgets.totalTime->setStyleSheet("");
        }
    }

    layout->addWidget(m_idLabel, 0, 1);
    layout->addWidget(m_nameLabel, 0, 2);
    layout->addWidget(m_totalTimeLabel, 0, 5);
    for (int i = 0; i < layout->columnCount() - 6; i++)
        layout->addWidget(m_timeLabels[i], 0, 6 + i);

    delete ui->participantsGroupBox->layout();
    ui->participantsGroupBox->setLayout(layout);
}

void MainWindow::saveParticipants(const QString& path)
{
    QJsonArray participants;

    for (const auto& participant : m_participants) {
        QJsonObject object;
        object["name"] = participant.name;
        object["id"] = participant.id;
        participants.append(object);
    }

    QJsonObject docObject;
    docObject["participants"] = participants;

    QJsonDocument doc(docObject);

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Unable to save", "Unable to save to " + path + ": " + file.errorString());
        return;
    }

    file.write(doc.toJson());
}

void MainWindow::loadParticipants(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Unable to load", "Unable to load from " + path + ": " + file.errorString());
        return;
    }

    auto doc = QJsonDocument::fromJson(file.readAll());

    auto participants = doc.object()["participants"].toArray();

    m_participants.clear();
    m_participantWidgets.clear();

    for (const auto& participant : participants)
        addParticipant(participant.toObject()["id"].toString(), participant.toObject()["name"].toString());
}

void MainWindow::saveParticipantTimes(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Unable to save", "Unable to save to " + path + ": " + file.errorString());
        return;
    }

    QTextStream out(&file);

    for (const auto& participant : m_participants) {
        out << participant.id << "," << participant.name;
        for (const auto& time : participant.times)
            out << ",," << time.toString();
        out << Qt::endl;
    }
}

void MainWindow::on_actionSave_participants_triggered()
{
    QString path = QFileDialog::getSaveFileName(this, "Choose file");
    if (!path.isNull()) {
        saveParticipants(path);
        QMessageBox::information(this, "Saved participants", "Saved participants to " + path);
    }
}

void MainWindow::on_actionLoad_participants_triggered()
{
    QString path = QFileDialog::getOpenFileName(this, "Choose file");
    if (!path.isNull()) {
        loadParticipants(path);
        QMessageBox::information(this, "Loaded participants", "Loaded participants from " + path);
    }
}

void MainWindow::on_actionSave_times_to_CSV_triggered()
{
    QString path = QFileDialog::getSaveFileName(this, "Choose file");
    if (!path.isNull()) {
        saveParticipantTimes(path);
        QMessageBox::information(this, "Saved times", "Saved participant times to " + path);
    }
}
