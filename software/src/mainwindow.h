#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include <RunnersModel.h>

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
    void replyFinished(QNetworkReply* reply);

    void on_raceConnectionsConnectButton_clicked();

    void on_getRunnersPushButton_clicked();

private:
    Ui::MainWindow *ui;

    QNetworkAccessManager *m_networkAccessManager;

    RunnersModel m_runnersModel;
};
#endif // MAINWINDOW_H
