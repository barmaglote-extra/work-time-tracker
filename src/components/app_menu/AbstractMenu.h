#pragma once
#include <QMenuBar>
#include <QWidget>

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
