#include "mainwindow.h"
#include "Login.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login login;
    login.show();
    MainWindow w;
    QObject::connect(&login, &Login::accepted, &w, &MainWindow::show);
    /**if (login.exec() == QDialog::Accepted) {
        w.show();
    }**/
    return QApplication::exec();
}