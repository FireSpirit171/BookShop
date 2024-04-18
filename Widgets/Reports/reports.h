#ifndef REPORTS_H
#define REPORTS_H

#include <QDialog>
#include "../AllOrders/allorders.h"
#include "../PublisherBooks/publisherbooks.h"
#include "../OrderByDate/orderbydate.h"
#include "../UsersOrdersWidget/usersorderswidget.h"
#include "../GistogrammBookPrice/gistogrammbookprice.h"

namespace Ui {
class Reports;
}

class Reports : public QDialog
{
    Q_OBJECT

public:
    explicit Reports(QWidget *parent = nullptr);
    ~Reports();

private slots:
    void showOrders();
    void AllOrdersWidgetClosed(); // Добавляем декларацию слота для обработки закрытия виджета книг
    void showPublisherBooks();
    void PublisherBooksWidgetClosed(); // Добавляем декларацию слота для обработки закрытия виджета книг
    void showOrdersByDate();
    void OrdersByDateWidgetClosed(); // Добавляем декларацию слота для обработки закрытия виджета книг
    void showClientOrders();
    void ClientOrdersWidgetClosed(); // Добавляем декларацию слота для обработки закрытия виджета книг
    void showGistogramm();
    void gistogrammWidgetClosed(); // Добавляем декларацию слота для обработки закрытия виджета книг

private:
    Ui::Reports *ui;
    AllOrders *allOrders;
    PublisherBooks *publisherBooks;
    OrderByDate *orderByDate;
    UsersOrdersWidget *clientOrders;
    GistogrammBookPrice *gistogrammBookPrice;
};

#endif // REPORTS_H
