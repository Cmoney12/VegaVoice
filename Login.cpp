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
    submit = new QPushButton;
    layout = new QGridLayout;
    layout->addWidget(phone_number_label, 0, 0);
    layout->addWidget(phone_number, 0, 1);
    layout->addWidget(password_password_label, 1,0);
    layout->addWidget(password, 1, 1);
    layout->addWidget(submit,2,1);
    connect(submit, &QPushButton::clicked, this, &Login::validate);

    setLayout(layout);

}

void Login::validate() {

}

Login::~Login()
{
    delete db_handler;
}