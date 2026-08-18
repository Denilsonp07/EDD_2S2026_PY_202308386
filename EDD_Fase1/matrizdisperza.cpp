#include "matrizdisperza.h"

MatrizDispersa::MatrizDispersa(int filas, int columnas) {
    totalFilas = filas;
    totalColumnas = columnas;
    cabecerasFila = nullptr;
    cabecerasColumna = nullptr;
}

// busca la cabecera de la fila indicada; si no existe la crea y la
// inserta ordenada dentro de la lista de cabeceras de fila
NodoCabecera* MatrizDispersa::obtenerOCrearCabeceraFila(int fila) {
    // caso: la lista esta vacia o el nuevo indice va antes que el primero
    if (cabecerasFila == nullptr || cabecerasFila->getIndice() > fila) {
        NodoCabecera* nueva = new NodoCabecera(fila);
        nueva->setSiguiente(cabecerasFila);
        cabecerasFila = nueva;
        return nueva;
    }

    NodoCabecera* actual = cabecerasFila;
    while (actual->getSiguiente() != nullptr && actual->getSiguiente()->getIndice() < fila) {
        actual = actual->getSiguiente();
    }

    // si ya existe una cabecera con ese indice, la reutilizamos
    if (actual->getIndice() == fila) {
        return actual;
    }
    if (actual->getSiguiente() != nullptr && actual->getSiguiente()->getIndice() == fila) {
        return actual->getSiguiente();
    }

    // si no existe, se crea e inserta entre actual y actual->siguiente
    NodoCabecera* nueva = new NodoCabecera(fila);
    nueva->setSiguiente(actual->getSiguiente());
    actual->setSiguiente(nueva);
    return nueva;
}

// misma logica que la anterior, pero para columnas
NodoCabecera* MatrizDispersa::obtenerOCrearCabeceraColumna(int columna) {
    if (cabecerasColumna == nullptr || cabecerasColumna->getIndice() > columna) {
        NodoCabecera* nueva = new NodoCabecera(columna);
        nueva->setSiguiente(cabecerasColumna);
        cabecerasColumna = nueva;
        return nueva;
    }

    NodoCabecera* actual = cabecerasColumna;
    while (actual->getSiguiente() != nullptr && actual->getSiguiente()->getIndice() < columna) {
        actual = actual->getSiguiente();
    }

    if (actual->getIndice() == columna) {
        return actual;
    }
    if (actual->getSiguiente() != nullptr && actual->getSiguiente()->getIndice() == columna) {
        return actual->getSiguiente();
    }

    NodoCabecera* nueva = new NodoCabecera(columna);
    nueva->setSiguiente(actual->getSiguiente());
    actual->setSiguiente(nueva);
    return nueva;
}

// reserva un asiento: crea el nodo y lo enlaza en su fila y columna
bool MatrizDispersa::reservar(int fila, int columna, const std::string& cliente) {
    if (fila < 1 || fila > totalFilas || columna < 1 || columna > totalColumnas) {
        std::cout << "Asiento fuera de rango." << std::endl;
        return false;
    }
    if (estaOcupado(fila, columna)) {
        std::cout << "El asiento (" << fila << "," << columna << ") ya esta ocupado." << std::endl;
        return false;
    }

    NodoAsiento* nuevo = new NodoAsiento(fila, columna, cliente);

    // --- enlazar en la lista horizontal (fila) ---
    NodoCabecera* cabFila = obtenerOCrearCabeceraFila(fila);
    if (cabFila->getPrimero() == nullptr || cabFila->getPrimero()->getColumna() > columna) {
        nuevo->setDerecha(cabFila->getPrimero());
        cabFila->setPrimero(nuevo);
    } else {
        NodoAsiento* actual = cabFila->getPrimero();
        while (actual->getDerecha() != nullptr && actual->getDerecha()->getColumna() < columna) {
            actual = actual->getDerecha();
        }
        nuevo->setDerecha(actual->getDerecha());
        actual->setDerecha(nuevo);
    }

    // --- enlazar en la lista vertical (columna) ---
    NodoCabecera* cabCol = obtenerOCrearCabeceraColumna(columna);
    if (cabCol->getPrimero() == nullptr || cabCol->getPrimero()->getFila() > fila) {
        nuevo->setAbajo(cabCol->getPrimero());
        cabCol->setPrimero(nuevo);
    } else {
        NodoAsiento* actual = cabCol->getPrimero();
        while (actual->getAbajo() != nullptr && actual->getAbajo()->getFila() < fila) {
            actual = actual->getAbajo();
        }
        nuevo->setAbajo(actual->getAbajo());
        actual->setAbajo(nuevo);
    }

    return true;
}

// revisa si un asiento esta ocupado recorriendo su fila
bool MatrizDispersa::estaOcupado(int fila, int columna) {
    NodoCabecera* cab = cabecerasFila;
    while (cab != nullptr && cab->getIndice() != fila) {
        cab = cab->getSiguiente();
    }
    if (cab == nullptr) return false; // no hay ocupados en esa fila

    NodoAsiento* actual = cab->getPrimero();
    while (actual != nullptr && actual->getColumna() < columna) {
        actual = actual->getDerecha();
    }
    return (actual != nullptr && actual->getColumna() == columna);
}

