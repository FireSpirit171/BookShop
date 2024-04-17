#ifndef PUBLISHERBOOKS_H
#define PUBLISHERBOOKS_H

#include <QDialog>
#include <QtSql>
#include <QTableWidget>
#include <QTextBrowser>

namespace Ui {
class PublisherBooks;
}

class PublisherBooks : public QDialog
{
    Q_OBJECT

public:
    explicit PublisherBooks(QWidget *parent = nullptr);
    ~PublisherBooks();

    void showPublisherBooks();

private:
    Ui::PublisherBooks *ui;
    QSqlDatabase db;
};

#endif // PUBLISHERBOOKS_H
