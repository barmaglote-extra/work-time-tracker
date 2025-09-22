#include "components/app_menu/MainMenu.h"
#include "windows/about_window/AboutWindow.h"
#include <QApplication>

void MainMenu::setupUIImpl() {
    auto fileMenu = addMenu("File");
    auto settingsAction = fileMenu->addAction("Settings");
    auto aboutAction = fileMenu->addAction("About Program");
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

    connect(aboutAction, &QAction::triggered, [this]() {
        AboutWindow* aboutWindow = new AboutWindow(nullptr, nullptr);
        aboutWindow->show();
        aboutWindow->raise();
        aboutWindow->activateWindow();
    });

    // Add View menu
    auto viewMenu = addMenu("View");
    auto mainViewAction = viewMenu->addAction("Current");
    auto statisticsViewAction = viewMenu->addAction("Statistics");
    auto dailyChartViewAction = viewMenu->addAction("Daily Chart");

    // Connect view actions to switch views
    connect(mainViewAction, &QAction::triggered, [this]() {
        if (centralWidget) {
            centralWidget->setCurrentView(0); // Main View
        }
    });

    connect(statisticsViewAction, &QAction::triggered, [this]() {
        if (centralWidget) {
            centralWidget->setCurrentView(1); // Statistics
        }
    });

    connect(dailyChartViewAction, &QAction::triggered, [this]() {
        if (centralWidget) {
            centralWidget->setCurrentView(2); // Daily Chart
            centralWidget->onDailyChartTabSelected(); // Notify the chart widget
        }
    });
}