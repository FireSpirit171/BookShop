#ifndef USERSORDERSWIDGET_H
#define USERSORDERSWIDGET_H

#include <QDialog>
#include <QTableWidget>
#include <QtSql>
#include <QTextBrowser>

namespace Ui {
class UsersOrdersWidget;
}

class UsersOrdersWidget : public QDialog
{
    Q_OBJECT

public:
    explicit UsersOrdersWidget(QWidget *parent = nullptr);
    ~UsersOrdersWidget();

    void showOrders();

private slots:
    void searchOrders();

private:
    Ui::UsersOrdersWidget *ui;
    QSqlDatabase db;
};

#endif // USERSORDERSWIDGET_H
