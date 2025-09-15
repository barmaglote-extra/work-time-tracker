#pragma once
#include "components/app_menu/AbstractMenu.h"
#include <QMenu>
#include <QAction>

class MainMenu : public AbstractMenu {
    Q_OBJECT

public:
    explicit MainMenu(QWidget* parent = nullptr) : AbstractMenu(parent) {};

    protected:
        void setupUIImpl() override;
};
