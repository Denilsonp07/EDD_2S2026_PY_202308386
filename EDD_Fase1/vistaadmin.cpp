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

vistaAdmin::vistaAdmin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::vistaAdmin)
{
    ui->setupUi(this);
    configurarTablaCargaMasiva();
    configurarTablaGestionar();
    configurarTablaCartelera();

    ui->tabla_asientos->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tabla_asientos->setSelectionMode(QAbstractItemView::NoSelection);

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

void vistaAdmin::llenarTablaCartelera()
{
    const ListaPeliculas peliculas = arbolPeliculas.obtenerPeliculasInorden();
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
    llenarTablaCartelera();

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

    const bool eliminado = arbolPeliculas.eliminarPorCodigo(codigo.toStdString());
    if (!eliminado) {
        QMessageBox::warning(this, "No encontrada", "No se encontro la pelicula a eliminar.");
        return;
    }

    llenarTablaCargaMasiva();
    llenarTablaGestionar();
    llenarTablaCartelera();
    QMessageBox::information(this, "Pelicula eliminada", "La pelicula se elimino correctamente.");
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

    Funcion funcion;
    funcion.pelicula = ui->tbCartelera->item(filaSeleccionada, 1)->text();
    funcion.horario = horario.trimmed();
    funcion.sala = sala;
    funcion.filas = filas;
    funcion.columnas = columnas;
    funcion.asientos = std::make_unique<MatrizDispersa>(filas, columnas);
    funciones.push_back(std::move(funcion));

    const Funcion& funcionCreada = funciones.back();
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
    ui->tabWidget->setCurrentWidget(ui->tab_8);

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