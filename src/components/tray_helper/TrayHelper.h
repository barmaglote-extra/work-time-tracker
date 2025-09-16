#pragma once
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QMainWindow>

namespace TrayHelper {
    void setupTray(QMainWindow* window, const QIcon& icon);
}