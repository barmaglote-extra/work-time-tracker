#include "StatsWidget.h"
#include <QScrollBar>
#include <QDateTime>
#include <QTime>

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
    pausesTable->setSelectionMode(QAbstractItemView::NoSelection);
    pausesTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    tablesLayout->addWidget(metricsTable);
    tablesLayout->addWidget(pausesTable);

    mainLayout->addLayout(tablesLayout);
    setLayout(mainLayout);
}

void StatsWidget::applyStyles() {
    QString tableStyle = R"(
        QTableWidget {
            background-color: #fff;
            border: 1px solid #ccc;
            border-radius: 4px;
            gridline-color: #ddd;
        }
        QHeaderView::section {
            background-color: #f0f0f0;
            padding: 4px;
            border: none; /* убираем бордеры у секций */
        }
        QTableWidget::item {
            padding: 4px;
        }
        QTableWidget::item:selected {
            background-color: #d0e7ff;
        }
    )";
    metricsTable->setStyleSheet(tableStyle);
    pausesTable->setStyleSheet(tableStyle);
}

StatsSummary StatsWidget::calculateStats(MainWindowState* state) {
    StatsSummary summary;
    if (!state) return summary;

    const auto& events = state->getTimerEvents();
    if (events.isEmpty()) return summary;

    // Check-In
    QDateTime startTime;
    for (const auto& e : events) {
        if (e.type == TimerEvent::Start) {
            startTime = e.timestamp;
            break;
        }
    }

    // Pauses
    int totalPauseSecs = 0;
    QDateTime pauseStart;
    for (const auto& e : events) {
        if (e.type == TimerEvent::Pause) {
            pauseStart = e.timestamp;
        } else if (e.type == TimerEvent::Resume && pauseStart.isValid()) {
            int dur = pauseStart.secsTo(e.timestamp);
            totalPauseSecs += dur;
            summary.pauses.push_back({
                pauseStart.time().toString("HH:mm"),
                e.timestamp.time().toString("HH:mm"),
                QString("%1:%2").arg(dur / 3600,2,10,QChar('0')).arg((dur % 3600)/60,2,10,QChar('0'))
            });
            pauseStart = QDateTime();
        }
    }
    if (pauseStart.isValid()) { // незавершенная пауза
        int dur = pauseStart.secsTo(QDateTime::currentDateTime());
        totalPauseSecs += dur;
        summary.pauses.push_back({
            pauseStart.time().toString("HH:mm"),
            QTime::currentTime().toString("HH:mm"),
            QString("%1:%2").arg(dur / 3600,2,10,QChar('0')).arg((dur % 3600)/60,2,10,QChar('0'))
        });
    }

    // Metrics
    QTime now = QTime::currentTime();
    int today = QDate::currentDate().dayOfWeek();
    int totalWork = state->getTotalSeconds();
    int minBreak = state->getMinBreakSecondsPerDay().value(today, 0);
    int lackBreak = qMax(0, minBreak - totalPauseSecs);
    int workedSecs = state->getValue();
    int leftOverSecs = totalWork - workedSecs;

    summary.metrics["Check-In"] = startTime.time().toString("HH:mm");
    summary.metrics["Current-Time"] = now.toString("HH:mm");
    summary.metrics["Hour Per day"] = QString("%1:%2").arg(totalWork/3600,2,10,QChar('0')).arg((totalWork%3600)/60,2,10,QChar('0'));
    summary.metrics["Pauses"] = QString("%1:%2").arg(totalPauseSecs/3600,2,10,QChar('0')).arg((totalPauseSecs%3600)/60,2,10,QChar('0'));
    summary.metrics["Min Pauses"] = QString("%1:%2").arg(minBreak/3600,2,10,QChar('0')).arg((minBreak%3600)/60,2,10,QChar('0'));
    summary.metrics["Lack Pauses"] = QString("%1:%2").arg(lackBreak/3600,2,10,QChar('0')).arg((lackBreak%3600)/60,2,10,QChar('0'));
    summary.metrics["Free at"] = state->calculateFinishTime().toString("HH:mm");
    summary.metrics["Left/Over"] = QString("%1:%2").arg(leftOverSecs/3600,2,10,QChar('0')).arg((leftOverSecs%3600)/60,2,10,QChar('0'));

    return summary;
}

void StatsWidget::updateTables() {
    if (!windowState) return;
    if (windowState->getTotalSeconds() < 0) return;

    StatsSummary summary = calculateStats(windowState);

    metricsTable->setRowCount(summary.metrics.size());
    int row = 0;
    for (auto it = summary.metrics.begin(); it != summary.metrics.end(); ++it, ++row) {
        metricsTable->setItem(row, 0, new QTableWidgetItem(it.key()));
        metricsTable->setItem(row, 1, new QTableWidgetItem(it.value()));
    }

    pausesTable->setRowCount(summary.pauses.size());
    for (int i = 0; i < summary.pauses.size(); ++i) {
        for (int j = 0; j < 3; ++j) {
            pausesTable->setItem(i, j, new QTableWidgetItem(summary.pauses[i][j]));
        }
    }

    if (!summary.pauses.empty()) {
        pausesTable->scrollToItem(pausesTable->item(summary.pauses.size() - 1, 0));
    }
}
