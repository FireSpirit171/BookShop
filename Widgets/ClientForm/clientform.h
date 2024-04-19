#ifndef CLIENTFORM_H
#define CLIENTFORM_H

#include <QDialog>
#include <QtSql>
#include <QMessageBox>

namespace Ui {
class ClientForm;
}

struct Client {
    int clientID;
    QString lastName;
    QString firstName;
    QString middleName;
    QDate dateOfBirth;
    QString gender;
    QString phoneNumber;
};

class ClientForm : public QDialog
{
    Q_OBJECT

public:
    explicit ClientForm(QWidget *parent = nullptr);
    ~ClientForm();

    void showClientForm();

private slots:
    void moveToNextClient();
    void moveToPreviousClient();
    void displayClient(); // Добавляем новый приватный слот для отображения текущего клиента
    void addClient(); // Добавляем слот для кнопки "Add"
    void saveClient(); // Добавляем слот для кнопки "Save"
    void deleteClient(); // Добавляем слот для кнопки "Delete"

private:
    Ui::ClientForm *ui;
    QSqlDatabase db;
    QVector<Client> clients; // Вектор для хранения данных о клиентах
    int currentClientIndex; // Индекс текущего клиента
};

#endif // CLIENTFORM_H
