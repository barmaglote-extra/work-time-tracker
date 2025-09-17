#include "windows/about_window/AboutWindow.h"
#include <QVBoxLayout>
#include <QLabel>

AboutWindow::AboutWindow(MainWindowState* state, QWidget* parent) : QWidget(parent), windowState(state) {
    setupUI();
}

void AboutWindow::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
}


