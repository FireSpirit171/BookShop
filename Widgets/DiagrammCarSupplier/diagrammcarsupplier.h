#ifndef DIAGRAMMCARSUPPLIER_H
#define DIAGRAMMCARSUPPLIER_H

#include <QDialog>
#include <QtSql>
#include <QtCharts>

namespace Ui {
class DiagrammCarSupplier;
}

class DiagrammCarSupplier : public QDialog
{
    Q_OBJECT

public:
    explicit DiagrammCarSupplier(QWidget *parent = nullptr);
    ~DiagrammCarSupplier();

    void showDiagramm();

private:
    Ui::DiagrammCarSupplier *ui;
    QSqlDatabase db;
};

#endif // DIAGRAMMCARSUPPLIER_H
