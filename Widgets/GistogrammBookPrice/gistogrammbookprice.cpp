#include "gistogrammbookprice.h"
#include "ui_gistogrammbookprice.h"

GistogrammBookPrice::GistogrammBookPrice(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GistogrammBookPrice)
{
    ui->setupUi(this);
    db = QSqlDatabase::database(); // Получение существующего соединения с базой данных
    if (!db.isValid()) {
        qDebug() << "Error: Failed to get database connection.";
        return;
    }

    drawGistogramm();
}

GistogrammBookPrice::~GistogrammBookPrice()
{
    delete ui;
}

void GistogrammBookPrice::drawGistogramm() {
    QSqlQuery query;
    query.exec("SELECT \"Title\", \"Price\" FROM \"Book\"");

    QStringList categories; // Список категорий для оси X
    QBarSeries *series = new QBarSeries();

    while (query.next()) {
        QString bookName = query.value(0).toString();
        QString priceStr = query.value(1).toString().trimmed(); // Получаем строку цены и удаляем лишние пробелы

        // Удаляем символы валюты, если они есть
        priceStr.remove(QRegularExpression("[^0-9.,]"));

        // Преобразуем строку в число
        QLocale locale;
        qreal price = locale.toDouble(priceStr);
        if (price != 0) { // Проверяем успешность преобразования
            QBarSet *set = new QBarSet(bookName);
            *set << price;
            series->append(set);
            // Добавляем название книги в список категорий
            categories.append(bookName);
        } else {
            qDebug() << "Error converting price to number for book: " << bookName;
        }
    }

    QChart *chart = new QChart();
    chart->setTitleFont(QFont("Arial", 16)); // Устанавливаем шрифт и размер для заголовка
    chart->setTitle("Стоимость книг"); // Устанавливаем новый заголовок
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Создание оси Y
    QValueAxis *axisY = new QValueAxis();

    // Добавление осей на график

    chart->addAxis(axisY, Qt::AlignLeft);

    // Добавление серии на график
    chart->addSeries(series);

    // Привязка серии к осям
    series->attachAxis(axisY);

    // Создание виджета для отображения графика
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Установка виджета с графиком в качестве центрального виджета для QDialog
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(chartView);
}

