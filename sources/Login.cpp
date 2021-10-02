//
// Created by corey on 8/21/21.
//

#include <QDir>
#include <iostream>
#include "../headers/Login.h"

Login::Login(QWidget *parent) :QDialog(parent)
{
    setWindowTitle( tr("User Login") );
    setModal(true);
    QDir current_dir = QDir::current();
    current_dir.cdUp();
    QString resources_path = current_dir.absolutePath();
    QString localdb_path = resources_path + "/resources/user_db.sqlite3";
    QByteArray ba = localdb_path.toLocal8Bit();
    const char *db_path = ba.data();
    db_handler = new database_handler(db_path);
    db_handler->check_tables();

    QString logo_path = resources_path + "/resources/vega_logo.png";
    QPixmap logo_(logo_path);

    auto *logo_label = new QLabel;
    logo_label->setPixmap(logo_);

    phone_number_label = new QLabel("Phone Number");
    phone_number = new QLineEdit;
    password_password_label = new QLabel("Password");
    password = new QLineEdit;
    submit = new QPushButton("Login");
    register_button = new QPushButton("Register");
    layout = new QGridLayout;
    layout->addWidget(logo_label, 0, 0, 1, 2);
    layout->addWidget(phone_number_label, 1, 0);
    layout->addWidget(phone_number, 1, 1);
    layout->addWidget(password_password_label, 2,0);
    layout->addWidget(password, 2, 1);
    layout->addWidget(submit,3,1);
    layout->addWidget(register_button, 4, 1);

    connect(submit, &QPushButton::clicked, this, &Login::validate);
    connect(register_button, &QPushButton::clicked, this, &Login::register_user);
    setLayout(layout);

}

void Login::validate() {
    std::string phone = phone_number->text().toStdString();
    const char* number = phone.c_str();

    std::string pass_phrase = password->text().toStdString();
    const char* pass = pass_phrase.c_str();

    if (db_handler->login(number, pass)) {
        accepted();
        close();
    }
}

void Login::register_user() const {
    std::string phone = phone_number->text().toStdString();
    std::string pass = password->text().toStdString();
    db_handler->register_user(phone, pass);
}

Login::~Login()
{
    delete db_handler;
}