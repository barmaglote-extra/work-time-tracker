inline QString getButtonStyle(int minSize = 50, int fontSize = 18) {
    return QString(R"(
        QPushButton {
            background-color: #ffffffff;
            border: 2px solid #518effff;
            border-radius: 10px;
            min-width: %1px;
            min-height: %1px;
            font-size: %2px;
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
    )").arg(minSize).arg(fontSize);
}
