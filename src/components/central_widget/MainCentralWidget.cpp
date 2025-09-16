#include "components/central_widget/MainCentralWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

void MainCentralWidget::setupUIImpl() {
    mainLayout = new QVBoxLayout(this);

    // ======================
    // Комбо для выбора вида
    // ======================
    auto viewSwitcher = new QComboBox(this);
    viewSwitcher->addItem("Main View");
    viewSwitcher->addItem("Statistics");

    // Стиль комбо, чтобы гармонировало с остальным UI
    viewSwitcher->setStyleSheet(R"(
        QComboBox {
            font: 14px "Segoe UI";
            padding: 5px 10px;
            border: 1px solid #518eff;
            border-radius: 6px;
            min-width: 120px;
        }
        QComboBox::drop-down {
            subcontrol-origin: padding;
            subcontrol-position: top right;
            width: 20px;
            border-left-width: 1px;
            border-left-color: #518eff;
            border-left-style: solid;
            border-top-right-radius: 6px;
            border-bottom-right-radius: 6px;
        }
        QComboBox QAbstractItemView {
            selection-background-color: #518eff;
            background-color: #ffffff;
            border: 1px solid #518eff;
        }
    )");

    mainLayout->addWidget(viewSwitcher, 0, Qt::AlignRight);

    // ======================
    // Stacked views
    // ======================
    stackedViews = new QStackedWidget(this);

    // --- Main view ---
    mainViewWidget = new QWidget(this);
    QVBoxLayout* mainViewLayout = new QVBoxLayout(mainViewWidget);

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

    mainViewLayout->addLayout(statsLayout);
    mainViewLayout->addLayout(controlsLayout);

    stackedViews->addWidget(mainViewWidget);

    // --- Statistics view ---
    statsWidget = new StatsWidget(this);
    stackedViews->addWidget(statsWidget);

    // ======================
    // Добавляем stacked widget
    // ======================
    mainLayout->addWidget(stackedViews);

    // ======================
    // Сигнал переключения вида
    // ======================
    connect(viewSwitcher, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](int index){
                if (stackedViews)
                    stackedViews->setCurrentIndex(index);
            });

    setLayout(mainLayout);
}

void MainCentralWidget::setCurrentView(int index) {
    if (!stackedViews) return;
    if (index < 0 || index >= stackedViews->count()) return;
    stackedViews->setCurrentIndex(index);
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

    if (statsWidget) {
        statsWidget->setState(windowState);
    }
}
