#ifndef VISTAADMIN_H
#define VISTAADMIN_H

#include <QDialog>
#include "arbolbinariopeliculas.h"
#include "listapromociones.h"
#include "listabeneficios.h"

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
    void actualizarTablaPromociones();
    void actualizarTablaBeneficios(std::string codigoPromo);

private slots:
    void on_btnCargarPeli_clicked();
    void on_btnAgregarPeli_clicked();
    void on_btnEliminarPeli_clicked();
    void on_btnNuevaPromo_clicked();
    void on_btnEliminarPromo_clicked();
    void on_btnAnadirBene_clicked();
    void on_tabla_promociones_cellClicked(int row, int column);

private:
    Ui::vistaAdmin *ui;
    ArbolBinarioDeBusquedaPeliculas arbolPeliculas;
    ListaCircularPromociones miListaPromociones;
};

#endif // VISTAADMIN_H
