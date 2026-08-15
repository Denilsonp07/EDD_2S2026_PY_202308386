// #include "mainwindow.h"
#include "login.h"
#include "vistaadmin.h"
#include "vistacliente.h"


#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //login ventanaLogin;
    //ventanaLogin.show();

    vistaAdmin ventanaAdmin;
    ventanaAdmin.show();

    //vistaCliente ventanaCliente;
    //ventanaCliente.show();


    return a.exec();
}
