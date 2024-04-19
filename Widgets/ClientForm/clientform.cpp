#include "clientform.h"
#include "ui_clientform.h"

ClientForm::ClientForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientForm)
{
    ui->setupUi(this);

    // Подключаем слоты к кнопкам
    connect(ui->next, &QPushButton::clicked, this, &ClientForm::moveToNextClient);
    connect(ui->previous, &QPushButton::clicked, this, &ClientForm::moveToPreviousClient);
    connect(ui->add, &QPushButton::clicked, this, &ClientForm::addClient);
    connect(ui->save, &QPushButton::clicked, this, &ClientForm::saveClient);
    connect(ui->delete_2, &QPushButton::clicked, this, &ClientForm::deleteClient);

    // Отображаем клиентов при открытии формы
    showClientForm();
}

ClientForm::~ClientForm()
{
    delete ui;
}

void ClientForm::showClientForm() {
    // Открываем соединение с базой данных
    db = QSqlDatabase::database();
    if (!db.isValid()) {
        qDebug() << "Error: Failed to get database connection.";
        return;
    }

    // Выполняем запрос к базе данных для получения данных из таблицы Client
    QSqlQuery query;
    if (!query.exec("SELECT * FROM \"Client\" ORDER BY \"ClientID\" ASC")) {
        qDebug() << "Error executing query:" << query.lastError().text();
        return;
    }

    // Если запрос успешно выполнен и есть записи в результате
    if (query.next()) {
        // Получаем все записи о клиентах и сохраняем их для последующего использования
        do {
            Client client;
            client.clientID = query.value("ClientID").toInt();
            client.lastName = query.value("LastName").toString();
            client.firstName = query.value("FirstName").toString();
            client.middleName = query.value("MiddleName").toString();
            client.dateOfBirth = query.value("DateOfBirth").toDate();
            client.gender = query.value("Gender").toString();
            client.phoneNumber = query.value("PhoneNumber").toString();
            clients.append(client);
        } while (query.next());

        // Отображаем первого клиента
        currentClientIndex = 0;
        displayClient();
    }
}

void ClientForm::moveToNextClient() {
    if (currentClientIndex < clients.size() - 1) {
        ++currentClientIndex;
        displayClient();
    }
}

void ClientForm::moveToPreviousClient() {
    if (currentClientIndex > 0) {
        --currentClientIndex;
        displayClient();
    }
}

void ClientForm::addClient() {
    // Получаем ID для нового издательства: находим максимальный ID из списка издательств и увеличиваем его на 1
    int maxID = 0;
    for (const auto& client : clients) {
        maxID = qMax(maxID, client.clientID);
    }
    int nextID = maxID + 1;

    ui->client_id->setText(QString::number(nextID));
    ui->last_name->clear();
    ui->first_name->clear();
    ui->middle_name->clear();
    ui->date_of_birth->setDate(QDate::currentDate());
    ui->male->setChecked(false);
    ui->female->setChecked(false);
    ui->phone_number->clear();
}

void ClientForm::displayClient() {
    // Получаем данные текущего клиента
    const Client& currentClient = clients[currentClientIndex];

    // Отображаем данные текущего клиента на форме
    ui->client_id->setText(QString::number(currentClient.clientID));
    ui->last_name->setText(currentClient.lastName);
    ui->first_name->setText(currentClient.firstName);
    ui->middle_name->setText(currentClient.middleName);
    ui->date_of_birth->setDate(currentClient.dateOfBirth);
    if (currentClient.gender == "Муж") {
        ui->male->setChecked(true);
    } else if (currentClient.gender == "Жен") {
        ui->female->setChecked(true);
    }
    ui->phone_number->setText(currentClient.phoneNumber);
}

