#include "publisherbooks.h"
#include "ui_publisherbooks.h"

PublisherBooks::PublisherBooks(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PublisherBooks)
{
    ui->setupUi(this);
    db = QSqlDatabase::database(); // Получаем соединение с базой данных

    // Вызываем функцию для отображения книг каждого издательства при открытии окна
    showPublisherBooks();
}

PublisherBooks::~PublisherBooks()
{
    delete ui;
}

void PublisherBooks::showPublisherBooks()
{
    // Очистка таблицы перед выводом новых данных
    ui->tableWidget->clearContents();

    // Выполнение SQL-запроса для получения списка издательств
    QSqlQuery publisherQuery(db);
    QString publisherQueryString = "SELECT \"PublisherID\", \"Name\" FROM \"Publisher\"";
    if (!publisherQuery.exec(publisherQueryString)) {
        qDebug() << "Publisher query execution error:" << publisherQuery.lastError().text();
        return;
    }

    // Установка количества строк в tableWidget
    ui->tableWidget->setRowCount(publisherQuery.size());

    // Установка ширины столбцов
    ui->tableWidget->setColumnWidth(0, 150); // Первый столбец (Название издательства)
    ui->tableWidget->setColumnWidth(1, 250); // Второй столбец (Книги)
    ui->tableWidget->setColumnWidth(2, 80); // Третий столбец (Количество)

    // Установка заголовков столбцов
    QStringList headers;
    headers << "Издательство" << "Книги" << "Количество";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // Отображение результатов запроса в таблице
    int row = 0;
    while (publisherQuery.next()) {
        int publisherId = publisherQuery.value("PublisherID").toInt();
        QString publisherName = publisherQuery.value("Name").toString();

        // Выполнение SQL-запроса для получения списка книг каждого издательства
        QSqlQuery booksQuery(db);
        QString booksQueryString = QString("SELECT \"Title\", \"Author\", COUNT(*) AS \"BookCount\" "
                                           "FROM \"Book\" "
                                           "WHERE \"Publisher\" = %1 "
                                           "GROUP BY \"Title\",\"Author\"").arg(publisherId);
        if (!booksQuery.exec(booksQueryString)) {
            qDebug() << "Books query execution error:" << booksQuery.lastError().text();
            return;
        }

        // Создание строки для столбца "Книги"
        QStringList booksList;
        while (booksQuery.next()) {
            QString title = booksQuery.value("Title").toString();
            QString author = booksQuery.value("Author").toString();
            booksList << QString("%1 - %2").arg(title).arg(author);
        }
        QString booksString = booksList.join("<br>");

        QTextBrowser *booksItem = new QTextBrowser();
        booksItem->setHtml(booksString);
        booksItem->setReadOnly(true);
        ui->tableWidget->setCellWidget(row, 1, booksItem); // Установка виджета в ячейку таблицы
        booksItem->setAlignment(Qt::AlignCenter);

        QTableWidgetItem *publisherNameItem = new QTableWidgetItem(publisherName);
        QTableWidgetItem *bookCountItem = new QTableWidgetItem(QString::number(booksList.size()));

        // Установка выравнивания текста в ячейке по центру
        publisherNameItem->setTextAlignment(Qt::AlignCenter);
        bookCountItem->setTextAlignment(Qt::AlignCenter);

        ui->tableWidget->setItem(row, 0, publisherNameItem);
        ui->tableWidget->setItem(row, 2, bookCountItem);

        if (booksList.size()==1){
            ui->tableWidget->setRowHeight(row, 27);
        } else {
            ui->tableWidget->setRowHeight(row, booksList.size() * 22);
        }

        row++;
    }
}
