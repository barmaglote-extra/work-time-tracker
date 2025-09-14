#pragma once
#include "components/central_widget/AbstractCentralWidget.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

class MainCentralWidget : public AbstractCentralWidget {
    Q_OBJECT

    public:
        explicit MainCentralWidget(QWidget* parent) : AbstractCentralWidget(parent) {
            setupUIImpl();
        }
    protected:
        void setupUIImpl() override;

    private:
        QLabel* label1;
        QLabel* label2;
        QVBoxLayout* mainLayout;
        QHBoxLayout* statsLayout;
        QHBoxLayout* controlsLayout;
};