#ifndef EMPLOYEEWINDOW_H
#define EMPLOYEEWINDOW_H

#include <QDialog>

namespace Ui {
class EmployeeWindow;
}

class EmployeeWindow : public QDialog
{
    Q_OBJECT

public:
    explicit EmployeeWindow(QWidget *parent = nullptr);
    ~EmployeeWindow();

private:
    Ui::EmployeeWindow *ui;
};

#endif // EMPLOYEEWINDOW_H
