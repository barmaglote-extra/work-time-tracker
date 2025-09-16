#include "components/central_widget/MainCentralWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

void MainCentralWidget::setupUIImpl() {
    mainLayout = new QVBoxLayout(this);

    statsLayout = new QVBoxLayout();
    auto timerChart = new TimerChart(nullptr);
    finishWidget = new FinishTimeWidget(this);
    statsLayout->addWidget(finishWidget);
    statsLayout->addWidget(timerChart);
    controlsLayout = new QHBoxLayout();

    auto timeLayout = new QHBoxLayout();
    auto timerWidget = new TimerWidget(nullptr);

    timeLayout->addWidget(timerWidget);
    timeLayout->setAlignment(timerWidget, Qt::AlignCenter);

    controlPanel = new ControlPanel(this);

    controlsLayout->addLayout(timeLayout);
    controlsLayout->addWidget(controlPanel);
    controlsLayout->setStretch(0, 1);
    controlsLayout->setStretch(1, 1);

    mainLayout->addLayout(statsLayout);
    mainLayout->addLayout(controlsLayout);
}

void MainCentralWidget::setState(MainWindowState* state) {
    windowState = state;
    if (!windowState) return;

    for (QObject* obj : findChildren<QObject*>()) {
        if (auto chart = qobject_cast<TimerChart*>(obj)) {
            chart->setState(windowState);
        }
        if (auto timer = qobject_cast<TimerWidget*>(obj)) {
            timer->setState(windowState);
        }
    }

    if (controlPanel) {
        controlPanel->setState(windowState);
        controlPanel->updateButtonStates(windowState->getStatus());
    }

    if (finishWidget) {
        finishWidget->setState(windowState);
    }
}
