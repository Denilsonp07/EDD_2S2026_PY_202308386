#include "vistacliente.h"
#include "ui_vistacliente.h"

#include <QBrush>
#include <QColor>
#include <QDate>
#include <QMessageBox>
#include <QHeaderView>
#include <QTableWidgetItem>

static QDate convertirFechaPromocion(const QString& texto)
{
    const QString fecha = texto.trimmed();
    QDate resultado = QDate::fromString(fecha, "yyyy-MM-dd");
    if (!resultado.isValid()) {
        resultado = QDate::fromString(fecha, "dd/MM/yyyy");
    }
    if (!resultado.isValid()) {
        resultado = QDate::fromString(fecha, "dd-MM-yyyy");
    }
    return resultado;
}

static bool promocionVigenteHoy(const std::string& vigencia)
{
    const QString textoVigencia = QString::fromStdString(vigencia).trimmed();
    QString rango = textoVigencia;
    rango.replace(" al ", " / ", Qt::CaseInsensitive);
    rango.replace(" - ", " / ");
    QStringList fechas = rango.split(" / ", Qt::KeepEmptyParts);
    if (fechas.size() != 2) {
        fechas = textoVigencia.split("/", Qt::KeepEmptyParts);
    }
    if (fechas.size() != 2) {
        return false;
    }

    const QDate inicio = convertirFechaPromocion(fechas.at(0));
    const QDate fin = convertirFechaPromocion(fechas.at(1));
    const QDate hoy = QDate::currentDate();
    return inicio.isValid() && fin.isValid() && inicio <= hoy && hoy <= fin;
}

vistaCliente::vistaCliente(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::vistaCliente)
{
    ui->setupUi(this);
    ui->tabla_asientos_cliente->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tabla_asientos_cliente->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tabla_promociones_cliente->setColumnCount(4);
    ui->tabla_promociones_cliente->setHorizontalHeaderLabels(
        QStringList() << "Codigo" << "Nombre" << "Vigencia" << "Dias aplicables");
    ui->tabla_promociones_cliente->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tabla_promociones_cliente->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tabla_promociones_cliente->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tabla_promociones_cliente->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tabla_beneficios_cliente->setColumnCount(3);
    ui->tabla_beneficios_cliente->setHorizontalHeaderLabels(
        QStringList() << "Tipo" << "Descripcion" << "Valor");
    ui->tabla_beneficios_cliente->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tabla_beneficios_cliente->horizontalHeader()->setStretchLastSection(true);
    cargarPeliculas();
    cargarPromocionesVigentes();
}

void vistaCliente::cargarPromocionesVigentes()
{
    ui->tabla_promociones_cliente->setRowCount(0);
    ui->tabla_beneficios_cliente->setRowCount(0);

    NodoPromocion* cabeza = datosCine().promociones.getCabeza();
    if (cabeza == nullptr) {
        return;
    }

    NodoPromocion* actual = cabeza;
    do {
        if (promocionVigenteHoy(actual->promocion.vigencia)) {
            const int fila = ui->tabla_promociones_cliente->rowCount();
            ui->tabla_promociones_cliente->insertRow(fila);
            ui->tabla_promociones_cliente->setItem(fila, 0,
                new QTableWidgetItem(QString::fromStdString(actual->promocion.codigo)));
            ui->tabla_promociones_cliente->setItem(fila, 1,
                new QTableWidgetItem(QString::fromStdString(actual->promocion.nombre)));
            ui->tabla_promociones_cliente->setItem(fila, 2,
                new QTableWidgetItem(QString::fromStdString(actual->promocion.vigencia)));
            ui->tabla_promociones_cliente->setItem(fila, 3,
                new QTableWidgetItem(QString::fromStdString(actual->promocion.diasAplicables)));
        }
        actual = actual->siguiente;
    } while (actual != cabeza);

    ui->tabla_promociones_cliente->resizeRowsToContents();
}

void vistaCliente::mostrarBeneficiosPromocion(const QString& codigo)
{
    ui->tabla_beneficios_cliente->setRowCount(0);
    NodoPromocion* promocion = datosCine().promociones.buscarPromocion(codigo.toStdString());
    if (promocion == nullptr || !promocionVigenteHoy(promocion->promocion.vigencia)) {
        return;
    }

    NodoBeneficio* actual = promocion->listaBeneficios.getCabeza();
    while (actual != nullptr) {
        const int fila = ui->tabla_beneficios_cliente->rowCount();
        ui->tabla_beneficios_cliente->insertRow(fila);
        ui->tabla_beneficios_cliente->setItem(fila, 0,
            new QTableWidgetItem(QString::fromStdString(actual->beneficio.tipo)));
        ui->tabla_beneficios_cliente->setItem(fila, 1,
            new QTableWidgetItem(QString::fromStdString(actual->beneficio.descripcion)));
        ui->tabla_beneficios_cliente->setItem(fila, 2,
            new QTableWidgetItem(QString::fromStdString(actual->beneficio.valor)));
        actual = actual->siguiente;
    }
    ui->tabla_beneficios_cliente->resizeRowsToContents();
}

