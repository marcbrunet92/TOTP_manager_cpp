#include <QApplication>
#include "controllers/AccountController.h"
#include "gui/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    AccountController controller;
    MainWindow window(&controller);
    window.show();

    return app.exec();
}