#include "components/finish_time_widget/FinishTimeWidget.h"
#include "styles/FinishTimeStyles.h"
#include <QPixmap>
#include <QDateTime>

FinishTimeWidget::FinishTimeWidget(QWidget* parent) : QWidget(parent) {
    startLabel = new QLabel("Start at: --:--", this);
    finishLabel = new QLabel("Free at: --:--", this);

    startLabel->setStyleSheet(FinishTimeStyles::labelStyle());
    finishLabel->setStyleSheet(FinishTimeStyles::labelStyle());

    auto layout = new QHBoxLayout(this);

    QLabel* startIcon = new QLabel(this);
    startIcon->setPixmap(QIcon(":/res/resources/icons/clock.svg").pixmap(24, 24));
    startIcon->setStyleSheet(FinishTimeStyles::iconStyle());

    QLabel* finishIcon = new QLabel(this);
    finishIcon->setPixmap(QIcon(":/res/resources/icons/clock.svg").pixmap(24, 24));
    finishIcon->setStyleSheet(FinishTimeStyles::iconStyle());

    QLabel* middleIcon = new QLabel(this);
    middleIcon->setPixmap(QIcon(":/res/resources/icons/app_icon.svg").pixmap(64, 64));
    middleIcon->setAlignment(Qt::AlignCenter);
    middleIcon->setStyleSheet(FinishTimeStyles::iconStyle());

    layout->addWidget(startIcon);
    layout->addWidget(startLabel);
    layout->addStretch(1);
    layout->addWidget(middleIcon);
    layout->addStretch(1);
    layout->addWidget(finishIcon);
    layout->addWidget(finishLabel);

    setLayout(layout);
}

void FinishTimeWidget::setState(MainWindowState* state) {
    windowState = state;
    if (!windowState) return;

    connect(windowState, &MainWindowState::finishTimeChanged,
            this, &FinishTimeWidget::setFinishTime);

    connect(windowState, &MainWindowState::timerValueChanged, [this]() {
        if (!windowState) return;
        setStartTime(windowState->getStartTime());
    });

    setStartTime(windowState->getStartTime());
    setFinishTime(windowState->calculateFinishTime());
}

void FinishTimeWidget::setStartTime(const QTime& startTime) {
    startLabel->setText("Start at: " + startTime.toString("HH:mm"));
}

void FinishTimeWidget::setFinishTime(const QTime& finishTime) {
    finishLabel->setText("Free at: " + finishTime.toString("HH:mm"));
}
