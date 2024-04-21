#include "orderform.h"
#include "ui_orderform.h"
#include <QDebug>
#include <iostream>

OrderForm::OrderForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OrderForm)
{
    ui->setupUi(this);

    // Подключаем слоты к кнопкам
    connect(ui->next, &QPushButton::clicked, this, &OrderForm::moveToNextOrder);
    connect(ui->previous, &QPushButton::clicked, this, &OrderForm::moveToPreviousOrder);
    connect(ui->add, &QPushButton::clicked, this, &OrderForm::addOrder);
    connect(ui->save, &QPushButton::clicked, this, &OrderForm::saveOrder);
    connect(ui->delete_2, &QPushButton::clicked, this, &OrderForm::deleteOrder);
    connect(ui->addbook, &QPushButton::clicked, this, &OrderForm::addBookInOrder);
    connect(ui->deletebook, &QPushButton::clicked, this, &OrderForm::deleteBookFromOrder);
    connect(ui->find, &QPushButton::clicked, this, &OrderForm::findBooks);

    loadClients();
    loadSellers();

    // Отображаем заказы при открытии формы
    showOrderForm();
}

OrderForm::~OrderForm()
{
    delete ui;
}

void OrderForm::showOrderForm() {
    // Открываем соединение с базой данных
    db = QSqlDatabase::database();
    if (!db.isValid()) {
        qDebug() << "Error: Failed to get database connection.";
        return;
    }

    // Выполняем запрос к базе данных для получения данных из таблицы Orders
    QSqlQuery query;
    if (!query.exec("SELECT * FROM \"Order\" ORDER BY \"OrderID\" ASC")) {
        qDebug() << "Error executing query:" << query.lastError().text();
        return;
    }

    // Если запрос успешно выполнен и есть записи в результате
    if (query.next()) {
        // Получаем все записи о заказах и сохраняем их для последующего использования
        do {
            Order order;
            order.id = query.value("OrderID").toInt();
            order.client = query.value("Client").toString();
            order.seller = query.value("Seller").toString();
            QString totalpriceString = query.value("TotalPrice").toString();
            totalpriceString.chop(2); // Обрезаем последние два символа
            order.totalprice = totalpriceString;
            order.date = query.value("Date").toDate();
            // Загружаем книги для каждого заказа
            loadOrderBooks(order);
            orders.append(order);
        } while (query.next());

        // Отображаем первый заказ
        currentOrderIndex = 0;
        displayOrder();
    }
}

void OrderForm::moveToNextOrder() {
    if (currentOrderIndex < orders.size() - 1) {
        ++currentOrderIndex;
        displayOrder();
    }
}

void OrderForm::moveToPreviousOrder() {
    if (currentOrderIndex > 0) {
        --currentOrderIndex;
        displayOrder();
    }
}

void OrderForm::displayOrder() {
    // Получаем данные текущего заказа
    const Order& currentOrder = orders[currentOrderIndex];

    // Отображаем данные текущего заказа на форме
    ui->id->setText(QString::number(currentOrder.id));

    // Находим фамилию клиента по его ID
    QString clientName;
    for (const auto& client : clients) {
        if (client.first == currentOrder.client.toInt()) {
            clientName = client.second;
            break;
        }
    }
    ui->client->setCurrentText(clientName);

    // Находим фамилию продавца по его ID
    QString sellerName;
    for (const auto& seller : sellers) {
        if (seller.first == currentOrder.seller.toInt()) {
            sellerName = seller.second;
            break;
        }
    }
    ui->seller->setCurrentText(sellerName);

    QString _price = currentOrder.totalprice;
    _price.append('$');
    ui->totalprice->setText(_price);
    ui->date->setDate(currentOrder.date);

    // Отображаем книги текущего заказа в QTableWidget
    ui->books->clearContents();
    ui->books->setRowCount(currentOrder.orderBooks.size());
    QStringList headers;
    headers << "Книги" << "Стоимость";
    ui->books->setHorizontalHeaderLabels(headers);
    for (int i = 0; i < currentOrder.orderBooks.size(); ++i) {
        const Book_& book = currentOrder.orderBooks[i];
        QTableWidgetItem *titleItem = new QTableWidgetItem(book.title);
        QString _price = book.price;
        _price.chop(2);
        _price.append('$');
        QTableWidgetItem *priceItem = new QTableWidgetItem(_price);
        ui->books->setItem(i, 0, titleItem);
        ui->books->setItem(i, 1, priceItem);
    }
}



