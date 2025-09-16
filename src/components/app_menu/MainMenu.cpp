#include "components/app_menu/MainMenu.h"

void MainMenu::setupUIImpl() {
    auto fileMenu = addMenu("File");
    auto newAction = fileMenu->addAction("New");
    auto exitAction = fileMenu->addAction("Exit");
    auto settingsAction = fileMenu->addAction("Settings");

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