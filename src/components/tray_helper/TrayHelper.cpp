#include "TrayHelper.h"
#include "styles/TrayStyles.h"
#include "utils/TimeCalculator.h"
#include "states/main_window_state/MainWindowState.h"
#include "models/TimerEvent.h"
#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDateTime>
#include <QTimer>
#include <QWidgetAction>

namespace TrayHelper {

// Helper function to get status text from timer status
QString getStatusText(MainWindowState::TimerStatus status) {
    switch (status) {
        case MainWindowState::TimerStatus::Running: return "Running";
        case MainWindowState::TimerStatus::Paused:  return "Paused";
        case MainWindowState::TimerStatus::Stopped: return "Stopped";
        case MainWindowState::TimerStatus::Resumed: return "Resumed";
        default: return "Unknown";
    }
}

// Helper function to generate tooltip text
QString generateTooltipText(int seconds, int total, MainWindowState* state) {
    QString statusText = getStatusText(state->getStatus());

    // Use TimeCalculator for consistent time calculations
    QVector<TimerEvent> timerEvents = state->getTimerEvents();
    int todayOfWeek = QDate::currentDate().dayOfWeek();
    int minBreakSeconds = state->getMinBreakSecondsPerDay().value(todayOfWeek, 0);
    QDateTime currentTime = QDateTime::currentDateTime();

    QTime elapsedTime = TimeCalculator::calculateElapsedTime(
        seconds, timerEvents, minBreakSeconds, currentTime);
    QString formattedElapsedTime = elapsedTime.toString("hh:mm:ss");
    QTime remainingTime = TimeCalculator::calculateRemainingTime(
        seconds, total, timerEvents, minBreakSeconds, currentTime);

    QString tooltip = QString("Elapsed: %1 / Remaining: %2\nStatus: %3")
                      .arg(elapsedTime.toString("hh:mm:ss"))
                      .arg(remainingTime.toString("hh:mm:ss"))
                      .arg(statusText);

    // Add finish time information based on timer status
    if (state->getStatus() == MainWindowState::Running || state->getStatus() == MainWindowState::Resumed) {
        // Timer is running, show calculated finish time
        QTime finishTime = state->calculateFinishTime();
        tooltip += QString("\nExpected finish: %1").arg(finishTime.toString("HH:mm"));
    } else if (state->getStatus() == MainWindowState::Stopped) {
        // Timer is stopped, show when it was stopped
        QVector<TimerEvent> events = state->getTimerEvents();
        QDateTime stopTime = TimeCalculator::findLastStopTime(events);
        if (stopTime.isValid()) {
            tooltip += QString("\nStopped at: %1").arg(stopTime.toString("HH:mm"));
        }
    }

    return tooltip;
}

void setupTray(MainWindow* window, const QIcon& icon) {
    if (!QSystemTrayIcon::isSystemTrayAvailable()) return;

    auto* state = window->getAppState();

    auto* trayIcon = new QSystemTrayIcon(window);
    trayIcon->setIcon(icon);

    auto* trayMenu = new QMenu(window);

    auto* headerWidget = new QWidget(trayMenu);
    headerWidget->setAutoFillBackground(true);
    headerWidget->setStyleSheet(TrayStyles::headerStyle());

    auto* headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(10, 5, 10, 5);
    headerLayout->setSpacing(10);

    auto* appIconLabel = new QLabel(headerWidget);
    appIconLabel->setObjectName("appIconLabel");
    appIconLabel->setPixmap(icon.pixmap(48, 48));
    headerLayout->addWidget(appIconLabel, 0, Qt::AlignVCenter);

    auto* titleLabel = new QLabel("Work Time Tracker", headerWidget);
    titleLabel->setObjectName("titleLabel");
    titleLabel->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    headerLayout->addWidget(titleLabel, 1);

    auto* headerAction = new QWidgetAction(trayMenu);
    headerAction->setDefaultWidget(headerWidget);
    trayMenu->addAction(headerAction);

    auto* timerWidget = new TimerWidget(trayMenu);
    timerWidget->setState(state);
    auto* timerAction = new QWidgetAction(trayMenu);
    timerAction->setDefaultWidget(timerWidget);
    trayMenu->addAction(timerAction);

    auto* trayPanel = new ControlPanel(window);
    trayPanel->setState(state);
    auto* widgetAction = new QWidgetAction(trayMenu);
    widgetAction->setDefaultWidget(trayPanel);
    trayMenu->addAction(widgetAction);

    auto createMenuItem = [](const QString& text, const QIcon& icon) -> QWidget* {
        QWidget* widget = new QWidget;
        QHBoxLayout* layout = new QHBoxLayout(widget);
        layout->setContentsMargins(10, 2, 10, 2);
        layout->setSpacing(10);

        QLabel* iconLabel = new QLabel(widget);
        iconLabel->setPixmap(icon.pixmap(20, 20));
        layout->addWidget(iconLabel);

        QLabel* textLabel = new QLabel(text, widget);
        textLabel->setStyleSheet(TrayStyles::menuItemStyle());
        textLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        layout->addWidget(textLabel, 1);

        return widget;
    };

    // Add View menu items
    auto* mainViewWidget = createMenuItem("Current", QIcon(":/res/resources/icons/timer.svg"));
    auto* mainViewAction = new QWidgetAction(trayMenu);
    mainViewAction->setDefaultWidget(mainViewWidget);
    trayMenu->addAction(mainViewAction);

    auto* statisticsWidget = createMenuItem("Statistics", QIcon(":/res/resources/icons/statistics.svg"));
    auto* statisticsAction = new QWidgetAction(trayMenu);
    statisticsAction->setDefaultWidget(statisticsWidget);
    trayMenu->addAction(statisticsAction);

    auto* dailyChartWidget = createMenuItem("Daily Chart", QIcon(":/res/resources/icons/chart.svg"));
    auto* dailyChartAction = new QWidgetAction(trayMenu);
    dailyChartAction->setDefaultWidget(dailyChartWidget);
    trayMenu->addAction(dailyChartAction);

    trayMenu->addSeparator();

    auto* settingsWidget = createMenuItem("Settings", QIcon(":/res/resources/icons/settings.svg"));
    auto* settingsAction = new QWidgetAction(trayMenu);
    settingsAction->setDefaultWidget(settingsWidget);
    trayMenu->addAction(settingsAction);

    auto settingsWindow = window->getSettingsWindow();
    QObject::connect(settingsAction, &QAction::triggered, [settingsWindow]() {
        if (!settingsWindow) return;
        settingsWindow->show();
        settingsWindow->raise();
        settingsWindow->activateWindow();
    });

    trayMenu->addSeparator();

    auto* quitWidget = createMenuItem("Exit", QIcon(":/res/resources/icons/exit.svg"));
    auto* quitAction = new QWidgetAction(trayMenu);
    quitAction->setDefaultWidget(quitWidget);
    trayMenu->addAction(quitAction);

    // Connect view actions to switch views
    QObject::connect(mainViewAction, &QAction::triggered, [window]() {
        auto central = window->getCentralWidget();
        if (central) {
            central->setCurrentView(0); // Main View
        }
        // Show the window when switching views
        if (!window->isVisible()) {
            window->show();
            window->raise();
            window->activateWindow();
        }
    });

    QObject::connect(statisticsAction, &QAction::triggered, [window]() {
        auto central = window->getCentralWidget();
        if (central) {
            central->setCurrentView(1); // Statistics
        }
        // Show the window when switching views
        if (!window->isVisible()) {
            window->show();
            window->raise();
            window->activateWindow();
        }
    });

    QObject::connect(dailyChartAction, &QAction::triggered, [window]() {
        auto central = window->getCentralWidget();
        if (central) {
            central->setCurrentView(2); // Daily Chart
            central->onDailyChartTabSelected(); // Notify the chart widget
        }
        // Show the window when switching views
        if (!window->isVisible()) {
            window->show();
            window->raise();
            window->activateWindow();
        }
    });

    QObject::connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);

