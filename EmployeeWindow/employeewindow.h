#ifndef EMPLOYEEWINDOW_H
#define EMPLOYEEWINDOW_H

#include <QDialog>
#include "../Widgets/Reports/reports.h" // Включаем заголовочный файл Reports

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

private:
    Ui::EmployeeWindow *ui;
    Reports *report; // Добавляем указатель на объект класса Reports
};

#endif // EMPLOYEEWINDOW_H
