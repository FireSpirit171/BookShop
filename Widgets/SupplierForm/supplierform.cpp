#include "supplierform.h"
#include "ui_supplierform.h"

SupplierForm::SupplierForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SupplierForm)
{
    ui->setupUi(this);

    connect(ui->next, &QPushButton::clicked, this, &SupplierForm::moveToNextSupplier);
    connect(ui->previous, &QPushButton::clicked, this, &SupplierForm::moveToPreviousSupplier);
    connect(ui->add, &QPushButton::clicked, this, &SupplierForm::addSupplier);
    connect(ui->save, &QPushButton::clicked, this, &SupplierForm::saveSupplier);
    connect(ui->delete_2, &QPushButton::clicked, this, &SupplierForm::deleteSupplier);

    showSupplierForm();
}

SupplierForm::~SupplierForm()
{
    delete ui;
}

void SupplierForm::showSupplierForm() {
    db = QSqlDatabase::database();
    if (!db.isValid()) {
        qDebug() << "Error: Failed to get database connection.";
        return;
    }

    QSqlQuery query;
    if (!query.exec("SELECT \"SupplierID\", \"Name\" FROM \"Supplier\" ORDER BY \"SupplierID\" ASC")) {
        qDebug() << "Error executing query:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        int supplierID = query.value(0).toInt();
        QString supplierName = query.value(1).toString();
        suppliers.append(qMakePair(supplierID, supplierName));
    }

    if (!suppliers.isEmpty()) {
        currentSupplierIndex = 0;
        displaySupplier();
    }
}

void SupplierForm::moveToNextSupplier() {
    if (currentSupplierIndex < suppliers.size() - 1) {
        ++currentSupplierIndex;
        displaySupplier();
    }
}

void SupplierForm::moveToPreviousSupplier() {
    if (currentSupplierIndex > 0) {
        --currentSupplierIndex;
        displaySupplier();
    }
}

void SupplierForm::displaySupplier() {
    ui->supplier_id->setText(QString::number(suppliers[currentSupplierIndex].first));
    ui->supplier_name->setText(suppliers[currentSupplierIndex].second);
}

void SupplierForm::addSupplier() {
    int newID = suppliers.isEmpty() ? 1 : suppliers.last().first + 1;
    ui->supplier_id->setText(QString::number(newID));
    ui->supplier_name->clear();
}

void SupplierForm::saveSupplier() {
    int supplierID = ui->supplier_id->text().toInt();
    QString supplierName = ui->supplier_name->text();

    bool existingSupplier = false;
    for (auto& supplier : suppliers) {
        if (supplier.first == supplierID) {
            existingSupplier = true;
            supplier.second = supplierName;
            break;
        }
    }

    QSqlQuery query;
    if (existingSupplier) {
        QString updateQuery = QString("UPDATE \"Supplier\" SET \"Name\" = '%1' WHERE \"SupplierID\" = %2")
                                  .arg(supplierName).arg(supplierID);
        if (!query.exec(updateQuery)) {
            qDebug() << "Error updating supplier:" << query.lastError().text();
        } else {
            QMessageBox::information(this, "Успех", "Запись успешно обновлена!");
        }
    } else {
        QString insertQuery = QString("INSERT INTO \"Supplier\" (\"SupplierID\", \"Name\") VALUES (%1, '%2')")
                                  .arg(supplierID).arg(supplierName);
        if (!query.exec(insertQuery)) {
            qDebug() << "Error inserting supplier:" << query.lastError().text();
        } else {
            suppliers.append(qMakePair(supplierID, supplierName));
            QMessageBox::information(this, "Успех", "Запись добавлена!");
        }
    }

    int index = -1;
    for (int i = 0; i < suppliers.size(); ++i) {
        if (suppliers[i].first == supplierID) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        currentSupplierIndex = index;
        displaySupplier();
    }
}

void SupplierForm::deleteSupplier() {
    int supplierID = ui->supplier_id->text().toInt();

    QSqlQuery query;
    QString deleteQuery = QString("DELETE FROM \"Supplier\" WHERE \"SupplierID\" = %1").arg(supplierID);
    if (!query.exec(deleteQuery)) {
        qDebug() << "Error deleting supplier:" << query.lastError().text();
        QMessageBox::information(this, "Ошибка", "Нельзя удалить запись, так как на нее ссылаются\n элементы таблицы \"Книга\" и \"Машина\"!");
    } else {
        for (auto it = suppliers.begin(); it != suppliers.end(); ++it) {
            if ((*it).first == supplierID) {
                it = suppliers.erase(it);
                break;
            }
        }

        ui->supplier_id->clear();
        ui->supplier_name->clear();

        if (!suppliers.isEmpty()) {
            if (currentSupplierIndex > 0) {
                --currentSupplierIndex;
            } else if (!suppliers.isEmpty()) {
                ++currentSupplierIndex;
            }
            displaySupplier();
        }
        QMessageBox::information(this, "Успех", "Запись удалена!");
    }
}
