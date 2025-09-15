#pragma once
#include "components/central_widget/AbstractCentralWidget.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include "states/main_window_state/MainWindowState.h"

class MainCentralWidget : public AbstractCentralWidget {
    Q_OBJECT

    public:
        explicit MainCentralWidget(QWidget* parent) : AbstractCentralWidget(parent) {
            windowState = new MainWindowState(this);
        }
    protected:
        void setupUIImpl() override;

    private:
        QLabel* label1;
        QLabel* label2;
        QVBoxLayout* mainLayout;
        QHBoxLayout* statsLayout;
        QHBoxLayout* controlsLayout;
        MainWindowState* windowState;
        QPushButton* startButton;
        QPushButton* pauseButton;
        QPushButton* resumeButton;
        QPushButton* stopButton;
        void updateButtonStates(MainWindowState::TimerStatus status);
};