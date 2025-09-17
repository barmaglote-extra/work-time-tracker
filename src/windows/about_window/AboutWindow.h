#pragma once
#include <QWidget>
#include <QPushButton>
#include "states/main_window_state/MainWindowState.h"

class AboutWindow : public QWidget {
    Q_OBJECT
    public:
        explicit AboutWindow(MainWindowState* state, QWidget* parent = nullptr);

    private:
        MainWindowState* windowState;
        QPushButton* okButton;
        void setupUI();
};
