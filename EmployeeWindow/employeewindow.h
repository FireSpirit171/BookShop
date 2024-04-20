#ifndef EMPLOYEEWINDOW_H
#define EMPLOYEEWINDOW_H

#include <QDialog>
#include "../Widgets/Reports/reports.h" // Включаем заголовочный файл Reports
#include "../Widgets/PublisherForm/publisherform.h"
#include "../Widgets/SupplierForm/supplierform.h"
#include "../Widgets/ClientForm/clientform.h"
#include "../Widgets/SellerForm/sellerform.h"
#include "../Widgets/ReferenceInformation/referenceinformation.h"
#include "../Widgets/BookForm/bookform.h"

namespace Ui {
class EmployeeWindow;
}

class EmployeeWindow : public QDialog
{
    Q_OBJECT

public:
    explicit EmployeeWindow(QWidget *parent = nullptr);
    ~EmployeeWindow();

private slots:
    void showReports();
    void ReportsWidgetClosed(); // Добавляем декларацию слота для обработки закрытия виджета книг
    void showPublisherForm();
    void PublisherFormClosed(); // Добавляем декларацию слота для обработки закрытия виджета книг
    void showSupplierForm();
    void SupplierFormClosed(); // Добавляем декларацию слота для обработки закрытия виджета книг
    void showClientForm();
    void ClientFormClosed(); // Добавляем декларацию слота для обработки закрытия виджета книг
    void showSellerForm();
    void SellerFormClosed(); // Добавляем декларацию слота для обработки закрытия виджета книг
    void showReferenceInformation();
    void ReferenceInformationClosed(); // Добавляем декларацию слота для обработки закрытия виджета книг
    void showBookForm();
    void BookFormClosed(); // Добавляем декларацию слота для обработки закрытия виджета книг

private:
    Ui::EmployeeWindow *ui;
    Reports *report; // Добавляем указатель на объект класса Reports
    PublisherForm *publisherForm;
    SupplierForm *supplierFoprm;
    ClientForm *clientForm;
    SellerForm *sellerForm;
    ReferenceInformation *referenceInformation;
    BookForm *bookForm;
};

#endif // EMPLOYEEWINDOW_H
