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

    auto* trayPanel = new ControlPanel(window);
    trayPanel->setState(state);

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
