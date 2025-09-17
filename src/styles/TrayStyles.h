#pragma once
#include <QString>

namespace TrayStyles {

inline QString headerStyle() {
    return QString(R"(
        QWidget {
            background-color: #ffffff;
        }
        QLabel#titleLabel {
            font-size: 14pt;
            font-weight: bold;
            color: #333333;
        }
        QLabel#appIconLabel {
            margin-right: 10px;
        }
    )");
}

inline QString menuItemStyle() {
    return QString(R"(
        QLabel {
            font-size: 16px;
            color: #333333;
            padding: 6px 0px;
            border-radius: 6px;
        }
        QLabel:hover {
            background-color: #518eff;
            color: white;
        }
    )");
}

}
