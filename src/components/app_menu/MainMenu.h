#pragma once
#include "components/app_menu/AbstractMenu.h"
#include <QMenu>
#include <QAction>
#include <QApplication>
#include "windows/settings_window/SettingsWindow.h"

class MainMenu : public AbstractMenu {
    Q_OBJECT

public:
    explicit MainMenu(QWidget* parent = nullptr) : AbstractMenu(parent), settingsWindow(nullptr) {};

    void setSettingsWindow(SettingsWindow* window) {
        settingsWindow = window;
    }

    protected:
        void setupUIImpl() override;

    private:
    SettingsWindow* settingsWindow;
};
