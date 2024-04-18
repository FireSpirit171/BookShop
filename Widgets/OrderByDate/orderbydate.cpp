#include "orderbydate.h"
#include "ui_orderbydate.h"

OrderByDate::OrderByDate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OrderByDate)
{
    ui->setupUi(this);
    db = QSqlDatabase::database(); // Получаем соединение с базой данных

    // Установка максимальной даты в QDateEdit
    ui->dateEdit->setMaximumDate(QDate::currentDate());

    // Вызываем функцию для отображения заказов при открытии окна
    showOrders();

    // Установка минимальной даты в QDateEdit
    QSqlQuery minDateQuery(db);
    if (minDateQuery.exec("SELECT MIN(\"Date\") FROM \"Order\"")) {
        if (minDateQuery.next()) {
            QDate minDate = minDateQuery.value(0).toDate();
            // Получаем предыдущий день от минимальной даты
            QDate prevDate = minDate.addDays(-1);
            ui->dateEdit->setMinimumDate(prevDate);
        }
    } else {
        qDebug() << "Error getting minimum date:" << minDateQuery.lastError().text();
    }

    // Подключаем сигнал изменения даты к слоту обновления заказов
    connect(ui->dateEdit, &QDateEdit::dateChanged, this, &OrderByDate::showOrders);
}


OrderByDate::~OrderByDate()
{
    delete ui;
}

void OrderByDate::showOrders()
{
    // Очистка таблицы перед выводом новых данных
    ui->tableWidget->clearContents();

    // Получение выбранной даты из QDateEdit
    QDate selectedDate = ui->dateEdit->date();

    // Выполнение SQL-запроса для получения заказов на выбранную дату
    QSqlQuery query(db);
    QString queryString = QString("SELECT * FROM \"Order\" WHERE \"Date\" = '%1' ORDER BY \"OrderID\" ASC").arg(selectedDate.toString("yyyy-MM-dd"));
    if (!query.exec(queryString)) {
        qDebug() << "Query execution error:" << query.lastError().text();
        return;
    }

    // Установка количества строк в tableWidget
    ui->tableWidget->setRowCount(query.size());

    // Установка ширины столбцов
    ui->tableWidget->setColumnWidth(0, 80); // Номер заказа
    ui->tableWidget->setColumnWidth(1, 100); // Дата
    ui->tableWidget->setColumnWidth(2, 220); // Книги
    ui->tableWidget->setColumnWidth(3, 120); // Клиент

    // Установка заголовков столбцов
    QStringList headers;
    headers << "№ Заказа" << "Дата" << "Книги" << "Клиент";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // Отображение результатов запроса в таблице
    int row = 0;
    while (query.next()) {
        int orderId = query.value("OrderID").toInt();
        QString date = query.value("Date").toDate().toString("dd-MM-yyyy");

        // Получение информации о клиенте
        int clientId = query.value("Client").toInt();
        QSqlQuery clientQuery(db);
        clientQuery.prepare("SELECT * FROM \"Client\" WHERE \"ClientID\" = :clientId");
        clientQuery.bindValue(":clientId", clientId);
        if (!clientQuery.exec()) {
            qDebug() << "Client query execution error:" << clientQuery.lastError().text();
            return;
        }
        QString clientInfo;
        if (clientQuery.next()) {
            QString lastName = clientQuery.value("LastName").toString();
            QString firstName = clientQuery.value("FirstName").toString();
            QString middleName = clientQuery.value("MiddleName").toString();
            clientInfo = QString("%1 %2.%3.").arg(lastName).arg(firstName.at(0)).arg(middleName.at(0));
        }

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
            booksList << QString("\"%1\" - %2").arg(title).arg(author);
        }

        // Создание строки для столбца "Книги"
        QString booksString = booksList.join("<br>");

        QTextBrowser *booksItem = new QTextBrowser();
        booksItem->setHtml(booksString);
        booksItem->setReadOnly(true);
        ui->tableWidget->setCellWidget(row, 2, booksItem); // Установка виджета в ячейку таблицы
        booksItem->setAlignment(Qt::AlignCenter);

        QTableWidgetItem *orderIdItem = new QTableWidgetItem(QString::number(orderId));
        QTableWidgetItem *dateItem = new QTableWidgetItem(date);
        QTableWidgetItem *clientItem = new QTableWidgetItem(clientInfo);

        // Установка выравнивания текста в ячейке по центру
        orderIdItem->setTextAlignment(Qt::AlignCenter);
        dateItem->setTextAlignment(Qt::AlignCenter);
        clientItem->setTextAlignment(Qt::AlignCenter);

        ui->tableWidget->setItem(row, 0, orderIdItem);
        ui->tableWidget->setItem(row, 1, dateItem);
        ui->tableWidget->setItem(row, 3, clientItem);

        if (booksList.size() == 1) {
            ui->tableWidget->setRowHeight(row, 27);
        } else {
            ui->tableWidget->setRowHeight(row, booksList.size() * 22);
        }

        row++;
    }
}
