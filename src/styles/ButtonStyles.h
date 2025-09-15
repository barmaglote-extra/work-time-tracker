#pragma once
#include <QString>

inline QString getButtonStyle() {
    return R"(
        QPushButton {
            background-color: #ffffffff;
            border: 2px solid #518effff;
            border-radius: 10px;
            min-width: 50px;
            min-height: 50px;
            font-size: 18px;
            color: white;
            padding: 5px;
        }

        QPushButton:hover {
            background-color: #518effff;
            border: 2px solid #1366ffff;
        }

        QPushButton:pressed {
            border: 2px solid #1366ffff;
        }

        QPushButton:disabled {
            border: 2px solid #000000ff;
        }
    )";
}
