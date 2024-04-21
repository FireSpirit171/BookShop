#ifndef ORDERFORM_H
#define ORDERFORM_H

#include <QDialog>
#include <QtSql>
#include <QMessageBox>

namespace Ui {
class OrderForm;
}

struct Book_ {
    QString isbn;
    QString title;
    QString price;
};

struct Order {
    int id;
    QString client;
    QString seller;
    QString totalprice;
    QDate date;
    QVector<Book_> orderBooks;
};

class OrderForm : public QDialog
{
    Q_OBJECT

public:
    explicit OrderForm(QWidget *parent = nullptr);
    ~OrderForm();

    void showOrderForm();
    void loadClients();
    void loadSellers();
    void loadOrderBooks(Order&);
    void recalculateTotalPrice();

private slots:
    void moveToNextOrder();
    void moveToPreviousOrder();
    void displayOrder(); // Добавляем новый приватный слот для отображения текущей книги
    void addOrder(); // Добавляем слот для кнопки "Add"
    void saveOrder(); // Добавляем слот для кнопки "Save"
    void deleteOrder(); // Добавляем слот для кнопки "Delete"
    void addBookInOrder(); // Добавляем слот для кнопки "Delete"
    void deleteBookFromOrder(); // Добавляем слот для кнопки "Delete"
    void findBooks();

private:
    Ui::OrderForm *ui;
    QSqlDatabase db;
    QVector<Order> orders; // Вектор для хранения данных о заказах
    int currentOrderIndex; // Индекс текущего клиента
    QVector<Book_> books; // Вектор для хранения данных о книгах
    QVector<QPair<int, QString>> clients;
    QVector<QPair<int, QString>> sellers;
};

#endif // ORDERFORM_H
