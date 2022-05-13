#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QtNetwork/QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_networkAccessManager = new QNetworkAccessManager(this);
    m_networkAccessManager->setAutoDeleteReplies(true);
    connect(m_networkAccessManager, &QNetworkAccessManager::finished, this, &MainWindow::replyFinished);
    ui->runnersTableView->setModel(&m_runnersModel);
}

void MainWindow::replyFinished(QNetworkReply* reply)
{
    const auto replyBytes = reply->readAll();
    const auto str(replyBytes);

    qDebug() << "Get reply" << str;
    const auto document = QJsonDocument::fromJson(replyBytes);

    const auto documentObject = document.object();
    if (documentObject.contains("races")) {
        const auto races = documentObject["races"];

        const auto array = races.toArray();
        std::vector<RunnersModel::Runner> data;
        for (const auto& entry : array) {
            QJsonObject obj = entry.toObject();

            const int id = obj["id"].toString().remove("\"").toInt();
            const QString name = obj["name"].toString();
            ui->availableRacesComboBox->addItem("[" + QString::number(id) + "]" + name);
        }
    }


    if (documentObject.contains("runners")) {
        const auto runners = documentObject["runners"];

        const auto array = runners.toArray();
        std::vector<RunnersModel::Runner> data;
        for (const auto& entry : array) {
            QJsonObject obj = entry.toObject();

            const int id = obj["id"].toString().remove("\"").toInt();
            const int number = obj["number"].toString().remove("\"").toInt();
            const QString name = obj["name"].toString();
            const QString surname = obj["surname"].toString();
            data.push_back({id, number, name, surname });
        }
        m_runnersModel.setData(data);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_raceConnectionsConnectButton_clicked()
{
    const auto url = QUrl("http://" + ui->websiteLineEdit->text() + "/getraces_rest.php?&apikey=" + ui->apiKeyLineEdit->text());
    qDebug() << "Requesting " + url.url();
    m_networkAccessManager->get(QNetworkRequest(url));
}


void MainWindow::on_getRunnersPushButton_clicked()
{
    const int id = ui->availableRacesComboBox->currentText().mid(1,1).toInt();
    const auto url = QUrl("http://" + ui->websiteLineEdit->text() + "/getrunners_rest.php?&apikey=" + ui->apiKeyLineEdit->text() + "&raceid=" + QString::number(id));
    qDebug() << "Requesting " + url.url();
    m_networkAccessManager->get(QNetworkRequest(url));
}

