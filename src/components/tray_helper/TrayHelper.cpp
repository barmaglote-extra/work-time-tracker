#include "TrayHelper.h"
#include <QWidgetAction>
#include <QApplication>

namespace TrayHelper {

    void setupTray(MainWindow* window, const QIcon& icon) {
        if (!QSystemTrayIcon::isSystemTrayAvailable()) return;

        auto* state = window->getAppState();

        auto* trayIcon = new QSystemTrayIcon(window);
        trayIcon->setIcon(icon);

        auto* trayMenu = new QMenu(window);

        auto* trayPanel = new TrayPanel(state);

        auto* widgetAction = new QWidgetAction(trayMenu);
        widgetAction->setDefaultWidget(trayPanel);
        trayMenu->addAction(widgetAction);

        auto* quitAction = new QAction("Exit", window);
        QObject::connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);
        trayMenu->addAction(quitAction);

        trayIcon->setContextMenu(trayMenu);
        trayIcon->show();
    }

}