// devuelve el nombre del cliente en un asiento, o "" si esta libre
std::string MatrizDispersa::getClienteAsiento(int fila, int columna) {
    NodoCabecera* cab = cabecerasFila;
    while (cab != nullptr && cab->getIndice() != fila) {
        cab = cab->getSiguiente();
    }
    if (cab == nullptr) return "";

    NodoAsiento* actual = cab->getPrimero();
    while (actual != nullptr && actual->getColumna() < columna) {
        actual = actual->getDerecha();
    }
    if (actual != nullptr && actual->getColumna() == columna) {
        return actual->getCliente();
    }
    return "";
}

// cancela una reserva: desenlaza el nodo de su fila y de su columna
bool MatrizDispersa::cancelar(int fila, int columna) {
    if (!estaOcupado(fila, columna)) {
        std::cout << "El asiento (" << fila << "," << columna << ") ya esta libre." << std::endl;
        return false;
    }

    // desenlazar de la fila
    NodoCabecera* cabFila = cabecerasFila;
    while (cabFila->getIndice() != fila) cabFila = cabFila->getSiguiente();

    NodoAsiento* objetivo = nullptr;
    if (cabFila->getPrimero()->getColumna() == columna) {
        objetivo = cabFila->getPrimero();
        cabFila->setPrimero(objetivo->getDerecha());
    } else {
        NodoAsiento* actual = cabFila->getPrimero();
        while (actual->getDerecha()->getColumna() != columna) actual = actual->getDerecha();
        objetivo = actual->getDerecha();
        actual->setDerecha(objetivo->getDerecha());
    }

    // desenlazar de la columna
    NodoCabecera* cabCol = cabecerasColumna;
    while (cabCol->getIndice() != columna) cabCol = cabCol->getSiguiente();

    if (cabCol->getPrimero()->getFila() == fila) {
        cabCol->setPrimero(cabCol->getPrimero()->getAbajo());
    } else {
        NodoAsiento* actual = cabCol->getPrimero();
        while (actual->getAbajo()->getFila() != fila) actual = actual->getAbajo();
        actual->setAbajo(actual->getAbajo()->getAbajo());
    }

    delete objetivo;
    return true;
}

// cuenta cuantos asientos estan ocupados recorriendo todas las filas
int MatrizDispersa::contarOcupados() {
    int contador = 0;
    NodoCabecera* cabFila = cabecerasFila;
    while (cabFila != nullptr) {
        NodoAsiento* asiento = cabFila->getPrimero();
        while (asiento != nullptr) {
            contador++;
            asiento = asiento->getDerecha();
        }
        cabFila = cabFila->getSiguiente();
    }
    return contador;
}

int MatrizDispersa::contarLibres() {
    return (totalFilas * totalColumnas) - contarOcupados();
}

// genera el reporte grafico de la matriz dispersa con Graphviz,
// siguiendo la estructura que pide el enunciado (cabeceras + nodos de valor)
void MatrizDispersa::generarDot(const std::string& tituloFuncion) {
    std::ofstream archivo("matriz_asientos.dot");
    archivo << "digraph MatrizDispersa {\n";
    archivo << "labelloc=\"t\";\nlabel=\"" << tituloFuncion << "\";\n";
    archivo << "rankdir=TB;\n";
    archivo << "node [shape=box, style=filled];\n";

    // nodo raiz "Funcion"
    archivo << "\"Funcion\" [shape=diamond, fillcolor=white];\n";

    // cabeceras de columna en una fila horizontal
    NodoCabecera* cabCol = cabecerasColumna;
    std::string colAnterior = "Funcion";
    while (cabCol != nullptr) {
        std::string nombreCol = "ColCab" + std::to_string(cabCol->getIndice());
        archivo << nombreCol << " [label=\"C" << cabCol->getIndice() << "\", fillcolor=gold];\n";
        archivo << colAnterior << " -> " << nombreCol << ";\n";
        colAnterior = nombreCol;
        cabCol = cabCol->getSiguiente();
    }

    // cabeceras de fila y sus nodos de asiento ocupados
    NodoCabecera* cabFila = cabecerasFila;
    std::string filaAnterior = "Funcion";
    while (cabFila != nullptr) {
        std::string nombreFila = "FilaCab" + std::to_string(cabFila->getIndice());
        archivo << nombreFila << " [label=\"F" << cabFila->getIndice() << "\", fillcolor=gold];\n";
        archivo << filaAnterior << " -> " << nombreFila << ";\n";
        filaAnterior = nombreFila;

        NodoAsiento* asiento = cabFila->getPrimero();
        std::string origenFila = nombreFila;
        while (asiento != nullptr) {
            std::string nombreNodo = "A_" + std::to_string(asiento->getFila()) + "_" + std::to_string(asiento->getColumna());
            archivo << nombreNodo << " [label=\"" << asiento->getCliente() << "\", shape=circle, fillcolor=salmon];\n";
            archivo << origenFila << " -> " << nombreNodo << ";\n";
            origenFila = nombreNodo;
            asiento = asiento->getDerecha();
        }
        cabFila = cabFila->getSiguiente();
    }

    // estadisticas
    archivo << "Stats [shape=note, fillcolor=white, label=\"Ocupados: " << contarOcupados()
            << "\\nLibres: " << contarLibres()
            << "\\nTotal: " << (totalFilas * totalColumnas) << "\"];\n";

    archivo << "}\n";
    archivo.close();

    system("dot -Tpng matriz_asientos.dot -o matriz_asientos.png");
    system("start matriz_asientos.png");
}