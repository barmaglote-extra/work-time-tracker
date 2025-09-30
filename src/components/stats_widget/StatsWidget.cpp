#include "StatsWidget.h"
#include <QScrollBar>
#include <QDateTime>
#include <QTime>
#include <QStringList>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include "utils/TimeCalculator.h"

StatsWidget::StatsWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
    applyStyles();
}

void StatsWidget::setState(MainWindowState* state) {
    windowState = state;
    if (!windowState) return;

    connect(windowState, &MainWindowState::timerValueChanged, this, &StatsWidget::updateTables);
    connect(windowState, &MainWindowState::timerStatusChanged, this, &StatsWidget::updateTables);
    connect(windowState, &MainWindowState::finishTimeChanged, this, &StatsWidget::updateTables);

    updateTables();
}

void StatsWidget::setupUI() {
    mainLayout = new QVBoxLayout(this);

    QHBoxLayout* tablesLayout = new QHBoxLayout();

    metricsTable = new QTableWidget(this);
    metricsTable->setColumnCount(2);
    metricsTable->setHorizontalHeaderLabels({"Metric", "Value"});
    metricsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    metricsTable->verticalHeader()->setVisible(false);
    metricsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    metricsTable->setSelectionMode(QAbstractItemView::NoSelection);

    pausesTable = new QTableWidget(this);
    pausesTable->setColumnCount(3);
    pausesTable->setHorizontalHeaderLabels({"Start", "End", "Duration"});
    pausesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    pausesTable->verticalHeader()->setVisible(false);
    pausesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // Enable selection for deletion
    pausesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    pausesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    pausesTable->setContextMenuPolicy(Qt::CustomContextMenu);
    pausesTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Connect context menu signal
    connect(pausesTable, &QTableWidget::customContextMenuRequested, this, &StatsWidget::showPauseContextMenu);

    tablesLayout->addWidget(metricsTable);
    tablesLayout->addWidget(pausesTable);

    mainLayout->addLayout(tablesLayout);
    setLayout(mainLayout);
}

void StatsWidget::applyStyles() {
    metricsTable->setStyleSheet(StatsStyles::getTablesStyle());
    pausesTable->setStyleSheet(StatsStyles::getTablesStyle());
}

StatsSummary StatsWidget::calculateStats(MainWindowState* state) {
    StatsSummary summary;
    if (!state) return summary;

    const auto& allEvents = state->getTimerEvents();
    int totalWork = state->getTotalSeconds();

    // Filter events to only include those from the current day
    QDate today = QDate::currentDate();
    QVector<TimerEvent> todayEvents;
    for (const auto& event : allEvents) {
        if (event.timestamp.date() == today) {
            todayEvents.append(event);
        }
    }

    // If no events today, return default values
    if (todayEvents.isEmpty()) {
        QTime now = QTime::currentTime();
        summary.metrics["Check-In"] = "00:00";
        summary.metrics["Current-Time"] = now.toString("HH:mm");
        summary.metrics["Hour Per day"] = QString("%1:%2").arg(totalWork/3600,2,10,QChar('0')).arg((totalWork%3600)/60,2,10,QChar('0'));
        summary.metrics["Pauses"] = "00:00";
        summary.metrics["Min Pauses"] = "00:00";
        summary.metrics["Lack Pauses"] = "00:00";
        summary.metrics["Free at"] = "00:00";
        summary.metrics["Left/Over"] = QString("%1:%2").arg(totalWork/3600,2,10,QChar('0')).arg((totalWork%3600)/60,2,10,QChar('0'));
        // Return early with empty pauses
        return summary;
    }

    // Check if timer is stopped to use the correct time reference
    bool isTimerStopped = (state->getStatus() == MainWindowState::Stopped);

    // Use TimeCalculator utility functions with today's events only
    QDateTime firstStart = TimeCalculator::findFirstStartTime(todayEvents);

    // For stopped timer, we should use a fixed time reference
    QDateTime referenceTime;
    if (isTimerStopped && !todayEvents.isEmpty()) {
        // Use the time of the last event as reference time when timer is stopped
        referenceTime = todayEvents.last().timestamp;
    } else {
        // Use current time when timer is running
        referenceTime = QDateTime::currentDateTime();
    }

    int totalPauseSecs = TimeCalculator::calculateTotalPauseSeconds(todayEvents, referenceTime);

    QTime now = QTime::currentTime();
    int todayOfWeek = today.dayOfWeek();
    int minBreak = state->getMinBreakSecondsPerDay().value(todayOfWeek, 0);
    int lackBreak = qMax(0, minBreak - totalPauseSecs);

    // For Left/Over calculation, use the correct reference
    int workedSecs;
    if (isTimerStopped && !todayEvents.isEmpty()) {
        // Calculate worked seconds based on the last event time
        workedSecs = firstStart.secsTo(referenceTime) - totalPauseSecs;
    } else {
        // Use the current timer value
        workedSecs = state->getValue();
    }

    int leftOverSecs = totalWork - workedSecs + lackBreak;

    QTime finishTime = TimeCalculator::calculateFinishTime(
        todayEvents,
        totalWork,
        minBreak,
        referenceTime
    );

    // Store metrics in a specific order by manually controlling the iteration order in updateTables
    summary.metrics["Check-In"] = firstStart.time().toString("HH:mm");
    summary.metrics["Current-Time"] = now.toString("HH:mm");
    summary.metrics["Hour Per day"] = QString("%1:%2").arg(totalWork/3600,2,10,QChar('0')).arg((totalWork%3600)/60,2,10,QChar('0'));
    summary.metrics["Pauses"] = QString("%1:%2").arg(totalPauseSecs/3600,2,10,QChar('0')).arg((totalPauseSecs%3600)/60,2,10,QChar('0'));
    summary.metrics["Min Pauses"] = QString("%1:%2").arg(minBreak/3600,2,10,QChar('0')).arg((minBreak%3600)/60,2,10,QChar('0'));
    summary.metrics["Lack Pauses"] = QString("%1:%2").arg(lackBreak/3600,2,10,QChar('0')).arg((lackBreak%3600)/60,2,10,QChar('0'));
    summary.metrics["Free at"] = finishTime.toString("HH:mm");
    summary.metrics["Left/Over"] = QString("%1:%2").arg(leftOverSecs/3600,2,10,QChar('0')).arg((leftOverSecs%3600)/60,2,10,QChar('0'));

    // Pauses - show historical pauses for today only
    QDateTime pauseStart;
    for (const auto& e : todayEvents) {
        if (e.type == TimerEvent::Pause) {
            pauseStart = e.timestamp;
        } else if (e.type == TimerEvent::Resume && pauseStart.isValid()) {
            int dur = pauseStart.secsTo(e.timestamp);
            summary.pauses.push_back({
                pauseStart.time().toString("HH:mm"),
                e.timestamp.time().toString("HH:mm"),
                QString("%1:%2").arg(dur / 3600,2,10,QChar('0')).arg((dur % 3600)/60,2,10,QChar('0'))
            });
            pauseStart = QDateTime();
        }
    }

    // Only show ongoing pause if timer is running
    if (pauseStart.isValid() && !isTimerStopped) {
        int dur = pauseStart.secsTo(QDateTime::currentDateTime());
        summary.pauses.push_back({
            pauseStart.time().toString("HH:mm"),
            QTime::currentTime().toString("HH:mm"),
            QString("%1:%2").arg(dur / 3600,2,10,QChar('0')).arg((dur % 3600)/60,2,10,QChar('0'))
        });
    }

    return summary;
}

