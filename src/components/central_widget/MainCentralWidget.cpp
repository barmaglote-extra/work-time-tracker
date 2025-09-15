#include "components/central_widget/MainCentralWidget.h"
#include <QLabel>
#include <QButtonGroup>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "components/timer_widget/TimerWidget.h"
#include "components/timer_chart/TimerChart.h"
#include "styles/ButtonStyles.h"

void MainCentralWidget::setupUIImpl() {
    mainLayout = new QVBoxLayout(this);

    statsLayout = new QHBoxLayout();
    auto timerChart = new TimerChart(nullptr);
    statsLayout->addWidget(timerChart);

    auto buttonGroup = new QButtonGroup(this);
    startButton = new QPushButton();
    pauseButton = new QPushButton();
    resumeButton = new QPushButton();
    stopButton = new QPushButton();

    startButton->setIcon(QIcon(":/res/resources/icons/start.svg"));
    pauseButton->setIcon(QIcon(":/res/resources/icons/pause.svg"));
    resumeButton->setIcon(QIcon(":/res/resources/icons/resume.svg"));
    stopButton->setIcon(QIcon(":/res/resources/icons/stop.svg"));

    startButton->setStyleSheet(getButtonStyle());
    pauseButton->setStyleSheet(getButtonStyle());
    resumeButton->setStyleSheet(getButtonStyle());
    stopButton->setStyleSheet(getButtonStyle());

    startButton->setIconSize(QSize(32,32));
    pauseButton->setIconSize(QSize(32,32));
    resumeButton->setIconSize(QSize(32,32));
    stopButton->setIconSize(QSize(32,32));

    buttonGroup->addButton(startButton);
    buttonGroup->addButton(pauseButton);
    buttonGroup->addButton(resumeButton);
    buttonGroup->addButton(stopButton);

    controlsLayout = new QHBoxLayout();
    auto timeLayout = new QHBoxLayout();
    auto buttonsLayout = new QHBoxLayout();

    auto timerWidget = new TimerWidget(nullptr);

    timeLayout->addWidget(timerWidget);
    timeLayout->setAlignment(timerWidget, Qt::AlignCenter);

    buttonsLayout->addWidget(startButton);
    buttonsLayout->addWidget(pauseButton);
    buttonsLayout->addWidget(resumeButton);
    buttonsLayout->addWidget(stopButton);

    controlsLayout->addLayout(timeLayout);
    controlsLayout->addLayout(buttonsLayout);
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

    connect(startButton, &QPushButton::clicked, [this]() {
        windowState->setTimeStatus(MainWindowState::TimerStatus::Running);
    });
    connect(pauseButton, &QPushButton::clicked, [this]() {
        windowState->setTimeStatus(MainWindowState::TimerStatus::Paused);
    });
    connect(resumeButton, &QPushButton::clicked, [this]() {
        windowState->setTimeStatus(MainWindowState::TimerStatus::Resumed);
    });
    connect(stopButton, &QPushButton::clicked, [this]() {
        windowState->setTimeStatus(MainWindowState::TimerStatus::Stopped);
    });

    connect(windowState, &MainWindowState::timerStatusChanged, this, &MainCentralWidget::updateButtonStates);
    updateButtonStates(windowState->getStatus());
}


void MainCentralWidget::updateButtonStates(MainWindowState::TimerStatus status) {
    switch (status) {
        case MainWindowState::TimerStatus::Running:
            startButton->setEnabled(false);
            resumeButton->setEnabled(false);
            pauseButton->setEnabled(true);
            stopButton->setEnabled(true);
            break;

        case MainWindowState::TimerStatus::Paused:
            startButton->setEnabled(false);
            resumeButton->setEnabled(true);
            pauseButton->setEnabled(false);
            stopButton->setEnabled(false);
            break;

        case MainWindowState::TimerStatus::Stopped:
            startButton->setEnabled(true);
            resumeButton->setEnabled(false);
            pauseButton->setEnabled(false);
            stopButton->setEnabled(false);
            break;

        case MainWindowState::TimerStatus::Resumed:
            startButton->setEnabled(false);
            resumeButton->setEnabled(false);
            pauseButton->setEnabled(true);
            stopButton->setEnabled(true);
            break;
    }
}