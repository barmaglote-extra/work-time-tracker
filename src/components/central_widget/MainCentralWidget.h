#pragma once
#include "components/central_widget/AbstractCentralWidget.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include "states/main_window_state/MainWindowState.h"
#include "components/timer_widget/TimerWidget.h"
#include "components/timer_chart/TimerChart.h"
#include "components/control_panel/ControlPanel.h"
#include "components/finish_time_widget/FinishTimeWidget.h"

class MainCentralWidget : public AbstractCentralWidget {
    Q_OBJECT

    public:
        explicit MainCentralWidget(QWidget* parent) : AbstractCentralWidget(parent) {}
        void setState(MainWindowState* state);

    protected:
        void setupUIImpl() override;

    private:
        QVBoxLayout* mainLayout;
        QVBoxLayout* statsLayout;
        QHBoxLayout* controlsLayout;
        MainWindowState* windowState;
        FinishTimeWidget* finishWidget;
        ControlPanel* controlPanel;
        QPushButton* startButton;
        QPushButton* pauseButton;
        QPushButton* resumeButton;
        QPushButton* stopButton;
        void updateButtonStates(MainWindowState::TimerStatus status);
};