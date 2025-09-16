#pragma once
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QIcon>
#include "states/main_window_state/MainWindowState.h"

class FinishTimeWidget : public QWidget {
    Q_OBJECT

public:
    explicit FinishTimeWidget(QWidget* parent = nullptr);
    void setState(MainWindowState* state);

private slots:
    void onFinishTimeChanged(const QTime& time);

private:
    MainWindowState* windowState = nullptr;
    QLabel* finishlabel;
    QLabel* startLabel;
};
