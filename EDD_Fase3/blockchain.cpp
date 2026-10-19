#include "blockchain.h"

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <sys/stat.h>

using nlohmann::json;

Transaccion::Transaccion(string codigoTransaccion, string codigoReserva,
                         string idCliente, string codigoFuncion, int fila_,
                         int columna_, float monto_, string metodoPago,
                         string fechaPago)
    : codigo_transaccion(codigoTransaccion), codigo_reserva(codigoReserva),
      id_cliente(idCliente), codigo_funcion(codigoFuncion), fila(fila_),
      columna(columna_), monto(monto_), metodo_pago(metodoPago),
      fecha_pago(fechaPago) {}

string Transaccion::aCadena() const {
    ostringstream salida;
    salida << codigo_transaccion << "|" << codigo_reserva << "|" << id_cliente
           << "|" << codigo_funcion << "|" << fila << "|" << columna << "|"
           << fixed << setprecision(2) << monto << "|" << metodo_pago << "|"
           << fecha_pago;
    return salida.str();
}

json Transaccion::aJson() const {
    return json{{"codigo_transaccion", codigo_transaccion},
                {"codigo_reserva", codigo_reserva},
                {"id_cliente", id_cliente},
                {"codigo_funcion", codigo_funcion},
                {"fila", fila},
                {"columna", columna},
                {"monto", monto},
                {"metodo_pago", metodo_pago},
                {"fecha_pago", fecha_pago}};
}

Transaccion Transaccion::desdeJson(const json& objeto) {
    return Transaccion(objeto.value("codigo_transaccion", ""),
                       objeto.value("codigo_reserva", ""),
                       objeto.value("id_cliente", ""),
                       objeto.value("codigo_funcion", ""),
                       objeto.value("fila", 0), objeto.value("columna", 0),
                       objeto.value("monto", 0.0f),
                       objeto.value("metodo_pago", ""),
                       objeto.value("fecha_pago", ""));
}

ListaTransacciones::ListaTransacciones() : cabeza(nullptr), cantidad(0) {}

ListaTransacciones::ListaTransacciones(const ListaTransacciones& otra)
    : cabeza(nullptr), cantidad(0) {
    for (NodoTransaccion* actual = otra.cabeza; actual != nullptr;
         actual = actual->siguiente) {
        insertarAlFinal(actual->transaccion);
    }
}

ListaTransacciones::~ListaTransacciones() {
    limpiar();
}

ListaTransacciones& ListaTransacciones::operator=(
    const ListaTransacciones& otra) {
    if (this != &otra) {
        limpiar();
        for (NodoTransaccion* actual = otra.cabeza; actual != nullptr;
             actual = actual->siguiente) {
            insertarAlFinal(actual->transaccion);
        }
    }
    return *this;
}

void ListaTransacciones::insertar(const Transaccion& transaccion) {
    NodoTransaccion* nuevo = new NodoTransaccion(transaccion);
    nuevo->siguiente = cabeza;
    cabeza = nuevo;
    ++cantidad;
}

void ListaTransacciones::insertarAlFinal(const Transaccion& transaccion) {
    NodoTransaccion* nuevo = new NodoTransaccion(transaccion);
    if (cabeza == nullptr) {
        cabeza = nuevo;
    } else {
        NodoTransaccion* actual = cabeza;
        while (actual->siguiente != nullptr) actual = actual->siguiente;
        actual->siguiente = nuevo;
    }
    ++cantidad;
}

bool ListaTransacciones::estaVacia() const {
    return cabeza == nullptr;
}

int ListaTransacciones::size() const {
    return cantidad;
}

NodoTransaccion* ListaTransacciones::obtenerCabeza() const {
    return cabeza;
}

const Transaccion* ListaTransacciones::buscarPorCodigo(
    const string& codigo) const {
    for (NodoTransaccion* actual = cabeza; actual != nullptr;
         actual = actual->siguiente) {
        if (actual->transaccion.codigo_transaccion == codigo) {
            return &actual->transaccion;
        }
    }
    return nullptr;
}

