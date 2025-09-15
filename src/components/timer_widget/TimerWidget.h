#include "components/central_widget/MainCentralWidget.h"
#include <QWidget>
#include "states/main_window_state/MainWindowState.h"

class TimerWidget : public QWidget {
    Q_OBJECT
    public:
        explicit TimerWidget(QWidget* parent = nullptr, MainWindowState* state = nullptr);

    private slots:
        void onStatusChanged(MainWindowState::TimerStatus status);
        void onValueChanged(int seconds);

    private:
        QLabel* timeLabel;
        QVBoxLayout* layout;
        MainWindowState* windowState;
};
