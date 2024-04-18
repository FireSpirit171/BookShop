#ifndef ORDERBYDATE_H
#define ORDERBYDATE_H

#include <QDialog>
#include <QtSql>
#include <QTableWidget>
#include <QTextBrowser>
#include <QDateEdit>
#include <QDate>

namespace Ui {
class OrderByDate;
}

class OrderByDate : public QDialog
{
    Q_OBJECT

public:
    explicit OrderByDate(QWidget *parent = nullptr);
    ~OrderByDate();

    void showOrders();

private:
    Ui::OrderByDate *ui;
    QSqlDatabase db;
};

#endif // ORDERBYDATE_H
