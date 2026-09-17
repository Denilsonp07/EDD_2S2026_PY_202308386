#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

class vistaAdmin;
class vistaCliente;

namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();

private:
    Ui::login *ui;
    vistaAdmin* ventanaAdmin;
    vistaCliente* ventanaCliente;

private slots:
    void on_btnIngresar_clicked();
    void on_btnRegistro_clicked();
    void on_btnCrearCuenta_clicked();
};

#endif // LOGIN_H
