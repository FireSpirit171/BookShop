#include "bookform.h"
#include "ui_bookform.h"
#include <iostream>
#include <QDebug>

BookForm::BookForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BookForm)
{
    ui->setupUi(this);

    // Подключаем слоты к кнопкам
    connect(ui->next, &QPushButton::clicked, this, &BookForm::moveToNextBook);
    connect(ui->previous, &QPushButton::clicked, this, &BookForm::moveToPreviousBook);
    connect(ui->add, &QPushButton::clicked, this, &BookForm::addBook);
    connect(ui->save, &QPushButton::clicked, this, &BookForm::saveBook);
    connect(ui->delete_2, &QPushButton::clicked, this, &BookForm::deleteBook);

    loadPublishers();
    loadSuppliers();

    // Отображаем книги при открытии формы
    showBookForm();
}

BookForm::~BookForm()
{
    delete ui;
}

void BookForm::showBookForm() {
    // Открываем соединение с базой данных
    db = QSqlDatabase::database();
    if (!db.isValid()) {
        qDebug() << "Error: Failed to get database connection.";
        return;
    }

    // Выполняем запрос к базе данных для получения данных из таблицы Book
    QSqlQuery query;
    if (!query.exec("SELECT * FROM \"Book\" ORDER BY \"Title\" ASC")) {
        qDebug() << "Error executing query:" << query.lastError().text();
        return;
    }

    // Если запрос успешно выполнен и есть записи в результате
    if (query.next()) {
        // Получаем все записи о книгах и сохраняем их для последующего использования
        do {
            Book book;
            book.isbn = query.value("ISBN").toString();
            book.title = query.value("Title").toString();
            book.author = query.value("Author").toString();
            QString priceString = query.value("Price").toString();
            priceString.chop(2); // Обрезаем последние два символа
            book.price = priceString;
            book.genre = query.value("Genre").toString();
            book.publisher = query.value("Publisher").toInt();
            book.supplier = query.value("Supplier").toInt();
            books.append(book);
        } while (query.next());

        // Отображаем первую книгу
        currentBookIndex = 0;
        displayBook();
    }
}

void BookForm::moveToNextBook() {
    if (currentBookIndex < books.size() - 1) {
        ++currentBookIndex;
        displayBook();
    }
}

void BookForm::moveToPreviousBook() {
    if (currentBookIndex > 0) {
        --currentBookIndex;
        displayBook();
    }
}

void BookForm::addBook() {
    // Очищаем поля ввода
    ui->isbn->clear();
    ui->title->clear();
    ui->author->clear();
    ui->price->clear();
    ui->price->setInputMask("00,00$");
    ui->price->setText("00,00$");
    ui->genre->clear();

    // Устанавливаем текущий индекс в -1 для выпадающих списков, чтобы они стали пустыми
    ui->publisher->setCurrentIndex(-1);
    ui->supplier->setCurrentIndex(-1);
}


void BookForm::displayBook() {
    // Получаем данные текущей книги
    const Book& currentBook = books[currentBookIndex];

    // Отображаем данные текущей книги на форме
    ui->isbn->setText(currentBook.isbn);
    ui->title->setText(currentBook.title);
    ui->author->setText(currentBook.author);
    QString _price = currentBook.price;
    _price.append('$');
    ui->price->setText(_price);
    ui->genre->setText(currentBook.genre);

    // Находим имя издательства по его ID
    QString publisherName;
    for (const auto& publisher : publishers) {
        if (publisher.first == currentBook.publisher) {
            publisherName = publisher.second;
            break;
        }
    }
    ui->publisher->setCurrentText(publisherName);

    // Находим имя поставщика по его ID
    QString supplierName;
    for (const auto& supplier : suppliers) {
        if (supplier.first == currentBook.supplier) {
            supplierName = supplier.second;
            break;
        }
    }
    ui->supplier->setCurrentText(supplierName);
}

