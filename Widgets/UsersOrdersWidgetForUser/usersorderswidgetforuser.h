#ifndef USERSORDERSWIDGETFORUSER_H
#define USERSORDERSWIDGETFORUSER_H

#include <QDialog>
#include <QTableWidget>
#include <QtSql>
#include <QTextBrowser>

namespace Ui {
class UsersOrdersWidgetForUser;
}

class UsersOrdersWidgetForUser : public QDialog
{
    Q_OBJECT

public:
    explicit UsersOrdersWidgetForUser(int client_id, QWidget *parent = nullptr);
    ~UsersOrdersWidgetForUser();

    void showOrders();

private:
    Ui::UsersOrdersWidgetForUser *ui;
    QSqlDatabase db;
    int CLIENT_ID;
};

#endif // USERSORDERSWIDGETFORUSER_H
