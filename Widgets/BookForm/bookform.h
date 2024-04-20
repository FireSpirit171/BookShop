#ifndef BOOKFORM_H
#define BOOKFORM_H

#include <QDialog>
#include <QtSql>
#include <QMessageBox>

namespace Ui {
class BookForm;
}

struct Book {
    QString isbn;
    QString title;
    QString author;
    QString price;
    QString genre;
    int publisher;
    int supplier;
};


class BookForm : public QDialog
{
    Q_OBJECT

public:
    explicit BookForm(QWidget *parent = nullptr);
    ~BookForm();

    void showBookForm();
    void loadPublishers();
    void loadSuppliers();

private slots:
    void moveToNextBook();
    void moveToPreviousBook();
    void displayBook(); // Добавляем новый приватный слот для отображения текущей книги
    void addBook(); // Добавляем слот для кнопки "Add"
    void saveBook(); // Добавляем слот для кнопки "Save"
    void deleteBook(); // Добавляем слот для кнопки "Delete"

private:
    Ui::BookForm *ui;
    QSqlDatabase db;
    QVector<Book> books; // Вектор для хранения данных о клиентах
    int currentBookIndex; // Индекс текущего клиента
    QVector<QPair<int, QString>> publishers;
    QVector<QPair<int, QString>> suppliers;
};

#endif // BOOKFORM_H
