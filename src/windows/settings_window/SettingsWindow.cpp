#include "windows/settings_window/SettingsWindow.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QFile>
#include <QJsonDocument>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>

SettingsWindow::SettingsWindow(MainWindowState* state, QWidget* parent)
    : QWidget(parent), windowState(state)
{
    setupUI();
    loadSettings();
}

void SettingsWindow::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    this->setStyleSheet(SettingsStyles::windowStyle());

    tabs = new QTabWidget(this);
    workTab = new QWidget(this);
    tabs->addTab(workTab, "Work Settings");

    QGridLayout* gridLayout = new QGridLayout(workTab);

    QStringList days = {"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};

    int col = 0;
    for (const QString& day : days) {
        QLabel* dayLabel = new QLabel(day);
        dayLabel->setAlignment(Qt::AlignCenter);
        dayLabel->setStyleSheet(SettingsStyles::dayLabelStyle());
        gridLayout->addWidget(dayLabel, 0, col);

        QTimeEdit* workTime = new QTimeEdit(QTime(9,0));
        workTime->setDisplayFormat("hh:mm");
        workTime->setAlignment(Qt::AlignCenter);
        workTime->setStyleSheet(SettingsStyles::timeEditStyle());
        gridLayout->addWidget(workTime, 1, col);

        QTimeEdit* breakTime = new QTimeEdit(QTime(1,0));
        breakTime->setDisplayFormat("hh:mm");
        breakTime->setAlignment(Qt::AlignCenter);
        breakTime->setStyleSheet(SettingsStyles::timeEditStyle());
        gridLayout->addWidget(breakTime, 2, col);

        workTimeEdits[day] = workTime;
        breakTimeEdits[day] = breakTime;

        col++;
    }

    mainLayout->addWidget(tabs);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    saveButton = new QPushButton("Save");
    cancelButton = new QPushButton("Cancel");
    saveButton->setStyleSheet(SettingsStyles::buttonStyle());
    cancelButton->setStyleSheet(SettingsStyles::buttonStyle());

    buttonLayout->addStretch();
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonLayout);

    connect(saveButton, &QPushButton::clicked, this, &SettingsWindow::saveSettings);
    connect(cancelButton, &QPushButton::clicked, this, &SettingsWindow::close);
}

void SettingsWindow::loadSettings() {
    QFile file("settings.json");
    if (!file.open(QIODevice::ReadOnly)) return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject obj = doc.object();

    for (const QString& day : workTimeEdits.keys()) {
        if (obj.contains(day)) {
            QJsonObject dayObj = obj[day].toObject();

            int workSeconds = dayObj.value("workSeconds").toInt(9 * 3600);
            int workH = workSeconds / 3600;
            int workM = (workSeconds % 3600) / 60;
            workTimeEdits[day]->setTime(QTime(workH, workM));

            int breakSeconds = dayObj.value("breakSeconds").toInt(3600);
            int breakH = breakSeconds / 3600;
            int breakM = (breakSeconds % 3600) / 60;
            breakTimeEdits[day]->setTime(QTime(breakH, breakM));
        }
    }

    file.close();
}

void SettingsWindow::saveSettings() {
    QJsonObject json;

    for (const QString& day : workTimeEdits.keys()) {
        QJsonObject dayObj;

        int workSeconds = workTimeEdits[day]->time().hour() * 3600 +
                          workTimeEdits[day]->time().minute() * 60;
        int breakSeconds = breakTimeEdits[day]->time().hour() * 3600 +
                           breakTimeEdits[day]->time().minute() * 60;

        dayObj["workSeconds"] = workSeconds;
        dayObj["breakSeconds"] = breakSeconds;
        json[day] = dayObj;

        int dayIndex = QDate::fromString(day, "dddd").dayOfWeek(); // 1=Monday .. 7=Sunday
        dayObj["workSeconds"] = workSeconds;
        dayObj["breakSeconds"] = breakSeconds;
        json[QString::number(dayIndex)] = dayObj;
    }

    QFile file("settings.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(json).toJson());
        file.close();
    }

    QMessageBox::information(this, "Settings", "Settings saved successfully!");
    close();
}

