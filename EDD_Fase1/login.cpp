#include "login.h"
#include "ui_login.h"
#include "vistaadmin.h"
#include "vistacliente.h"

#include <QMessageBox>

login::login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::login)
{
    ui->setupUi(this);
}

login::~login()
{
    delete ui;
}

void login::on_btnIngresar_clicked()
{
    const QString usuario = ui->lineEdit->text().trimmed();
    const QString contrasena = ui->lineEdit_2->text();

    QDialog* ventanaDestino = nullptr;
    if (usuario == "admin" && contrasena == "admin123") {
        ventanaDestino = new vistaAdmin(this);
    } else if (usuario == "usuario" && contrasena == "usuario123") {
        ventanaDestino = new vistaCliente(this);
    } else {
        QMessageBox::warning(
            this,
            "Credenciales invalidas",
            "El usuario o la contrasena son incorrectos.");
        ui->lineEdit_2->clear();
        ui->lineEdit_2->setFocus();
        return;
    }

    ventanaDestino->setAttribute(Qt::WA_DeleteOnClose);
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ventanaDestino->show();
    hide();
}
