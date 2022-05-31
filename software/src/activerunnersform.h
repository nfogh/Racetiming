#pragma once

#include <QWidget>
#include <QTimer>
#include <chrono>
#include <optional>
#include <QLabel>

namespace Ui {
class ActiveRunnersForm;
}

class ActiveRunnersForm : public QWidget
{
    Q_OBJECT

public:
    explicit ActiveRunnersForm(QWidget *parent = nullptr);
    ~ActiveRunnersForm();

    void runnerStart(const std::string& name);
    void runnerFinish(const std::string& name);

private slots:
    void timer_timeout();

private:
    void updateUI();

    struct ActiveRunner {
        std::string name;
        std::chrono::steady_clock::time_point start;
        std::optional<std::chrono::steady_clock::time_point> stop;

        QLabel* nameLabel;
        QLabel* timeLabel;
    };

    std::vector<ActiveRunner> m_runners;

    QTimer m_timer;
};
