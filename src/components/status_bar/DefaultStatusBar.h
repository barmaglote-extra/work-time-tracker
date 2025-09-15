#pragma once
#include "AbstractStatusBar.h"
#include <QLabel>

class DefaultStatusBar : public AbstractStatusBar {
    Q_OBJECT

    public:
        explicit DefaultStatusBar(QWidget* parent) : AbstractStatusBar(parent) {};

    protected:
        void setupUIImpl() override;

    private:
        QLabel* label;
};