void OrderForm::addOrder() {
    // Устанавливаем следующий номер заказа в качестве временного ID
    int maxID = 0;
    for (const auto& order : orders) {
        maxID = qMax(maxID, order.id);
    }
    int nextID = maxID + 1;
    ui->id->setText(QString::number(nextID));

    // Очищаем поля ввода
    ui->client->setCurrentIndex(-1);
    ui->seller->setCurrentIndex(-1);
    ui->totalprice->clear();
    ui->totalprice->setInputMask("00,00$");
    ui->totalprice->setText("00,00$");
    ui->date->setDate(QDate::currentDate());

    // Очищаем поле поиска
    ui->findbook->clear();

    // Очищаем список доступных книг
    ui->availablebooks->clear();

    // Очищаем поле с книгами заказа
    ui->books->clearContents();
    ui->books->setRowCount(0);

    Order newOrder;
    newOrder.id = nextID;
    newOrder.client = "";
    newOrder.seller = "";
    newOrder.totalprice = "0.00$";
    newOrder.date = QDate::currentDate();
    newOrder.orderBooks.clear(); // Очищаем список книг заказа

    // Добавляем новый заказ в вектор заказов
    orders.push_back(newOrder);
    currentOrderIndex = orders.size()-1;
}

void OrderForm::saveOrder() {
    int currentId = ui->id->text().toInt();

    // Проверяем, существует ли уже запись с таким ID в базе данных
    bool existingOrder = false;
    for (auto& order : orders) {
        //qDebug() << order.id << " " << order.client << " " << order.seller << " " << order.totalprice << " " << order.date;
        if (order.id == currentId && order.client!="") {
            existingOrder = true;
            break;
        }
    }

    // Получаем данные текущего заказа
    Order& currentOrder = orders[currentOrderIndex];
    QString _client = ui->client->currentData().toString();
    currentOrder.client = _client;
    QString _seller = ui->seller->currentData().toString();
    currentOrder.seller = _seller;
    currentOrder.totalprice = ui->totalprice->text();
    currentOrder.date = ui->date->date();

    // Сохраняем заказ в базе данных
    QSqlQuery query;
    if (!existingOrder) {
        QString _price = currentOrder.totalprice;
        _price.chop(1);
        // Если это новый заказ, добавляем его в базу данных
        QString insertQuery = QString("INSERT INTO \"Order\" (\"OrderID\", \"Client\", \"Seller\", \"TotalPrice\", \"Date\") "
                                      "VALUES (%1, '%2', '%3', '%4', '%5')")
                                  .arg(currentId).arg(currentOrder.client).arg(currentOrder.seller)
                                  .arg(_price).arg(currentOrder.date.toString("yyyy-MM-dd"));
        if (!query.exec(insertQuery)) {
            qDebug() << "Error inserting order:" << query.lastError().text();
        } else {
            for (const Book_& book : currentOrder.orderBooks) {
                QString insertBookOrderQuery = QString("INSERT INTO \"Book_Order\" (\"OrderID\", \"ISBN\") "
                                                        "VALUES (%1, '%2')")
                                                        .arg(currentId).arg(book.isbn);
                if (!query.exec(insertBookOrderQuery)) {
                    qDebug() << "Error inserting book into order:" << query.lastError().text();
                }
            }
            QMessageBox::information(this, "Успех", "Запись успешно добавлена!");
        }
    } else {
        // Получаем предыдущий список книг в заказе
        QVector<QString> oldBooks;
        QSqlQuery oldBooksQuery;
        QString oldBooksSelectQuery = QString("SELECT \"ISBN\" FROM \"Book_Order\" WHERE \"OrderID\" = %1")
                                            .arg(currentOrder.id);
        if (!oldBooksQuery.exec(oldBooksSelectQuery)) {
            qDebug() << "Error retrieving old books in order:" << oldBooksQuery.lastError().text();
        } else {
            while (oldBooksQuery.next()) {
                oldBooks.append(oldBooksQuery.value("ISBN").toString());
            }
        }

        // Получаем новый список книг в заказе
        QVector<QString> newBooks;
        for (const Book_& book : currentOrder.orderBooks) {
            newBooks.append(book.isbn);
        }

        // Находим книги, которые были удалены из заказа
        QVector<QString> deletedBooks;
        for (const QString& oldBook : oldBooks) {
            if (!newBooks.contains(oldBook)) {
                deletedBooks.append(oldBook);
            }
        }

        // Удаляем записи о удаленных книгах из таблицы Book_Order
        for (const QString& deletedBook : deletedBooks) {
            QString deleteQuery = QString("DELETE FROM \"Book_Order\" WHERE \"OrderID\" = %1 AND \"ISBN\" = '%2'")
                                            .arg(currentOrder.id).arg(deletedBook);
            if (!query.exec(deleteQuery)) {
                qDebug() << "Error deleting book from order:" << query.lastError().text();
            }
        }

        // Добавляем записи о новых книгах в таблицу Book_Order
        for (const QString& newBook : newBooks) {
            if (!oldBooks.contains(newBook)) {
                QString insertQuery = QString("INSERT INTO \"Book_Order\" (\"OrderID\", \"ISBN\") VALUES (%1, '%2')")
                                                .arg(currentOrder.id).arg(newBook);
                if (!query.exec(insertQuery)) {
                    qDebug() << "Error inserting book into order:" << query.lastError().text();
                }
            }
        }

        // Обновляем заказ в базе данных
        QString _price = currentOrder.totalprice;
        _price.chop(1);

        if (_price.contains('.')) {
            _price.replace('.', ',');
        }
        QString updateQuery = QString("UPDATE \"Order\" SET "
                                      "\"Client\" = '%1', "
                                      "\"Seller\" = '%2', "
                                      "\"TotalPrice\" = '%3', "
                                      "\"Date\" = '%4' "
                                      "WHERE \"OrderID\" = %5")
                                  .arg(currentOrder.client).arg(currentOrder.seller)
                                  .arg(_price).arg(currentOrder.date.toString("yyyy-MM-dd"))
                                  .arg(currentOrder.id);
        if (!query.exec(updateQuery)) {
            qDebug() << "Error updating order:" << query.lastError().text();
        } else {
            QMessageBox::information(this, "Успех", "Запись успешно обновлена!");
        }
    }
}


