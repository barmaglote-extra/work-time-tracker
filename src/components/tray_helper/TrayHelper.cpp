#include "TrayHelper.h"

namespace TrayHelper {
    void setupTray(QMainWindow* window, const QIcon& icon) {
        if (!QSystemTrayIcon::isSystemTrayAvailable()) return;

        auto* trayIcon = new QSystemTrayIcon(qApp);
        trayIcon->setIcon(icon);

        auto* trayMenu = new QMenu(window);

        auto* showAction = new QAction("Main window", trayMenu);
        auto* startAction = new QAction("Start", trayMenu);
        auto* pauseAction = new QAction("Pause", trayMenu);
        auto* resetAction = new QAction("Resume", trayMenu);
        auto* stopAction = new QAction("Stop", trayMenu);
        auto* quitAction = new QAction("Выход", trayMenu);

        QObject::connect(showAction, &QAction::triggered, window, [window]() {
            window->showNormal();
            window->raise();
            window->activateWindow();
        });

        QObject::connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);

        trayMenu->addAction(showAction);
        trayMenu->addSeparator();
        trayMenu->addAction(startAction);
        trayMenu->addAction(pauseAction);
        trayMenu->addAction(resetAction);
        trayMenu->addAction(stopAction);
        trayMenu->addSeparator();
        trayMenu->addAction(quitAction);

        trayIcon->setContextMenu(trayMenu);
        trayIcon->show();

        // 👉 Здесь можно пробросить сигналы в MainWindow
        // QObject::connect(startAction, &QAction::triggered, window, &MainWindow::onStart);
        // QObject::connect(pauseAction, &QAction::triggered, window, &MainWindow::onPause);
        // QObject::connect(resetAction, &QAction::triggered, window, &MainWindow::onReset);
    }
}
