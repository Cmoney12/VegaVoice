#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFormLayout>
#include <QSplitter>
#include <QMenuBar>
#include <QListView>
#include <QPushButton>
#include <QGridLayout>
#include "button.h"
#include "StringList.h"
#include <QLineEdit>
#include "database_handler.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QFormLayout *left_layout;
    QSplitter *splitter;
    QFormLayout *right_layout;
    QWidget *right_widget;
    QWidget *left_widget;
    QMenuBar *menu;
    QListView *contact_view;
    QGridLayout *keypad_layout;
    QWidget *keypad_widget;
    enum {digit_button = 10};
    QLineEdit *number_line;
    Button *phone_button[digit_button];
    Button *createButton(const QString &text, const char *member);
    QPushButton *call_button;
    StringList *string_list;
    database_handler *db_handler;
    QPushButton *back_space_button;

public slots:
    void key_pressed();
    void back_space() const;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
