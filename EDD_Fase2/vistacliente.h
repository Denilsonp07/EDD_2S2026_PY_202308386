#ifndef VISTACLIENTE_H
#define VISTACLIENTE_H

#include <QDialog>
#include "datoscine.h"

namespace Ui {
class vistaCliente;
}

class vistaCliente : public QDialog
{
    Q_OBJECT

public:
    explicit vistaCliente(Cliente* cliente, QWidget *parent = nullptr);
    ~vistaCliente();

private:
    void cargarPeliculas();
    void mostrarPelicula(int indice);
    void cargarHorarios();
    void mostrarAsientos(int indiceFuncion);
    void cargarPromocionesVigentes();
    void mostrarBeneficiosPromocion(const QString& codigo);
    void cargarPerfil();
    void cargarHistorial();

    Ui::vistaCliente *ui;
    Cliente* clienteActual;

private slots:
    void on_btnSalir_clicked();
    void on_combo_peliculas_currentIndexChanged(int index);
    void on_combo_horarios_currentIndexChanged(int index);
    void on_btn_reservar_clicked();
    void on_btn_eliminar_clicked();
    void on_tabla_promociones_cliente_cellClicked(int row, int column);
    void on_tabWidget_currentChanged(int index);
    void on_btnEditarPerfil_clicked();
    void on_btn_guardar_perfil_clicked();
    void on_btn_cancelar_reserva_clicked();
};

#endif // VISTACLIENTE_H
