#ifndef ORDERFORMFORUSER_H
#define ORDERFORMFORUSER_H

#include <QDialog>
#include <QtSql>
#include <QMessageBox>

namespace Ui {
class OrderFormForUser;
}

struct _Book_ {
    QString isbn;
    QString title;
    QString price;
};

struct Order_ {
    int id;
    QString client;
    QString seller;
    QString totalprice;
    QDate date;
    QVector<_Book_> orderBooks;
};

class OrderFormForUser : public QDialog
{
    Q_OBJECT

public:
    explicit OrderFormForUser(int clientid, QWidget *parent = nullptr);
    ~OrderFormForUser();

    void showOrderFormForUser();
    void loadClients();
    void loadSellers();
    void loadOrderBooks(Order_&);
    void recalculateTotalPrice();

private slots:
    void displayOrder(); // Добавляем новый приватный слот для отображения текущей книги
    void addOrder(); // Добавляем слот для кнопки "Add"
    void saveOrder(); // Добавляем слот для кнопки "Save"
    void addBookInOrder(); // Добавляем слот для кнопки "Delete"
    void deleteBookFromOrder(); // Добавляем слот для кнопки "Delete"
    void findBooks();
    void getQueue(int, const QString&, const QString&, const QDate&, const QString&, const QVector<_Book_>&);

private:
    Ui::OrderFormForUser *ui;
    QSqlDatabase db;
    QVector<Order_> orders; // Вектор для хранения данных о заказах
    int currentOrderIndex; // Индекс текущего клиента
    QVector<_Book_> books; // Вектор для хранения данных о книгах
    QVector<QPair<int, QString>> clients;
    QVector<QPair<int, QString>> sellers;
    int CLIENT_ID;
};

#endif // ORDERFORMFORUSER_H
