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

    QMap<QDate, int> dailyDurations = windowState->getDailyWorkDurations();

    QBarSeries *series = new QBarSeries();
    QBarSet *setDifference = new QBarSet("Difference");

    QStringList categories;

    QDate currentDate = QDate::currentDate();
    for (int i = 6; i >= 0; i--) {
        if (i == 0 && windowState->getStatus() != MainWindowState::TimerStatus::Stopped) {
            continue;
        }

        QDate date = currentDate.addDays(-i);
        QString dayName = date.toString("ddd dd.MM");
        categories << dayName;

        int dayOfWeek = date.dayOfWeek();
        int requiredSeconds = windowState->getWorkSecondsPerDay().value(dayOfWeek, 8 * 3600);
        int minBreakSeconds = windowState->getMinBreakSecondsPerDay().value(dayOfWeek, 0);
        int requiredTotalSeconds = requiredSeconds + minBreakSeconds;

        int actualSeconds = dailyDurations.value(date, 0);

        int diffMinutes = (actualSeconds - requiredTotalSeconds) / 60;

        *setDifference << diffMinutes;
    }

    series->append(setDifference);

    QChart *newChart = new QChart();
    newChart->setTitle("Daily Work Difference (Minutes)");
    newChart->setAnimationOptions(QChart::SeriesAnimations);
    newChart->addSeries(series);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    newChart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Minutes");
    axisY->setLabelFormat("%d");
    newChart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    newChart->legend()->setVisible(true);
    newChart->legend()->setAlignment(Qt::AlignBottom);

    for (int i = 0; i < setDifference->count(); ++i) {
        if ((*setDifference)[i] >= 0)
            setDifference->setColor(QColor(Qt::green));
        else
            setDifference->setColor(QColor(Qt::red));
    }

    chartView->setChart(newChart);

    if (chart && chart != newChart) {
        chart->removeAllSeries();
        QList<QAbstractAxis*> axes = chart->axes();
        for (QAbstractAxis* axis : axes)
            chart->removeAxis(axis);
        delete chart;
    }

    chart = newChart;
}
