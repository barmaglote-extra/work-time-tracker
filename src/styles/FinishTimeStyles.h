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

    inline QString lineEditStyle() {
        return QString(R"(
            QLineEdit {
                border: 1px solid #d0d0d0;
                border-radius: 6px;
                padding: 4px 8px;
                background-color: #ffffff;
                color: #333;
                min-width: 70px;
            }

            QLineEdit:focus {
                border: 2px solid #518eff;
                background: #f9fbff;
            }

            QLineEdit::up-button, QTimeEdit::down-button {
                border: none;
                background: #f2f2f2;
                width: 18px;
                margin: 0;
                border-radius: 3px;
            }

            QLineEdit::up-button:hover, QTimeEdit::down-button:hover {
                background: #cce0ff;
            }

            QLineEdit::up-button:pressed, QTimeEdit::down-button:pressed {
                background: #99c2ff;
            }

            QLineEdit::up-arrow {
                image: url(:/res/resources/icons/up-arrow.svg);
                width: 8px;
                height: 8px;
            }

            QLineEdit::down-arrow {
                image: url(:/res/resources/icons/down-arrow.svg);
                width: 8px;
                height: 8px;
            }
        )");
    }
}
