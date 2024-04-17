#include "availablebookswidget.h"
#include "ui_availablebookswidget.h"

AvailableBooksWidget::AvailableBooksWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AvailableBooksWidget)
{
    ui->setupUi(this);
    db = QSqlDatabase::database(); // Получаем соединение с базой данных
}

AvailableBooksWidget::~AvailableBooksWidget()
{
    delete ui;
}

void AvailableBooksWidget::showBooksInStock()
{
    // Выполнение SQL-запроса для получения всех книг в наличии
    QSqlQuery query(db);
    if (!query.exec("SELECT * FROM \"Book\"")) {
        qDebug() << "Query execution error:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        QString bookTitle = query.value("Title").toString();
        QString author = query.value("Author").toString();
        QString price = query.value("Price").toString();
        price = price.left(price.length() - 2); // Обрезаем пробел и знак вопроса
        QString genre = query.value("Genre").toString();

        QString bookInfo = QString("%1 - %2 - Стоимость: %3$ - Жанр: %4").arg(bookTitle).arg(author).arg(price).arg(genre);
        QListWidgetItem *item = new QListWidgetItem(bookInfo);
        ui->listWidget->addItem(item);
    }


    // Отображаем виджет книг как модальное окно
    this->exec();
}
