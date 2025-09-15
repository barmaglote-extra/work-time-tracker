#include "MainWindow.h"

void MainWindow::setupUI() {
    // Setup any additional UI elements specific to MainWindow
    setWindowTitle("Work Day Tracker");
}

void MainWindow::closeEvent(QCloseEvent* event) {
    if (appState) {
        appState->saveToFile("state.json");
    }
    BaseWindow::closeEvent(event);
}