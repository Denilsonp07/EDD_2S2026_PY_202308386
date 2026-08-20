#include "vistacliente.h"
#include "ui_vistacliente.h"

vistaCliente::vistaCliente(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::vistaCliente)
{
    ui->setupUi(this);
}

vistaCliente::~vistaCliente()
{
    delete ui;
}

void vistaCliente::on_btnSalir_clicked()
{
    if (parentWidget() != nullptr) {
        parentWidget()->show();
    }
    close();
}
