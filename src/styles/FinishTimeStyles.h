#pragma once
#include <QString>

namespace FinishTimeStyles {

inline QString labelStyle() {
    return QString(R"(
        QLabel {
            font-size: 18px;
            color: #333333;
            padding: 4px 6px;
        }
    )");
}

inline QString iconStyle() {
    return QString(R"(
        QLabel {
            margin: 0px 4px;
        }
    )");
}

}
