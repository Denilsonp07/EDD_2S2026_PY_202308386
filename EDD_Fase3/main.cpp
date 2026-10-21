// #include "mainwindow.h"
#include "login.h"
#include "vistaadmin.h"
#include "vistacliente.h"
#include "persistencia.h"


#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QObject::connect(&a, &QCoreApplication::aboutToQuit, []() {
        guardarEstadoSistema();
    });

    login ventanaLogin;
    ventanaLogin.show();


    return a.exec();
}
