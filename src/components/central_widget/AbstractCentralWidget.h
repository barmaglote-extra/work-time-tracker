#pragma once
#include <QStatusBar>
#include <QWidget>

class AbstractCentralWidget : public QWidget {
    Q_OBJECT

    public:
        explicit AbstractCentralWidget(QWidget* parent) : QWidget(parent) {}
        virtual ~AbstractCentralWidget() = default;

        void init() {
            configureBase();
            setupUIImpl();
        }

        virtual void setupUIImpl() = 0;
    private:
        void configureBase();
};