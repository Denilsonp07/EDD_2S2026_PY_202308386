// #include "mainwindow.h"
#include "login.h"
#include "vistaadmin.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //login ventanaLogin;
    //ventanaLogin.show();

    vistaAdmin ventanaAdmin;
    ventanaAdmin.show();


    return a.exec();
}
