#include "components/central_widget/MainCentralWidget.h"
#include <QLabel>
#include <QButtonGroup>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "components/timer_widget/TimerWidget.h"

void MainCentralWidget::setupUIImpl() {
    mainLayout = new QVBoxLayout(this);

    statsLayout = new QHBoxLayout();
    statsLayout->addWidget(new QPushButton("Тут будет чарт"));


    auto buttonGroup = new QButtonGroup(this);
    auto startButton = new QPushButton("Start");
    auto pauseButton = new QPushButton("Pause");
    auto resumeButton = new QPushButton("Resume");
    auto stopButton = new QPushButton("Stop");

    connect(startButton, &QPushButton::clicked, [this]() {
        windowState->setTimeStatus(MainWindowState::TimerStatus::Running);
    });
    connect(pauseButton, &QPushButton::clicked, [this]() {
        windowState->setTimeStatus(MainWindowState::TimerStatus::Paused);
    });
    connect(resumeButton, &QPushButton::clicked, [this]() {
        windowState->setTimeStatus(MainWindowState::TimerStatus::Running);
    });
    connect(stopButton, &QPushButton::clicked, [this]() {
        windowState->setTimeStatus(MainWindowState::TimerStatus::Stopped);
    });

    buttonGroup->addButton(startButton);
    buttonGroup->addButton(pauseButton);
    buttonGroup->addButton(resumeButton);
    buttonGroup->addButton(stopButton);

    controlsLayout = new QHBoxLayout();

    auto timeLayout = new QHBoxLayout();
    auto buttonsLayout = new QHBoxLayout();
    auto timerWidget = new TimerWidget(nullptr, windowState);

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

    this->setLayout(mainLayout);
}