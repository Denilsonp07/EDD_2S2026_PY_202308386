#ifndef VISTAADMIN_H
#define VISTAADMIN_H

#include <QDialog>
#include <QString>
#include <memory>
#include <vector>
#include "arbolbinariopeliculas.h"
#include "listapromociones.h"
#include "listabeneficios.h"
#include "matrizdisperza.h"

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
    void configurarTablaCartelera();
    void llenarTablaCargaMasiva();
    void llenarTablaGestionar();
    void llenarTablaCartelera();
    void actualizarTablaPromociones();
    void actualizarTablaBeneficios(std::string codigoPromo);

private slots:
    void on_btnCargarPeli_clicked();
    void on_btnAgregarPeli_clicked();
    void on_btnEliminarPeli_clicked();
    void on_btnCrearFuncion_clicked();
    void on_btnSalir_clicked();
    void on_btnNuevaPromo_clicked();
    void on_btnEliminarPromo_clicked();
    void on_btnAnadirBene_clicked();
    void on_tabla_promociones_cellClicked(int row, int column);
    

private:
    Ui::vistaAdmin *ui;
    ArbolBinarioDeBusquedaPeliculas arbolPeliculas;
    ListaCircularPromociones miListaPromociones;

    struct Funcion {
        QString pelicula;
        QString horario;
        QString sala;
        int filas;
        int columnas;
        std::unique_ptr<MatrizDispersa> asientos;
    };

    std::vector<Funcion> funciones;
};

#endif // VISTAADMIN_H
