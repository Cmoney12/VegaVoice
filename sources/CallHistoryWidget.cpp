//
// Created by corey on 9/12/21.
//

#include "../headers/CallHistoryWidget.h"

CallHistoryWidget::CallHistoryWidget(QWidget *parent) {
    history_layout = new QFormLayout;
    history_label_ = new QLabel("Call History");

    history_table = new QTableWidget;

    history_table->setColumnCount(3);

    history_layout->addRow(history_label_);
    history_layout->addRow(history_table);

    auto *receiver_number_ = new QTableWidgetItem();
    receiver_number_->setText("Receiver Number");
    history_table->setHorizontalHeaderItem(0, receiver_number_);

    auto *action_type = new QTableWidgetItem();
    receiver_number_->setText("Action");
    history_table->setHorizontalHeaderItem(1, action_type);

    auto *date_time_ = new QTableWidgetItem();
    receiver_number_->setText("Date Time");
    history_table->setHorizontalHeaderItem(2, date_time_);

    setLayout(history_layout);

}

void CallHistoryWidget::set_history(std::list<std::tuple<std::string, std::string, std::int64_t>>& history) {
    history_table->setRowCount(history.size());
    auto l_front = history.begin();
    for(int i=0; i < history_table->rowCount(); i++) {

        for (int j = 0; j < history_table->columnCount(); j++) {
            QTableWidgetItem *pCell = history_table->item(i, j);
            if(!pCell)
            {
                pCell = new QTableWidgetItem;
                history_table->setItem(i, j, pCell);
            }
            if (j % 3 == 0) {
                std::string date_ = std::to_string(std::get<2>(*l_front));
                QString date_time = QString::fromStdString(date_);
                pCell->setText(date_time);
            }
            else if (j % 2 == 0) {
                std::string data_ = std::get<1>(*l_front);
                pCell->setText(QString::fromStdString(data_));
            }
            else {
                std::string data_ = std::get<0>(*l_front);
                pCell->setText(QString::fromStdString(data_));
            }
        }
        std::advance(l_front, 1);
    }
}



