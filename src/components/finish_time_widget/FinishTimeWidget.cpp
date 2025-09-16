#include "components/finish_time_widget/FinishTimeWidget.h"
#include <QPixmap>
#include <QDateTime>

FinishTimeWidget::FinishTimeWidget(QWidget* parent)
    : QWidget(parent)
{
    startLabel = new QLabel("Start at: --:--", this);
    finishlabel = new QLabel("Free at: --:--", this);

    auto layout = new QHBoxLayout(this);

    QLabel* finishIcon = new QLabel(this);
    finishIcon->setPixmap(QIcon(":/res/resources/icons/clock.svg").pixmap(16,16));

    QLabel* startIcon = new QLabel(this);
    startIcon->setPixmap(QIcon(":/res/resources/icons/clock.svg").pixmap(16,16));

    layout->addWidget(startIcon);
    layout->addWidget(startLabel);

    layout->addSpacing(10);

    layout->addWidget(finishIcon);
    layout->addWidget(finishlabel);

    layout->addStretch();
    setLayout(layout);
}


void FinishTimeWidget::setState(MainWindowState* state) {
    windowState = state;
    if (!windowState) return;

    connect(windowState, &MainWindowState::finishTimeChanged,
            this, &FinishTimeWidget::onFinishTimeChanged);

    connect(windowState, &MainWindowState::timerValueChanged, [this]() {
        if (!windowState) return;
        startLabel->setText("Start at: " + windowState->getStartTime().toString("HH:mm"));
    });

    onFinishTimeChanged(windowState->calculateFinishTime());

    startLabel->setText("Start at: " + windowState->getStartTime().toString("HH:mm"));
}

void FinishTimeWidget::onFinishTimeChanged(const QTime& finishTime) {
    finishlabel->setText("Free at: " + finishTime.toString("HH:mm"));
}
