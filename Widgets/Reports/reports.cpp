#include "reports.h"
#include "ui_reports.h"

Reports::Reports(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Reports),
    allOrders(nullptr),
    publisherBooks(nullptr),
    orderByDate(nullptr),
    clientOrders(nullptr),
    gistogrammBookPrice(nullptr)
{
    ui->setupUi(this);
    connect(ui->all_orders, &QPushButton::clicked, this, &Reports::showOrders);
    connect(ui->publisher_books, &QPushButton::clicked, this, &Reports::showPublisherBooks);
    connect(ui->orders_by_date, &QPushButton::clicked, this, &Reports::showOrdersByDate);
    connect(ui->client_orders, &QPushButton::clicked, this, &Reports::showClientOrders);
    connect(ui->gistogramm, &QPushButton::clicked, this, &Reports::showGistogramm);
}

Reports::~Reports()
{
    delete ui;
}

void Reports::showOrders(){
    if (!allOrders)
    {
        allOrders = new AllOrders();
        connect(allOrders, &AllOrders::finished, this, &Reports::AllOrdersWidgetClosed);
    }

    allOrders->exec();
}

void Reports::AllOrdersWidgetClosed()
{
    // Удаляем виджет книг
    delete allOrders;
    allOrders = nullptr;
}

void Reports::showPublisherBooks(){
    if (!publisherBooks)
    {
        publisherBooks = new PublisherBooks();
        connect(publisherBooks, &PublisherBooks::finished, this, &Reports::PublisherBooksWidgetClosed);
    }

    publisherBooks->exec();
}

void Reports::PublisherBooksWidgetClosed()
{
    // Удаляем виджет книг
    delete publisherBooks;
    publisherBooks = nullptr;
}

void Reports::showOrdersByDate(){
    if (!orderByDate)
    {
        orderByDate = new OrderByDate();
        connect(orderByDate, &OrderByDate::finished, this, &Reports::OrdersByDateWidgetClosed);
    }

    orderByDate->exec();
}

void Reports::OrdersByDateWidgetClosed()
{
    // Удаляем виджет книг
    delete orderByDate;
    orderByDate = nullptr;
}

void Reports::showClientOrders(){
    if (!clientOrders)
    {
        clientOrders = new UsersOrdersWidget();
        connect(clientOrders, &UsersOrdersWidget::finished, this, &Reports::ClientOrdersWidgetClosed);
    }

    clientOrders->exec();
}

void Reports::ClientOrdersWidgetClosed()
{
    // Удаляем виджет книг
    delete clientOrders;
    clientOrders = nullptr;
}

void Reports::showGistogramm(){
    if (!gistogrammBookPrice)
    {
        gistogrammBookPrice = new GistogrammBookPrice();
        connect(gistogrammBookPrice, &GistogrammBookPrice::finished, this, &Reports::gistogrammWidgetClosed);
    }

    gistogrammBookPrice->exec();
}

void Reports::gistogrammWidgetClosed()
{
    // Удаляем виджет книг
    delete gistogrammBookPrice;
    gistogrammBookPrice = nullptr;
}
