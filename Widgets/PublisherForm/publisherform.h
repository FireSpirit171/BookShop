#ifndef PUBLISHERFORM_H
#define PUBLISHERFORM_H

#include <QDialog>
#include <QtSql>
#include <QMessageBox>

namespace Ui {
class PublisherForm;
}

class PublisherForm : public QDialog
{
    Q_OBJECT

public:
    explicit PublisherForm(QWidget *parent = nullptr);
    ~PublisherForm();

    void showPublisherForm();

private slots:
    void moveToNextPublisher();
    void moveToPreviousPublisher();
    void displayPublisher(); // Добавляем новый приватный слот для отображения текущего издательства
    void addPublisher(); // Добавляем слот для кнопки "Add"
    void savePublisher(); // Добавляем слот для кнопки "Save"
    void deletePublisher(); // Добавляем слот для кнопки "Delete"

private:
    Ui::PublisherForm *ui;
    QSqlDatabase db;
    QVector<QPair<int, QString>> publishers; // Вектор для хранения данных об издательствах
    int currentPublisherIndex; // Индекс текущего издательства
};

#endif // PUBLISHERFORM_H
