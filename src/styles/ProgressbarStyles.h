#pragma once
#include <QString>

inline QString progressBarStyle(const QString& colorStart, const QString& colorEnd) {
    return QString(R"(
        QProgressBar {
            border: 1px solid #d0d0d0;
            border-radius: 10px;
            text-align: center;
            background: #f9f9f9;
            height: 24px;
            font: bold 12px "Segoe UI";
            color: #444;
        }
        QProgressBar::chunk {
            border-radius: 10px;
            background: qlineargradient(
                x1:0, y1:0, x2:1, y2:0,
                stop:0 %1,
                stop:1 %2
            );
        }
    )").arg(colorStart).arg(colorEnd);
}