void BookForm::saveBook() {
    // Получаем значения из полей
    QString isbn = ui->isbn->text();
    QString title = ui->title->text();
    QString author = ui->author->text();
    QString price = ui->price->text();
    price.chop(1);
    QString genre = ui->genre->text();
    int publisherID = ui->publisher->currentData().toInt();
    int supplierID = ui->supplier->currentData().toInt();

    // Проверяем, существует ли уже запись с таким ISBN в базе данных
    bool existingBook = false;
    for (auto& book : books) {
        if (book.isbn == isbn) {
            // Если запись найдена, обновляем ее в локальном списке
            book.title = title;
            book.author = author;
            book.price = price;
            book.genre = genre;
            book.publisher = publisherID;
            book.supplier = supplierID;
            existingBook = true;
            break;
        }
    }

    QSqlQuery query;
    if (existingBook) {
        // Обновляем существующую запись в базе данных
        QString updateQuery = QString("UPDATE \"Book\" SET "
                                      "\"Title\" = '%1', "
                                      "\"Author\" = '%2', "
                                      "\"Price\" = '%3', "
                                      "\"Genre\" = '%4', "
                                      "\"Publisher\" = '%5', "
                                      "\"Supplier\" = '%6' "
                                      "WHERE \"ISBN\" = '%7'")
                                  .arg(title).arg(author).arg(price)
                                  .arg(genre).arg(publisherID).arg(supplierID)
                                  .arg(isbn);
        if (!query.exec(updateQuery)) {
            qDebug() << "Error updating book:" << query.lastError().text();
        } else {
            QMessageBox::information(this, "Успех", "Запись успешно обновлена!");
        }
    } else {
        // Добавляем новую запись в базу данных
        QString insertQuery = QString("INSERT INTO \"Book\" (\"ISBN\", \"Title\", \"Author\", "
                                      "\"Price\", \"Genre\", \"Publisher\", \"Supplier\") "
                                      "VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7')")
                                  .arg(isbn).arg(title).arg(author).arg(price)
                                  .arg(genre).arg(publisherID).arg(supplierID);
        if (!query.exec(insertQuery)) {
            qDebug() << "Error inserting book:" << query.lastError().text();
        } else {
            // Если запись успешно добавлена, обновляем локальный список
            Book newBook;
            newBook.isbn = isbn;
            newBook.title = title;
            newBook.author = author;
            newBook.price = price;
            newBook.genre = genre;
            newBook.publisher = publisherID;
            newBook.supplier = supplierID;
            books.append(newBook);
            QMessageBox::information(this, "Успех", "Запись добавлена!");
        }
    }

    // Находим индекс только что сохраненной или обновленной книги в локальном списке
    int index = -1;
    for (int i = 0; i < books.size(); ++i) {
        if (books[i].isbn == isbn) {
            index = i;
            break;
        }
    }

    // Если элемент найден, отображаем его на форме
    if (index != -1) {
        currentBookIndex = index;
        displayBook();
    }
}

void BookForm::deleteBook() {
    // Получаем ISBN текущей книги
    QString isbn = ui->isbn->text();

    QSqlQuery query;
    QString deleteQuery = QString("DELETE FROM \"Book\" WHERE \"ISBN\" = '%1'").arg(isbn);
    if (!query.exec(deleteQuery)) {
        qDebug() << "Error deleting book:" << query.lastError().text();
        QMessageBox::information(this, "Ошибка", "Нельзя удалить запись, так как на нее ссылаются элементы таблицы \"Order\"!");
    } else {
        // Удаляем книгу из локального списка
        for (auto it = books.begin(); it != books.end(); ++it) {
            if ((*it).isbn == isbn) {
                it = books.erase(it);
                break;
            }
        }

        // Очищаем поля формы
        ui->isbn->clear();
        ui->title->clear();
        ui->author->clear();
        ui->price->clear();
        ui->genre->clear();
        ui->publisher->setCurrentIndex(-1);
        ui->supplier->setCurrentIndex(-1);

        // Перемещаемся к предыдущей книге или следующей, если предыдущей нет
        if (currentBookIndex > 0) {
            --currentBookIndex;
        } else if (!books.isEmpty()) {
            ++currentBookIndex;
        }

        // Отображаем текущую книгу
        displayBook();

        QMessageBox::information(this, "Успех", "Запись удалена!");
    }
}

void BookForm::loadPublishers() {
    // Очищаем вектор издателей перед загрузкой новых данных
    publishers.clear();

    // Очищаем комбобокс издателей перед загрузкой новых данных
    ui->publisher->clear();

    // Выполняем запрос к базе данных для получения всех издательств
    QSqlQuery query("SELECT * FROM \"Publisher\" ORDER BY \"PublisherID\"");
    while (query.next()) {
        int publisherID = query.value("PublisherID").toInt();
        QString publisherName = query.value("Name").toString();
        publishers.append(qMakePair(publisherID, publisherName));
        ui->publisher->addItem(publisherName, publisherID);
    }
}

void BookForm::loadSuppliers() {
    // Очищаем вектор поставщиков перед загрузкой новых данных
    suppliers.clear();

    // Очищаем комбобокс поставщиков перед загрузкой новых данных
    ui->supplier->clear();

    // Выполняем запрос к базе данных для получения всех поставщиков
    QSqlQuery query("SELECT * FROM \"Supplier\" ORDER BY \"SupplierID\"");
    while (query.next()) {
        int supplierID = query.value("SupplierID").toInt();
        QString supplierName = query.value("Name").toString();
        suppliers.append(qMakePair(supplierID, supplierName));
        ui->supplier->addItem(supplierName, supplierID);
    }
}

