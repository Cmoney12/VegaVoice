//
// Created by corey on 9/12/21.
//

#ifndef VEGAVOICE_CALLHISTORYWIDGET_H
#define VEGAVOICE_CALLHISTORYWIDGET_H


#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QFormLayout>
#include <list>
#include <tuple>
#include <QPushButton>

class CallHistoryWidget : public QWidget {
    Q_OBJECT
public:
    explicit CallHistoryWidget(QWidget *parent = nullptr);

public:
    QTableWidget *history_table;
    QLabel *history_label_;
    QFormLayout *history_layout;
    QPushButton *clear_history_;
    void set_history(std::list<std::tuple<std::string, std::string, std::int64_t>>& history);

};


#endif //VEGAVOICE_CALLHISTORYWIDGET_H
