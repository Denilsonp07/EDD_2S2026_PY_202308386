#include "matrizdisperza.h"

#include <cstdio>

#include "json.hpp"

namespace {
std::string escaparGraphvizAsientos(const std::string& texto)
{
    std::string resultado;
    for (char caracter : texto) {
        if (caracter == '"' || caracter == '\\') {
            resultado += '\\';
        }
        resultado += caracter;
    }
    return resultado;
}
}

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

bool MatrizDispersa::generarReporteAsientos(
    const std::string& tituloFuncion, const std::string& rutaDot, const std::string& rutaPng)
{
    std::ofstream archivo(rutaDot);
    if (!archivo.is_open()) {
        return false;
    }

        archivo << "digraph MatrizOrtogonal {\n"
            << "rankdir=TB;\n"
            << "bgcolor=white;\n"
            << "labelloc=t;\n"
            << "label=\"" << escaparGraphvizAsientos(tituloFuncion) << "\";\n"
            << "nodesep=0.08; ranksep=0.18; splines=ortho;\n"
            << "node [shape=box, fixedsize=true, width=0.72, height=0.42, fontsize=8, fontname=Arial, color=black];\n"
            << "edge [color=\"#AAB7B8\", arrowsize=0.4, dir=both];\n";

            archivo << "raiz [shape=box, style=filled, fillcolor=\"#F8F9F9\", label=\"(-1,-1)\"];\n";
            archivo << "{ rank=same; raiz; ";
    for (int columna = 1; columna <= totalColumnas; ++columna) {
        archivo << "columna_" << columna << "; ";
    }
    archivo << "}\n";

    for (int columna = 1; columna <= totalColumnas; ++columna) {
        archivo << "columna_" << columna
            << " [shape=box, style=filled, fillcolor=\"#AED6F1\", label=\"Columna "
                << columna << "\"];\n";
        if (columna == 1) {
            archivo << "raiz -> columna_1 [dir=both];\n";
        } else {
            archivo << "columna_" << (columna - 1) << " -> columna_" << columna
                    << " [dir=both, color=\"#5D6D7E\"];\n";
        }
    }
    for (int fila = 1; fila <= totalFilas; ++fila) {
        archivo << "fila_" << fila
            << " [shape=box, style=filled, fillcolor=\"#AED6F1\", label=\"Fila "
                << fila << "\"];\n";
        if (fila == 1) {
            archivo << "raiz -> fila_1 [dir=both];\n";
        } else {
            archivo << "fila_" << (fila - 1) << " -> fila_" << fila
                    << " [dir=both, color=\"#5D6D7E\"];\n";
        }
    }

    int reservados = 0;
    for (int fila = 1; fila <= totalFilas; ++fila) {
        for (int columna = 1; columna <= totalColumnas; ++columna) {
            const std::string cliente = getClienteAsiento(fila, columna);
            const bool ocupado = !cliente.empty();
            if (ocupado) {
                ++reservados;
            }
            const std::string id = "asiento_" + std::to_string(fila) + "_" + std::to_string(columna);
            const std::string etiqueta = ocupado ? escaparGraphvizAsientos(cliente) : "Libre";
            const std::string color = ocupado ? "#F1948A" : "#82E0AA";
                archivo << id << " [shape=box, style=filled, fillcolor=\"" << color
                    << "\", label=\"" << etiqueta << "\"];\n";
                archivo << "fila_" << fila << " -> " << id
                    << " [dir=both, color=\"#85929E\", constraint=false];\n";
                archivo << "columna_" << columna << " -> " << id
                    << " [dir=both, color=\"#85929E\", constraint=false];\n";
        }
    }

    for (int fila = 1; fila <= totalFilas; ++fila) {
        for (int columna = 1; columna < totalColumnas; ++columna) {
            archivo << "asiento_" << fila << "_" << columna << " -> asiento_"
                    << fila << "_" << (columna + 1)
                    << " [dir=none, color=\"#D5DBDB\", constraint=false];\n";
        }
    }
    for (int fila = 1; fila < totalFilas; ++fila) {
        for (int columna = 1; columna <= totalColumnas; ++columna) {
            archivo << "asiento_" << fila << "_" << columna << " -> asiento_"
                    << (fila + 1) << "_" << columna
                    << " [dir=none, color=\"#D5DBDB\", constraint=false];\n";
        }
    }

    for (int fila = 1; fila <= totalFilas; ++fila) {
        archivo << "{ rank=same; fila_" << fila << ";";
        for (int columna = 1; columna <= totalColumnas; ++columna) {
            archivo << " asiento_" << fila << "_" << columna << ";";
        }
        archivo << " }\n";
    }

    const int total = totalFilas * totalColumnas;
        archivo << "estadisticas [shape=box, style=filled, fillcolor=\"#F8F9F9\", label=\""
            << "Total de asientos: " << total << "\\n"
            << "Reservados: " << reservados << "\\n"
            << "Libres: " << total - reservados << "\"];\n"
            << "}\n";
    archivo.close();

        std::remove(rutaPng.c_str());
    const std::string comando = "C:/Progra~1/Graphviz/bin/dot.exe -Tpng \""
        + rutaDot + "\" -o \"" + rutaPng + "\"";
    return system(comando.c_str()) == 0;
}






