#include "headers/mainwindow.h"
#include "headers/Login.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    auto *login = new Login;
    login->show();
    MainWindow w;
    login->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(login, &Login::accepted, &w, &MainWindow::show);
    return QApplication::exec();
}