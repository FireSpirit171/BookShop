#include "userwindow.h"
#include "ui_userwindow.h"
#include <QtSql>

UserWindow::UserWindow(int clientid, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserWindow),
    client_id(clientid),
    booksWidget(nullptr),
    ordersWidgetForUser(nullptr),
    orderFormForUser(nullptr)
{
    ui->setupUi(this);

    connect(ui->available_books, &QPushButton::clicked, this, &UserWindow::showBooksInStock);
    connect(ui->users_orders, &QPushButton::clicked, this, &UserWindow::showOrders);
    connect(ui->new_order, &QPushButton::clicked, this, &UserWindow::showOrderFormForUser);
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

void UserWindow::showOrders(){
    if (!ordersWidgetForUser)
    {
        ordersWidgetForUser = new UsersOrdersWidgetForUser(client_id);
        connect(ordersWidgetForUser, &UsersOrdersWidgetForUser::finished, this, &UserWindow::ordersWidgetClosed);
    }

    ordersWidgetForUser->exec();
}

void UserWindow::ordersWidgetClosed()
{
    // Удаляем виджет книг
    delete ordersWidgetForUser;
    ordersWidgetForUser = nullptr;
}

void UserWindow::showOrderFormForUser(){
    if (!orderFormForUser)
    {
        orderFormForUser = new OrderFormForUser(client_id);
        connect(orderFormForUser, &OrderFormForUser::finished, this, &UserWindow::OrderFormForUserClosed);
    }

    orderFormForUser->exec();
}

void UserWindow::OrderFormForUserClosed()
{
    // Удаляем виджет книг
    delete orderFormForUser;
    orderFormForUser = nullptr;
}
