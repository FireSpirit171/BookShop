#include "orderformforuser.h"
#include "ui_orderformforuser.h"
#include <QDebug>

OrderFormForUser::OrderFormForUser(int client_id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OrderFormForUser),
    CLIENT_ID(client_id)
{
    ui->setupUi(this);

    // Подключаем слоты к кнопкам
    connect(ui->save, &QPushButton::clicked, this, &OrderFormForUser::saveOrder);
    connect(ui->addbook, &QPushButton::clicked, this, &OrderFormForUser::addBookInOrder);
    connect(ui->deletebook, &QPushButton::clicked, this, &OrderFormForUser::deleteBookFromOrder);
    connect(ui->find, &QPushButton::clicked, this, &OrderFormForUser::findBooks);

    loadClients();
    loadSellers();

    // Отображаем заказы при открытии формы
    addOrder();
}

OrderFormForUser::~OrderFormForUser()
{
    delete ui;
}

void OrderFormForUser::showOrderFormForUser() {
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
            Order_ order;
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


void OrderFormForUser::displayOrder() {
    // Получаем данные текущего заказа
    const Order_& currentOrder = orders[currentOrderIndex];

    // Отображаем данные текущего заказа на форме
    ui->id->setText(QString::number(currentOrder.id));
    ui->id->setReadOnly(true);

    // Находим фамилию клиента по его ID
    QString clientName;
    for (const auto& client : clients) {
        if (client.first == CLIENT_ID) {
            clientName = client.second;
            break;
        }
    }
    ui->client->setText(clientName);
    ui->client->setReadOnly(true);

    QString _price = currentOrder.totalprice;
    if(_price=="0.00"){
        _price.append('0');
    }
    _price.append('$');
    ui->totalprice->setText(_price);
    ui->totalprice->setReadOnly(true);
    ui->date->setDate(currentOrder.date);
    ui->date->setMinimumDate(currentOrder.date);

    // Отображаем книги текущего заказа в QTableWidget
    ui->books->clearContents();
    ui->books->setRowCount(currentOrder.orderBooks.size());
    QStringList headers;
    headers << "Книги" << "Стоимость";
    ui->books->setHorizontalHeaderLabels(headers);
    for (int i = 0; i < currentOrder.orderBooks.size(); ++i) {
        const _Book_& book = currentOrder.orderBooks[i];
        QTableWidgetItem *titleItem = new QTableWidgetItem(book.title);
        QString _price = book.price;
        _price.chop(2);
        _price.append('$');
        QTableWidgetItem *priceItem = new QTableWidgetItem(_price);
        ui->books->setItem(i, 0, titleItem);
        ui->books->setItem(i, 1, priceItem);
    }
}



void OrderFormForUser::addOrder() {
    db = QSqlDatabase::database();
    if (!db.isValid()) {
        qDebug() << "Error: Failed to get database connection.";
        return;
    }

    // Выполняем запрос для поиска максимального ID в таблице заказов
    int maxId = 0;
    QSqlQuery maxIdQuery;
    if (maxIdQuery.exec("SELECT MAX(\"OrderID\") FROM \"Order\"") && maxIdQuery.next()) {
        maxId = maxIdQuery.value(0).toInt();
    } else {
        // Если произошла ошибка при выполнении запроса, выводим сообщение об ошибке
        qDebug() << "Error retrieving maximum order ID:" << maxIdQuery.lastError().text();
    }
    maxId += 1;
    ui->id->setText(QString::number(maxId));
    ui->id->setReadOnly(true);

    // Выполняем запрос для поиска клиент в таблице клиентов
    QString client;
    QSqlQuery clientQuery;
    clientQuery.prepare("SELECT \"LastName\" FROM \"Client\" WHERE \"ClientID\" = ?");
    clientQuery.bindValue(0, CLIENT_ID);
    if (clientQuery.exec() && clientQuery.next()) {
        client = clientQuery.value(0).toString();
    } else {
        // Если произошла ошибка при выполнении запроса, выводим сообщение об ошибке
        qDebug() << "Error:" << clientQuery.lastError().text();
    }
    ui->client->setText(client);
    ui->client->setReadOnly(true);

    // Очищаем поля ввода
    ui->totalprice->clear();
    ui->totalprice->setInputMask("00,00$");
    ui->totalprice->setText("00,00$");
    ui->totalprice->setReadOnly(true);
    ui->date->setDate(QDate::currentDate());
    ui->date->setMinimumDate(QDate::currentDate());

    // Очищаем поле поиска
    ui->findbook->clear();

    // Очищаем список доступных книг
    ui->availablebooks->clear();

    // Очищаем поле с книгами заказа
    ui->books->clearContents();
    ui->books->setRowCount(0);

    Order_ newOrder;
    newOrder.id = maxId;
    newOrder.client = "";
    newOrder.seller = "";
    newOrder.totalprice = "0.00$";
    newOrder.date = QDate::currentDate();
    newOrder.orderBooks.clear(); // Очищаем список книг заказа

    // Добавляем новый заказ в вектор заказов
    orders.push_back(newOrder);
    currentOrderIndex = orders.size()-1;
}

void OrderFormForUser::saveOrder() {
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
    Order_& currentOrder = orders[currentOrderIndex];
    QString _client = ui->client->text();
    currentOrder.client = _client;
    currentOrder.totalprice = ui->totalprice->text();
    currentOrder.date = ui->date->date();

    if(currentOrder.totalprice=="00,00$"){
        QMessageBox::information(this, "Ошибка", "Положите товары в корзину!");
        this->close();
        return;
    }

    int randomSellerId = 0;
    QSqlQuery randomSellerIdQuery;
    if (randomSellerIdQuery.exec("SELECT \"SellerID\" FROM \"Seller\" ORDER BY random() LIMIT 1") && randomSellerIdQuery.next()) {
        randomSellerId = randomSellerIdQuery.value(0).toInt();
    } else {
        // Если произошла ошибка при выполнении запроса, выводим сообщение об ошибке
        qDebug() << "Error retrieving random SellerID:" << randomSellerIdQuery.lastError().text();
    }


    // Сохраняем заказ в базе данных
    QSqlQuery query;
    if (!existingOrder) {
        QString _price = currentOrder.totalprice;
        _price.chop(1);
        // Если это новый заказ, добавляем его в базу данных
        QString insertQuery = QString("INSERT INTO \"Order\" (\"OrderID\", \"Client\", \"Seller\", \"TotalPrice\", \"Date\") "
                                      "VALUES (%1, '%2', '%3', '%4', '%5')")
                                  .arg(currentId).arg(CLIENT_ID).arg(randomSellerId)
                                  .arg(_price).arg(currentOrder.date.toString("yyyy-MM-dd"));
        if (!query.exec(insertQuery)) {
            qDebug() << "Error inserting order:" << query.lastError().text();
        } else {
            for (const _Book_& book : currentOrder.orderBooks) {
                QString insertBookOrderQuery = QString("INSERT INTO \"Book_Order\" (\"OrderID\", \"ISBN\") "
                                                        "VALUES (%1, '%2')")
                                                        .arg(currentId).arg(book.isbn);
                if (!query.exec(insertBookOrderQuery)) {
                    qDebug() << "Error inserting book into order:" << query.lastError().text();
                }
            }
            QString sellerLastName;
            QSqlQuery sellerQuery;
            if (sellerQuery.exec("SELECT \"LastName\" FROM \"Seller\" WHERE \"SellerID\" = " + QString::number(randomSellerId)) && sellerQuery.next()) {
                sellerLastName = sellerQuery.value(0).toString();
            } else {
                // Если произошла ошибка при выполнении запроса, выводим сообщение об ошибке
                qDebug() << "Error retrieving seller's last name:" << sellerQuery.lastError().text();
            }
            getQueue(currentId, _client, currentOrder.totalprice, currentOrder.date, sellerLastName, currentOrder.orderBooks);
            QMessageBox::information(this, "Успех", "Заказ оформлен! Кассовый чек\nотправлен Вам на почту");
            this->close();
        }
    } else {
        qDebug() << "Error";
    }
}

void OrderFormForUser::loadOrderBooks(Order_& order) {
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
        _Book_ book;
        book.isbn = query.value("ISBN").toString();
        book.title = query.value("Title").toString();
        book.price = query.value("Price").toString();
        order.orderBooks.append(book);
    }
}

