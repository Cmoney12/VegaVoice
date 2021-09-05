//
// Created by corey on 8/21/21.
//

#ifndef VOIP_CLIENT3_LOGIN_H
#define VOIP_CLIENT3_LOGIN_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QComboBox>
#include <QGridLayout>
#include <QStringList>
#include <QDebug>
#include <QWidget>
#include "database_handler.h"


class Login : public QDialog {
    Q_OBJECT
public:
    explicit Login(QWidget *parent = nullptr);
    ~Login() override;
    QLabel *phone_number_label;
    QLabel *password_password_label;
    QLineEdit *phone_number;
    QLineEdit *password;
    database_handler *db_handler;
    QPushButton *submit;
    QGridLayout *layout;
    QPushButton *register_button;
public slots:
    void validate();
    void register_user() const;
};



#endif //VOIP_CLIENT3_LOGIN_H
