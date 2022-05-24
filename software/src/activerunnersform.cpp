#include "activerunnersform.h"
#include "ui_activerunnersform.h"

ActiveRunnersForm::ActiveRunnersForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ActiveRunnersForm)
{
    ui->setupUi(this);

    m_timer.setInterval(100);
    m_timer.start();

}

void ActiveRunnersForm::on_timer_timeout()
{
    const auto now = std::chrono::steady_clock::now();
    for (const auto& runner : m_runners) {
        const auto dt = now - runner.start;
        const auto time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(dt).count();
        const auto time_s = std::chrono::duration_cast<std::chrono::seconds>(dt).count();
        const auto time_m = std::chrono::duration_cast<std::chrono::minutes>(dt).count();
        const auto time_h = std::chrono::duration_cast<std::chrono::hours>(dt).count();

        runner.timeLabel->setText(QString("%1:%2:%3.%4").arg(time_h, 2, '0').arg(time_m, 2, '0').arg(time_s, 2, '0').arg(time_ms, 2, '0'));
    }
}

ActiveRunnersForm::~ActiveRunnersForm()
{
    delete ui;
}

void ActiveRunnersForm::runnerStart(const std::string &name)
{

}

void ActiveRunnersForm::runnerFinish(const std::string &name)
{

}
