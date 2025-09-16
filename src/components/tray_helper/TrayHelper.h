#pragma once
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QMainWindow>
#include "styles/ButtonStyles.h"
#include "windows/main_window/MainWindow.h"
#include "states/main_window_state/MainWindowState.h"

namespace TrayHelper {
    void setupTray(MainWindow* window, const QIcon& icon);
}
