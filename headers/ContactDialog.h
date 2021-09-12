//
// Created by corey on 9/12/21.
//

#ifndef VOIP_CLIENT3_CONTACTDIALOG_H
#define VOIP_CLIENT3_CONTACTDIALOG_H


#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QFormLayout>
#include <QPushButton>
#include "database_handler.h"

class ContactDialog : public QDialog {
        Q_OBJECT
public:
    explicit ContactDialog(QWidget *parent = nullptr);
        ~ContactDialog() override;

public:
    QLineEdit *contact_name_edit;
    QLineEdit *contact_number_edit;
    QLabel *contact_name_label;
    QLabel *contact_number_label;
    QFormLayout *contact_layout;
    QPushButton *submit_button_;

public slots:
    void submit_contacts();


};


#endif //VOIP_CLIENT3_CONTACTDIALOG_H
