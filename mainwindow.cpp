#include <QDir>
#include <iostream>
#include <QMessageBox>
#include <cstring>
#include <memory>
#include <QHostAddress>
#include <QHostInfo>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "add_contact.h"
#include "Serialization.h"


MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow)
, socket(new QTcpSocket)
{
    ui->setupUi(this);


    QDir current_dir = QDir::current();
    current_dir.cdUp();
    QString resources_path = current_dir.absolutePath();
    QString localdb_path = resources_path + "/resources/user_db.sqlite3";
    QByteArray ba = localdb_path.toLocal8Bit();
    const char *db_path = ba.data();
    db_handler = new database_handler(db_path);
    db_handler->check_tables();

    //*****Contacts*******
    contact_label = new QLabel("Contacts");
    splitter = new QSplitter;
    left_widget = new QWidget;
    left_layout = new QFormLayout;
    contact_view = new QListView;
    string_list = new StringList;
    connect_button = new QPushButton("Connect");
    new_contact = new QPushButton("+");
    //new_contact->setStyleSheet("QPushButton {background-color: #A3C1DA; color: green;}");
    erase_contact = new QPushButton("-");
    //erase_contact->setStyleSheet("QPushButton {background-color: #A3C1DA; color: red;}");
    contact_view->setModel(string_list);
    auto *contacts_widgets = new QWidget;
    auto *contact_button_layout = new QGridLayout;
    contact_button_layout->addWidget(connect_button, 0, 0);
    contact_button_layout->addWidget(new_contact, 1, 0);
    contact_button_layout->addWidget(erase_contact, 1, 1);
    contacts_widgets->setLayout(contact_button_layout);

    left_layout->addRow(contact_label);
    left_layout->addRow(contacts_widgets);
    left_layout->addRow(contact_view);
    left_widget->setLayout(left_layout);

    //*****Phone********
    right_layout = new QFormLayout;
    right_widget = new QWidget;
    keypad_layout = new QGridLayout;
    keypad_widget = new QWidget;
    number_line = new QLineEdit;
    call_button = new QPushButton;
    connect(call_button, &QPushButton::clicked, this, &MainWindow::start_phone_call);
    back_space_button = new QPushButton;
    QString back_space_location = resources_path + "/resources/back_space_button.png";
    QPixmap back_space_pixmap(back_space_location);
    QIcon backspace_icon(back_space_pixmap);
    back_space_button->setIcon(backspace_icon);
    //back_space_button->setIconSize(QSize(30,30));


    QString picture_location = resources_path.append("/resources/phone1.png");
    QPixmap pixmap(picture_location);
    QIcon ButtonIcon(pixmap);
    call_button->setIcon(ButtonIcon);
    call_button->setIconSize(QSize(30,30));

    number_line->setAlignment(Qt::AlignRight);
    keypad_layout->addWidget(number_line, 0, 0, 1, 3);
    keypad_layout->addWidget(back_space_button, 0, 3);
    keypad_layout->addWidget(call_button, 6, 1);


    for (int i = digit_button; i >= 0; --i)
        phone_button[i] = createButton(QString::number(i), SLOT(key_pressed()));
    //digitButtons[i] = createButton(QString::number(i), SLOT(digitClicked()));

    for (int i = digit_button; i > 0; --i) {
        int row = ((9 - i) / 3) + 2;
        int column = ((i - 1) % 3) + 1;
        keypad_layout->addWidget(phone_button[i], row, column);
    }

    keypad_layout->addWidget(phone_button[0], 5, 2);
    keypad_widget->setLayout(keypad_layout);
    right_widget->setLayout(right_layout);
    right_layout->addWidget(keypad_widget);

    splitter->addWidget(left_widget);
    splitter->addWidget(right_widget);
    setCentralWidget(splitter);

    //connect(call_button, &QPushButton::clicked, this, &MainWindow::start_phone_call);
    connect(new_contact, &QPushButton::clicked, this, &MainWindow::add_new_contact);
    connect(back_space_button, &QPushButton::clicked, this, &MainWindow::back_space);
    connect(connect_button, &QPushButton::clicked, this, &MainWindow::connection);
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    hostname = QHostAddress("192.168.1.88");
}

