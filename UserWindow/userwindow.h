#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <QDialog>
#include "../Widgets/AvailableBooksWidget/availablebookswidget.h"
#include "../Widgets/UsersOrdersWidget/usersorderswidget.h"

namespace Ui {
class UserWindow;
}

class UserWindow : public QDialog
{
    Q_OBJECT

public:
    explicit UserWindow(QWidget *parent = nullptr);
    ~UserWindow();

private slots:
    void showBooksInStock();
    void showOrders();
    void booksWidgetClosed(); // Добавляем декларацию слота для обработки закрытия виджета книг
    void ordersWidgetClosed(); // Добавляем декларацию слота для обработки закрытия виджета книг

private:
    Ui::UserWindow *ui;
    AvailableBooksWidget *booksWidget; // Добавляем указатель на виджет книг
    UsersOrdersWidget *ordersWidget;
};

#endif // USERWINDOW_H
