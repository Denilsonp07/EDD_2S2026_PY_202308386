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

bool MatrizDispersa::generarReporteAsientos(const std::string& tituloFuncion, const std::string& rutaDot, const std::string& rutaPng) {
    std::ofstream archivo(rutaDot);
    if (!archivo.is_open()) return false;

    archivo << "digraph MatrizDispersa {\n";
    archivo << "    label=\"" << tituloFuncion << "\";\n";
    archivo << "    labelloc=\"t\";\n";
    archivo << "    node [shape=box, fontname=\"Arial\", style=filled];\n";
    
    // El nodo raíz de la matriz
    archivo << "    Raiz [label=\"Matriz\", group=0, fillcolor=gold];\n";

    // 1. GENERAR CABECERAS DE COLUMNA Y ALINEARLAS CON LA RAÍZ
    NodoCabecera* cabCol = cabecerasColumna;
    std::string rankCol = "{ rank=same; Raiz; ";
    if (cabCol != nullptr) {
        archivo << "    Raiz -> C" << cabCol->getIndice() << ";\n";
        while (cabCol != nullptr) {
            // El atributo 'group' fuerza a Graphviz a mantener la columna recta verticalmente
            archivo << "    C" << cabCol->getIndice() << " [label=\"C" << cabCol->getIndice() 
                    << "\", group=" << cabCol->getIndice() << ", fillcolor=lightblue];\n";
            rankCol += "C" + std::to_string(cabCol->getIndice()) + "; ";
            
            if (cabCol->getSiguiente() != nullptr) {
                archivo << "    C" << cabCol->getIndice() << " -> C" << cabCol->getSiguiente()->getIndice() << ";\n";
            }
            cabCol = cabCol->getSiguiente();
        }
    }
    rankCol += "}\n";
    archivo << rankCol; // Forzamos a que las cabeceras de columna estén horizontales

    // 2. GENERAR CABECERAS DE FILA
    NodoCabecera* cabFila = cabecerasFila;
    if (cabFila != nullptr) {
        archivo << "    Raiz -> F" << cabFila->getIndice() << ";\n";
        while (cabFila != nullptr) {
            archivo << "    F" << cabFila->getIndice() << " [label=\"F" << cabFila->getIndice() 
                    << "\", group=0, fillcolor=lightgreen];\n";
            
            if (cabFila->getSiguiente() != nullptr) {
                archivo << "    F" << cabFila->getIndice() << " -> F" << cabFila->getSiguiente()->getIndice() << ";\n";
            }
            cabFila = cabFila->getSiguiente();
        }
    }

    // 3. GENERAR NODOS OCUPADOS (ASIENTOS) Y ALINEARLOS HORIZONTALMENTE
    cabFila = cabecerasFila;
    while (cabFila != nullptr) {
        NodoAsiento* actual = cabFila->getPrimero();
        // Agrupamos la cabecera de fila con sus respectivos asientos
        std::string rankFila = "{ rank=same; F" + std::to_string(cabFila->getIndice()) + "; ";
        
        while (actual != nullptr) {
            std::string idNodo = "N" + std::to_string(actual->getFila()) + "_" + std::to_string(actual->getColumna());
            
            // Usamos getCliente() que ahora guarda el codigo_reserva
            archivo << "    " << idNodo << " [label=\"" << actual->getCliente() 
                    << "\", group=" << actual->getColumna() << ", fillcolor=salmon];\n";
            rankFila += idNodo + "; ";
            actual = actual->getDerecha();
        }
        rankFila += "}\n";
        archivo << rankFila; // Forzamos alineación horizontal por cada fila
        
        cabFila = cabFila->getSiguiente();
    }

    // 4. CREAR ENLACES HORIZONTALES (FILAS -> NODOS)
    cabFila = cabecerasFila;
    while (cabFila != nullptr) {
        NodoAsiento* actual = cabFila->getPrimero();
        if (actual != nullptr) {
            archivo << "    F" << cabFila->getIndice() << " -> N" << actual->getFila() << "_" << actual->getColumna() << ";\n";
            while (actual->getDerecha() != nullptr) {
                archivo << "    N" << actual->getFila() << "_" << actual->getColumna() << " -> N" 
                        << actual->getDerecha()->getFila() << "_" << actual->getDerecha()->getColumna() << ";\n";
                actual = actual->getDerecha();
            }
        }
        cabFila = cabFila->getSiguiente();
    }

    // 5. CREAR ENLACES VERTICALES (COLUMNAS -> NODOS)
    cabCol = cabecerasColumna;
    while (cabCol != nullptr) {
        NodoAsiento* actual = cabCol->getPrimero();
        if (actual != nullptr) {
            archivo << "    C" << cabCol->getIndice() << " -> N" << actual->getFila() << "_" << actual->getColumna() << ";\n";
            while (actual->getAbajo() != nullptr) {
                archivo << "    N" << actual->getFila() << "_" << actual->getColumna() << " -> N" 
                        << actual->getAbajo()->getFila() << "_" << actual->getAbajo()->getColumna() << ";\n";
                actual = actual->getAbajo();
            }
        }
        cabCol = cabCol->getSiguiente();
    }

    archivo << "}\n";
    archivo.close();

    // Ejecutar el comando para generar la imagen (Si estás en Linux/Mac puedes dejar solo dot. 
    // Si estás en Windows y Graphviz no está en variables de entorno, pon la ruta completa a dot.exe)
    const std::string comando = "dot -Tpng \"" + rutaDot + "\" -o \"" + rutaPng + "\"";
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