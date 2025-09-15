#pragma once
#include <QMainWindow>
#include "components/status_bar/AbstractStatusBar.h"
#include "components/central_widget/AbstractCentralWidget.h"
#include "components/app_menu/AbstractMenu.h"

class BaseWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit BaseWindow(AbstractStatusBar* statusBar, AbstractCentralWidget* centralWidget, AbstractMenu* menu, QWidget* parent = nullptr)
            : QMainWindow(parent),
              statusBar(statusBar),
              centralWidget(centralWidget),
              menu(menu)
        {
            this->statusBar->init();
            setStatusBar(this->statusBar);

            this->centralWidget->init();
            setCentralWidget(this->centralWidget);

            this->menu->init();
            setMenuWidget(this->menu);
        }
        virtual ~BaseWindow() override = default;

    protected:
        AbstractStatusBar* statusBar;
        AbstractCentralWidget* centralWidget;
        AbstractMenu* menu;
        virtual void setupUI() = 0;
};