#ifndef AVAILABLEBOOKSWIDGET_H
#define AVAILABLEBOOKSWIDGET_H

#include <QDialog>
#include <QListWidget>
#include <QtSql>

namespace Ui {
class AvailableBooksWidget;
}

class AvailableBooksWidget : public QDialog
{
    Q_OBJECT

public:
    explicit AvailableBooksWidget(QWidget *parent = nullptr);
    ~AvailableBooksWidget();

    void showBooksInStock();

private:
    Ui::AvailableBooksWidget *ui;
    QSqlDatabase db;
};

#endif // AVAILABLEBOOKSWIDGET_H