void vistaCliente::cargarPeliculas()
{
    ui->combo_peliculas->clear();
    const ListaPeliculas peliculas = datosCine().arbolPeliculas.obtenerPeliculasInorden();
    for (int indice = 0; indice < peliculas.size(); ++indice) {
        const Pelicula& pelicula = peliculas[indice];
        ui->combo_peliculas->addItem(
            QString::fromStdString(pelicula.getTitulo()),
            QString::fromStdString(pelicula.getCodigo()));
    }
    mostrarPelicula(ui->combo_peliculas->currentIndex());
}

void vistaCliente::mostrarPelicula(int indice)
{
    const ListaPeliculas peliculas = datosCine().arbolPeliculas.obtenerPeliculasInorden();
    if (indice < 0 || indice >= peliculas.size()) {
        ui->lbl_titulo->clear();
        ui->lbl_genero->clear();
        ui->lbl_clasificacion->clear();
        ui->combo_horarios->clear();
        ui->tabla_asientos_cliente->clear();
        ui->tabla_asientos_cliente->setRowCount(0);
        return;
    }

    const Pelicula& pelicula = peliculas[indice];
    ui->lbl_titulo->setText("Titulo: " + QString::fromStdString(pelicula.getTitulo()));
    ui->lbl_genero->setText("Genero: " + QString::fromStdString(pelicula.getGenero()));
    ui->lbl_clasificacion->setText("Clasificacion: " + QString::fromStdString(pelicula.getClasificacion()));
    cargarHorarios();
}

void vistaCliente::cargarHorarios()
{
    ui->combo_horarios->clear();
    const QString pelicula = ui->combo_peliculas->currentText();
    for (int indice = 0; indice < datosCine().totalFunciones; ++indice) {
        const FuncionCine* funcion = datosCine().obtenerFuncion(indice);
        if (funcion == nullptr) {
            continue;
        }
        if (funcion->pelicula == pelicula) {
            ui->combo_horarios->addItem(funcion->horario + " - " + funcion->sala, indice);
        }
    }
    mostrarAsientos(ui->combo_horarios->currentData().toInt());
}

void vistaCliente::mostrarAsientos(int indiceFuncion)
{
    FuncionCine* funcion = datosCine().obtenerFuncion(indiceFuncion);
    if (funcion == nullptr) {
        ui->tabla_asientos_cliente->clear();
        ui->tabla_asientos_cliente->setRowCount(0);
        return;
    }

    ui->tabla_asientos_cliente->setRowCount(funcion->filas);
    ui->tabla_asientos_cliente->setColumnCount(funcion->columnas);
    for (int fila = 1; fila <= funcion->filas; ++fila) {
        ui->tabla_asientos_cliente->setVerticalHeaderItem(fila - 1, new QTableWidgetItem(QString::number(fila)));
        for (int columna = 1; columna <= funcion->columnas; ++columna) {
            const bool ocupado = funcion->asientos->estaOcupado(fila, columna);
            auto* item = new QTableWidgetItem(ocupado ? "" : "");
            item->setTextAlignment(Qt::AlignCenter);
            item->setBackground(QBrush(QColor(ocupado ? "red" : "green")));
            ui->tabla_asientos_cliente->setItem(fila - 1, columna - 1, item);
        }
    }
    for (int columna = 1; columna <= funcion->columnas; ++columna) {
        ui->tabla_asientos_cliente->setHorizontalHeaderItem(columna - 1, new QTableWidgetItem(QString::number(columna)));
    }
    ui->tabla_asientos_cliente->resizeColumnsToContents();
}

void vistaCliente::on_combo_peliculas_currentIndexChanged(int index)
{
    mostrarPelicula(index);
}

void vistaCliente::on_combo_horarios_currentIndexChanged(int index)
{
    mostrarAsientos(ui->combo_horarios->itemData(index).toInt());
}

