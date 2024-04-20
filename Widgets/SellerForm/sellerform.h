#ifndef SELLERFORM_H
#define SELLERFORM_H

#include <QDialog>
#include <QtSql>
#include <QMessageBox>

namespace Ui {
class SellerForm;
}

struct Seller {
    int sellerID;
    QString lastName;
    QString firstName;
    QString middleName;
    QString phoneNumber;
};

class SellerForm : public QDialog
{
    Q_OBJECT

public:
    explicit SellerForm(QWidget *parent = nullptr);
    ~SellerForm();

    void showSellerForm();

private slots:
    void moveToNextClient();
    void moveToPreviousClient();
    void displaySeller(); // Добавляем новый приватный слот для отображения текущего сотрудника
    void addSeller(); // Добавляем слот для кнопки "Add"
    void saveSeller(); // Добавляем слот для кнопки "Save"
    void deleteSeller(); // Добавляем слот для кнопки "Delete"

private:
    Ui::SellerForm *ui;
    QSqlDatabase db;
    QVector<Seller> sellers; // Вектор для хранения данных о клиентах
    int currentSellerIndex; // Индекс текущего клиента
};

#endif // SELLERFORM_H
