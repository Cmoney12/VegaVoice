//
// Created by corey on 8/21/21.
//

#ifndef VOIP_CLIENT3_ADD_CONTACT_H
#define VOIP_CLIENT3_ADD_CONTACT_H


#include <QMainWindow>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include "database_handler.h"

class add_contact : public QMainWindow {
    Q_OBJECT
public:
    explicit add_contact(QWidget *parent = nullptr);
    ~add_contact() override;

public:
    QWidget *contact_widget;
    QGridLayout *contact_layout;
    QLabel *username_label;
    QLineEdit *username;
    QLabel *phone_label;
    QLineEdit *phone_number;
    database_handler *db_handler;
    QPushButton *submit;
public slots:
    void submit_contact() const;
};


#endif //VOIP_CLIENT3_ADD_CONTACT_H
