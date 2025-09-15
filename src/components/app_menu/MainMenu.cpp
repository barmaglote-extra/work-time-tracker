#include "components/app_menu/MainMenu.h"

void MainMenu::setupUIImpl() {
    auto fileMenu = addMenu("File");
    auto newAction = fileMenu->addAction("New");
    auto exitAction = fileMenu->addAction("Exit");

    //connect(exitAction, &QAction::triggered, qApp, &QApplication::quit);
}