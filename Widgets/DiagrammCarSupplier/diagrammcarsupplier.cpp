#include "diagrammcarsupplier.h"
#include "ui_diagrammcarsupplier.h"

DiagrammCarSupplier::DiagrammCarSupplier(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DiagrammCarSupplier)
{
    ui->setupUi(this);
    db = QSqlDatabase::database(); // Получение существующего соединения с базой данных
    if (!db.isValid()) {
        qDebug() << "Error: Failed to get database connection.";
        return;
    }

    showDiagramm();
}

DiagrammCarSupplier::~DiagrammCarSupplier()
{
    delete ui;
}

void DiagrammCarSupplier::showDiagramm() {
    QSqlQuery query;
    query.exec("SELECT S.\"Name\" AS \"Supplier\", COUNT(C.\"Number\") AS \"CarCount\" "
               "FROM \"Supplier\" S "
               "LEFT JOIN \"Car\" C ON S.\"SupplierID\" = C.\"Supplier\" "
               "GROUP BY S.\"Name\"");

    QPieSeries *series = new QPieSeries();
    while (query.next()) {
        QString supplier = query.value("Supplier").toString();
        int carCount = query.value("CarCount").toInt();

        series->append(supplier, carCount);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitleFont(QFont("Arial", 16));
    chart->setTitle("Количество машин поставщиков");
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(chartView);
}

