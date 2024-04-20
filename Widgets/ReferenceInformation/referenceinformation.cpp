#include "referenceinformation.h"
#include "ui_referenceinformation.h"

ReferenceInformation::ReferenceInformation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReferenceInformation)
{
    ui->setupUi(this);
}

ReferenceInformation::~ReferenceInformation()
{
    delete ui;
}
