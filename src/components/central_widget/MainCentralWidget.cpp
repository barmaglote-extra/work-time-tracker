#include "components/central_widget/MainCentralWidget.h"
#include <QLabel>
#include <QButtonGroup>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

void MainCentralWidget::setupUIImpl() {
    mainLayout = new QVBoxLayout(this);

    statsLayout = new QHBoxLayout();
    statsLayout->addWidget(new QPushButton("Тут будет чарт"));

    controlsLayout = new QHBoxLayout();

    auto buttonGroup = new QButtonGroup(this);
    auto startButton = new QPushButton("Start");
    auto pauseButton = new QPushButton("Pause");
    auto resumeButton = new QPushButton("Resume");
    auto stopButton = new QPushButton("Stop");

    buttonGroup->addButton(startButton);
    buttonGroup->addButton(pauseButton);
    buttonGroup->addButton(resumeButton);
    buttonGroup->addButton(stopButton);

    controlsLayout = new QHBoxLayout();
    controlsLayout->addWidget(startButton);
    controlsLayout->addWidget(pauseButton);
    controlsLayout->addWidget(resumeButton);
    controlsLayout->addWidget(stopButton);

    mainLayout->addLayout(statsLayout);
    mainLayout->addLayout(controlsLayout);

    this->setLayout(mainLayout);
}