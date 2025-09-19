#pragma once
#include "components/central_widget/AbstractCentralWidget.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QComboBox>
#include <QWidget>
#include <QStackedWidget>
#include "states/main_window_state/MainWindowState.h"
#include "components/timer_widget/TimerWidget.h"
#include "components/timer_chart/TimerChart.h"
#include "components/control_panel/ControlPanel.h"
#include "components/finish_time_widget/FinishTimeWidget.h"
#include "components/stats_widget/StatsWidget.h"
#include "components/daily_chart_widget/DailyChartWidget.h"

class MainCentralWidget : public AbstractCentralWidget {
    Q_OBJECT

    public:
        explicit MainCentralWidget(QWidget* parent) : AbstractCentralWidget(parent) {}
        void setState(MainWindowState* state);
        void setCurrentView(int index);
        void onDailyChartTabSelected(); // Called when daily chart tab is selected

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
        QStackedWidget* stackedViews;
        QWidget* mainViewWidget;
        StatsWidget* statsWidget;
        DailyChartWidget* dailyChartWidget;
        void updateButtonStates(MainWindowState::TimerStatus status);
};