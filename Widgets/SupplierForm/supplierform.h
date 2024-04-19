#ifndef SUPPLIERFORM_H
#define SUPPLIERFORM_H

#include <QDialog>
#include <QtSql>
#include <QMessageBox>

namespace Ui {
class SupplierForm;
}

class SupplierForm : public QDialog
{
    Q_OBJECT

public:
    explicit SupplierForm(QWidget *parent = nullptr);
    ~SupplierForm();

    void showSupplierForm();

private slots:
    void moveToNextSupplier();
    void moveToPreviousSupplier();
    void displaySupplier(); // Добавляем новый приватный слот для отображения текущего поставщика
    void addSupplier(); // Добавляем слот для кнопки "Add"
    void saveSupplier(); // Добавляем слот для кнопки "Save"
    void deleteSupplier(); // Добавляем слот для кнопки "Delete"

private:
    Ui::SupplierForm *ui;
    QSqlDatabase db;
    QVector<QPair<int, QString>> suppliers; // Вектор для хранения данных о поставщиках
    int currentSupplierIndex; // Индекс текущего поставщика
};

#endif // SUPPLIERFORM_H


