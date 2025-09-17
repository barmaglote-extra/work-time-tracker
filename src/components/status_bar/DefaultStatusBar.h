#pragma once
#include "AbstractStatusBar.h"
#include "states/main_window_state/MainWindowState.h"
#include <QLabel>

class DefaultStatusBar : public AbstractStatusBar {
    Q_OBJECT

    public:
        explicit DefaultStatusBar(QWidget* parent) : AbstractStatusBar(parent) {};
        void setState(MainWindowState* state);

    protected:
        void setupUIImpl() override;
        void updateStatus();

    private:
        QLabel* label;
        MainWindowState* windowState;
};