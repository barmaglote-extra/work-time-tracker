#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QChart>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QAbstractAxis>
#include "states/main_window_state/MainWindowState.h"

class DailyChartWidget : public QWidget {
    Q_OBJECT

public:
    explicit DailyChartWidget(QWidget* parent = nullptr);
    void setState(MainWindowState* state);
    void onTabSelected(); // Called when the tab is selected

protected:
    void showEvent(QShowEvent* event) override;

private:
    QVBoxLayout* mainLayout;
    QChartView* chartView;
    QChart* chart;
    MainWindowState* windowState = nullptr;
    bool needsUpdate = true; // Flag to track if update is needed

    void setupUI();
    void updateChart();
};