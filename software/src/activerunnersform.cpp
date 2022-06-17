#include "activerunnersform.h"
#include "qgridlayout.h"

ActiveRunnersForm::ActiveRunnersForm(QWidget *parent) :
    QWidget(parent)
{
    // This should be some odd number that makes it appear as if the numbers are
    // counting very fast, without actually changing that fast.
    m_timer.setInterval(37);
    connect(&m_timer, &QTimer::timeout, this, &ActiveRunnersForm::timer_timeout);
    m_timer.start();
}

void ActiveRunnersForm::timer_timeout()
{
    const auto now = std::chrono::steady_clock::now();

    // First, clean up finished runners after some time
    auto it = m_runners.begin();
    while (it != m_runners.end()) {
        if (it->stop && (now > *it->stop + std::chrono::seconds(20))) {
            delete it->nameLabel;
            delete it->timeLabel;
            it = m_runners.erase(it);
        } else {
            it++;
        }
    }

    for (const auto& runner : m_runners) {
        const auto dt = runner.stop? *runner.stop - runner.start : now - runner.start;

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

        if (runner.stop) {
            auto palette = runner.timeLabel->palette();
            palette.setColor(runner.timeLabel->foregroundRole(), Qt::green);
            runner.timeLabel->setPalette(palette);
            runner.nameLabel->setPalette(palette);
        }
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
    if (it != m_runners.end())
        it->stop = std::chrono::steady_clock::now();
}
