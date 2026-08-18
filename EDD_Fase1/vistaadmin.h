#ifndef VISTAADMIN_H
#define VISTAADMIN_H

#include <QDialog>
#include "arbolbinariopeliculas.h"

namespace Ui {
class vistaAdmin;
}

class vistaAdmin : public QDialog
{
    Q_OBJECT

public:
    explicit vistaAdmin(QWidget *parent = nullptr);
    ~vistaAdmin();

private:
    void configurarTablaCargaMasiva();
    void configurarTablaGestionar();
    void llenarTablaCargaMasiva();
    void llenarTablaGestionar();

private slots:
    void on_btnCargarPeli_clicked();
    void on_btnAgregarPeli_clicked();
    void on_btnEliminarPeli_clicked();

private:
    Ui::vistaAdmin *ui;
    ArbolBinarioDeBusquedaPeliculas arbolPeliculas;
};

#endif // VISTAADMIN_H
