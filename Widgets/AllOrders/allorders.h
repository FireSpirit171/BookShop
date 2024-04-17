#ifndef ALLORDERS_H
#define ALLORDERS_H

#include <QDialog>
#include <QtSql>
#include <QTableWidget>
#include <QTextBrowser>

namespace Ui {
class AllOrders;
}

class AllOrders : public QDialog
{
    Q_OBJECT

public:
    explicit AllOrders(QWidget *parent = nullptr);
    ~AllOrders();

    void showOrders();

private:
    Ui::AllOrders *ui;
    QSqlDatabase db;
};

#endif // ALLORDERS_H
