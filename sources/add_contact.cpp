#include <QDir>
#include <iostream>
#include "../headers/add_contact.h"

//
// Created by corey on 8/21/21.
//

add_contact::add_contact(QWidget *parent): QMainWindow(parent) {

    QDir current_dir = QDir::current();
    current_dir.cdUp();
    QString resources_path = current_dir.absolutePath();
    QString localdb_path = resources_path + "/resources/user_db.sqlite3";
    QByteArray ba = localdb_path.toLocal8Bit();
    const char *db_path = ba.data();
    db_handler = new database_handler(db_path);
    contact_layout = new QGridLayout;
    contact_widget = new QWidget;
    username_label = new QLabel("Contact Name");
    username = new QLineEdit;
    phone_label = new QLabel("Phone Number");
    phone_number = new QLineEdit;
    submit = new QPushButton("Submit");

    contact_layout->addWidget(username_label, 0,0);
    contact_layout->addWidget(username, 0, 1);
    contact_layout->addWidget(phone_label, 1,0);
    contact_layout->addWidget(phone_number, 1,1);
    contact_layout->addWidget(submit, 2,1);

    contact_widget->setLayout(contact_layout);
    setCentralWidget(contact_widget);
    connect(submit, &QPushButton::clicked, this, &add_contact::submit_contact);
    this->setAttribute(Qt::WA_DeleteOnClose);
}

void add_contact::submit_contact() const {
    std::string name = username->text().toStdString();
    std::string number = phone_number->text().toStdString();
    const char* contact_name = name.c_str();
    const char* contact_number = number.c_str();
    db_handler->create_contact(contact_name, contact_number);
}


add_contact::~add_contact()
{
    delete db_handler;
}
#include "../headers/add_contact.h"