void OrderForm::deleteOrder() {
    // Получаем ID текущего заказа
    int orderId = ui->id->text().toInt();

    QSqlQuery query;

    // Удаляем записи из таблицы Book_Order
    QString deleteBookOrderQuery = QString("DELETE FROM \"Book_Order\" WHERE \"OrderID\" = %1").arg(orderId);
    if (!query.exec(deleteBookOrderQuery)) {
        qDebug() << "Error deleting books from order:" << query.lastError().text();
        return;
    }

    // Удаляем сам заказ из таблицы Order
    QString deleteOrderQuery = QString("DELETE FROM \"Order\" WHERE \"OrderID\" = %1").arg(orderId);
    if (!query.exec(deleteOrderQuery)) {
        qDebug() << "Error deleting order:" << query.lastError().text();
        return;
    }

    // Удаляем заказ из локального списка
    orders.remove(currentOrderIndex);

    // Очищаем поля формы
    ui->id->clear();
    ui->client->setCurrentIndex(-1);
    ui->seller->setCurrentIndex(-1);
    ui->totalprice->clear();
    ui->date->setDate(QDate::currentDate());

    // Перемещаемся к предыдущему заказу или следующему, если предыдущего нет
    if (currentOrderIndex > 0) {
        --currentOrderIndex;
    } else if (!orders.isEmpty()) {
        ++currentOrderIndex;
    }

    // Отображаем текущий заказ
    displayOrder();

    QMessageBox::information(this, "Успех", "Заказ удален!");
}


void OrderForm::loadOrderBooks(Order& order) {
    // Выполняем запрос к базе данных для получения списка книг в заказе
    QSqlQuery query;
    QString selectQuery = QString("SELECT * FROM \"Book\" "
                                  "INNER JOIN \"Book_Order\" ON \"Book\".\"ISBN\" = \"Book_Order\".\"ISBN\" "
                                  "WHERE \"Book_Order\".\"OrderID\" = %1").arg(order.id);
    if (!query.exec(selectQuery)) {
        qDebug() << "Error executing query:" << query.lastError().text();
        return;
    }

    // Если запрос успешно выполнен и есть записи в результате
    while (query.next()) {
        Book_ book;
        book.isbn = query.value("ISBN").toString();
        book.title = query.value("Title").toString();
        book.price = query.value("Price").toString();
        order.orderBooks.append(book);
    }
}

