#include "grafosedes.h"

//---------------------------------------------------------------------
//     Constructor y Destructor
// --------------------------------------------------------------------
GrafoSedes::GrafoSedes() : cabeza(nullptr) {}

GrafoSedes::~GrafoSedes() {
    VerticeSede* actual = cabeza;
    while (actual != nullptr) {
        VerticeSede* siguiente = actual->siguiente;
        eliminarArcosDeVertice(actual); // Libera la lista de adyacencia
        delete actual;                  // Libera el vértice
        actual = siguiente;
    }
}

void GrafoSedes::eliminarArcosDeVertice(VerticeSede* vertice) {
    Arco* actual = vertice->listaAdyacencia;
    while (actual != nullptr) {
        Arco* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
    vertice->listaAdyacencia = nullptr;
}

// ---------------------------------------------------------
//           Búsqueda de Vértices
// ---------------------------------------------------------
VerticeSede* GrafoSedes::buscarVertice(const string& codigo) const {
    VerticeSede* actual = cabeza;
    while (actual != nullptr) {
        if (actual->sede.codigo == codigo) {
            return actual;
        }
        actual = actual->siguiente;
    }
    return nullptr; // No existe la sede
}

// ---------------------------------------------------------
//            Inserción de Vértices (Sedes)
// ---------------------------------------------------------
void GrafoSedes::insertarVertice(const Sede& sede) {
    if (buscarVertice(sede.codigo) != nullptr) {
        cout << "La sede " << sede.codigo << " ya existe en el grafo." << endl;
        return;
    }

    VerticeSede* nuevo = new VerticeSede(sede);
    
    // Inserción al final de la lista principal
    if (cabeza == nullptr) {
        cabeza = nuevo;
    } else {
        VerticeSede* actual = cabeza;
        while (actual->siguiente != nullptr) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevo;
    }
}

// ---------------------------------------------------------
//      Creación de Aristas (Conexiones)
// ---------------------------------------------------------
void GrafoSedes::agregarArcoDirigido(const string& origen, const string& destino, int peso) {
    VerticeSede* verticeOrigen = buscarVertice(origen);
    if (verticeOrigen == nullptr) return;

    // Verificar si el arco ya existe. Si es así, solo actualizamos el peso.
    Arco* actual = verticeOrigen->listaAdyacencia;
    while (actual != nullptr) {
        if (actual->codigoDestino == destino) {
            actual->peso = peso; 
            return;
        }
        actual = actual->siguiente;
    }

    // Si no existe, lo insertamos al inicio de la sub-lista para mayor velocidad
    Arco* nuevo = new Arco(destino, peso);
    nuevo->siguiente = verticeOrigen->listaAdyacencia;
    verticeOrigen->listaAdyacencia = nuevo;
}

void GrafoSedes::agregarAristaNoDirigida(const string& origen, const string& destino, int peso) {
    // Validamos que ambas sedes existan antes de conectarlas
    if (buscarVertice(origen) != nullptr && buscarVertice(destino) != nullptr) {
        // Un grafo no dirigido es simplemente dos arcos dirigidos en direcciones opuestas
        agregarArcoDirigido(origen, destino, peso);
        agregarArcoDirigido(destino, origen, peso);
    }
}

// ---------------------------------------------------------
//                  Utilidad para Depuración
// ---------------------------------------------------------
void GrafoSedes::mostrarGrafoConsola() const {
    VerticeSede* actual = cabeza;
    while (actual != nullptr) {
        cout << "Sede: " << actual->sede.codigo << " -> ";
        Arco* arco = actual->listaAdyacencia;
        while (arco != nullptr) {
            cout << "[" << arco->codigoDestino << " | Peso: " << arco->peso << "] ";
            arco = arco->siguiente;
        }
        cout << endl;
        actual = actual->siguiente;
    }
}