void StatsWidget::updateTables() {
    if (!windowState) return;
    if (windowState->getTotalSeconds() < 0) return;

    StatsSummary summary = calculateStats(windowState);

    QStringList metricOrder = {
        "Check-In",
        "Current-Time",
        "Hour Per day",
        "Pauses",
        "Min Pauses",
        "Lack Pauses",
        "Free at",
        "Left/Over"
    };

    metricsTable->setRowCount(metricOrder.size());
    for (int i = 0; i < metricOrder.size(); ++i) {
        QString key = metricOrder[i];
        if (summary.metrics.contains(key)) {
            metricsTable->setItem(i, 0, new QTableWidgetItem(key));
            metricsTable->setItem(i, 1, new QTableWidgetItem(summary.metrics[key]));
        }
    }

    if (windowState->getTotalSeconds() > 0) {
        pausesTable->setRowCount(summary.pauses.size());
        for (int i = 0; i < summary.pauses.size(); ++i) {
            for (int j = 0; j < 3; ++j) {
                pausesTable->setItem(i, j, new QTableWidgetItem(summary.pauses[i][j]));
            }
        }

        if (!summary.pauses.empty()) {
            pausesTable->scrollToItem(pausesTable->item(summary.pauses.size() - 1, 0));
        }
    } else {
        pausesTable->setRowCount(0);
    }
}

void StatsWidget::showPauseContextMenu(const QPoint& pos) {
    if (!windowState) return;

    QTableWidgetItem* item = pausesTable->itemAt(pos);
    if (!item) return;

    int row = item->row();

    QMenu contextMenu("Context menu", this);

    QAction* deleteAction = new QAction("Delete Pause", this);
    connect(deleteAction, &QAction::triggered, [this, row]() {
        deletePause(row);
    });

    contextMenu.addAction(deleteAction);
    contextMenu.exec(pausesTable->viewport()->mapToGlobal(pos));
}

void StatsWidget::deletePause(int row) {
    if (!windowState) return;

    // Show confirmation dialog
    QMessageBox msgBox;
    msgBox.setWindowTitle("Confirm Deletion");
    msgBox.setText("Are you sure you want to delete this pause?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);

    if (msgBox.exec() == QMessageBox::Ok) {
        // Remove the pause/resume pair from the state
        if (windowState->removePauseResumePair(row)) {
            // Update the tables to reflect the changes
            updateTables();
        } else {
            QMessageBox::warning(this, "Error", "Failed to delete pause.");
        }
    }
}