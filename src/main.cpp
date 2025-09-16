#include <QApplication>
#include "windows/main_window/MainWindow.h"
#include "components/tray_helper/TrayHelper.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);
    QApplication::setWindowIcon(QIcon(":/res/resources/icons/app_icon.svg"));
    MainWindow window;
    TrayHelper::setupTray(&window, QIcon(":/res/resources/icons/app_icon.svg"));

    window.show();

    return app.exec();
}