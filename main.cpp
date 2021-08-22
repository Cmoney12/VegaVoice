#include "mainwindow.h"
#include "Login.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Login login;
    if (login.exec() == QDialog::Accepted) {
        w.show();
    }
    return QApplication::exec();
}