void OrderFormForUser::addBookInOrder() {
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
            Order_& currentOrder = orders[currentOrderIndex];
            _Book_ newBook;
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


void OrderFormForUser::deleteBookFromOrder() {
    int selectedRow = ui->books->currentRow();
    if (selectedRow >= 0) {
        // Удаляем книгу из текущего заказа
        Order_& currentOrder = orders[currentOrderIndex];
        currentOrder.orderBooks.remove(selectedRow);

        recalculateTotalPrice();

        // Отображаем текущий заказ без удаленной книги
        displayOrder();
    }
}

void OrderFormForUser::findBooks() {
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

void OrderFormForUser::loadClients() {
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
    }
}

void OrderFormForUser::loadSellers() {
    // Очищаем вектор продавцов перед загрузкой новых данных
    sellers.clear();

    // Выполняем запрос к базе данных для получения всех продавцов
    QSqlQuery query("SELECT * FROM \"Seller\" ORDER BY \"SellerID\"");
    while (query.next()) {
        int sellerID = query.value("SellerID").toInt();
        QString sellerName = query.value("LastName").toString();
        sellers.append(qMakePair(sellerID, sellerName));
    }
}

void OrderFormForUser::recalculateTotalPrice() {
    double totalPrice = 0.0;
    Order_& currentOrder = orders[currentOrderIndex]; // Изменяем на немодифицируемый объект
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

void OrderFormForUser::getQueue(int currentID, const QString& client, const QString& totalprice, const QDate& date, const QString& seller, const QVector<_Book_>& orderBooks)
{
    // Создание имени файла с уникальным идентификатором заказа
    QString fileName = QString("F:/BookShop/Queues/%1.txt").arg(currentID);

    // Открытие файла для записи
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        // Создание текстового потока для записи данных в файл
        QTextStream out(&file);

        // Запись информации о заказе в файл
        out << "Номер заказа: " << currentID << "\n";
        out << "Дата заказа: " << date.toString("dd.MM.yyyy") << "\n";
        out << "Клиент: " << client << "\n\n";

        // Запись информации о книгах в заказе
        out << "Книги в заказе:\n";
        for (const _Book_& book : orderBooks) {
            QString _price = book.price;
            _price.chop(2);
            _price.append('$');
            out << "Название: " << book.title << "\n";
            out << "Цена: " << _price << "\n";
            out << "\n";
        }

        // Запись остальной информации о заказе
        QString _totalprice = totalprice;
        _totalprice.chop(2);
        _totalprice.append('$');
        out << "Общая стоимость: " << _totalprice << "\n";
        out << "Продавец: " << seller << "\n";

        // Закрытие файла после записи
        file.close();
    } else {
        // Если не удалось открыть файл для записи, выводим сообщение об ошибке
        qDebug() << "Error creating queue file.";
    }
}






