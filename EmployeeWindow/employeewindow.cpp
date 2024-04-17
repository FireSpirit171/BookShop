#include "employeewindow.h"
#include "ui_employeewindow.h"

EmployeeWindow::EmployeeWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmployeeWindow),
    report(nullptr) // Инициализируем указатель на объект класса Reports как nullptr
{
    ui->setupUi(this);
    connect(ui->reports, &QPushButton::clicked, this, &EmployeeWindow::showReports);
}

EmployeeWindow::~EmployeeWindow()
{
    delete ui;
}

void EmployeeWindow::showReports(){
    if (!report)
    {
        report = new Reports();
        connect(report, &Reports::finished, this, &EmployeeWindow::ReportsWidgetClosed);
    }

    report->exec();
}

void EmployeeWindow::ReportsWidgetClosed()
{
    // Удаляем виджет книг
    delete report;
    report = nullptr;
}


