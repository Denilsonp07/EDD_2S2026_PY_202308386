#include "vistaadmin.h"
#include "ui_vistaadmin.h"

vistaAdmin::vistaAdmin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::vistaAdmin)
{
    ui->setupUi(this);
}

vistaAdmin::~vistaAdmin()
{
    delete ui;
}