void ClientForm::saveClient() {
    // Получаем значения из полей
    int clientID = ui->client_id->text().toInt();
    QString lastName = ui->last_name->text();
    QString firstName = ui->first_name->text();
    QString middleName = ui->middle_name->text();
    QDate dateOfBirth = ui->date_of_birth->date();
    QString gender = ui->male->isChecked() ? "Муж" : "Жен";
    QString phoneNumber = ui->phone_number->text();

    // Проверяем, существует ли уже запись с таким ID в базе данных
    bool existingClient = false;
    for (auto& client : clients) {
        if (client.clientID == clientID) {
            // Если запись найдена, обновляем ее в локальном списке
            client.lastName = lastName;
            client.firstName = firstName;
            client.middleName = middleName;
            client.dateOfBirth = dateOfBirth;
            client.gender = gender;
            client.phoneNumber = phoneNumber;
            existingClient = true;
            break;
        }
    }

    QSqlQuery query;
    if (existingClient) {
        // Обновляем существующую запись в базе данных
        QString updateQuery = QString("UPDATE \"Client\" SET "
                                      "\"LastName\" = '%1', "
                                      "\"FirstName\" = '%2', "
                                      "\"MiddleName\" = '%3', "
                                      "\"DateOfBirth\" = '%4', "
                                      "\"Gender\" = '%5', "
                                      "\"PhoneNumber\" = '%6' "
                                      "WHERE \"ClientID\" = %7")
                                  .arg(lastName).arg(firstName).arg(middleName)
                                  .arg(dateOfBirth.toString(Qt::ISODate)).arg(gender)
                                  .arg(phoneNumber).arg(clientID);
        if (!query.exec(updateQuery)) {
            qDebug() << "Error updating client:" << query.lastError().text();
        } else {
            QMessageBox::information(this, "Успех", "Запись успешно обновлена!");
        }
    } else {
        // Добавляем новую запись в базу данных
        QString insertQuery = QString("INSERT INTO \"Client\" (\"LastName\", \"FirstName\", \"MiddleName\", "
                                      "\"DateOfBirth\", \"Gender\", \"PhoneNumber\") "
                                      "VALUES ('%1', '%2', '%3', '%4', '%5', '%6')")
                                  .arg(lastName).arg(firstName).arg(middleName)
                                  .arg(dateOfBirth.toString(Qt::ISODate)).arg(gender).arg(phoneNumber);
        if (!query.exec(insertQuery)) {
            qDebug() << "Error inserting client:" << query.lastError().text();
        } else {
            // Если запись успешно добавлена, получаем ее ID из базы данных и обновляем локальный список
            int newClientID = query.lastInsertId().toInt();
            Client newClient;
            newClient.clientID = newClientID;
            newClient.lastName = lastName;
            newClient.firstName = firstName;
            newClient.middleName = middleName;
            newClient.dateOfBirth = dateOfBirth;
            newClient.gender = gender;
            newClient.phoneNumber = phoneNumber;
            clients.append(newClient);
            QMessageBox::information(this, "Успех", "Запись добавлена!");
        }
    }

    // Находим индекс только что сохраненного или обновленного издательства в локальном списке
    int index = -1;
    for (int i = 0; i < clients.size(); ++i) {
        if (clients[i].clientID == clientID) {
            index = i;
            break;
        }
    }

    // Если элемент найден, отображаем его на форме
    if (index != -1) {
        currentClientIndex = index;
        displayClient();
    }
}

void ClientForm::deleteClient() {
    // Получаем ID текущего клиента
    int clientID = ui->client_id->text().toInt();

    QSqlQuery query;
    QString deleteQuery = QString("DELETE FROM \"Client\" WHERE \"ClientID\" = %1").arg(clientID);
    if (!query.exec(deleteQuery)) {
        qDebug() << "Error deleting client:" << query.lastError().text();
        QMessageBox::information(this, "Ошибка", "Нельзя удалить запись, так как на нее ссылаются элементы таблицы \"Заказ\"!");
    } else {
        // Удаляем клиента из локального списка
        for (auto it = clients.begin(); it != clients.end(); ++it) {
            if ((*it).clientID == clientID) {
                it = clients.erase(it);
                break;
            }
        }

        // Очищаем поля формы
        ui->client_id->clear();
        ui->last_name->clear();
        ui->first_name->clear();
        ui->middle_name->clear();
        ui->date_of_birth->setDate(QDate::currentDate());
        ui->male->setChecked(false);
        ui->female->setChecked(false);
        ui->phone_number->clear();

        // Перемещаемся к предыдущему клиенту или следующему, если предыдущего нет
        if (currentClientIndex > 0) {
            --currentClientIndex;
        } else if (!clients.isEmpty()) {
            ++currentClientIndex;
        }

        // Отображаем текущего клиента
        displayClient();

        QMessageBox::information(this, "Успех", "Запись удалена!");
    }
}