void ListaTransacciones::limpiar() {
    while (cabeza != nullptr) {
        NodoTransaccion* siguiente = cabeza->siguiente;
        delete cabeza;
        cabeza = siguiente;
    }
    cantidad = 0;
}

ArbolMerkle::ArbolMerkle() : raiz(nullptr) {}

ArbolMerkle::ArbolMerkle(const ListaTransacciones& transacciones)
    : raiz(nullptr) {
    construir(transacciones);
}

ArbolMerkle::~ArbolMerkle() {
    liberar(raiz);
}

void ArbolMerkle::liberar(NodoMerkle* nodo) {
    if (nodo == nullptr) return;
    liberar(nodo->izq);
    liberar(nodo->der);
    delete nodo;
}

NodoMerkle* ArbolMerkle::construirNivel(NodoMerkle** nodos, int cantidad) {
    if (cantidad == 1) {
        NodoMerkle* resultado = nodos[0];
        delete[] nodos;
        return resultado;
    }
    int siguienteCantidad = (cantidad + 1) / 2;
    NodoMerkle** padres = new NodoMerkle*[siguienteCantidad];
    for (int i = 0; i < siguienteCantidad; ++i) {
        int izquierda = i * 2;
        int derecha = izquierda + 1 < cantidad ? izquierda + 1 : izquierda;
        string combinado = nodos[izquierda]->hash + nodos[derecha]->hash;
        padres[i] = new NodoMerkle(sha256(combinado));
        padres[i]->izq = nodos[izquierda];
        padres[i]->der = nodos[derecha] == nodos[izquierda]
                             ? new NodoMerkle(nodos[izquierda]->hash)
                             : nodos[derecha];
    }
    delete[] nodos;
    return construirNivel(padres, siguienteCantidad);
}

void ArbolMerkle::construir(const ListaTransacciones& transacciones) {
    liberar(raiz);
    raiz = nullptr;
    if (transacciones.estaVacia()) return;

    int cantidad = transacciones.size();
    NodoMerkle** hojas = new NodoMerkle*[cantidad];
    int indice = 0;
    for (NodoTransaccion* actual = transacciones.obtenerCabeza();
         actual != nullptr; actual = actual->siguiente) {
        hojas[indice++] = new NodoMerkle(sha256(actual->transaccion.aCadena()));
    }
    raiz = construirNivel(hojas, cantidad);
}

string ArbolMerkle::obtenerRoot() const {
    return raiz == nullptr ? "" : raiz->hash;
}

NodoMerkle* ArbolMerkle::obtenerRaiz() const {
    return raiz;
}

void ArbolMerkle::escribirDot(NodoMerkle* nodo, ofstream& archivo,
                              int& contador) const {
    if (nodo == nullptr) return;
    int id = contador++;
    archivo << "  n" << id << " [label=\"" << nodo->hash
            << "\",shape=box];\n";
    if (nodo->izq != nullptr) {
        int previo = contador;
        escribirDot(nodo->izq, archivo, contador);
        archivo << "  n" << id << " -> n" << previo << ";\n";
    }
    if (nodo->der != nullptr) {
        int previo = contador;
        escribirDot(nodo->der, archivo, contador);
        archivo << "  n" << id << " -> n" << previo << ";\n";
    }
}

bool ArbolMerkle::generarReporte(const string& rutaDot,
                                 const string& rutaPng) const {
    ofstream archivo(rutaDot.c_str());
    if (!archivo.is_open()) return false;
    archivo << "digraph Merkle {\n";
    int contador = 0;
    escribirDot(raiz, archivo, contador);
    archivo << "}\n";
    archivo.close();
    string comando = "dot -Tpng \"" + rutaDot + "\" -o \"" + rutaPng + "\"";
    return system(comando.c_str()) == 0;
}

Bloque::Bloque()
    : INDEX(0), TIMESTAMP(""), NONCE(0), DATA(new ListaTransacciones()),
      ROOTMERKLE(""), PREVIOUSHASH(""), HASH(""), siguiente(nullptr) {}

Bloque::Bloque(int index, const string& timestamp, int nonce,
               ListaTransacciones* data, const string& rootMerkle,
               const string& previousHash, const string& hash)
    : INDEX(index), TIMESTAMP(timestamp), NONCE(nonce), DATA(data),
      ROOTMERKLE(rootMerkle), PREVIOUSHASH(previousHash), HASH(hash),
      siguiente(nullptr) {}

