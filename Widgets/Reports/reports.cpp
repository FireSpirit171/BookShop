#include "reports.h"
#include "ui_reports.h"

Reports::Reports(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Reports),
    allOrders(nullptr),
    publisherBooks(nullptr)
{
    ui->setupUi(this);
    connect(ui->all_orders, &QPushButton::clicked, this, &Reports::showOrders);
    connect(ui->publisher_books, &QPushButton::clicked, this, &Reports::showPublisherBooks);
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
        connect(publisherBooks, &PublisherBooks::finished, this, &Reports::AllOrdersWidgetClosed);
    }

    publisherBooks->exec();
}

void Reports::PublisherBooksWidgetClosed()
{
    // Удаляем виджет книг
    delete publisherBooks;
    publisherBooks = nullptr;
}
