#pragma once
#include <QStatusBar>

class AbstractStatusBar : public QStatusBar {
    Q_OBJECT

    public:
        explicit AbstractStatusBar(QWidget* parent) : QStatusBar(parent) { }
        virtual ~AbstractStatusBar() = default;

        void init() {
            configureBase();
            setupUIImpl();
        }

    protected:
        virtual void setupUIImpl() = 0;

    private:
        void configureBase();
};