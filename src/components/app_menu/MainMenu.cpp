#include "components/app_menu/MainMenu.h"

void MainMenu::setupUIImpl() {
    auto fileMenu = addMenu("File");
    auto newAction = fileMenu->addAction("New");
    auto settingsAction = fileMenu->addAction("Settings");
    fileMenu->addSeparator();
    auto exitAction = fileMenu->addAction("Exit");

    connect(exitAction, &QAction::triggered, qApp, &QApplication::quit);

    connect(settingsAction, &QAction::triggered, [this]() {
        if (!settingsWindow) {
            return;
        }
        settingsWindow->show();
        settingsWindow->raise();
        settingsWindow->activateWindow();
    });
}