#pragma once
#include "AbstractMenu.h"
#include <QMenu>
#include <QAction>
#include <QApplication>
#include "windows/settings_window/SettingsWindow.h"
#include "components/central_widget/MainCentralWidget.h"

class MainMenu : public AbstractMenu {
    Q_OBJECT

public:
    explicit MainMenu(QWidget* parent = nullptr) : AbstractMenu(parent), settingsWindow(nullptr), centralWidget(nullptr) {};

    void setSettingsWindow(SettingsWindow* window) {
        settingsWindow = window;
    }

    void setCentralWidget(MainCentralWidget* widget) {
        centralWidget = widget;
    }

protected:
    void setupUIImpl() override;

private:
    SettingsWindow* settingsWindow;
    MainCentralWidget* centralWidget;
};