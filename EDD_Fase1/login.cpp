#include "login.h"
#include "ui_login.h"
#include "vistaadmin.h"
#include "vistacliente.h"

#include <QMessageBox>

login::login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::login)
    , ventanaAdmin(nullptr)
    , ventanaCliente(nullptr)
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
        if (ventanaAdmin == nullptr) {
            ventanaAdmin = new vistaAdmin(this);
        }
        ventanaDestino = ventanaAdmin;
    } else if (usuario == "usuario" && contrasena == "usuario123") {
        if (ventanaCliente == nullptr) {
            ventanaCliente = new vistaCliente(this);
        }
        ventanaDestino = ventanaCliente;
    } else {
        QMessageBox::warning(
            this,
            "Credenciales invalidas",
            "El usuario o la contrasena son incorrectos.");
        ui->lineEdit_2->clear();
        ui->lineEdit_2->setFocus();
        return;
    }

    ventanaDestino->setAttribute(Qt::WA_DeleteOnClose, false);
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ventanaDestino->show();
    hide();
}
