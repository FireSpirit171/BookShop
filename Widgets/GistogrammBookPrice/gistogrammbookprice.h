#ifndef GISTOGRAMMBOOKPRICE_H
#define GISTOGRAMMBOOKPRICE_H

#include <QDialog>
#include <QtSql>
#include <QtCharts>
#include <QWidget>
#include <QRegularExpression>

namespace Ui {
class GistogrammBookPrice;
}

class GistogrammBookPrice : public QDialog
{
    Q_OBJECT

public:
    explicit GistogrammBookPrice(QWidget *parent = nullptr);
    ~GistogrammBookPrice();

    void drawGistogramm();

private:
    Ui::GistogrammBookPrice *ui;
    QSqlDatabase db;
};

#endif // GISTOGRAMMBOOKPRICE_H