    trayIcon->setContextMenu(trayMenu);

    // Generate initial tooltip using the helper function
    int seconds = state->getValue();
    int total = state->getTotalSeconds();
    QString tooltip = generateTooltipText(seconds, total, state);

    trayIcon->setToolTip(tooltip);
    trayIcon->show();

    QObject::connect(trayIcon, &QSystemTrayIcon::activated,
        [window](QSystemTrayIcon::ActivationReason reason) {
            if (reason == QSystemTrayIcon::DoubleClick) {
                if (!window->isVisible()) {
                    window->show();
                    window->raise();
                    window->activateWindow();
                } else {
                    window->hide();
                }
            }
        }
    );

    QObject::connect(state, &MainWindowState::timerValueChanged, [trayIcon, state](int seconds){
        int total = state->getTotalSeconds();

        // Generate tooltip using the helper function
        QString tooltip = generateTooltipText(seconds, total, state);
        trayIcon->setToolTip(tooltip);
    });

    QObject::connect(state, &MainWindowState::timerStatusChanged, [trayIcon, state](MainWindowState::TimerStatus){
        int seconds = state->getValue();
        state->timerValueChanged(seconds);
    });

    // Connect to the workdayEnded signal to show notification
    QObject::connect(state, &MainWindowState::workdayEnded, [trayIcon]() {
        trayIcon->showMessage(
            "Work Day Ended",
            "Your work day has finished! Time to go home!",
            QSystemTrayIcon::Information,
            5000 // 5 seconds
        );
    });
}

}