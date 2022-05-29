#include "activerunnersform.h"
#include "qgridlayout.h"
#include "ui_activerunnersform.h"

ActiveRunnersForm::ActiveRunnersForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ActiveRunnersForm)
{
    ui->setupUi(this);

    m_timer.setInterval(100);
    m_timer.start();
    connect(&m_timer, QTimer::timeout, this, ActiveRunnersForm::on_timer_timeout);
}

void ActiveRunnersForm::on_timer_timeout()
{
    const auto now = std::chrono::steady_clock::now();
    for (const auto& runner : m_runners) {
        const auto dt = now - runner.start;
        const auto time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(dt).count() % 1000;
        const auto time_s = std::chrono::duration_cast<std::chrono::seconds>(dt).count() % 60;
        const auto time_m = std::chrono::duration_cast<std::chrono::minutes>(dt).count() % 60;
        const auto time_h = std::chrono::duration_cast<std::chrono::hours>(dt).count() % 24;

        QString str = QString("%1.%2").arg(time_s, 2, 10, QChar('0')).arg(time_ms, 3, 10, QChar('0'));
        if (time_m > 0)
            str = QString("%1:").arg(time_m, 2, 10, QChar('0')) + str;
        if (time_h > 0)
            str = QString("%1:").arg(time_h, 2, 10, QChar('0')) + str;

        runner.timeLabel->setText(str);
    }
}

void ActiveRunnersForm::updateUI()
{
    QGridLayout* newLayout = new QGridLayout();
    for (int row = 0; row < m_runners.size(); row++) {
        newLayout->addWidget(m_runners[row].nameLabel, row, 0);
        newLayout->addWidget(m_runners[row].timeLabel, row, 1);
    }
    delete layout();
    setLayout(newLayout);
}

ActiveRunnersForm::~ActiveRunnersForm()
{
    delete ui;
}

void ActiveRunnersForm::runnerStart(const std::string &name)
{
    const auto it = std::find_if(m_runners.begin(), m_runners.end(), [&name](const auto& runner){ return runner.name == name; });
    if (it == m_runners.end()) {
        QFont font;
        font.setPointSize(54);
        font.setBold(true);
        ActiveRunner runner;
        runner.name = name;
        runner.nameLabel = new QLabel(this);
        runner.nameLabel->setFont(font);
        runner.nameLabel->setText(QString::fromStdString(name));
        runner.nameLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        runner.start = std::chrono::steady_clock::now();
        runner.timeLabel = new QLabel(this);
        runner.timeLabel->setFont(font);
        runner.timeLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        m_runners.push_back(runner);
        updateUI();
    }
}

void ActiveRunnersForm::runnerFinish(const std::string &name)
{
    const auto it = std::find_if(m_runners.begin(), m_runners.end(), [&name](const auto& runner){ return runner.name == name; });
    if (it != m_runners.end()) {
        delete it->nameLabel;
        delete it->timeLabel;
        m_runners.erase(it);
        updateUI();
    }
}