Bloque::~Bloque() {
    delete DATA;
}

Blockchain::Blockchain() : cabeza(nullptr), cantidadPendientes(0) {}

Blockchain::~Blockchain() {
    while (cabeza != nullptr) {
        Bloque* siguiente = cabeza->siguiente;
        delete cabeza;
        cabeza = siguiente;
    }
}

string Blockchain::obtenerTimestampActual() const {
    time_t ahora = time(nullptr);
    tm* tiempo = localtime(&ahora);
    char buffer[32];
    strftime(buffer, sizeof(buffer), "%d-%m-%Y::%H:%M:%S", tiempo);
    return string(buffer);
}

string Blockchain::calcularHash(const Bloque& bloque) const {
    ostringstream contenido;
    contenido << bloque.INDEX << bloque.TIMESTAMP << bloque.NONCE
              << bloque.ROOTMERKLE << bloque.PREVIOUSHASH;
    return sha256(contenido.str());
}

bool Blockchain::hashIntegro(const Bloque& bloque,
                             const string& hashAnterior) const {
    return bloque.PREVIOUSHASH == hashAnterior &&
           bloque.HASH == calcularHash(bloque) &&
           bloque.HASH.size() >= 4 &&
           bloque.HASH.substr(0, 4) == "0000";
}

void Blockchain::insertarBloqueAdelante(Bloque* bloque) {
    bloque->siguiente = cabeza;
    cabeza = bloque;
}

Bloque* Blockchain::buscarBloque(int index) const {
    for (Bloque* actual = cabeza; actual != nullptr;
         actual = actual->siguiente) {
        if (actual->INDEX == index) return actual;
    }
    return nullptr;
}

void Blockchain::agregarTransaccion(const Transaccion& transaccion) {
    pendientes.insertarAlFinal(transaccion);
    ++cantidadPendientes;
    if (cantidadPendientes == 4) generarBloque();
}

void Blockchain::registrarTransaccion(const Transaccion& transaccion) {
    agregarTransaccion(transaccion);
}

bool Blockchain::generarBloque() {
    if (pendientes.estaVacia()) return false;
    int index = cabeza == nullptr ? 0 : cabeza->INDEX + 1;
    string previo = cabeza == nullptr ? "0000" : cabeza->HASH;
    ListaTransacciones* datos = new ListaTransacciones(pendientes);
    ArbolMerkle arbol(*datos);
    Bloque* bloque = new Bloque(index, obtenerTimestampActual(), 0, datos,
                                arbol.obtenerRoot(), previo, "");
    do {
        bloque->HASH = calcularHash(*bloque);
        if (bloque->HASH.substr(0, 4) == "0000") break;
        ++bloque->NONCE;
    } while (true);
    insertarBloqueAdelante(bloque);
    pendientes.limpiar();
    cantidadPendientes = 0;
    return guardarBloqueJSON(bloque);
}

bool Blockchain::guardarBloqueJSONInterno(const Bloque* bloque) const {
    mkdir("blocks", 0777);
    string ruta = "blocks/block" + to_string(bloque->INDEX) + ".json";
    ofstream archivo(ruta.c_str());
    if (!archivo.is_open()) return false;
    json salida;
    salida["INDEX"] = bloque->INDEX;
    salida["TIMESTAMP"] = bloque->TIMESTAMP;
    salida["NONCE"] = bloque->NONCE;
    salida["ROOTMERKLE"] = bloque->ROOTMERKLE;
    salida["PREVIOUSHASH"] = bloque->PREVIOUSHASH;
    salida["HASH"] = bloque->HASH;
    salida["DATA"] = json::array();
    for (NodoTransaccion* actual = bloque->DATA->obtenerCabeza();
         actual != nullptr; actual = actual->siguiente) {
        salida["DATA"].push_back(actual->transaccion.aJson());
    }
    archivo << salida.dump(4);
    return true;
}

bool Blockchain::guardarBloqueJSON(Bloque* bloque) {
    if (bloque == nullptr) return false;
    return guardarBloqueJSONInterno(bloque);
}

