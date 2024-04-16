#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Подключение сигнала к кнопке входа
    connect(ui->loginButton, &QPushButton::clicked, this, &MainWindow::loginButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::getLogin() const{
    return ui->loginEdit->text();
}

QString MainWindow::getPassword() const{
    return ui->passwordEdit->text();
}

QString MainWindow::getRole() const{
    if (ui->userRadioButton->isChecked()) {
        return "Покупатель";
    } else if (ui->employeeRadioButton->isChecked()) {
        return "Сотрудник";
    } else {
        // Если не выбрана ни одна роль, выводим сообщение об ошибке
        return ""; // Возвращаемся из обработчика, не продолжая выполнение кода
    }
}
