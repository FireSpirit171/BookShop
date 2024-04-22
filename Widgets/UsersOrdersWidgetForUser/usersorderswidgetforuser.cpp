#include "usersorderswidgetforuser.h"
#include "ui_usersorderswidgetforuser.h"
#include <iostream>
#include <QDebug>

UsersOrdersWidgetForUser::UsersOrdersWidgetForUser(int client_id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UsersOrdersWidgetForUser),
    CLIENT_ID(client_id)
{
    ui->setupUi(this);

    showOrders();
}

UsersOrdersWidgetForUser::~UsersOrdersWidgetForUser()
{
    delete ui;
}

void UsersOrdersWidgetForUser::showOrders()
{
    // Очистка таблицы перед выводом новых данных
    ui->tableWidget->clearContents();

    // Выполнение SQL-запроса для получения заказов по id клиента
    QSqlQuery query(db);
    QString queryString = QString("SELECT \"OrderID\", \"TotalPrice\" FROM \"Order\" WHERE \"Client\" = %1").arg(CLIENT_ID);
    if (!query.exec(queryString)) {
        qDebug() << "Query execution error:" << query.lastError().text();
        return;
    }

    // Установка количества строк в tableWidget
    ui->tableWidget->setRowCount(query.size());

    // Установка ширины столбцов
    ui->tableWidget->setColumnWidth(0, 80); // Первый столбец
    ui->tableWidget->setColumnWidth(1, 280); // Второй столбец
    ui->tableWidget->setColumnWidth(2, 130); // Третий столбец

    // Установка заголовков столбцов
    QStringList headers;
    headers << "№ Заказа" << "Книги" << "Общая стоимость";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // Отображение результатов запроса в таблице
    int row = 0;
    while (query.next()) {
        int orderId = query.value("OrderID").toInt();
        QString total_price = query.value("TotalPrice").toString();
        total_price = total_price.left(total_price.length() - 2); // Обрезаем пробел и знак вопроса

        // Получение списка книг для данного заказа
        QSqlQuery booksQuery(db);
        QString booksQueryString = QString("SELECT b.\"Title\", b.\"Author\" "
                                           "FROM \"Book\" b "
                                           "JOIN \"Book_Order\" bo ON b.\"ISBN\" = bo.\"ISBN\" "
                                           "WHERE bo.\"OrderID\" = %1").arg(orderId);
        if (!booksQuery.exec(booksQueryString)) {
            qDebug() << "Books query execution error:" << booksQuery.lastError().text();
            return;
        }

        QStringList booksList;
        while (booksQuery.next()) {
            QString title = booksQuery.value("Title").toString();
            QString author = booksQuery.value("Author").toString();
            booksList << title + " - " + author;
        }

        // Создание строки для столбца "Книги"
        QString booksString = booksList.join("<br>");

        QTextBrowser *booksItem = new QTextBrowser();
        booksItem->setHtml(booksString);
        booksItem->setReadOnly(true);
        ui->tableWidget->setCellWidget(row, 1, booksItem); // Установка виджета в ячейку таблицы

        QTableWidgetItem *orderIdItem = new QTableWidgetItem(QString::number(orderId));
        QTableWidgetItem *totalPriceItem = new QTableWidgetItem(total_price+'$');

        // Установка выравнивания текста в ячейке по центру
        orderIdItem->setTextAlignment(Qt::AlignCenter);
        totalPriceItem->setTextAlignment(Qt::AlignCenter);

        ui->tableWidget->setItem(row, 0, orderIdItem);
        ui->tableWidget->setItem(row, 2, totalPriceItem); // Установка данных в новый столбец "Книги"

        if (booksList.size()==1){
            ui->tableWidget->setRowHeight(row, 27);
        } else {
            ui->tableWidget->setRowHeight(row, booksList.size() * 22);
        }

        row++;
    }
}
