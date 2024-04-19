#include "employeewindow.h"
#include "ui_employeewindow.h"

EmployeeWindow::EmployeeWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmployeeWindow),
    report(nullptr), // Инициализируем указатель на объект класса Reports как nullptr
    publisherForm(nullptr)
{
    ui->setupUi(this);
    connect(ui->reports, &QPushButton::clicked, this, &EmployeeWindow::showReports);
    connect(ui->publisher, &QPushButton::clicked, this, &EmployeeWindow::showPublisherForm);
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

void EmployeeWindow::showPublisherForm(){
    if (!publisherForm)
    {
        publisherForm = new PublisherForm();
        connect(publisherForm, &PublisherForm::finished, this, &EmployeeWindow::PublisherFormClosed);
    }

    publisherForm->exec();
}

void EmployeeWindow::PublisherFormClosed()
{
    // Удаляем виджет книг
    delete publisherForm;
    publisherForm = nullptr;
}


