#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QString>
#include <QHeaderView>
#include "states/main_window_state/MainWindowState.h"
#include "models/TimerEvent.h"
#include "styles/StatsStyles.h"

struct StatsSummary {
    QMap<QString, QString> metrics;
    QVector<QVector<QString>> pauses;
};

class StatsWidget : public QWidget {
    Q_OBJECT
public:
    explicit StatsWidget(QWidget* parent = nullptr);
    void setState(MainWindowState* state);

private:
    QTableWidget* metricsTable;
    QTableWidget* pausesTable;
    QVBoxLayout* mainLayout;
    MainWindowState* windowState = nullptr;

    void updateTables();
    static StatsSummary calculateStats(MainWindowState* state);
    void setupUI();
    void applyStyles();
};