void OrderForm::addBookInOrder() {
    QListWidgetItem* selectedItem = ui->availablebooks->currentItem();
    if (selectedItem) {
        // Получаем данные о книге из выбранного элемента QListWidget
        QString bookTitle = selectedItem->text();
        QString bookPrice = selectedItem->data(Qt::UserRole).toString(); // Примерно так, вы можете сохранять дополнительные данные в UserRole

        // Запрос в базу данных для получения ISBN книги по названию
        QSqlQuery query;
        QString selectQuery = QString("SELECT \"ISBN\" FROM \"Book\" WHERE \"Title\" = '%1'").arg(bookTitle);
        if (!query.exec(selectQuery)) {
            qDebug() << "Error selecting ISBN for book:" << query.lastError().text();
            return;
        }

        if (query.next()) {
            QString bookISBN = query.value("ISBN").toString();

            // Добавляем книгу в текущий заказ
            Order& currentOrder = orders[currentOrderIndex];
            Book_ newBook;
            newBook.title = bookTitle;
            newBook.isbn = bookISBN; // Устанавливаем полученный ISBN
            newBook.price = bookPrice;
            currentOrder.orderBooks.append(newBook);

            recalculateTotalPrice();

            // Отображаем текущий заказ с добавленной книгой
            displayOrder();
        } else {
            qDebug() << "Book not found in database:" << bookTitle;
        }
    }
}


void OrderForm::deleteBookFromOrder() {
    int selectedRow = ui->books->currentRow();
    if (selectedRow >= 0) {
        // Удаляем книгу из текущего заказа
        Order& currentOrder = orders[currentOrderIndex];
        currentOrder.orderBooks.remove(selectedRow);

        recalculateTotalPrice();

        // Отображаем текущий заказ без удаленной книги
        displayOrder();
    }
}

void OrderForm::findBooks() {
    QString searchText = ui->findbook->text();

    // Выполняем поиск книг в базе данных
    QSqlQuery query;
    QString selectQuery = QString("SELECT * FROM \"Book\" WHERE \"Title\" LIKE '%%1%'").arg(searchText);
    if (!query.exec(selectQuery)) {
        qDebug() << "Error executing query:" << query.lastError().text();
        return;
    }

    // Очищаем список доступных книг перед добавлением новых результатов поиска
    ui->availablebooks->clear();

    // Если запрос успешно выполнен и есть записи в результате
    while (query.next()) {
        QString bookTitle = query.value("Title").toString();
        QString bookPrice = query.value("Price").toString();
        QListWidgetItem *item = new QListWidgetItem(bookTitle);
        item->setData(Qt::UserRole, bookPrice); // Сохраняем цену книги в UserRole
        ui->availablebooks->addItem(item);
    }
}

void OrderForm::loadClients() {
    // Очищаем вектор клиентов перед загрузкой новых данных
    clients.clear();

    // Очищаем комбобокс клиентов перед загрузкой новых данных
    ui->client->clear();

    // Выполняем запрос к базе данных для получения всех клиентов
    QSqlQuery query("SELECT * FROM \"Client\" ORDER BY \"ClientID\"");
    while (query.next()) {
        int clientID = query.value("ClientID").toInt();
        QString clientName = query.value("LastName").toString();
        clients.append(qMakePair(clientID, clientName));
        ui->client->addItem(clientName, clientID);
    }
}

void OrderForm::loadSellers() {
    // Очищаем вектор продавцов перед загрузкой новых данных
    sellers.clear();

    // Очищаем комбобокс продавцов перед загрузкой новых данных
    ui->seller->clear();

    // Выполняем запрос к базе данных для получения всех продавцов
    QSqlQuery query("SELECT * FROM \"Seller\" ORDER BY \"SellerID\"");
    while (query.next()) {
        int sellerID = query.value("SellerID").toInt();
        QString sellerName = query.value("LastName").toString();
        sellers.append(qMakePair(sellerID, sellerName));
        ui->seller->addItem(sellerName, sellerID);
    }
}

void OrderForm::recalculateTotalPrice() {
    double totalPrice = 0.0;
    Order& currentOrder = orders[currentOrderIndex]; // Изменяем на немодифицируемый объект
    for (auto& book : currentOrder.orderBooks) { // Перебираем книги по ссылке для изменения
        // Предполагая, что цена книги хранится в формате "xx.xx$"
        QString price = book.price;
        price.replace(",", "."); // Заменяем запятые на точки
        price.chop(2); // Удаляем символ валюты '$'
        totalPrice += price.toDouble();
    }
    QString new_price = QString::number(totalPrice, 'f', 2);
    currentOrder.totalprice = new_price;
    ui->totalprice->setText(currentOrder.totalprice);
}