bool Blockchain::cargarYVerificarCadena(TablaHash& tablaReservas) {
    for (Bloque* actual = cabeza; actual != nullptr;) {
        Bloque* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
    cabeza = nullptr;
    string hashAnterior = "0000";
    bool cadenaValida = true;
    for (int index = 0;; ++index) {
        string ruta = "blocks/block" + to_string(index) + ".json";
        ifstream archivo(ruta.c_str());
        if (!archivo.is_open()) break;
        try {
            json objeto;
            archivo >> objeto;
            ListaTransacciones* datos = new ListaTransacciones();
            if (objeto.contains("DATA") && objeto["DATA"].is_array()) {
                for (const json& dato : objeto["DATA"]) {
                    datos->insertarAlFinal(Transaccion::desdeJson(dato));
                }
            }
            Bloque* bloque = new Bloque(
                objeto.value("INDEX", index), objeto.value("TIMESTAMP", ""),
                objeto.value("NONCE", 0), datos,
                objeto.value("ROOTMERKLE", ""), objeto.value("PREVIOUSHASH", ""),
                objeto.value("HASH", ""));
            ArbolMerkle merkle(*datos);
            bool merkleValido = merkle.obtenerRoot() == bloque->ROOTMERKLE;
            bool valido = cadenaValida && merkleValido &&
                          hashIntegro(*bloque, hashAnterior);
            if (!valido) cadenaValida = false;
            if (cadenaValida) {
                for (NodoTransaccion* transaccion = datos->obtenerCabeza();
                     transaccion != nullptr;
                     transaccion = transaccion->siguiente) {
                    const Transaccion& valor = transaccion->transaccion;
                    tablaReservas.insertar(Reserva(
                        valor.codigo_reserva, valor.codigo_funcion, valor.fila,
                        valor.columna, valor.fecha_pago, valor.id_cliente,
                        valor.monto, valor.metodo_pago, valor.codigo_transaccion));
                }
            }
            hashAnterior = bloque->HASH;
            insertarBloqueAdelante(bloque);
        } catch (const json::exception& error) {
            cerr << "Error leyendo " << ruta << ": " << error.what() << endl;
            cadenaValida = false;
        }
    }
    return cadenaValida;
}

void Blockchain::generarReporteBlockchain() const {
    ofstream archivo("reporte_blockchain.dot");
    if (!archivo.is_open()) return;
    archivo << "digraph Blockchain {\n  rankdir=LR;\n";
    Bloque* anterior = nullptr;
    for (Bloque* bloque = cabeza; bloque != nullptr;
         bloque = bloque->siguiente) {
        string hashAnterior = "0000";
        if (bloque->INDEX > 0) {
            Bloque* previo = buscarBloque(bloque->INDEX - 1);
            if (previo != nullptr) hashAnterior = previo->HASH;
        }
        bool integro = hashIntegro(*bloque, hashAnterior);
        archivo << "  b" << bloque->INDEX << " [shape=box,style=filled,"
                << "fillcolor=" << (integro ? "green" : "red")
                << ",label=\"INDEX: " << bloque->INDEX
                << "\\nNONCE: " << bloque->NONCE << "\\nHASH: "
                << bloque->HASH << "\"];\n";
        if (anterior != nullptr) {
            archivo << "  b" << anterior->INDEX << " -> b" << bloque->INDEX
                    << ";\n";
        }
        anterior = bloque;
    }
    archivo << "}\n";
    archivo.close();
    system("dot -Tpng reporte_blockchain.dot -o reporte_blockchain.png");
}

bool Blockchain::generarReporteMerkle(int indexBloque) const {
    Bloque* bloque = buscarBloque(indexBloque);
    if (bloque == nullptr) return false;
    ArbolMerkle arbol(*bloque->DATA);
    string base = "reporte_merkle_block" + to_string(indexBloque);
    return arbol.generarReporte(base + ".dot", base + ".png");
}

Bloque* Blockchain::obtenerCabeza() const {
    return cabeza;
}

int Blockchain::obtenerCantidadPendientes() const {
    return cantidadPendientes;
}
