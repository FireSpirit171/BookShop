#include "allorders.h"
#include "ui_allorders.h"

AllOrders::AllOrders(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AllOrders)
{
    ui->setupUi(this);
    db = QSqlDatabase::database(); // Получаем соединение с базой данных

    // Вызываем функцию для отображения всех заказов при открытии окна
    showOrders();
}

AllOrders::~AllOrders()
{
    delete ui;
}

void AllOrders::showOrders()
{
    // Очистка таблицы перед выводом новых данных
    ui->tableWidget->clearContents();

    // Выполнение SQL-запроса для получения всех заказов
    QSqlQuery query(db);
    QString queryString = "SELECT o.\"OrderID\", c.\"LastName\", o.\"TotalPrice\" "
                          "FROM \"Order\" o "
                          "JOIN \"Client\" c ON o.\"Client\" = c.\"ClientID\" "
                          "ORDER BY \"OrderID\" ASC";
    if (!query.exec(queryString)) {
        qDebug() << "Query execution error:" << query.lastError().text();
        return;
    }

    // Установка количества строк в tableWidget
    ui->tableWidget->setRowCount(query.size());

    // Установка ширины столбцов
    ui->tableWidget->setColumnWidth(0, 80); // Первый столбец (№ Заказа)
    ui->tableWidget->setColumnWidth(1, 80); // Второй столбец (Клиент)
    ui->tableWidget->setColumnWidth(2, 220); // Третий столбец (Книги)
    ui->tableWidget->setColumnWidth(3, 120); // Третий столбец (Общая стоимость)


    // Установка заголовков столбцов
    QStringList headers;
    headers << "№ Заказа" << "Клиент" << "Книги" << "Общая стоимость";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // Отображение результатов запроса в таблице
    int row = 0;
    while (query.next()) {
        int orderId = query.value("OrderID").toInt();
        QString clientLastName = query.value("LastName").toString();
        QString totalPrice = query.value("TotalPrice").toString();
        totalPrice = totalPrice.left(totalPrice.length() - 2); // Обрезаем пробел и знак вопроса

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
            booksList << "\"" + title + "\"" + " - " + author;
        }

        // Создание строки для столбца "Книги"
        QString booksString = booksList.join("<br>");

        QTextBrowser *booksItem = new QTextBrowser();
        booksItem->setHtml(booksString);
        booksItem->setReadOnly(true);
        ui->tableWidget->setCellWidget(row, 2, booksItem); // Установка виджета в ячейку таблицы
        booksItem->setAlignment(Qt::AlignCenter);

        QTableWidgetItem *orderIdItem = new QTableWidgetItem(QString::number(orderId));
        QTableWidgetItem *clientItem = new QTableWidgetItem(clientLastName);
        QTableWidgetItem *totalPriceItem = new QTableWidgetItem(totalPrice + '$');

        // Установка выравнивания текста в ячейке по центру
        orderIdItem->setTextAlignment(Qt::AlignCenter);
        clientItem->setTextAlignment(Qt::AlignCenter);
        totalPriceItem->setTextAlignment(Qt::AlignCenter);

        ui->tableWidget->setItem(row, 0, orderIdItem);
        ui->tableWidget->setItem(row, 1, clientItem);
        ui->tableWidget->setItem(row, 3, totalPriceItem);

        if (booksList.size()==1){
            ui->tableWidget->setRowHeight(row, 27);
        } else {
            ui->tableWidget->setRowHeight(row, booksList.size() * 22);
        }

        row++;
    }
}
