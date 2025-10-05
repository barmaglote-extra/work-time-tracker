#include "FinishTimeWidget.h"
#include "styles/FinishTimeStyles.h"
#include <QPixmap>
#include <QDateTime>
#include <QLineEdit>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QApplication>
#include <QDebug>

FinishTimeWidget::FinishTimeWidget(QWidget* parent) : QWidget(parent) {
    startLabel = new QLabel("Start at: --:--", this);
    finishLabel = new QLabel("Free at: --:--", this);

    startLabel->setStyleSheet(FinishTimeStyles::labelStyle());
    finishLabel->setStyleSheet(FinishTimeStyles::labelStyle());

    finishTimeEdit = new QLineEdit(this);
    finishTimeEdit->setStyleSheet(FinishTimeStyles::lineEditStyle());
    finishTimeEdit->hide();

    startLabel->setCursor(Qt::PointingHandCursor);

    startTimeEdit = new QLineEdit(this);
    startTimeEdit->setStyleSheet(FinishTimeStyles::lineEditStyle());
    startTimeEdit->hide();

    connect(startTimeEdit, &QLineEdit::editingFinished, this, &FinishTimeWidget::onStartTimeEditFinished);
    connect(finishTimeEdit, &QLineEdit::editingFinished, this, &FinishTimeWidget::onFinishTimeEditFinished);

    auto layout = new QHBoxLayout(this);

    QLabel* startIcon = new QLabel(this);
    startIcon->setPixmap(QIcon(":/res/resources/icons/clock.svg").pixmap(24, 24));
    startIcon->setStyleSheet(FinishTimeStyles::iconStyle());

    QLabel* finishIcon = new QLabel(this);
    finishIcon->setPixmap(QIcon(":/res/resources/icons/clock.svg").pixmap(24, 24));
    finishIcon->setStyleSheet(FinishTimeStyles::iconStyle());

    middleIcon = new QLabel(this);  // Store reference to middle icon
    middleIcon->setPixmap(QIcon(":/res/resources/icons/app_icon.svg").pixmap(64, 64));
    middleIcon->setAlignment(Qt::AlignCenter);
    middleIcon->setStyleSheet(FinishTimeStyles::iconStyle());

    layout->addWidget(startIcon);
    layout->addWidget(startLabel);
    layout->addWidget(startTimeEdit);
    layout->addStretch(1);
    layout->addWidget(middleIcon);
    layout->addStretch(1);
    layout->addWidget(finishIcon);
    layout->addWidget(finishLabel);
    layout->addWidget(finishTimeEdit);

    setLayout(layout);

    startLabel->installEventFilter(this);
    finishLabel->installEventFilter(this);
}

void FinishTimeWidget::setState(MainWindowState* state) {
    windowState = state;
    if (!windowState) return;

    connect(windowState, &MainWindowState::finishTimeChanged,
            this, &FinishTimeWidget::setFinishTime);

    connect(windowState, &MainWindowState::timerValueChanged, [this]() {
        if (!windowState) return;
        setStartTime(windowState->getStartTime());
    });

    // Connect to timer status changes to update the middle icon
    connect(windowState, &MainWindowState::timerStatusChanged,
            this, &FinishTimeWidget::onTimerStatusChanged);

    setStartTime(windowState->getStartTime());
    setFinishTime(windowState->getStatus() == TimerEvent::Stop ? windowState->getFinishTime() : windowState->calculateFinishTime());
    
    // Set initial icon based on current status
    updateMiddleIcon(windowState->getStatus());
}

void FinishTimeWidget::setStartTime(const QTime& startTime) {
    currentStartTime = startTime;
    startLabel->setText("Start at: " + startTime.toString("HH:mm"));
}

void FinishTimeWidget::setFinishTime(const QTime& finishTime) {
    finishLabel->setText("Free at: " + finishTime.toString("HH:mm"));
}

void FinishTimeWidget::showStartTimeEdit() {
    startTimeEdit->setText(currentStartTime.toString("HH:mm"));
    startLabel->hide();
    startTimeEdit->show();
    startTimeEdit->setFocus();
    startTimeEdit->selectAll();
}

void FinishTimeWidget::showFinishTimeEdit() {
    finishTimeEdit->setText(currentFinishTime.toString("HH:mm"));
    finishLabel->hide();
    finishTimeEdit->show();
    finishTimeEdit->setFocus();
    finishTimeEdit->selectAll();
}

void FinishTimeWidget::hideStartTimeEdit() {
    startTimeEdit->hide();
    startLabel->show();
}

void FinishTimeWidget::hideFinishTimeEdit() {
    finishTimeEdit->hide();
    finishLabel->show();
}

void FinishTimeWidget::onStartLabelClicked() {
    if (windowState) {
        showStartTimeEdit();
    }
}

void FinishTimeWidget::onFinishLabelClicked() {
    if (windowState && windowState->getStatus() == MainWindowState::TimerStatus::Stopped) {
        showFinishTimeEdit();
    }
}

void FinishTimeWidget::onStartTimeEditFinished() {
    QString timeText = startTimeEdit->text();
    QTime newTime = QTime::fromString(timeText, "HH:mm");

    if (newTime.isValid() && windowState) {
        windowState->updateStartTime(newTime);

        setStartTime(newTime);
    }

    hideStartTimeEdit();
}

void FinishTimeWidget::onFinishTimeEditFinished() {
    QString timeText = finishTimeEdit->text();
    QTime newTime = QTime::fromString(timeText, "HH:mm");

    if (newTime.isValid() && windowState) {
        windowState->updateFinishTime(newTime);

        setFinishTime(newTime);
    }

    hideFinishTimeEdit();
}

void FinishTimeWidget::onTimerStatusChanged(MainWindowState::TimerStatus status) {
    updateMiddleIcon(status);
}

void FinishTimeWidget::updateMiddleIcon(MainWindowState::TimerStatus status) {
    if (status == MainWindowState::TimerStatus::Paused) {
        middleIcon->setPixmap(QIcon(":/res/resources/icons/app_icon_pause.svg").pixmap(64, 64));
    } else {
        middleIcon->setPixmap(QIcon(":/res/resources/icons/app_icon.svg").pixmap(64, 64));
    }
}

bool FinishTimeWidget::eventFilter(QObject* obj, QEvent* event) {
    if (obj == startLabel) {
        if (event->type() == QEvent::MouseButtonRelease) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                onStartLabelClicked();
                return true;
            }
        } else if (event->type() == QEvent::MouseButtonPress) {
            return true;
        }
    }

    if (obj == finishLabel) {
        if (event->type() == QEvent::MouseButtonRelease) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                onFinishLabelClicked();
                return true;
            }
        } else if (event->type() == QEvent::MouseButtonPress) {
            return true;
        }
    }

    return QWidget::eventFilter(obj, event);
}