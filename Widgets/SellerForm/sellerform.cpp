#include "sellerform.h"
#include "ui_sellerform.h"

SellerForm::SellerForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SellerForm)
{
    ui->setupUi(this);

    // Подключаем слоты к кнопкам
    connect(ui->next, &QPushButton::clicked, this, &SellerForm::moveToNextClient);
    connect(ui->previous, &QPushButton::clicked, this, &SellerForm::moveToPreviousClient);
    connect(ui->add, &QPushButton::clicked, this, &SellerForm::addSeller);
    connect(ui->save, &QPushButton::clicked, this, &SellerForm::saveSeller);
    connect(ui->delete_2, &QPushButton::clicked, this, &SellerForm::deleteSeller);

    // Отображаем продавцов при открытии формы
    showSellerForm();
}

SellerForm::~SellerForm()
{
    delete ui;
}

void SellerForm::showSellerForm() {
    // Открываем соединение с базой данных
    db = QSqlDatabase::database();
    if (!db.isValid()) {
        qDebug() << "Error: Failed to get database connection.";
        return;
    }

    // Выполняем запрос к базе данных для получения данных из таблицы Seller
    QSqlQuery query;
    if (!query.exec("SELECT * FROM \"Seller\" ORDER BY \"LastName\" ASC")) {
        qDebug() << "Error executing query:" << query.lastError().text();
        return;
    }

    // Если запрос успешно выполнен и есть записи в результате
    if (query.next()) {
        // Получаем все записи о продавцах и сохраняем их для последующего использования
        do {
            Seller seller;
            seller.sellerID = query.value("SellerID").toInt();
            seller.lastName = query.value("LastName").toString();
            seller.firstName = query.value("FirstName").toString();
            seller.middleName = query.value("MiddleName").toString();
            seller.phoneNumber = query.value("PhoneNumber").toString();
            sellers.append(seller);
        } while (query.next());

        // Отображаем первого продавца
        currentSellerIndex = 0;
        displaySeller();
    }
}

void SellerForm::moveToNextClient() {
    if (currentSellerIndex < sellers.size() - 1) {
        ++currentSellerIndex;
        displaySeller();
    }
}

void SellerForm::moveToPreviousClient() {
    if (currentSellerIndex > 0) {
        --currentSellerIndex;
        displaySeller();
    }
}

void SellerForm::addSeller() {
    // Очищаем поля формы
    ui->seller_id->clear();
    ui->last_name->clear();
    ui->first_name->clear();
    ui->middle_name->clear();
    ui->phone_number->clear();
}

void SellerForm::displaySeller() {
    // Получаем данные текущего продавца
    const Seller& currentSeller = sellers[currentSellerIndex];

    // Отображаем данные текущего продавца на форме
    ui->seller_id->setText(QString::number(currentSeller.sellerID));
    ui->last_name->setText(currentSeller.lastName);
    ui->first_name->setText(currentSeller.firstName);
    ui->middle_name->setText(currentSeller.middleName);
    ui->phone_number->setText(currentSeller.phoneNumber);
}

void SellerForm::saveSeller() {
    // Получаем значения из полей
    int sellerID = ui->seller_id->text().toInt();
    QString lastName = ui->last_name->text();
    QString firstName = ui->first_name->text();
    QString middleName = ui->middle_name->text();
    QString phoneNumber = ui->phone_number->text();

    // Проверяем, существует ли уже запись с таким ID в базе данных
    bool existingSeller = false;
    for (auto& seller : sellers) {
        if (seller.sellerID == sellerID) {
            // Если запись найдена, обновляем ее в локальном списке
            seller.lastName = lastName;
            seller.firstName = firstName;
            seller.middleName = middleName;
            seller.phoneNumber = phoneNumber;
            existingSeller = true;
            break;
        }
    }

    QSqlQuery query;
    if (existingSeller) {
        // Обновляем существующую запись в базе данных
        QString updateQuery = QString("UPDATE \"Seller\" SET "
                                      "\"LastName\" = '%1', "
                                      "\"FirstName\" = '%2', "
                                      "\"MiddleName\" = '%3', "
                                      "\"PhoneNumber\" = '%4' "
                                      "WHERE \"SellerID\" = %5")
                                  .arg(lastName).arg(firstName).arg(middleName)
                                  .arg(phoneNumber).arg(sellerID);
        if (!query.exec(updateQuery)) {
            qDebug() << "Error updating seller:" << query.lastError().text();
        } else {
            QMessageBox::information(this, "Успех", "Запись успешно обновлена!");
        }
    } else {
        // Добавляем новую запись в базу данных
        QString insertQuery = QString("INSERT INTO \"Seller\" (\"SellerID\", \"LastName\", \"FirstName\", \"MiddleName\", "
                                      "\"PhoneNumber\") "
                                      "VALUES ('%1', '%2', '%3', '%4', '%5')")
                                  .arg(sellerID).arg(lastName).arg(firstName).arg(middleName).arg(phoneNumber);
        if (!query.exec(insertQuery)) {
            qDebug() << "Error inserting seller:" << query.lastError().text();
        } else {
            // Если запись успешно добавлена, получаем ее ID из базы данных и обновляем локальный список
            Seller newSeller;
            newSeller.sellerID = sellerID;
            newSeller.lastName = lastName;
            newSeller.firstName = firstName;
            newSeller.middleName = middleName;
            newSeller.phoneNumber = phoneNumber;
            sellers.append(newSeller);
            QMessageBox::information(this, "Успех", "Запись добавлена!");
        }
    }

    // Находим индекс только что сохраненного или обновленного продавца в локальном списке
    int index = -1;
    for (int i = 0; i < sellers.size(); ++i) {
        if (sellers[i].sellerID == sellerID) {
            index = i;
            break;
        }
    }

    // Если элемент найден, отображаем его на форме
    if (index != -1) {
        currentSellerIndex = index;
        displaySeller();
    }
}

void SellerForm::deleteSeller() {
    // Получаем ID текущего продавца
    int sellerID = ui->seller_id->text().toInt();

    QSqlQuery query;
    QString deleteQuery = QString("DELETE FROM \"Seller\" WHERE \"SellerID\" = %1").arg(sellerID);
    if (!query.exec(deleteQuery)) {
        qDebug() << "Error deleting seller:" << query.lastError().text();
        QMessageBox::information(this, "Ошибка", "Нельзя удалить запись, так как на нее\n ссылаются элементы таблицы \"Заказ\"!");
    } else {
        // Удаляем продавца из локального списка
        for (auto it = sellers.begin(); it != sellers.end(); ++it) {
            if ((*it).sellerID == sellerID) {
                it = sellers.erase(it);
                break;
            }
        }

        // Очищаем поля формы
        ui->seller_id->clear();
        ui->last_name->clear();
        ui->first_name->clear();
        ui->middle_name->clear();
        ui->phone_number->clear();

        // Перемещаемся к предыдущему продавцу или следующему, если предыдущего нет
        if (currentSellerIndex > 0) {
            --currentSellerIndex;
        } else if (!sellers.isEmpty()) {
            ++currentSellerIndex;
        }

        // Отображаем текущего продавца
        displaySeller();

        QMessageBox::information(this, "Успех", "Запись удалена!");
    }
}
