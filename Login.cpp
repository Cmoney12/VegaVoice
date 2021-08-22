//
// Created by corey on 8/21/21.
//

#include <QDir>
#include "Login.h"

Login::Login(QWidget *parent) :QDialog(parent)
{
    setWindowTitle( tr("User Login") );
    QDir current_dir = QDir::current();
    current_dir.cdUp();
    QString resources_path = current_dir.absolutePath();
    QString localdb_path = resources_path + "/resources/user_db.sqlite3";
    QByteArray ba = localdb_path.toLocal8Bit();
    const char *db_path = ba.data();
    db_handler = new database_handler(db_path);
    phone_number_label = new QLabel("Phone Number");
    phone_number = new QLineEdit;
    password_password_label = new QLabel("Password");
    password = new QLineEdit;
    layout = new QGridLayout;
    central_widget = new QWidget;



}

Login::~Login()
{
    delete db_handler;
}