#pragma once
#include <QString>

namespace SettingsStyles {

inline QString windowStyle() {
    return QString(R"(
        QWidget {
            background-color: #fafafa;
            font-family: "Segoe UI", "Roboto", sans-serif;
            font-size: 14px;
            color: #333;
        }

        QTabWidget::pane {
            border: none;
            background: transparent;
        }

        QTabBar::tab {
            background: #e9e9e9;
            padding: 10px 18px;
            border-radius: 8px 8px 0 0;
            margin-right: 2px;
        }

        QTabBar::tab:selected {
            background: #ffffff;
            color: #000;
            font-weight: bold;
        }

        QTabBar::tab:hover {
            background: #f0f6ff;
        }
    )");
}

inline QString timeEditStyle() {
    return QString(R"(
        QTimeEdit {
            border: 1px solid #d0d0d0;
            border-radius: 6px;
            padding: 4px 8px;
            background-color: #ffffff;
            color: #333;
            min-width: 70px;
        }

        QTimeEdit:focus {
            border: 2px solid #518eff;
            background: #f9fbff;
        }

        /* Кнопки ↑↓ */
        QTimeEdit::up-button, QTimeEdit::down-button {
            border: none;
            background: #f2f2f2;
            width: 18px;
            margin: 0;
            border-radius: 3px;
        }

        /* Подсветка при наведении */
        QTimeEdit::up-button:hover, QTimeEdit::down-button:hover {
            background: #cce0ff;
        }

        QTimeEdit::up-button:pressed, QTimeEdit::down-button:pressed {
            background: #99c2ff;
        }

        /* Вставляем SVG иконки для стрелок */
        QTimeEdit::up-arrow {
            image: url(:/res/resources/icons/up-arrow.svg);
            width: 8px;
            height: 8px;
        }

        QTimeEdit::down-arrow {
            image: url(:/res/resources/icons/down-arrow.svg);
            width: 8px;
            height: 8px;
        }
    )");
}

inline QString buttonStyle() {
    return QString(R"(
        QPushButton {
            background-color: #518eff;
            color: white;
            border-radius: 6px;
            padding: 8px 20px;
            font-size: 14px;
            font-weight: 500;
        }

        QPushButton:hover {
            background-color: #72a8ff;
        }

        QPushButton:pressed {
            background-color: #1366ff;
        }

        QPushButton:disabled {
            background-color: #e0e0e0;
            color: #888;
        }
    )");
}

inline QString dayLabelStyle() {
    return QString(R"(
        QLabel {
            font-weight: 600;
            color: #444;
            padding: 4px 0;
        }
    )");
}

}
