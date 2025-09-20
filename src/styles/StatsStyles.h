#pragma once
#include <QString>

namespace StatsStyles {
    inline QString getTablesStyle() {
        return QString(R"(
            QTableWidget {
                background-color: #fff;
                border: 1px solid #ccc;
                border-radius: 4px;
                gridline-color: #ddd;
            }
            QHeaderView::section {
                background-color: #f0f0f0;
                padding: 4px;
                border: none;
            }
            QTableWidget::item {
                padding: 4px;
            }
            QTableWidget::item:selected {
                background-color: #d0e7ff;
            }
        )");
    }
}