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
        QPalette pal = headerWidget->palette();
        pal.setColor(QPalette::Window, Qt::white);
        headerWidget->setPalette(pal);

        auto* headerLayout = new QHBoxLayout(headerWidget);
        headerLayout->setContentsMargins(10, 5, 10, 5);
        headerLayout->setSpacing(10);

        auto* appIconLabel = new QLabel(headerWidget);
        appIconLabel->setPixmap(icon.pixmap(48, 48));
        headerLayout->addWidget(appIconLabel, 0, Qt::AlignVCenter);

        auto* titleLabel = new QLabel("Work Time Tracker", headerWidget);
        QFont font = titleLabel->font();
        font.setPointSize(14);
        font.setBold(true);
        titleLabel->setFont(font);
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

        auto* settingsAction = new QAction("Settings", window);
        trayMenu->addAction(settingsAction);

        auto* quitAction = new QAction("Exit", window);
        QObject::connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);
        trayMenu->addAction(quitAction);

        trayIcon->setContextMenu(trayMenu);

        int seconds = state->getValue();
        int total = state->getTotalSeconds();
        int remaining = total - seconds;

        QString statusText;
        switch (state->getStatus()) {
            case MainWindowState::TimerStatus::Running: statusText = "Running"; break;
            case MainWindowState::TimerStatus::Paused:  statusText = "Paused";  break;
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
