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
#include <QLabel>
#include <QTcpSocket>
#include <QHostAddress>
#include <thread>
#include "database_handler.h"
#include "Audio.h"
#include "UdpCall.h"
#include "ContactDialog.h"
#include "CallHistoryWidget.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    ContactDialog *contact_dialog;
    QTabWidget *tab_widget;
    QTab *phone_tab;
    QFormLayout *left_layout;
    QSplitter *splitter;
    QFormLayout *phone_layout;
    QWidget *phone_widget;
    CallHistoryWidget *history_widget;
    QWidget *left_widget;
    QMenuBar *menu;
    QMenu *settings_menu;
    QListView *contact_view;
    QGridLayout *keypad_layout;
    QWidget *keypad_widget;
    enum {digit_button = 10};
    QLineEdit *number_line;
    Button *phone_button[digit_button]{};
    Button *createButton(const QString &text, const char *member);
    QPushButton *call_button;
    StringList *string_list;
    database_handler *db_handler;
    QPushButton *back_space_button;
    QLabel *contact_label;
    QPushButton *new_contact;
    QPushButton *erase_contact;
    std::string users_phone_number;
    QPushButton *connect_button;
    QHostAddress hostname;
    Audio* audio;
    UdpCall *phone_call;
    std::thread *call_thread;
    bool call_in_progress = false;
    void accept_call_request(const char* send_num);
    void call_established(char* ip_address, char* port);
    void end_call();
    void display_contacts() const;
    void hang_up();
    std::int64_t current_date_time();


public slots:
    void start_phone_call();
    void onReadyRead();
    void connection();
    void key_pressed();
    void back_space() const;
    void add_new_contact() const;
    void set_default_host_ip();
    void set_default_udp_port();
    void delete_contact() const;
    void set_recipient() const;
    void contact_submission();
    void tab_selected() const;

private:
    void send_call_request();
    void clear_history() const;

private:
    QTcpSocket *socket;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
