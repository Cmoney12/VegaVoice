#include <QDir>
#include <iostream>
#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow)
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
    splitter = new QSplitter;
    left_widget = new QWidget;
    left_layout = new QFormLayout;
    contact_view = new QListView;
    string_list = new StringList;
    contact_view->setModel(string_list);
    left_layout->addWidget(contact_view);
    left_widget->setLayout(left_layout);

    //*****Phone********
    right_layout = new QFormLayout;
    right_widget = new QWidget;
    keypad_layout = new QGridLayout;
    keypad_widget = new QWidget;
    number_line = new QLineEdit;
    call_button = new QPushButton;

    back_space_button = new QPushButton;
    QString back_space_location = resources_path + "/resources/back_space_button.png";
    QPixmap back_space_pixmap(back_space_location);
    QIcon backspace_icon(back_space_pixmap);
    back_space_button->setIcon(backspace_icon);
    //back_space_button->setIconSize(QSize(30,30));


    call_button->setStyleSheet("QPushButton { background-color: rgb(18, 18, 204);}");
    QString picture_location = resources_path.append("/resources/phone1.png");
    QPixmap pixmap(picture_location);
    QIcon ButtonIcon(pixmap);
    call_button->setIcon(ButtonIcon);
    call_button->setIconSize(QSize(30,30));
    call_button->setStyleSheet("QPushButton { background-color: rgb(18, 18, 204);}");

    number_line->setAlignment(Qt::AlignRight);
    keypad_layout->addWidget(number_line, 0, 0, 1, 3);
    keypad_layout->addWidget(back_space_button, 0, 3);
    keypad_layout->addWidget(call_button, 6, 1);


    for (int i = 0; i <= digit_button; ++i)
        phone_button[i] = createButton(QString::number(i), SLOT(key_pressed()));
    //digitButtons[i] = createButton(QString::number(i), SLOT(digitClicked()));

    for (int i = digit_button; i > 0; --i) {
        int row = ((9 - i) / 3) + 2;
        int column = ((i - 1) % 3) + 1;
        keypad_layout->addWidget(phone_button[i], row, column);
    }

    keypad_layout->addWidget(phone_button[0], 5, 2);

    connect(back_space_button, &QPushButton::clicked, this, &MainWindow::back_space);

    keypad_widget->setLayout(keypad_layout);
    right_widget->setLayout(right_layout);
    right_layout->addWidget(keypad_widget);

    splitter->addWidget(left_widget);
    splitter->addWidget(right_widget);
    setCentralWidget(splitter);
}

Button *MainWindow::createButton(const QString &text, const char *member)
{
    auto *button = new Button(text);
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}

void MainWindow::key_pressed() {
    auto *clickedButton = qobject_cast<Button *>(sender());
    int digitValue = clickedButton->text().toInt();
    number_line->setText(number_line->text() + QString::number(digitValue));
}

void MainWindow::back_space() const {
    number_line->backspace();
}

MainWindow::~MainWindow()
{
    delete db_handler;
    delete ui;
}
