#pragma once
#include "windows/base_windows/BaseWindow.h"
#include "components/central_widget/MainCentralWidget.h"
#include "components/status_bar/DefaultStatusBar.h"
#include "components/app_menu/MainMenu.h"
#include "states/main_window_state/MainWindowState.h"

class MainWindow : public BaseWindow {
    Q_OBJECT
    public:
        explicit MainWindow(QWidget* parent = nullptr)
            : BaseWindow(
                new DefaultStatusBar(nullptr),
                new MainCentralWidget(nullptr),
                new MainMenu(nullptr),
                parent) {

            appState = new MainWindowState(this);
            appState->loadFromFile("state.json");

            auto central = dynamic_cast<MainCentralWidget*>(this->centralWidget);
            if (central) {
                central->setState(appState);
            }

            setupUI();
        }

        ~MainWindow() override = default;

    protected:
        void setupUI() override;
        void closeEvent(QCloseEvent* event) override;

    private:
        MainWindowState* appState;
};