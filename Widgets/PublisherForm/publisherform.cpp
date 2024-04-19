#include "publisherform.h"
#include "ui_publisherform.h"

PublisherForm::PublisherForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PublisherForm)
{
    ui->setupUi(this);

    connect(ui->next, &QPushButton::clicked, this, &PublisherForm::moveToNextPublisher);
    connect(ui->previous, &QPushButton::clicked, this, &PublisherForm::moveToPreviousPublisher);
    connect(ui->add, &QPushButton::clicked, this, &PublisherForm::addPublisher);
    connect(ui->save, &QPushButton::clicked, this, &PublisherForm::savePublisher);
    connect(ui->delete_2, &QPushButton::clicked, this, &PublisherForm::deletePublisher);

    // Отобразим издательства при открытии формы
    showPublisherForm();
}

PublisherForm::~PublisherForm()
{
    delete ui;
}

void PublisherForm::showPublisherForm() {
    // Открываем соединение с базой данных
    db = QSqlDatabase::database();
    if (!db.isValid()) {
        qDebug() << "Error: Failed to get database connection.";
        return;
    }

    // Выполняем запрос к базе данных для получения данных из таблицы Publisher
    QSqlQuery query;
    if (!query.exec("SELECT \"PublisherID\", \"Name\" FROM \"Publisher\" ORDER BY \"PublisherID\" ASC")) {
        qDebug() << "Error executing query:" << query.lastError().text();
        return;
    }

    // Если запрос успешно выполнен и есть записи в результате
    if (query.next()) {
        // Получаем все записи о издательствах и сохраняем их для последующего использования
        do {
            int publisherID = query.value(0).toInt();
            QString publisherName = query.value(1).toString();
            publishers.append(qMakePair(publisherID, publisherName));
        } while (query.next());

        // Отображаем первое издательство
        currentPublisherIndex = 0;
        displayPublisher();
    }
}

void PublisherForm::moveToNextPublisher() {
    if (currentPublisherIndex < publishers.size() - 1) {
        ++currentPublisherIndex;
        displayPublisher();
    }
}

void PublisherForm::moveToPreviousPublisher() {
    if (currentPublisherIndex > 0) {
        --currentPublisherIndex;
        displayPublisher();
    }
}

void PublisherForm::displayPublisher() {
    int publisherID = publishers[currentPublisherIndex].first;
    QString publisherName = publishers[currentPublisherIndex].second;
    ui->publisher_id->setText(QString::number(publisherID));
    ui->publisher_name->setText(publisherName);
}

void PublisherForm::addPublisher() {
    // Получаем ID для нового издательства: находим максимальный ID из списка издательств и увеличиваем его на 1
    int maxID = 0;
    for (const auto& publisher : publishers) {
        maxID = qMax(maxID, publisher.first);
    }
    int nextID = maxID + 1;

    // Отображаем полученный ID в поле "publisher_id"
    ui->publisher_id->setText(QString::number(nextID));

    // Очищаем поле "publisher_name"
    ui->publisher_name->clear();
}

void PublisherForm::savePublisher() {
    // Получаем значения из полей "publisher_id" и "publisher_name"
    int publisherID = ui->publisher_id->text().toInt();
    QString publisherName = ui->publisher_name->text();

    // Проверяем, существует ли уже запись с таким ID в базе данных
    bool existingPublisher = false;
    for (auto& publisher : publishers) {
        if (publisher.first == publisherID) {
            existingPublisher = true;
            // Если запись найдена, обновляем ее в локальном списке
            publisher.second = publisherName;
            break;
        }
    }

    QSqlQuery query;
    if (existingPublisher) {
        // Обновляем существующую запись в базе данных
        QString updateQuery = QString("UPDATE \"Publisher\" SET \"Name\" = '%1' WHERE \"PublisherID\" = %2")
                                  .arg(publisherName).arg(publisherID);
        if (!query.exec(updateQuery)) {
            qDebug() << "Error updating publisher:" << query.lastError().text();
        } else {
            QMessageBox::information(this, "Успех", "Запись успешно обновлена!");
        }
    } else {
        // Добавляем новую запись в базу данных
        QString insertQuery = QString("INSERT INTO \"Publisher\" (\"PublisherID\", \"Name\") VALUES (%1, '%2')")
                                  .arg(publisherID).arg(publisherName);
        if (!query.exec(insertQuery)) {
            qDebug() << "Error inserting publisher:" << query.lastError().text();
        } else {
            // Если запись успешно добавлена, добавляем ее в локальный список издательств
            publishers.append(qMakePair(publisherID, publisherName));
            QMessageBox::information(this, "Успех", "Запись добавлена!");
        }
    }

    // Находим индекс только что сохраненного или обновленного издательства в локальном списке
    int index = -1;
    for (int i = 0; i < publishers.size(); ++i) {
        if (publishers[i].first == publisherID) {
            index = i;
            break;
        }
    }

    // Если элемент найден, отображаем его на форме
    if (index != -1) {
        currentPublisherIndex = index;
        displayPublisher();
    }
}



void PublisherForm::deletePublisher() {
    // Получаем ID текущего издательства
    int publisherID = ui->publisher_id->text().toInt();

    QSqlQuery query;
    QString deleteQuery = QString("DELETE FROM \"Publisher\" WHERE \"PublisherID\" = %1").arg(publisherID);
    if (!query.exec(deleteQuery)) {
        qDebug() << "Error deleting publisher:" << query.lastError().text();
        QMessageBox::information(this, "Ошибка", "Нельзя удалить запись, так как на нее ссылаются элементы таблицы \"Книга\"!");
    } else {
        // Удаляем издательство из локального списка
        for (auto it = publishers.begin(); it != publishers.end(); ++it) {
            if ((*it).first == publisherID) {
                it = publishers.erase(it);
                break;
            }
        }

        // Очищаем поля формы
        ui->publisher_id->clear();
        ui->publisher_name->clear();

        // Перемещаемся к предыдущему издательству или следующему, если предыдущего нет
        if (currentPublisherIndex > 0) {
            --currentPublisherIndex;
        } else if (!publishers.isEmpty()) {
            ++currentPublisherIndex;
        }

        QMessageBox::information(this, "Успех", "Запись удалена!");
        // Отображаем текущее издательство
        displayPublisher();
    }
}

