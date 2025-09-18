#pragma once
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QIcon>
#include "states/main_window_state/MainWindowState.h"
#include "styles/FinishTimeStyles.h"

class FinishTimeWidget : public QWidget {
    Q_OBJECT

    public:
        explicit FinishTimeWidget(QWidget* parent = nullptr);
        void setState(MainWindowState* state);

    private:
        MainWindowState* windowState = nullptr;
        QLabel* finishLabel;
        QLabel* startLabel;
        void setStartTime(const QTime& startTime);
        void setFinishTime(const QTime& finishTime);
};
