#pragma once
#include <QMainWindow>
#include "components/status_bar/AbstractStatusBar.h"
#include "components/central_widget/AbstractCentralWidget.h"

class BaseWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit BaseWindow(AbstractStatusBar* statusBar, AbstractCentralWidget* centralWidget, QWidget* parent = nullptr)
            : QMainWindow(parent),
              statusBar(statusBar),
              centralWidget(centralWidget)
        {
            this->statusBar->init();
            setStatusBar(this->statusBar);

            this->centralWidget->init();
            setCentralWidget(this->centralWidget);
        }
        virtual ~BaseWindow() override = default;

    protected:
        AbstractStatusBar* statusBar;
        AbstractCentralWidget* centralWidget;
        virtual void setupUI() = 0;
};