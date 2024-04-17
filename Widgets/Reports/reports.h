#ifndef REPORTS_H
#define REPORTS_H

#include <QDialog>
#include "../AllOrders/allorders.h"
#include "../PublisherBooks/publisherbooks.h"

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

private:
    Ui::Reports *ui;
    AllOrders *allOrders;
    PublisherBooks *publisherBooks;

};

#endif // REPORTS_H
