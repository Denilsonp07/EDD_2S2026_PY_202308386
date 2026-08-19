#include "vistaadmin.h"
#include "ui_vistaadmin.h"



#include <QString>
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

    if (miListaPromociones.buscarPromocion(codigo.toStdString()) != nullptr) {
        QMessageBox::warning(this, "Codigo duplicado", "Ya existe una promocion con ese codigo.");
        return;
    }

    miListaPromociones.insertarPromocion(
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
    if (miListaPromociones.eliminarPromocion(codigo.toStdString())) {
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
    NodoPromocion* promocion = miListaPromociones.buscarPromocion(codigoPromo.toStdString());
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
    NodoPromocion* actual = miListaPromociones.getCabeza();

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
    } while (actual != miListaPromociones.getCabeza()); 
}




void vistaAdmin::actualizarTablaBeneficios(std::string codigoPromo) {
    ui->tabla_beneficios->setRowCount(0); // Limpiar tabla de beneficios

    // 1. Buscamos el nodo de la promoción seleccionada
    NodoPromocion* nodoPromo = miListaPromociones.buscarPromocion(codigoPromo);
    
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