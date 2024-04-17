#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <QDialog>
#include "../Widgets/AvailableBooksWidget/availablebookswidget.h"

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
    void booksWidgetClosed(); // Добавляем декларацию слота для обработки закрытия виджета книг

private:
    Ui::UserWindow *ui;
    AvailableBooksWidget *booksWidget; // Добавляем указатель на виджет книг
};

#endif // USERWINDOW_H
