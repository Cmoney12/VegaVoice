//
// Created by corey on 9/12/21.
//

#include <QDir>
#include "../headers/ContactDialog.h"

ContactDialog::ContactDialog(QWidget *parent) :QDialog(parent)
{
    setModal(true);

    contact_name_label = new QLabel("Contact Name");
    contact_name_edit = new QLineEdit;
    contact_number_label = new QLabel("Contact Number");
    contact_number_edit = new QLineEdit;
    contact_layout = new QFormLayout;
    submit_button_ = new QPushButton("Submit");

    contact_layout->addRow(contact_name_label, contact_name_edit);
    contact_layout->addRow(contact_number_label, contact_number_edit);
    contact_layout->addRow(submit_button_);
    setLayout(contact_layout);

    connect(submit_button_, &QPushButton::clicked, this, &ContactDialog::submit_contacts);
}

void ContactDialog::submit_contacts() {
    accepted();
    close();
}

ContactDialog::~ContactDialog() {
}
