#include "vistaadmin.h"
#include "ui_vistaadmin.h"


#include <QFileDialog>
#include <QHeaderView>
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

void vistaAdmin::poblarTablaCargaMasiva()
{
    const std::vector<Pelicula> peliculas = arbolPeliculas.obtenerPeliculasInorden();

    ui->tbCargaMasiva->setRowCount(0);
    ui->tbCargaMasiva->setRowCount(static_cast<int>(peliculas.size()));

    for (int fila = 0; fila < static_cast<int>(peliculas.size()); ++fila) {
        const Pelicula& pelicula = peliculas[static_cast<size_t>(fila)];

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

void vistaAdmin::poblarTablaGestionar()
{
    const std::vector<Pelicula> peliculas = arbolPeliculas.obtenerPeliculasInorden();

    ui->tbGestionar->setRowCount(0);
    ui->tbGestionar->setRowCount(static_cast<int>(peliculas.size()));

    for (int fila = 0; fila < static_cast<int>(peliculas.size()); ++fila) {
        const Pelicula& pelicula = peliculas[static_cast<size_t>(fila)];

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
    poblarTablaCargaMasiva();
    poblarTablaGestionar();

    QMessageBox::information(
        this,
        "Carga masiva completada",
        "Las peliculas se cargaron y se mostraron en tbCargaMasiva y tbGestionar.");
}
