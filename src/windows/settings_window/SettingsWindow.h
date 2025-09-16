#pragma once
#include <QWidget>
#include <QMap>
#include <QSpinBox>
#include <QJsonObject>
#include <QTabWidget>
#include <QPushButton>
#include <QTimeEdit>
#include "states/main_window_state/MainWindowState.h"

class SettingsWindow : public QWidget {
    Q_OBJECT
public:
    explicit SettingsWindow(MainWindowState* state, QWidget* parent = nullptr);

private:
    MainWindowState* windowState;

    QTabWidget* tabs;
    QWidget* workTab;
    QPushButton* saveButton;
    QPushButton* cancelButton;

    QMap<QString, QTimeEdit*> workTimeEdits;
    QMap<QString, QTimeEdit*> breakTimeEdits;

    void setupUI();
    void loadSettings();
    void saveSettings();
};
