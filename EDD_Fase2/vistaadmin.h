#ifndef VISTAADMIN_H
#define VISTAADMIN_H

#include <QDialog>
#include <QString>
#include "arbolbinariopeliculas.h"
#include "datoscine.h"
#include "arbolavlfunciones.h"


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
    void configurarTablaClientesCargaMasiva();
    void configurarTablaGestionar();
    void configurarTablaGestionarClientes();
    void configurarTablaCartelera();
    void llenarTablaCargaMasiva();
    void llenarTablaClientesCargaMasiva();
    void llenarTablaGestionar();
    void llenarTablaGestionarClientes();
    void llenarTablaCartelera();
    void actualizarTablaPromociones();
    void actualizarTablaBeneficios(std::string codigoPromo);

private slots:
    void on_btnCargarPeli_clicked();
    void on_btnCargarClientes_clicked();
    void on_btnAgregarPeli_clicked();
    void on_btnEliminarPeli_clicked();
    void on_btnEditarPeli_clicked();
    void on_comboOrden_currentIndexChanged(int index);
    void on_comboCliente_currentIndexChanged(int index);
    void on_btnEliminarCliente_clicked();
    void on_btnCrearFuncion_clicked();
    void on_btnSalir_clicked();
    void on_btnNuevaPromo_clicked();
    void on_btnEliminarPromo_clicked();
    void on_btnAnadirBene_clicked();
    void on_tabla_promociones_cellClicked(int row, int column);
    void on_btnReporte1_clicked();
    void on_btnReporte2_clicked();
    void on_btnReporte3_clicked();
    void on_btnReporte4_clicked();
    void on_btnReporte5_clicked();
    

private:
    Ui::vistaAdmin *ui;
};

#endif // VISTAADMIN_H
