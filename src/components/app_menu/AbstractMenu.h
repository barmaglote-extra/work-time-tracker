#pragma once
#include <QMenuBar>
#include <QWidget>
#include "states/main_window_state/MainWindowState.h"
#include "models/TimerEvent.h"

class AbstractMenu : public QMenuBar {
    Q_OBJECT

public:
    explicit AbstractMenu(QWidget* parent = nullptr) : QMenuBar(parent) { }
    virtual ~AbstractMenu() = default;

    void init() {
        configureBase();
        setupUIImpl();
    }

    protected:
        virtual void setupUIImpl() = 0;

    private:
        void configureBase();
};
