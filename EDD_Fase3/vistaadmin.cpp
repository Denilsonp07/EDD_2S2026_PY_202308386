#include "vistaadmin.h"
#include "ui_vistaadmin.h"



#include <QString>
#include <QDate>
#include <QFileDialog>
#include <QHeaderView>
#include <QInputDialog>
#include <QMessageBox>
#include <QBrush>
#include <QColor>
#include <QStandardItemModel>
#include <QStringList>
#include <QTableWidgetItem>
#include <QDate>
#include <QCoreApplication>
#include <QPixmap>
#include <QDir>

namespace {
QString reservasComoTexto(const Cliente* cliente)
{
    QStringList reservas;
    for (NodoReservaRef* reserva = cliente->getCabezaReservas();
         reserva != nullptr;
         reserva = reserva->siguiente) {
        reservas.append(QString::fromStdString(reserva->codigo_reserva));
    }
    return reservas.join(", ");
}
}

vistaAdmin::vistaAdmin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::vistaAdmin)
{
    ui->setupUi(this);
    configurarTablaCargaMasiva();
    configurarTablaClientesCargaMasiva();
    configurarTablaGestionar();
    configurarTablaGestionarClientes();
    configurarTablaCartelera();

    ui->comboOrden->addItems(QStringList() << "Inorden" << "Preorden" << "Postorden");
    ui->comboCliente->addItems(QStringList() << "Inorden" << "Preorden" << "Postorden");

    ui->tabla_asientos->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tabla_asientos->setSelectionMode(QAbstractItemView::NoSelection);
    ui->lblReporte1->hide();
    ui->lblReporte2->hide();
    ui->lblReporte3->hide();
    ui->lblReporte4->hide();
    ui->lblReporte5->hide();

    ui->tabla_promociones->setColumnCount(4);
    ui->tabla_promociones->setHorizontalHeaderLabels(
        QStringList() << "Codigo" << "Nombre" << "Vigencia" << "Dias aplicables");
    ui->tabla_promociones->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tabla_promociones->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tabla_promociones->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tabla_promociones->horizontalHeader()->setStretchLastSection(true);
    ui->tabla_promociones->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->tabla_beneficios->setColumnCount(3);
    ui->tabla_beneficios->setHorizontalHeaderLabels(
        QStringList() << "Tipo" << "Descripcion" << "Valor");
    ui->tabla_beneficios->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tabla_beneficios->horizontalHeader()->setStretchLastSection(true);
    ui->tabla_beneficios->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

vistaAdmin::~vistaAdmin()
{
    delete ui;
}

void vistaAdmin::on_btnSalir_clicked()
{
    if (parentWidget() != nullptr) {
        parentWidget()->show();
    }
    close();
}

//-------------------------------------------------------------------------------
//                        CARGA MASIVA Y GESTION DE PELICULAS
//-------------------------------------------------------------------------------



void vistaAdmin::configurarTablaCargaMasiva()
{
    ui->tbCargaMasiva->setColumnCount(8);
    ui->tbCargaMasiva->setHorizontalHeaderLabels(
        QStringList() << "Codigo" << "Titulo" << "Genero" << "Duracion"
                      << "Clasificacion" << "Idioma" << "Fecha estreno" << "Fecha fin");

    ui->tbCargaMasiva->setRowCount(0);
    ui->tbCargaMasiva->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tbCargaMasiva->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tbCargaMasiva->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tbCargaMasiva->horizontalHeader()->setStretchLastSection(true);
    ui->tbCargaMasiva->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void vistaAdmin::configurarTablaClientesCargaMasiva()
{
    ui->tbCargaMasiva2->setColumnCount(7);
    ui->tbCargaMasiva2->setHorizontalHeaderLabels(
        QStringList() << "ID" << "Nombre" << "Correo" << "Telefono"
                      << "Contrasena" << "Tipo" << "Reservas");
    ui->tbCargaMasiva2->setRowCount(0);
    ui->tbCargaMasiva2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tbCargaMasiva2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tbCargaMasiva2->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tbCargaMasiva2->horizontalHeader()->setStretchLastSection(true);
    ui->tbCargaMasiva2->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void vistaAdmin::configurarTablaGestionar()
{
    ui->tbGestionar->setColumnCount(8);
    ui->tbGestionar->setHorizontalHeaderLabels(
        QStringList() << "Codigo" << "Titulo" << "Genero" << "Duracion"
                      << "Clasificacion" << "Idioma" << "Fecha estreno" << "Fecha fin");

    ui->tbGestionar->setRowCount(0);
    ui->tbGestionar->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tbGestionar->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tbGestionar->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tbGestionar->horizontalHeader()->setStretchLastSection(true);
    ui->tbGestionar->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void vistaAdmin::configurarTablaGestionarClientes()
{
    ui->tbGestionarCli->setColumnCount(7);
    ui->tbGestionarCli->setHorizontalHeaderLabels(
        QStringList() << "ID" << "Nombre" << "Correo" << "Telefono"
                      << "Contrasena" << "Tipo" << "Reservas");
    ui->tbGestionarCli->setRowCount(0);
    ui->tbGestionarCli->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tbGestionarCli->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tbGestionarCli->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tbGestionarCli->horizontalHeader()->setStretchLastSection(true);
    ui->tbGestionarCli->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void vistaAdmin::configurarTablaCartelera()
{
    ui->tbCartelera->setColumnCount(8);
    ui->tbCartelera->setHorizontalHeaderLabels(
        QStringList() << "Codigo" << "Titulo" << "Genero" << "Duracion"
                      << "Clasificacion" << "Idioma" << "Fecha estreno" << "Fecha fin");

    ui->tbCartelera->setRowCount(0);
    ui->tbCartelera->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tbCartelera->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tbCartelera->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tbCartelera->horizontalHeader()->setStretchLastSection(true);
    ui->tbCartelera->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void vistaAdmin::llenarTablaCargaMasiva()
{
    const ListaPeliculas peliculas = datosCine().arbolPeliculas.obtenerPeliculasInorden();

    ui->tbCargaMasiva->setRowCount(0);
    ui->tbCargaMasiva->setRowCount(peliculas.size());

    for (int fila = 0; fila < peliculas.size(); ++fila) {
        const Pelicula& pelicula = peliculas[fila];

        ui->tbCargaMasiva->setItem(fila, 0, new QTableWidgetItem(QString::fromStdString(pelicula.getCodigo())));
        ui->tbCargaMasiva->setItem(fila, 1, new QTableWidgetItem(QString::fromStdString(pelicula.getTitulo())));
        ui->tbCargaMasiva->setItem(fila, 2, new QTableWidgetItem(QString::fromStdString(pelicula.getGenero())));
        ui->tbCargaMasiva->setItem(fila, 3, new QTableWidgetItem(QString::fromStdString(pelicula.getDuracion())));
        ui->tbCargaMasiva->setItem(fila, 4, new QTableWidgetItem(QString::fromStdString(pelicula.getClasificacion())));
        ui->tbCargaMasiva->setItem(fila, 5, new QTableWidgetItem(QString::fromStdString(pelicula.getIdioma())));
        ui->tbCargaMasiva->setItem(fila, 6, new QTableWidgetItem(QString::fromStdString(pelicula.getFechaEstreno())));
        ui->tbCargaMasiva->setItem(fila, 7, new QTableWidgetItem(QString::fromStdString(pelicula.getFechaFin())));
    }

    ui->tbCargaMasiva->resizeRowsToContents();
}

void vistaAdmin::llenarTablaClientesCargaMasiva()
{
    const std::vector<Cliente*> clientes = datosCine().arbolClientes.obtenerClientesInorden();

    ui->tbCargaMasiva2->setRowCount(0);
    ui->tbCargaMasiva2->setRowCount(static_cast<int>(clientes.size()));

    for (int fila = 0; fila < static_cast<int>(clientes.size()); ++fila) {
        const Cliente* cliente = clientes[static_cast<size_t>(fila)];
        ui->tbCargaMasiva2->setItem(fila, 0,
            new QTableWidgetItem(QString::fromStdString(cliente->getId())));
        ui->tbCargaMasiva2->setItem(fila, 1,
            new QTableWidgetItem(QString::fromStdString(cliente->getNombre())));
        ui->tbCargaMasiva2->setItem(fila, 2,
            new QTableWidgetItem(QString::fromStdString(cliente->getCorreo())));
        ui->tbCargaMasiva2->setItem(fila, 3,
            new QTableWidgetItem(QString::fromStdString(cliente->getTelefono())));
        ui->tbCargaMasiva2->setItem(fila, 4,
            new QTableWidgetItem(QString::fromStdString(cliente->getPassword())));
        ui->tbCargaMasiva2->setItem(fila, 5,
            new QTableWidgetItem(QString::fromStdString(cliente->getTipo())));
        ui->tbCargaMasiva2->setItem(fila, 6,
            new QTableWidgetItem(reservasComoTexto(cliente)));
    }

    ui->tbCargaMasiva2->resizeRowsToContents();
}

void vistaAdmin::llenarTablaGestionar()
{
    const int orden = ui->comboOrden->currentIndex();
    const ListaPeliculas peliculas = orden == 1
        ? datosCine().arbolPeliculas.obtenerPeliculasPreorden()
        : (orden == 2
            ? datosCine().arbolPeliculas.obtenerPeliculasPostorden()
            : datosCine().arbolPeliculas.obtenerPeliculasInorden());

    ui->tbGestionar->setRowCount(0);
    ui->tbGestionar->setRowCount(peliculas.size());

    for (int fila = 0; fila < peliculas.size(); ++fila) {
        const Pelicula& pelicula = peliculas[fila];

        ui->tbGestionar->setItem(fila, 0, new QTableWidgetItem(QString::fromStdString(pelicula.getCodigo())));
        ui->tbGestionar->setItem(fila, 1, new QTableWidgetItem(QString::fromStdString(pelicula.getTitulo())));
        ui->tbGestionar->setItem(fila, 2, new QTableWidgetItem(QString::fromStdString(pelicula.getGenero())));
        ui->tbGestionar->setItem(fila, 3, new QTableWidgetItem(QString::fromStdString(pelicula.getDuracion())));
        ui->tbGestionar->setItem(fila, 4, new QTableWidgetItem(QString::fromStdString(pelicula.getClasificacion())));
        ui->tbGestionar->setItem(fila, 5, new QTableWidgetItem(QString::fromStdString(pelicula.getIdioma())));
        ui->tbGestionar->setItem(fila, 6, new QTableWidgetItem(QString::fromStdString(pelicula.getFechaEstreno())));
        ui->tbGestionar->setItem(fila, 7, new QTableWidgetItem(QString::fromStdString(pelicula.getFechaFin())));
    }

    ui->tbGestionar->resizeRowsToContents();
}

void vistaAdmin::llenarTablaGestionarClientes()
{
    const int orden = ui->comboCliente->currentIndex();
    std::vector<Cliente*> clientes;
    if (orden == 1) {
        clientes = datosCine().arbolClientes.obtenerClientesPreorden();
    } else if (orden == 2) {
        clientes = datosCine().arbolClientes.obtenerClientesPostorden();
    } else {
        clientes = datosCine().arbolClientes.obtenerClientesInorden();
    }

    ui->tbGestionarCli->setRowCount(static_cast<int>(clientes.size()));

    for (int fila = 0; fila < static_cast<int>(clientes.size()); ++fila) {
        const Cliente* cliente = clientes[static_cast<size_t>(fila)];
        ui->tbGestionarCli->setItem(fila, 0, new QTableWidgetItem(QString::fromStdString(cliente->getId())));
        ui->tbGestionarCli->setItem(fila, 1, new QTableWidgetItem(QString::fromStdString(cliente->getNombre())));
        ui->tbGestionarCli->setItem(fila, 2, new QTableWidgetItem(QString::fromStdString(cliente->getCorreo())));
        ui->tbGestionarCli->setItem(fila, 3, new QTableWidgetItem(QString::fromStdString(cliente->getTelefono())));
        ui->tbGestionarCli->setItem(fila, 4, new QTableWidgetItem(QString::fromStdString(cliente->getPassword())));
        ui->tbGestionarCli->setItem(fila, 5, new QTableWidgetItem(QString::fromStdString(cliente->getTipo())));
        ui->tbGestionarCli->setItem(fila, 6, new QTableWidgetItem(reservasComoTexto(cliente)));
    }
    ui->tbGestionarCli->resizeRowsToContents();
}

void vistaAdmin::llenarTablaCartelera()
{
    const ListaPeliculas peliculas = datosCine().arbolPeliculas.obtenerPeliculasInorden();
    const QDate fechaActual = QDate::currentDate();

    ui->tbCartelera->setRowCount(0);
    ui->tbCartelera->setRowCount(peliculas.size());

    for (int fila = 0; fila < peliculas.size(); ++fila) {
        const Pelicula& pelicula = peliculas[fila];

        ui->tbCartelera->setItem(fila, 0, new QTableWidgetItem(QString::fromStdString(pelicula.getCodigo())));
        ui->tbCartelera->setItem(fila, 1, new QTableWidgetItem(QString::fromStdString(pelicula.getTitulo())));
        ui->tbCartelera->setItem(fila, 2, new QTableWidgetItem(QString::fromStdString(pelicula.getGenero())));
        ui->tbCartelera->setItem(fila, 3, new QTableWidgetItem(QString::fromStdString(pelicula.getDuracion())));
        ui->tbCartelera->setItem(fila, 4, new QTableWidgetItem(QString::fromStdString(pelicula.getClasificacion())));
        ui->tbCartelera->setItem(fila, 5, new QTableWidgetItem(QString::fromStdString(pelicula.getIdioma())));
        ui->tbCartelera->setItem(fila, 6, new QTableWidgetItem(QString::fromStdString(pelicula.getFechaEstreno())));
        ui->tbCartelera->setItem(fila, 7, new QTableWidgetItem(QString::fromStdString(pelicula.getFechaFin())));

        const QString fechaFinTexto = QString::fromStdString(pelicula.getFechaFin()).trimmed();
        QDate fechaFin = QDate::fromString(fechaFinTexto, "yyyy-MM-dd");
        if (!fechaFin.isValid()) {
            fechaFin = QDate::fromString(fechaFinTexto, "dd/MM/yyyy");
        }
        if (!fechaFin.isValid()) {
            fechaFin = QDate::fromString(fechaFinTexto, "dd-MM-yyyy");
        }
        if (!fechaFin.isValid()) {
            fechaFin = QDate::fromString(fechaFinTexto, "d/M/yyyy");
        }
        if (!fechaFin.isValid()) {
            fechaFin = QDate::fromString(fechaFinTexto, "d-M-yyyy");
        }

        const int diasRestantes = fechaActual.daysTo(fechaFin);
        if (fechaFin.isValid() && diasRestantes >= 0 && diasRestantes < 7) {
            for (int columna = 0; columna < ui->tbCartelera->columnCount(); ++columna) {
                if (QTableWidgetItem* item = ui->tbCartelera->item(fila, columna)) {
                    item->setBackground(QBrush(QColor("yellow")));
                }
            }
        }
    }

    ui->tbCartelera->resizeRowsToContents();
}

void vistaAdmin::on_btnCargarPeli_clicked()
{
    const QString rutaCsv = QFileDialog::getOpenFileName(
        this,
        "Seleccionar archivo JSON de peliculas",
        QString(),
        "Archivos JSON (*.json);;Todos los archivos (*.*)");

    if (rutaCsv.isEmpty()) {
        return;
    }

    datosCine().arbolPeliculas.limpiar();
    const ResultadoCargaPeliculas resultado = datosCine().arbolPeliculas.cargarJSON(
        rutaCsv.toStdString(), datosCine().arbolFunciones, datosCine().grafoSedes);
    llenarTablaCargaMasiva();
    llenarTablaGestionar();
    llenarTablaGestionarClientes();
    llenarTablaCartelera();

    if (!resultado.error.empty()) {
        QMessageBox::warning(
            this,
            "Error en la carga masiva",
            QString::fromStdString(resultado.error));
        return;
    }

    QString mensaje = QString("Peliculas cargadas: %1\nFunciones cargadas: %2")
        .arg(resultado.peliculasInsertadas)
        .arg(resultado.funcionesInsertadas);

    if (!resultado.peliculasRechazadas.empty()) {
        QStringList rechazadas;
        for (int indice = 0; indice < resultado.peliculasRechazadas.size(); ++indice) {
            rechazadas.append(QString::fromStdString(
                resultado.peliculasRechazadas[indice]));
        }
        mensaje += QString("\n\nAviso: %1 pelicula(s) no se pudieron cargar por errores del JSON:\n%2")
            .arg(resultado.peliculasRechazadas.size())
            .arg(rechazadas.join(", "));

        QMessageBox::warning(this, "Carga masiva con advertencias", mensaje);
    } else {
        QMessageBox::information(this, "Carga masiva completada", mensaje);
    }
}

void vistaAdmin::on_btnCargarClientes_clicked()
{
    const QString rutaJson = QFileDialog::getOpenFileName(
        this,
        "Seleccionar archivo JSON de clientes",
        QString(),
        "Archivos JSON (*.json);;Todos los archivos (*.*)");

    if (rutaJson.isEmpty()) {
        return;
    }

    datosCine().arbolClientes.cargarJSON(
        rutaJson.toStdString(), datosCine().tablaReservas);
    llenarTablaClientesCargaMasiva();
    llenarTablaGestionarClientes();

    QMessageBox::information(
        this,
        "Carga masiva completada",
        "Los clientes se insertaron en el Arbol B y sus reservas en la Tabla Hash.");
}

void vistaAdmin::on_btnAgregarPeli_clicked()
{
    bool ok = false;

    const QString codigo = QInputDialog::getText(this, "Agregar pelicula", "Codigo:", QLineEdit::Normal, QString(), &ok);
    if (!ok || codigo.trimmed().isEmpty()) {
        return;
    }

    const QString titulo = QInputDialog::getText(this, "Agregar pelicula", "Titulo:", QLineEdit::Normal, QString(), &ok);
    if (!ok || titulo.trimmed().isEmpty()) {
        return;
    }

    const QString genero = QInputDialog::getText(this, "Agregar pelicula", "Genero:", QLineEdit::Normal, QString(), &ok);
    if (!ok) {
        return;
    }

    const QString duracion = QInputDialog::getText(this, "Agregar pelicula", "Duracion (min):", QLineEdit::Normal, QString(), &ok);
    if (!ok) {
        return;
    }

    const QString clasificacion = QInputDialog::getText(this, "Agregar pelicula", "Clasificacion:", QLineEdit::Normal, QString(), &ok);
    if (!ok) {
        return;
    }

    const QString idioma = QInputDialog::getText(this, "Agregar pelicula", "Idioma:", QLineEdit::Normal, QString(), &ok);
    if (!ok) {
        return;
    }

    const QString fechaEstreno = QInputDialog::getText(this, "Agregar pelicula", "Fecha estreno:", QLineEdit::Normal, QString(), &ok);
    if (!ok) {
        return;
    }

    const QString fechaFin = QInputDialog::getText(this, "Agregar pelicula", "Fecha fin:", QLineEdit::Normal, QString(), &ok);
    if (!ok) {
        return;
    }

    const int totalAntes = datosCine().arbolPeliculas.obtenerPeliculasInorden().size();

    Pelicula nuevaPelicula(
        codigo.toStdString(),
        titulo.toStdString(),
        genero.toStdString(),
        duracion.toStdString(),
        clasificacion.toStdString(),
        idioma.toStdString(),
        fechaEstreno.toStdString(),
        fechaFin.toStdString());

    datosCine().arbolPeliculas.insertar(nuevaPelicula);

    const int totalDespues = datosCine().arbolPeliculas.obtenerPeliculasInorden().size();
    if (totalDespues == totalAntes) {
        QMessageBox::warning(this, "Codigo duplicado", "Ya existe una pelicula con ese codigo. No se agrego.");
        return;
    }

    llenarTablaCargaMasiva();
    llenarTablaGestionar();
    llenarTablaCartelera();
    QMessageBox::information(this, "Pelicula agregada", "La pelicula se agrego correctamente.");
}

void vistaAdmin::on_btnEliminarPeli_clicked()
{
    const int filaSeleccionada = ui->tbGestionar->currentRow();
    if (filaSeleccionada < 0) {
        QMessageBox::warning(this, "Seleccion requerida", "Selecciona una pelicula en la tabla para eliminar.");
        return;
    }

    QTableWidgetItem* itemCodigo = ui->tbGestionar->item(filaSeleccionada, 0);
    if (itemCodigo == nullptr || itemCodigo->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Dato invalido", "No se pudo obtener el codigo de la fila seleccionada.");
        return;
    }

    const QString codigo = itemCodigo->text().trimmed();
    const QMessageBox::StandardButton confirmacion = QMessageBox::question(
        this,
        "Confirmar eliminacion",
        "¿Deseas eliminar la pelicula con codigo " + codigo + "?");

    if (confirmacion != QMessageBox::Yes) {
        return;
    }

    const bool eliminado = datosCine().arbolPeliculas.eliminarPorCodigo(codigo.toStdString());
    if (!eliminado) {
        QMessageBox::warning(this, "No encontrada", "No se encontro la pelicula a eliminar.");
        return;
    }

    llenarTablaCargaMasiva();
    llenarTablaGestionar();
    llenarTablaCartelera();
    QMessageBox::information(this, "Pelicula eliminada", "La pelicula se elimino correctamente.");
}

void vistaAdmin::on_comboOrden_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    llenarTablaGestionar();
}

void vistaAdmin::on_comboCliente_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    llenarTablaGestionarClientes();
}

void vistaAdmin::on_btnEditarPeli_clicked()
{
    const int filaSeleccionada = ui->tbGestionar->currentRow();
    if (filaSeleccionada < 0 || ui->tbGestionar->item(filaSeleccionada, 0) == nullptr) {
        QMessageBox::warning(this, "Seleccion requerida", "Selecciona una pelicula para editar.");
        return;
    }

    const QString codigo = ui->tbGestionar->item(filaSeleccionada, 0)->text().trimmed();
    Pelicula* pelicula = datosCine().arbolPeliculas.buscarPorCodigo(codigo.toStdString());
    if (pelicula == nullptr) {
        QMessageBox::warning(this, "Pelicula no encontrada", "No se encontro la pelicula seleccionada.");
        return;
    }

    bool ok = false;
    const QString titulo = QInputDialog::getText(this, "Editar pelicula", "Titulo:",
        QLineEdit::Normal, QString::fromStdString(pelicula->getTitulo()), &ok);
    if (!ok || titulo.trimmed().isEmpty()) return;
    const QString genero = QInputDialog::getText(this, "Editar pelicula", "Genero:",
        QLineEdit::Normal, QString::fromStdString(pelicula->getGenero()), &ok);
    if (!ok) return;
    const QString duracion = QInputDialog::getText(this, "Editar pelicula", "Duracion (min):",
        QLineEdit::Normal, QString::fromStdString(pelicula->getDuracion()), &ok);
    if (!ok) return;
    const QString clasificacion = QInputDialog::getText(this, "Editar pelicula", "Clasificacion:",
        QLineEdit::Normal, QString::fromStdString(pelicula->getClasificacion()), &ok);
    if (!ok) return;
    const QString idioma = QInputDialog::getText(this, "Editar pelicula", "Idioma:",
        QLineEdit::Normal, QString::fromStdString(pelicula->getIdioma()), &ok);
    if (!ok) return;
    const QString fechaEstreno = QInputDialog::getText(this, "Editar pelicula", "Fecha estreno:",
        QLineEdit::Normal, QString::fromStdString(pelicula->getFechaEstreno()), &ok);
    if (!ok) return;
    const QString fechaFin = QInputDialog::getText(this, "Editar pelicula", "Fecha fin:",
        QLineEdit::Normal, QString::fromStdString(pelicula->getFechaFin()), &ok);
    if (!ok) return;

    pelicula->setTitulo(titulo.trimmed().toStdString());
    pelicula->setGenero(genero.trimmed().toStdString());
    pelicula->setDuracion(duracion.trimmed().toStdString());
    pelicula->setClasificacion(clasificacion.trimmed().toStdString());
    pelicula->setIdioma(idioma.trimmed().toStdString());
    pelicula->setFechaEstreno(fechaEstreno.trimmed().toStdString());
    pelicula->setFechaFin(fechaFin.trimmed().toStdString());

    llenarTablaCargaMasiva();
    llenarTablaGestionar();
    llenarTablaCartelera();
    QMessageBox::information(this, "Pelicula actualizada", "La pelicula se edito correctamente.");
}

void vistaAdmin::on_btnEliminarCliente_clicked()
{
    bool ok = false;
    const QString id = QInputDialog::getText(
        this, "Eliminar cliente", "ID del cliente:", QLineEdit::Normal, QString(), &ok).trimmed();
    if (!ok || id.isEmpty()) {
        return;
    }

    if (datosCine().arbolClientes.buscar(id.toStdString()) == nullptr) {
        QMessageBox::warning(this, "Cliente no encontrado", "No existe un cliente con ese ID.");
        return;
    }

    const QMessageBox::StandardButton confirmacion = QMessageBox::question(
        this, "Confirmar eliminacion", "¿Deseas eliminar el cliente " + id + "?");
    if (confirmacion != QMessageBox::Yes) {
        return;
    }

    if (!datosCine().arbolClientes.eliminarCliente(id.toStdString(), datosCine().tablaReservas)) {
        QMessageBox::warning(this, "No se pudo eliminar", "El cliente no pudo eliminarse.");
        return;
    }

    llenarTablaClientesCargaMasiva();
    llenarTablaGestionarClientes();
    QMessageBox::information(this, "Cliente eliminado", "El cliente se elimino correctamente.");
}




//-------------------------------------------------------------------------------
//                          FUNCIONES Y ASIENTOS
//-------------------------------------------------------------------------------   




void vistaAdmin::on_btnCrearFuncion_clicked()
{
    const int filaSeleccionada = ui->tbCartelera->currentRow();
    if (filaSeleccionada < 0 || ui->tbCartelera->item(filaSeleccionada, 1) == nullptr) {
        QMessageBox::warning(this, "Seleccion requerida", "Selecciona una pelicula de la cartelera.");
        return;
    }

    bool ok = false;
    const QString horario = QInputDialog::getItem(
        this, "Horario de la funcion", "Selecciona o escribe un horario:",
        QStringList() << "14:00" << "17:00" << "20:00", 0, true, &ok);
    if (!ok || horario.trimmed().isEmpty()) {
        return;
    }

    const QString sala = QInputDialog::getItem(
        this, "Sala de la funcion", "Selecciona una sala:",
        QStringList() << "Sala 1" << "Sala 2" << "Sala 3" << "Sala 4", 0, false, &ok);
    if (!ok || sala.isEmpty()) {
        return;
    }

    const int filas = ui->lineFilas->text().trimmed().toInt(&ok);
    if (!ok || filas <= 0) {
        QMessageBox::warning(this, "Filas invalidas", "Ingresa un numero de filas mayor que cero.");
        return;
    }

    const int columnas = ui->lineColumnas->text().trimmed().toInt(&ok);
    if (!ok || columnas <= 0) {
        QMessageBox::warning(this, "Columnas invalidas", "Ingresa un numero de columnas mayor que cero.");
        return;
    }

    FuncionCine* funcion = new FuncionCine();
    funcion->pelicula = ui->tbCartelera->item(filaSeleccionada, 1)->text();
    funcion->horario = horario.trimmed();
    funcion->sala = sala;
    funcion->filas = filas;
    funcion->columnas = columnas;
    funcion->asientos = new MatrizDispersa(filas, columnas);
    datosCine().agregarFuncion(funcion);

    const FuncionCine& funcionCreada = *funcion;
    auto* modelo = new QStandardItemModel(funcionCreada.filas, funcionCreada.columnas, ui->tabla_asientos);
    for (int fila = 0; fila < funcionCreada.filas; ++fila) {
        for (int columna = 0; columna < funcionCreada.columnas; ++columna) {
            auto* asiento = new QStandardItem("");
            asiento->setTextAlignment(Qt::AlignCenter);
            asiento->setBackground(QBrush(QColor("green")));
            modelo->setItem(fila, columna, asiento);
        }
    }

    for (int fila = 0; fila < funcionCreada.filas; ++fila) {
        modelo->setHeaderData(fila, Qt::Vertical, QString::number(fila + 1));
    }
    for (int columna = 0; columna < funcionCreada.columnas; ++columna) {
        modelo->setHeaderData(columna, Qt::Horizontal, QString::number(columna + 1));
    }
    ui->tabla_asientos->setModel(modelo);
    ui->tabla_asientos->resizeColumnsToContents();

    ui->labelFuncion->setText(
        "Pelicula: " + funcionCreada.pelicula + " | " + funcionCreada.sala
        + " | Horario: " + funcionCreada.horario);
    ui->tbGestionarClientes->setCurrentWidget(ui->tab_8);

    QMessageBox::information(this, "Funcion creada", "La funcion se creo con todos sus asientos libres.");
}





//-------------------------------------------------------------------------------
//                          PROMOCIONES Y BENEFICIOS
//-------------------------------------------------------------------------------





void vistaAdmin::on_btnNuevaPromo_clicked()
{
    const QString codigo = ui->txtCodigoPromo->text().trimmed();
    const QString nombre = ui->txtNombrePromo->text().trimmed();
    const QString vigencia = ui->txtVigenciaPromo->text().trimmed();
    const QString dias = ui->txtDiasPromo->text().trimmed();

    if (codigo.isEmpty() || nombre.isEmpty() || vigencia.isEmpty() || dias.isEmpty()) {
        QMessageBox::warning(this, "Datos incompletos", "Completa todos los campos de la promocion.");
        return;
    }

    if (datosCine().promociones.buscarPromocion(codigo.toStdString()) != nullptr) {
        QMessageBox::warning(this, "Codigo duplicado", "Ya existe una promocion con ese codigo.");
        return;
    }

    datosCine().promociones.insertarPromocion(
        Promocion(codigo.toStdString(), nombre.toStdString(), vigencia.toStdString(), dias.toStdString()));

    actualizarTablaPromociones();
    ui->txtCodigoPromo->clear();
    ui->txtNombrePromo->clear();
    ui->txtVigenciaPromo->clear();
    ui->txtDiasPromo->clear();
    QMessageBox::information(this, "Promocion agregada", "La promocion se agrego correctamente.");
}

void vistaAdmin::on_btnEliminarPromo_clicked()
{
    const int fila = ui->tabla_promociones->currentRow();
    if (fila < 0 || ui->tabla_promociones->item(fila, 0) == nullptr) {
        QMessageBox::warning(this, "Seleccion requerida", "Selecciona una promocion para eliminar.");
        return;
    }

    const QString codigo = ui->tabla_promociones->item(fila, 0)->text().trimmed();
    if (datosCine().promociones.eliminarPromocion(codigo.toStdString())) {
        actualizarTablaPromociones();
        ui->tabla_beneficios->setRowCount(0);
        QMessageBox::information(this, "Promocion eliminada", "La promocion se elimino correctamente.");
    }
}

void vistaAdmin::on_btnAnadirBene_clicked()
{
    const int fila = ui->tabla_promociones->currentRow();
    if (fila < 0 || ui->tabla_promociones->item(fila, 0) == nullptr) {
        QMessageBox::warning(this, "Seleccion requerida", "Selecciona una promocion para agregar el beneficio.");
        return;
    }

    const QString codigoPromo = ui->tabla_promociones->item(fila, 0)->text().trimmed();
    NodoPromocion* promocion = datosCine().promociones.buscarPromocion(codigoPromo.toStdString());
    if (promocion == nullptr) {
        QMessageBox::warning(this, "Promocion no encontrada", "No se encontro la promocion seleccionada.");
        return;
    }

    const QString tipo = ui->txtTipoBeneficio->text().trimmed();
    const QString descripcion = ui->txtDescripcionBeneficio->text().trimmed();
    const QString valor = ui->txtValorBeneficio->text().trimmed();
    if (tipo.isEmpty() || descripcion.isEmpty() || valor.isEmpty()) {
        QMessageBox::warning(this, "Datos incompletos", "Completa todos los campos del beneficio.");
        return;
    }

    promocion->listaBeneficios.insertarFinal(
        Beneficio(tipo.toStdString(), descripcion.toStdString(), valor.toStdString()));
    actualizarTablaBeneficios(codigoPromo.toStdString());
    ui->txtTipoBeneficio->clear();
    ui->txtDescripcionBeneficio->clear();
    ui->txtValorBeneficio->clear();
    QMessageBox::information(this, "Beneficio agregado", "El beneficio se agrego correctamente.");
}



void vistaAdmin::actualizarTablaPromociones() {
    ui->tabla_promociones->setRowCount(0); // Limpiar tabla

    // 1. Obtenemos el primer nodo de la lista circular
    NodoPromocion* actual = datosCine().promociones.getCabeza();

    // 2. Si la lista está vacía, no hacemos nada más
    if (actual == nullptr) {
        return; 
    }

    // 3. Recorremos la lista circular nodo por nodo directamente
    do {
        int fila = ui->tabla_promociones->rowCount();
        ui->tabla_promociones->insertRow(fila);

        // Extraemos los datos del nodo actual
        Promocion p = actual->promocion;

        ui->tabla_promociones->setItem(fila, 0, new QTableWidgetItem(QString::fromStdString(p.codigo)));
        ui->tabla_promociones->setItem(fila, 1, new QTableWidgetItem(QString::fromStdString(p.nombre)));
        ui->tabla_promociones->setItem(fila, 2, new QTableWidgetItem(QString::fromStdString(p.vigencia)));
        ui->tabla_promociones->setItem(fila, 3, new QTableWidgetItem(QString::fromStdString(p.diasAplicables)));

        // Saltamos al siguiente nodo
        actual = actual->siguiente;

    // Nos detenemos cuando el 'siguiente' vuelve a ser la cabeza
    } while (actual != datosCine().promociones.getCabeza()); 
}




void vistaAdmin::actualizarTablaBeneficios(std::string codigoPromo) {
    ui->tabla_beneficios->setRowCount(0); // Limpiar tabla de beneficios

    // 1. Buscamos el nodo de la promoción seleccionada
    NodoPromocion* nodoPromo = datosCine().promociones.buscarPromocion(codigoPromo);
    
    // Si no existe, dejamos la tabla en blanco y salimos
    if (nodoPromo == nullptr) return; 

    // 2. Obtenemos la cabeza de su Lista Doblemente Enlazada interna
    NodoBeneficio* actual = nodoPromo->listaBeneficios.getCabeza();

    // 3. Recorremos la lista linealmente
    while (actual != nullptr) {
        int fila = ui->tabla_beneficios->rowCount();
        ui->tabla_beneficios->insertRow(fila);

        // Extraemos los datos del beneficio
        Beneficio b = actual->beneficio;

        ui->tabla_beneficios->setItem(fila, 0, new QTableWidgetItem(QString::fromStdString(b.tipo)));
        ui->tabla_beneficios->setItem(fila, 1, new QTableWidgetItem(QString::fromStdString(b.descripcion)));
        ui->tabla_beneficios->setItem(fila, 2, new QTableWidgetItem(QString::fromStdString(b.valor)));

        // Saltamos al siguiente beneficio
        actual = actual->siguiente;
    }
}



void vistaAdmin::on_tabla_promociones_cellClicked(int row, int column) {
    Q_UNUSED(column);
    if (ui->tabla_promociones->item(row, 0) == nullptr) {
        return;
    }

    QString codigo = ui->tabla_promociones->item(row, 0)->text();
    actualizarTablaBeneficios(codigo.toStdString());
}

void vistaAdmin::on_btnReporte1_clicked()
{
    if (datosCine().arbolPeliculas.estaVacio()) {
        QMessageBox::warning(this, "Arbol vacio", "No hay peliculas registradas para generar el reporte.");
        return;
    }
    datosCine().arbolPeliculas.generarDot();
    const QPixmap imagen(QDir::currentPath() + "/arbol_peliculas.png");
    if (imagen.isNull()) {
        QMessageBox::critical(this, "Error de reporte", "No se pudo cargar el reporte del BST.");
        return;
    }
    ui->lblReporte1->setPixmap(imagen.scaled(ui->lblReporte1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->lblReporte1->show();
    ui->lblReporte2->hide(); ui->lblReporte3->hide(); ui->lblReporte4->hide(); ui->lblReporte5->hide();
}

void vistaAdmin::on_btnReporte2_clicked()
{
    datosCine().arbolFunciones.generarReporte();
    const QPixmap imagen(QDir::currentPath() + "/reporte_avl.png");
    if (imagen.isNull()) {
        QMessageBox::critical(this, "Error de reporte", "No se pudo cargar el reporte del AVL.");
        return;
    }
    ui->lblReporte2->setPixmap(imagen.scaled(ui->lblReporte2->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->lblReporte1->hide(); ui->lblReporte2->show(); ui->lblReporte3->hide(); ui->lblReporte4->hide(); ui->lblReporte5->hide();
}

void vistaAdmin::on_btnReporte3_clicked()
{
    datosCine().arbolClientes.generarReporte();
    const QPixmap imagen(QDir::currentPath() + "/reporte_arbolb.png");
    if (imagen.isNull()) {
        QMessageBox::critical(this, "Error de reporte", "No se pudo cargar el reporte del Arbol B.");
        return;
    }
    ui->lblReporte3->setPixmap(imagen.scaled(ui->lblReporte3->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->lblReporte1->hide(); ui->lblReporte2->hide(); ui->lblReporte3->show(); ui->lblReporte4->hide(); ui->lblReporte5->hide();
}

void vistaAdmin::on_btnReporte4_clicked()
{
    datosCine().tablaReservas.generarReporte();
    const QPixmap imagen(QDir::currentPath() + "/reporte_hash.png");
    if (imagen.isNull()) {
        QMessageBox::critical(this, "Error de reporte", "No se pudo cargar el reporte de la Tabla Hash.");
        return;
    }
    ui->lblReporte4->setPixmap(imagen.scaled(ui->lblReporte4->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->lblReporte1->hide(); ui->lblReporte2->hide(); ui->lblReporte3->hide(); ui->lblReporte4->show(); ui->lblReporte5->hide();
}

void vistaAdmin::on_btnReporte5_clicked()
{
    if (datosCine().totalFunciones == 0) {
        QMessageBox::warning(this, "Funciones vacias", "No hay funciones creadas para generar el reporte de asientos.");
        return;
    }

    QStringList opciones;
    for (int indice = 0; indice < datosCine().totalFunciones; ++indice) {
        const FuncionCine* funcion = datosCine().obtenerFuncion(indice);
        if (funcion != nullptr) {
            opciones << funcion->pelicula + " - " + funcion->horario + " - " + funcion->sala;
        }
    }

    bool ok = false;
    const QString seleccion = QInputDialog::getItem(
        this, "Seleccionar funcion", "Funcion para el reporte:", opciones, 0, false, &ok);
    if (!ok || seleccion.isEmpty()) {
        return;
    }

    int indiceFuncion = -1;
    for (int indice = 0; indice < datosCine().totalFunciones; ++indice) {
        const FuncionCine* funcion = datosCine().obtenerFuncion(indice);
        if (funcion != nullptr && seleccion == funcion->pelicula + " - " + funcion->horario + " - " + funcion->sala) {
            indiceFuncion = indice;
            break;
        }
    }

    FuncionCine* funcion = datosCine().obtenerFuncion(indiceFuncion);
    if (funcion == nullptr) {
        return;
    }

    const QString carpeta = QDir::currentPath();
    const QString rutaDot = carpeta + "/reporte_matriz_asientos.dot";
    const QString rutaPng = carpeta + "/reporte_matriz_asientos.png";
    const QString titulo = "Funcion: " + funcion->pelicula + " - " + funcion->horario
        + " - " + funcion->sala;
    if (!funcion->asientos->generarReporteAsientos(
            titulo.toStdString(), rutaDot.toStdString(), rutaPng.toStdString())) {
        QMessageBox::critical(this, "Error de reporte", "No se pudo generar el grafo de la matriz dispersa.");
        return;
    }

    const QPixmap imagen(rutaPng);
    if (imagen.isNull()) {
        QMessageBox::critical(this, "Error de reporte", "Graphviz no genero una imagen valida.");
        return;
    }
    ui->lblReporte5->setPixmap(imagen.scaled(ui->lblReporte5->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->lblReporte1->hide(); ui->lblReporte2->hide(); ui->lblReporte3->hide(); ui->lblReporte4->hide(); ui->lblReporte5->show();
}