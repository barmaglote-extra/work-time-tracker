#include "components/finish_time_widget/FinishTimeWidget.h"
#include <QPixmap>
#include <QDateTime>

FinishTimeWidget::FinishTimeWidget(QWidget* parent)
    : QWidget(parent)
{
    label = new QLabel("Free at: --:--", this);

    auto layout = new QHBoxLayout(this);
    QLabel* iconLabel = new QLabel(this);
    iconLabel->setPixmap(QIcon(":/res/resources/icons/clock.svg").pixmap(16,16));

    layout->addWidget(iconLabel);
    layout->addWidget(label);
    layout->addStretch();

    setLayout(layout);
}

void FinishTimeWidget::setState(MainWindowState* state) {
    windowState = state;
    if (!windowState) return;

    connect(windowState, &MainWindowState::finishTimeChanged,
            this, &FinishTimeWidget::onFinishTimeChanged);

     onFinishTimeChanged(windowState->calculateFinishTime());
}

void FinishTimeWidget::onFinishTimeChanged(const QTime& finishTime) {
    label->setText("Free at: " + finishTime.toString("HH:mm"));
}