void vistaCliente::on_btn_reservar_clicked()
{
    const int indiceFuncion = ui->combo_horarios->currentData().toInt();
    FuncionCine* funcion = datosCine().obtenerFuncion(indiceFuncion);
    if (funcion == nullptr) {
        QMessageBox::warning(this, "Funcion requerida", "Selecciona una pelicula y un horario disponible.");
        return;
    }
    const QString cliente = ui->txt_cliente->text().trimmed();
    if (cliente.isEmpty()) {
        QMessageBox::warning(this, "Cliente requerido", "Ingresa el nombre del cliente.");
        return;
    }

    const int fila = ui->spin_fila->value();
    const int columna = ui->spin_columna->value();
    if (funcion->asientos->estaOcupado(fila, columna)) {
        QMessageBox::warning(this, "Asiento ocupado", "Ese asiento ya esta reservado. Selecciona otro.");
        return;
    }
    if (!funcion->asientos->reservar(fila, columna, cliente.toStdString())) {
        QMessageBox::warning(this, "Asiento invalido", "La fila o el numero de asiento estan fuera del rango de la sala.");
        return;
    }
    
    mostrarAsientos(indiceFuncion);
    QMessageBox::information(this, "Reserva realizada", "El asiento se reservo correctamente.");
}

void vistaCliente::on_btn_eliminar_clicked()
{
    const int indiceFuncion = ui->combo_horarios->currentData().toInt();
    FuncionCine* funcion = datosCine().obtenerFuncion(indiceFuncion);
    if (funcion == nullptr) {
        QMessageBox::warning(this, "Funcion requerida", "Selecciona una pelicula y un horario disponible.");
        return;
    }

    const int fila = ui->spin_fila->value();
    const int columna = ui->spin_columna->value();
    if (!funcion->asientos->estaOcupado(fila, columna)) {
        QMessageBox::warning(this, "Asiento libre", "El asiento indicado no tiene una reserva para eliminar.");
        return;
    }

    const bool cancelada = funcion->asientos->cancelar(fila, columna);
    if (!cancelada) {
        QMessageBox::warning(this, "No se pudo eliminar", "La reserva no pudo ser eliminada.");
        return;
    }

    mostrarAsientos(indiceFuncion);
    QMessageBox::information(this, "Reserva eliminada", "La reserva del asiento fue eliminada correctamente.");
}

void vistaCliente::on_tabla_promociones_cliente_cellClicked(int row, int column)
{
    Q_UNUSED(column);
    QTableWidgetItem* codigo = ui->tabla_promociones_cliente->item(row, 0);
    if (codigo != nullptr) {
        mostrarBeneficiosPromocion(codigo->text());
    }
}

void vistaCliente::on_tabWidget_currentChanged(int index)
{
    if (index == 2) {
        cargarPromocionesVigentes();
    }
}

vistaCliente::~vistaCliente()
{
    delete ui;
}

void vistaCliente::on_btnSalir_clicked()
{
    if (parentWidget() != nullptr) {
        parentWidget()->show();
    }
    close();
}



//------------------------  PROMOCIONES EN INTERFAZZZZ ----------------------------

// Función que evalúa si la promoción es válida hoy
bool esPromocionVigente(const std::string& vigenciaStr) {
    QString vigencia = QString::fromStdString(vigenciaStr);
    
    // Partimos el string usando el separador " / "
    QStringList fechas = vigencia.split(" / ");
    
    if (fechas.size() == 2) {
        // Convertimos los textos a objetos QDate
        QDate fechaInicio = QDate::fromString(fechas[0].trimmed(), "yyyy-MM-dd");
        QDate fechaFin = QDate::fromString(fechas[1].trimmed(), "yyyy-MM-dd");
        
        // Obtenemos la fecha actual del sistema
        QDate hoy = QDate::currentDate(); 
        
        // Verificamos que las fechas sean válidas y que hoy esté dentro del rango
        if (fechaInicio.isValid() && fechaFin.isValid()) {
            return (hoy >= fechaInicio && hoy <= fechaFin);
        }
    }
    return false; // Si el formato falla o está vencida, retorna falso
}

void cargarPromocionesEnInterfaz(ListaCircularPromociones& listaPromos, QComboBox* comboBox) {
    comboBox->clear();
    
    if (listaPromos.estaVacia()) return;

    NodoPromocion* actual = listaPromos.getCabeza();
    
    // Recorrido estricto con punteros para la lista circular
    do {
        // Aplicamos el filtro de fecha
        if (esPromocionVigente(actual->promocion.vigencia)) {
            QString textoMostrar = QString::fromStdString(actual->promocion.nombre + " - Aplica: " + actual->promocion.diasAplicables);
            
            // Guardamos el código de la promoción como "UserData" oculto en el item
            // para poder buscarlo fácilmente cuando el cliente le haga clic
            comboBox->addItem(textoMostrar, QString::fromStdString(actual->promocion.codigo));
        }
        actual = actual->siguiente;
    } while (actual != listaPromos.getCabeza());
}