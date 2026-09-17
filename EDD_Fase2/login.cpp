#include "login.h"
#include "ui_login.h"
#include "vistaadmin.h"
#include "vistacliente.h"

#include <QMessageBox>
#include <QLineEdit>

static QString siguienteIdCliente()
{
    int numero = 1;
    while (datosCine().arbolClientes.buscar("C" + QString::number(numero).toStdString()) != nullptr) {
        ++numero;
    }
    return QString("C%1").arg(numero, 3, 10, QChar('0'));
}

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
    if (usuario.compare("AdminCine@gmail.com", Qt::CaseInsensitive) == 0
        && contrasena == "admin123Pass") {
        if (ventanaAdmin == nullptr) {
            ventanaAdmin = new vistaAdmin(this);
        }
        ventanaDestino = ventanaAdmin;
    } else {
        Cliente* cliente = datosCine().arbolClientes.buscarPorCorreo(usuario.toStdString());
        if (cliente == nullptr || cliente->getPassword() != contrasena.toStdString()) {
            QMessageBox::warning(
                this,
                "Credenciales invalidas",
                "El correo o la contrasena son incorrectos.");
            ui->lineEdit_2->clear();
            ui->lineEdit_2->setFocus();
            return;
        }
        delete ventanaCliente;
        ventanaCliente = new vistaCliente(cliente, this);
        ventanaDestino = ventanaCliente;
    }

    ventanaDestino->setAttribute(Qt::WA_DeleteOnClose, false);
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ventanaDestino->show();
    hide();
}

void login::on_btnRegistro_clicked()
{
    const bool mostrar = !ui->panelRegistro->isVisible();
    ui->panelRegistro->setVisible(mostrar);
    ui->btnCrearCuenta->setVisible(mostrar);
    ui->btnRegistro->setText(mostrar ? "OCULTAR REGISTRO" : "REGISTRO");
    adjustSize();
}

void login::on_btnCrearCuenta_clicked()
{
    const QString nombre = ui->txtRegistroNombre->text().trimmed();
    const QString correo = ui->txtRegistroCorreo->text().trimmed();
    const QString telefono = ui->txtRegistroTelefono->text().trimmed();
    const QString contrasena = ui->txtRegistroPassword->text();

    if (nombre.isEmpty() || correo.isEmpty() || telefono.isEmpty() || contrasena.isEmpty()) {
        QMessageBox::warning(this, "Registro incompleto", "Completa todos los campos del registro.");
        return;
    }
    if (!correo.contains('@') || !correo.contains('.')) {
        QMessageBox::warning(this, "Correo invalido", "Ingresa un correo electronico valido.");
        return;
    }
    if (datosCine().arbolClientes.buscarPorCorreo(correo.toStdString()) != nullptr) {
        QMessageBox::warning(this, "Correo utilizado", "Ese correo ya esta registrado.");
        return;
    }

    const QString id = siguienteIdCliente();
    datosCine().arbolClientes.insertar(
        Cliente(id.toStdString(), nombre.toStdString(), correo.toStdString(),
                telefono.toStdString(), contrasena.toStdString()));

    ui->txtRegistroNombre->clear();
    ui->txtRegistroCorreo->clear();
    ui->txtRegistroTelefono->clear();
    ui->txtRegistroPassword->clear();
    ui->panelRegistro->hide();
    ui->btnCrearCuenta->hide();
    ui->btnRegistro->setText("REGISTRO");
    QMessageBox::information(this, "Registro exitoso", "Cliente registrado con ID " + id + ". Ya puedes iniciar sesion.");
}
