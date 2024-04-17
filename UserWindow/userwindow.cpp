#include "userwindow.h"
#include "ui_userwindow.h"
#include <iostream>
#include <QtSql>

UserWindow::UserWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserWindow),
    booksWidget(nullptr)
{
    ui->setupUi(this);

    connect(ui->available_books, &QPushButton::clicked, this, &UserWindow::showBooksInStock);
}

UserWindow::~UserWindow()
{
    delete ui;
    if (booksWidget)
        delete booksWidget;
}

void UserWindow::showBooksInStock()
{
    if (!booksWidget)
    {
        booksWidget = new AvailableBooksWidget();
        // Устанавливаем соединение для обработки сигнала о закрытии виджета
        connect(booksWidget, &AvailableBooksWidget::finished, this, &UserWindow::booksWidgetClosed);
    }

    booksWidget->showBooksInStock();
}

void UserWindow::booksWidgetClosed()
{
    // Удаляем виджет книг
    delete booksWidget;
    booksWidget = nullptr;
}
