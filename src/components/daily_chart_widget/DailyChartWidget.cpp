#include "DailyChartWidget.h"
#include <QDateTime>
#include <QDate>
#include <QDebug>
#include <QShowEvent>

DailyChartWidget::DailyChartWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
}

void DailyChartWidget::setupUI() {
    mainLayout = new QVBoxLayout(this);

    // Create chart
    chart = new QChart();
    chart->setTitle("Daily Work Progress");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Create chart view
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    mainLayout->addWidget(chartView);
    setLayout(mainLayout);
    updateChart();
}

void DailyChartWidget::setState(MainWindowState* state) {
    windowState = state;
    if (!windowState) return;

    // Connect to state changes but don't update immediately
    connect(windowState, &MainWindowState::timerStatusChanged, this, [this]() {
        needsUpdate = true;
        if (isVisible()) {
            updateChart();
        }
    });

}

void DailyChartWidget::onTabSelected() {
    if (needsUpdate && windowState) {
        updateChart();
        needsUpdate = false;
    }
}

void DailyChartWidget::updateChart() {
    if (!windowState) return;

    // Get daily durations
    QMap<QDate, int> dailyDurations = windowState->getDailyWorkDurations();

    // Create bar series
    QBarSeries *series = new QBarSeries();

    // Create bar sets
    QBarSet *setCompleted = new QBarSet("Completed");
    QBarSet *setRequired = new QBarSet("Required");

    // Prepare data for chart
    QStringList categories;

    // Get the last 7 days
    QDate currentDate = QDate::currentDate();
    for (int i = 6; i >= 0; i--) {
        QDate date = currentDate.addDays(-i);
        QString dayName = date.toString("ddd dd.MM");
        categories << dayName;

        // Get required work time for this day of week (1=Monday, 7=Sunday)
        int dayOfWeek = date.dayOfWeek();
        int requiredSeconds = windowState->getWorkSecondsPerDay().value(dayOfWeek, 8 * 3600);
        int minBreakSeconds = windowState->getMinBreakSecondsPerDay().value(dayOfWeek, 0);
        int requiredTotalSeconds = requiredSeconds + minBreakSeconds;

        // Get actual work time for this date
        int actualSeconds = dailyDurations.value(date, 0);

        // Add data to sets
        *setCompleted << (actualSeconds / 3600.0); // Convert to hours
        *setRequired << (requiredTotalSeconds / 3600.0); // Convert to hours
    }

    // Add sets to series
    series->append(setCompleted);
    series->append(setRequired);

    // Create a new chart
    QChart *newChart = new QChart();
    newChart->setTitle("Daily Work Progress (Last 7 Days)");
    newChart->setAnimationOptions(QChart::SeriesAnimations);

    // Add series to chart
    newChart->addSeries(series);

    // Create axes
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    newChart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Hours");
    axisY->setLabelFormat("%.1f");
    newChart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Create legend
    newChart->legend()->setVisible(true);
    newChart->legend()->setAlignment(Qt::AlignBottom);

    // Replace the chart in the view
    chartView->setChart(newChart);

    // Clean up the old chart
    if (chart && chart != newChart) {
        // Clear series and axes from the old chart before deleting
        chart->removeAllSeries();
        QList<QAbstractAxis*> axes = chart->axes();
        for (QAbstractAxis* axis : axes) {
            chart->removeAxis(axis);
        }
        delete chart;
    }

    // Update the chart pointer
    chart = newChart;
}