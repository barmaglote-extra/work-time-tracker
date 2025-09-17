#include "TrayHelper.h"
#include <QWidgetAction>
#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QIcon>
#include <QPalette>

namespace TrayHelper {

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

    auto* statisticsWidget = createMenuItem("Statistics", QIcon(":/res/resources/icons/stats.svg"));
    auto* statisticsAction = new QWidgetAction(trayMenu);
    statisticsAction->setDefaultWidget(statisticsWidget);
    trayMenu->addAction(statisticsAction);

    trayMenu->addSeparator();

    auto* quitWidget = createMenuItem("Exit", QIcon(":/res/resources/icons/exit.svg"));
    auto* quitAction = new QWidgetAction(trayMenu);
    quitAction->setDefaultWidget(quitWidget);
    trayMenu->addAction(quitAction);

    QObject::connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);

    trayIcon->setContextMenu(trayMenu);

    int seconds = state->getValue();
    int total = state->getTotalSeconds();
    int remaining = total - seconds;

    QString statusText;
    switch (state->getStatus()) {
        case MainWindowState::TimerStatus::Running: statusText = "Running"; break;
        case MainWindowState::TimerStatus::Paused:  statusText = "Paused"; break;
        case MainWindowState::TimerStatus::Stopped: statusText = "Stopped"; break;
        case MainWindowState::TimerStatus::Resumed: statusText = "Resumed"; break;
    }

    QTime elapsedTime(0,0); elapsedTime = elapsedTime.addSecs(seconds);
    QTime remainingTime(0,0); remainingTime = remainingTime.addSecs(remaining);

    QString tooltip = QString("Elapsed: %1 / Remaining: %2\nStatus: %3")
                      .arg(elapsedTime.toString("hh:mm:ss"))
                      .arg(remainingTime.toString("hh:mm:ss"))
                      .arg(statusText);

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
        int remaining = total - seconds;

        QString statusText;
        switch (state->getStatus()) {
            case MainWindowState::TimerStatus::Running: statusText = "Running"; break;
            case MainWindowState::TimerStatus::Paused: statusText = "Paused"; break;
            case MainWindowState::TimerStatus::Stopped: statusText = "Stopped"; break;
            case MainWindowState::TimerStatus::Resumed: statusText = "Resumed"; break;
        }

        QTime elapsedTime(0,0);
        elapsedTime = elapsedTime.addSecs(seconds);
        QTime remainingTime(0,0);
        remainingTime = remainingTime.addSecs(remaining);

        QString tooltip = QString("Elapsed: %1 / Remaining: %2\nStatus: %3")
                          .arg(elapsedTime.toString("hh:mm:ss"))
                          .arg(remainingTime.toString("hh:mm:ss"))
                          .arg(statusText);

        trayIcon->setToolTip(tooltip);
    });

    QObject::connect(state, &MainWindowState::timerStatusChanged, [trayIcon, state](MainWindowState::TimerStatus){
        int seconds = state->getValue();
        state->timerValueChanged(seconds);
    });
}

}
