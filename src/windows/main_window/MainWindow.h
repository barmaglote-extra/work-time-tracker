#pragma once
#include "windows/base_windows/BaseWindow.h"
#include "components\central_widget\MainCentralWidget.h"
#include "components\status_bar\DefaultStatusBar.h"

class MainWindow : public BaseWindow {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget* parent = nullptr): BaseWindow(
          new DefaultStatusBar(nullptr),
          new MainCentralWidget(nullptr),
          parent) {
          setupUI();
        }

        ~MainWindow() override = default;

    protected:
        void setupUI() override;
};