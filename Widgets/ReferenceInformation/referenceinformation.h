#ifndef REFERENCEINFORMATION_H
#define REFERENCEINFORMATION_H

#include <QDialog>

namespace Ui {
class ReferenceInformation;
}

class ReferenceInformation : public QDialog
{
    Q_OBJECT

public:
    explicit ReferenceInformation(QWidget *parent = nullptr);
    ~ReferenceInformation();

private:
    Ui::ReferenceInformation *ui;
};

#endif // REFERENCEINFORMATION_H
