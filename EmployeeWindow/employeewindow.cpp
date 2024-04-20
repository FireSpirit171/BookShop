#include "employeewindow.h"
#include "ui_employeewindow.h"

EmployeeWindow::EmployeeWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmployeeWindow),
    report(nullptr), // Инициализируем указатель на объект класса Reports как nullptr
    publisherForm(nullptr),
    supplierFoprm(nullptr),
    clientForm(nullptr),
    sellerForm(nullptr),
    referenceInformation(nullptr)
{
    ui->setupUi(this);
    connect(ui->reports, &QPushButton::clicked, this, &EmployeeWindow::showReports);
    connect(ui->publisher, &QPushButton::clicked, this, &EmployeeWindow::showPublisherForm);
    connect(ui->supplier, &QPushButton::clicked, this, &EmployeeWindow::showSupplierForm);
    connect(ui->client, &QPushButton::clicked, this, &EmployeeWindow::showClientForm);
    connect(ui->seller, &QPushButton::clicked, this, &EmployeeWindow::showSellerForm);
    connect(ui->information, &QPushButton::clicked, this, &EmployeeWindow::showReferenceInformation);
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

void EmployeeWindow::showSupplierForm(){
    if (!supplierFoprm)
    {
        supplierFoprm = new SupplierForm();
        connect(supplierFoprm, &SupplierForm::finished, this, &EmployeeWindow::SupplierFormClosed);
    }

    supplierFoprm->exec();
}

void EmployeeWindow::SupplierFormClosed()
{
    // Удаляем виджет книг
    delete supplierFoprm;
    supplierFoprm = nullptr;
}

void EmployeeWindow::showClientForm(){
    if (!clientForm)
    {
        clientForm = new ClientForm();
        connect(clientForm, &ClientForm::finished, this, &EmployeeWindow::ClientFormClosed);
    }

    clientForm->exec();
}

void EmployeeWindow::ClientFormClosed()
{
    // Удаляем виджет книг
    delete clientForm;
    clientForm = nullptr;
}

void EmployeeWindow::showSellerForm(){
    if (!sellerForm)
    {
        sellerForm = new SellerForm();
        connect(sellerForm, &SellerForm::finished, this, &EmployeeWindow::SellerFormClosed);
    }

    sellerForm->exec();
}

void EmployeeWindow::SellerFormClosed()
{
    // Удаляем виджет книг
    delete sellerForm;
    sellerForm = nullptr;
}

void EmployeeWindow::showReferenceInformation(){
    if (!referenceInformation)
    {
        referenceInformation = new ReferenceInformation();
        connect(referenceInformation, &ReferenceInformation::finished, this, &EmployeeWindow::ReferenceInformationClosed);
    }

    referenceInformation->exec();
}

void EmployeeWindow::ReferenceInformationClosed()
{
    // Удаляем виджет книг
    delete referenceInformation;
    referenceInformation = nullptr;
}




