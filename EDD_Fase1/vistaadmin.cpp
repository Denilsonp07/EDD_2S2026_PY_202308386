#include "vistaadmin.h"
#include "ui_vistaadmin.h"


#include <QFileDialog>
#include <QHeaderView>
#include <QInputDialog>
#include <QMessageBox>
#include <QStringList>
#include <QTableWidgetItem>

vistaAdmin::vistaAdmin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::vistaAdmin)
{
    ui->setupUi(this);
    configurarTablaCargaMasiva();
    configurarTablaGestionar();
}

vistaAdmin::~vistaAdmin()
{
    delete ui;
}

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

void vistaAdmin::llenarTablaCargaMasiva()
{
    const ListaPeliculas peliculas = arbolPeliculas.obtenerPeliculasInorden();

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

void vistaAdmin::llenarTablaGestionar()
{
    const ListaPeliculas peliculas = arbolPeliculas.obtenerPeliculasInorden();

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

void vistaAdmin::on_btnCargarPeli_clicked()
{
    const QString rutaCsv = QFileDialog::getOpenFileName(
        this,
        "Seleccionar archivo CSV de peliculas",
        QString(),
        "Archivos CSV (*.csv);;Todos los archivos (*.*)");

    if (rutaCsv.isEmpty()) {
        return;
    }

    arbolPeliculas.limpiar();
    arbolPeliculas.cargarCSV(rutaCsv.toStdString());
    llenarTablaCargaMasiva();
    llenarTablaGestionar();

    QMessageBox::information(
        this,
        "Carga masiva completada",
        "Las peliculas se cargaron y se mostraron en tbCargaMasiva y tbGestionar.");
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

    const int totalAntes = arbolPeliculas.obtenerPeliculasInorden().size();

    Pelicula nuevaPelicula(
        codigo.toStdString(),
        titulo.toStdString(),
        genero.toStdString(),
        duracion.toStdString(),
        clasificacion.toStdString(),
        idioma.toStdString(),
        fechaEstreno.toStdString(),
        fechaFin.toStdString());

    arbolPeliculas.insertar(nuevaPelicula);

    const int totalDespues = arbolPeliculas.obtenerPeliculasInorden().size();
    if (totalDespues == totalAntes) {
        QMessageBox::warning(this, "Codigo duplicado", "Ya existe una pelicula con ese codigo. No se agrego.");
        return;
    }

    llenarTablaCargaMasiva();
    llenarTablaGestionar();
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

    const bool eliminado = arbolPeliculas.eliminarPorCodigo(codigo.toStdString());
    if (!eliminado) {
        QMessageBox::warning(this, "No encontrada", "No se encontro la pelicula a eliminar.");
        return;
    }

    llenarTablaCargaMasiva();
    llenarTablaGestionar();
    QMessageBox::information(this, "Pelicula eliminada", "La pelicula se elimino correctamente.");
}