Button *MainWindow::createButton(const QString &text, const char *member)
{
    auto *button = new Button(text);
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}

void MainWindow::connection() {
    users_phone_number = db_handler->get_phone_number();
    std::string send_number = users_phone_number + "\n";
    socket->connectToHost(hostname, 1234);

    if (socket->state() != QTcpSocket::ConnectedState) {
        socket->write(QString(send_number.c_str()).toUtf8());
    }
    else {
        QMessageBox::warning(this, "Connection", "Unsuccessful Connection");
    }
    QHostInfo::fromName(QHostInfo::localHostName()).addresses().toStdList();


}

void MainWindow::start_phone_call() {
    if (call_in_progress) {
        call_in_progress = false;
    }
    else if (!call_in_progress && !number_line->text().isEmpty()) {
        send_call_request();
    }
}

void MainWindow::key_pressed() {
    auto *clickedButton = qobject_cast<Button *>(sender());
    int digitValue = clickedButton->text().toInt();
    number_line->setText(number_line->text() + QString::number(digitValue));
}

void MainWindow::back_space() const {
    number_line->backspace();
}

void MainWindow::add_new_contact() {
    auto *contact_window = new add_contact;
    contact_window->setBaseSize(QSize(300,300));
    contact_window->show();
}

void MainWindow::send_call_request() {
    auto *serial = new Serialization;
    Protocol protocol_packet{};
    std::memcpy(protocol_packet.senders_number, users_phone_number.c_str(), users_phone_number.size() + 1);
    std::string receivers_string  = number_line->text().toStdString();
    std::memcpy(protocol_packet.receivers_number, receivers_string.c_str(), receivers_string.size() + 1);
    protocol_packet.status_code = 100; //trying

    serial->create_bson(protocol_packet);
    uint8_t *data = serial->create_packet();

    socket->write((char*)data, serial->length());
    call_in_progress = true;
    delete serial;
}


void MainWindow::accept_call_request() {
    auto *serial = new Serialization;
    Protocol protocol_packet{};
    std::memcpy(protocol_packet.senders_number, users_phone_number.c_str(), users_phone_number.size() + 1);
    std::string receivers_string  = number_line->text().toStdString();
    std::memcpy(protocol_packet.receivers_number, receivers_string.c_str(), receivers_string.size() + 1);
    protocol_packet.status_code = 100; //trying

    serial->create_bson(protocol_packet);
    uint8_t *data = serial->create_packet();

    socket->write((char*)data, serial->length());
    call_in_progress = true;
    delete serial;
}


void MainWindow::onReadyRead() {
    auto *serial = new Serialization;
    if (socket->bytesAvailable() >= 4) {

        socket->read((char*)serial->head(), Serialization::HEADER_LENGTH);

        if (socket->bytesAvailable() >= Serialization::HEADER_LENGTH && serial->decode_header()) {

            socket->read((char*)serial->body(), serial->body_length());

            Protocol protocol = serial->parse_bson();

            if (protocol.status_code == 100) {
                bool accept;
                QMessageBox::information(this, "Call Request", protocol.senders_number);
                call_in_progress = true;
                std::cout << protocol.data << std::endl;
                std::cout << "200" << std::endl;
            }

            else if (protocol.status_code == 202) {
                //call was accepted instantiate audio start sending date
                std::cout << "202" << std::endl;
            }

            else if (protocol.status_code == 603) {
                call_in_progress = false;
                std::cout << "603" << std::endl;
            }
        }
    }
    delete serial;
}

MainWindow::~MainWindow()
{
    delete db_handler;
    delete ui;
}
