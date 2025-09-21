#pragma once
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QIcon>
#include <QTime>
#include <QEvent>
#include <QObject>
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
        QLineEdit* startTimeEdit;
        bool isEditing = false;
        QTime currentStartTime;

        void setStartTime(const QTime& startTime);
        void setFinishTime(const QTime& finishTime);
        void showStartTimeEdit();
        void hideStartTimeEdit();

    protected:
        bool eventFilter(QObject* obj, QEvent* event) override;

    private slots:
        void onStartLabelClicked();
        void onStartTimeEditFinished();
};