using json = nlohmann::json;

// -------------------------------- GESTIÓN DE MEMORIA ---------------------------------------

MatrizDispersa::~MatrizDispersa() {
    limpiar();
}

void MatrizDispersa::limpiar() {
    // 1. Eliminar todos los nodos de asientos recorriendo las filas
    NodoCabecera* cabFila = cabecerasFila;
    while (cabFila != nullptr) {
        NodoAsiento* asiento = cabFila->getPrimero();
        while (asiento != nullptr) {
            NodoAsiento* tempAsiento = asiento;
            asiento = asiento->getDerecha();
            delete tempAsiento;
        }
        // Eliminar la cabecera de la fila
        NodoCabecera* tempCabFila = cabFila;
        cabFila = cabFila->getSiguiente();
        delete tempCabFila;
    }
    cabecerasFila = nullptr;

    // 2. Eliminar las cabeceras de columna (los asientos ya fueron eliminados)
    NodoCabecera* cabCol = cabecerasColumna;
    while (cabCol != nullptr) {
        NodoCabecera* tempCabCol = cabCol;
        cabCol = cabCol->getSiguiente();
        delete tempCabCol;
    }
    cabecerasColumna = nullptr;
}




// -------------------------------- PERSISTENCIA de JSON --------------------------------



void MatrizDispersa::cargarDesdeJSON(const std::string& rutaArchivo, int filas, int columnas) {
    limpiar(); // Vaciamos la matriz si tenía datos de otra función
    
    totalFilas = filas;
    totalColumnas = columnas;

    std::ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        return; // El archivo puede no existir aún si la función es nueva
    }

    try {
        json j;
        archivo >> j;

        // Leer únicamente las celdas ocupadas y reconstruir la matriz en memoria
        for (const auto& asientoJson : j["asientos_ocupados"]) {
            int f = asientoJson["fila"];
            int c = asientoJson["columna"];
            std::string codReserva = asientoJson["codigo_reserva"];
            
            // Usamos tu método reservar (que internamente enlaza los nodos)
            reservar(f, c, codReserva); 
        }
    } catch (json::parse_error& e) {
        std::cout << "Error al leer JSON de asientos: " << e.what() << std::endl;
    }
    archivo.close();
}

void MatrizDispersa::guardarAJSON(const std::string& rutaArchivo, const std::string& codigoFuncion) {
    json j;
    j["codigo_funcion"] = codigoFuncion;
    j["asientos_ocupados"] = json::array(); // Inicializamos el arreglo vacío

    // Recorremos la matriz para guardar solo los nodos existentes
    NodoCabecera* cab = cabecerasFila;
    while (cab != nullptr) {
        NodoAsiento* asiento = cab->getPrimero();
        while (asiento != nullptr) {
            json nodoJson;
            nodoJson["fila"] = asiento->getFila();
            nodoJson["columna"] = asiento->getColumna();
            // Tu método original getCliente() ahora devuelve el codigo_reserva
            nodoJson["codigo_reserva"] = asiento->getCliente(); 
            
            j["asientos_ocupados"].push_back(nodoJson);
            asiento = asiento->getDerecha();
        }
        cab = cab->getSiguiente();
    }

    // Escribir al archivo
    std::ofstream archivo(rutaArchivo);
    if (archivo.is_open()) {
        archivo << j.dump(4); // Indentación de 4 espacios para que sea legible
        archivo.close();
    }
}