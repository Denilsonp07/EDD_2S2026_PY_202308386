
#include "arbolavlfunciones.h"
#include <iostream>
#include <cstdlib> // Para atoi
#include <cstdio> // Para std::remove

#include <fstream>

ArbolAVLFunciones::ArbolAVLFunciones() : raiz(nullptr) {}

ArbolAVLFunciones::~ArbolAVLFunciones() {
    eliminarNodos(raiz);
}

void ArbolAVLFunciones::eliminarNodos(NodoAVL* nodo) {
    if (nodo != nullptr) {
        eliminarNodos(nodo->izq);
        eliminarNodos(nodo->der);
        delete nodo;
    }
}

int ArbolAVLFunciones::extraerNumero(const string& codigo) {
    std::string digitos = "";
    for (char c : codigo) {
        if (isdigit(c)) {
            digitos += c;
        }
    }
    return digitos.empty() ? 0 : atoi(digitos.c_str());
}

// === FUNCIONES MATEMATICAS DE BALANCEO ===

int ArbolAVLFunciones::obtenerAltura(NodoAVL* nodo) {
    if (nodo == nullptr) return 0;
    return nodo->altura;
}

int ArbolAVLFunciones::obtenerEquilibrio(NodoAVL* nodo) {
    if (nodo == nullptr) return 0;
    // Factor de balance: Altura Izquierda - Altura Derecha
    return obtenerAltura(nodo->izq) - obtenerAltura(nodo->der);
}

// === ROTACIONES ===

NodoAVL* ArbolAVLFunciones::rotacionDerecha(NodoAVL* y) {
    NodoAVL* x = y->izq;
    NodoAVL* T2 = x->der;

    // Realizar la rotacion (intercambio de punteros)
    x->der = y;
    y->izq = T2;

    // Actualizar las alturas leyendo cuál de sus hijos es mas alto
    int altYIzq = obtenerAltura(y->izq);
    int altYDer = obtenerAltura(y->der);
    y->altura = ((altYIzq > altYDer) ? altYIzq : altYDer) + 1;

    int altXIzq = obtenerAltura(x->izq);
    int altXDer = obtenerAltura(x->der);
    x->altura = ((altXIzq > altXDer) ? altXIzq : altXDer) + 1;

    return x; // La nueva raiz temporal de este subarbol
}

NodoAVL* ArbolAVLFunciones::rotacionIzquierda(NodoAVL* x) {
    NodoAVL* y = x->der;
    NodoAVL* T2 = y->izq;

    // Realizar la rotacion (intercambio de punteros)
    y->izq = x;
    x->der = T2;

    // Actualizar las alturas
    int altXIzq = obtenerAltura(x->izq);
    int altXDer = obtenerAltura(x->der);
    x->altura = ((altXIzq > altXDer) ? altXIzq : altXDer) + 1;

    int altYIzq = obtenerAltura(y->izq);
    int altYDer = obtenerAltura(y->der);
    y->altura = ((altYIzq > altYDer) ? altYIzq : altYDer) + 1;

    return y; // La nueva raiz temporal de este subarbol
}

// === INSERCION Y BALANCEO ===

void ArbolAVLFunciones::insertar(const Funcion& funcion) {
    // Generar el nombre del archivo JSON automaticamente al crear la funcion
    Funcion nuevaFuncion = funcion;
    nuevaFuncion.setArchivoAsientos(funcion.getCodigoFuncion() + "_funcion.json");
    
    raiz = insertarNodo(raiz, nuevaFuncion);
}

NodoAVL* ArbolAVLFunciones::insertarNodo(NodoAVL* nodo, const Funcion& funcion) {
    // 1. Insercion normal tipo arbol Binario de Busqueda
    if (nodo == nullptr) {
        return new NodoAVL(funcion);
    }

    int codigoNuevo = extraerNumero(funcion.getCodigoFuncion());
    int codigoActual = extraerNumero(nodo->funcion.getCodigoFuncion());

    if (codigoNuevo < codigoActual) {
        nodo->izq = insertarNodo(nodo->izq, funcion);
    } else if (codigoNuevo > codigoActual) {
        nodo->der = insertarNodo(nodo->der, funcion);
    } else {
        return nodo; // No se permiten codigos de función duplicados
    }

    // 2. Actualizar la altura de este nodo ancestro
    int altIzq = obtenerAltura(nodo->izq);
    int altDer = obtenerAltura(nodo->der);
    nodo->altura = 1 + ((altIzq > altDer) ? altIzq : altDer);

    // 3. Obtener el factor de equilibrio para verificar si se desbalanceó
    int balance = obtenerEquilibrio(nodo);

    // 4. Si el nodo se desbalancea, aplicar una de las 4 rotaciones:

    // Caso 1: Izquierda Izquierda (El peso esta todo a la izquierda)
    if (balance > 1 && codigoNuevo < extraerNumero(nodo->izq->funcion.getCodigoFuncion())) {
        return rotacionDerecha(nodo);
    }

    // Caso 2: Derecha Derecha (El peso esta todo a la derecha)
    if (balance < -1 && codigoNuevo > extraerNumero(nodo->der->funcion.getCodigoFuncion())) {
        return rotacionIzquierda(nodo);
    }

    // Caso 3: Izquierda Derecha (El peso esta a la izquierda, pero en el hijo derecho)
    if (balance > 1 && codigoNuevo > extraerNumero(nodo->izq->funcion.getCodigoFuncion())) {
        nodo->izq = rotacionIzquierda(nodo->izq);
        return rotacionDerecha(nodo);
    }

    // Caso 4: Derecha Izquierda (El peso esta a la derecha, pero en el hijo izquierdo)
    if (balance < -1 && codigoNuevo < extraerNumero(nodo->der->funcion.getCodigoFuncion())) {
        nodo->der = rotacionDerecha(nodo->der);
        return rotacionIzquierda(nodo);
    }

    return nodo; // Retornar el puntero sin cambios si ya esta balanceado
}

// === BÚSQUEDA ===

Funcion* ArbolAVLFunciones::buscar(const string& codigo) {
    NodoAVL* actual = raiz;
    int codBuscado = extraerNumero(codigo);

    // La busqueda se realiza utilizando el codigo_funcion como clave
    while (actual != nullptr) {
        int codActual = extraerNumero(actual->funcion.getCodigoFuncion());
        
        if (codBuscado == codActual) {
            return &(actual->funcion); // Retorna el puntero directo al objeto Función
        } else if (codBuscado < codActual) {
            actual = actual->izq;
        } else {
            actual = actual->der;
        }
    }
    return nullptr; // Si el catalogo crece mucho, la busqueda sigue siendo optima gracias al AVL
}

NodoAVL* ArbolAVLFunciones::obtenerNodoMinimo(NodoAVL* nodo) {
    NodoAVL* actual = nodo;
    // Viajamos lo más a la izquierda posible
    while (actual->izq != nullptr) {
        actual = actual->izq;
    }
    return actual;
}

bool ArbolAVLFunciones::eliminarPorCodigo(const string& codigo) {
    bool eliminado = false;
    
    // 1. Buscamos la función antes de borrarla para obtener el nombre de su archivo JSON
    Funcion* f = buscar(codigo);
    std::string archivoAEliminar = "";
    
    if (f != nullptr) {
        archivoAEliminar = f->getArchivoAsientos();
    }

    // 2. Ejecutamos la eliminación en el árbol AVL
    raiz = eliminarNodo(raiz, extraerNumero(codigo), eliminado);

    // 3. Si se eliminó del árbol, borramos el archivo físico
    if (eliminado && !archivoAEliminar.empty()) {
        std::remove(archivoAEliminar.c_str());
    }

    return eliminado;
}

NodoAVL* ArbolAVLFunciones::eliminarNodo(NodoAVL* nodo, int codigoNumerico, bool& eliminado) {
    // --- FASE 1: ELIMINACIÓN ESTÁNDAR (Como en el BST) ---
    if (nodo == nullptr) {
        return nodo;
    }

    int codigoActual = extraerNumero(nodo->funcion.getCodigoFuncion());

    if (codigoNumerico < codigoActual) {
        nodo->izq = eliminarNodo(nodo->izq, codigoNumerico, eliminado);
    } else if (codigoNumerico > codigoActual) {
        nodo->der = eliminarNodo(nodo->der, codigoNumerico, eliminado);
    } else {
        // Encontramos el nodo a eliminar
        eliminado = true;

        // Caso 1 o 2: Nodo con un solo hijo o sin hijos
        if (nodo->izq == nullptr || nodo->der == nullptr) {
            NodoAVL* temp = nodo->izq ? nodo->izq : nodo->der;

            if (temp == nullptr) { // Sin hijos
                temp = nodo;
                nodo = nullptr;
            } else { // Un hijo
                *nodo = *temp; // Copiamos el contenido del hijo
            }
            delete temp;
        } else {
            // Caso 3: Nodo con dos hijos
            // Buscamos el sucesor inorden (el menor del subárbol derecho)
            NodoAVL* temp = obtenerNodoMinimo(nodo->der);

            // Copiamos los datos del sucesor al nodo actual
            nodo->funcion = temp->funcion;

            // Eliminamos al sucesor original en el subárbol derecho
            bool elimAux = false;
            nodo->der = eliminarNodo(nodo->der, extraerNumero(temp->funcion.getCodigoFuncion()), elimAux);
        }
    }

    // Si el árbol tenía un solo nodo, retornamos
    if (nodo == nullptr) return nodo;

    

    // --- FASE 2: ACTUALIZACIÓN DE ALTURA ---
    int altIzq = obtenerAltura(nodo->izq);
    int altDer = obtenerAltura(nodo->der);
    nodo->altura = 1 + ((altIzq > altDer) ? altIzq : altDer);

    // --- FASE 3: OBTENER EQUILIBRIO Y BALANCEAR ---
    int balance = obtenerEquilibrio(nodo);

    // Caso Izquierda-Izquierda
    if (balance > 1 && obtenerEquilibrio(nodo->izq) >= 0) {
        return rotacionDerecha(nodo);
    }

    // Caso Izquierda-Derecha
    if (balance > 1 && obtenerEquilibrio(nodo->izq) < 0) {
        nodo->izq = rotacionIzquierda(nodo->izq);
        return rotacionDerecha(nodo);
    }

    // Caso Derecha-Derecha
    if (balance < -1 && obtenerEquilibrio(nodo->der) <= 0) {
        return rotacionIzquierda(nodo);
    }

    // Caso Derecha-Izquierda
    if (balance < -1 && obtenerEquilibrio(nodo->der) > 0) {
        nodo->der = rotacionDerecha(nodo->der);
        return rotacionIzquierda(nodo);
    }

    return nodo;
}





// --------------------------------- REPORTEE DEL ARBOL AVL ---------------------------------



void ArbolAVLFunciones::generarDotAux(NodoAVL* nodo, std::ofstream& archivo) {
    if (nodo == nullptr) return;

    // Etiqueta básica con los datos requeridos
    std::string label = nodo->funcion.getCodigoFuncion() + "\\n" +
                        "Peli: " + nodo->funcion.getCodigoPelicula() + "\\n" +
                        "Horario: " + nodo->funcion.getHorario() + "\\n" +
                        "Sala: " + nodo->funcion.getSala();

    // Nodos circulares, color verde (Programada)
    archivo << "    \"" << nodo->funcion.getCodigoFuncion() 
            << "\" [label=\"" << label << "\", shape=circle, style=filled, fillcolor=forestgreen, fontcolor=white];\n";

    if (nodo->izq != nullptr) {
        archivo << "    \"" << nodo->funcion.getCodigoFuncion() << "\" -> \"" 
                << nodo->izq->funcion.getCodigoFuncion() << "\";\n";
        generarDotAux(nodo->izq, archivo);
    }
    if (nodo->der != nullptr) {
        archivo << "    \"" << nodo->funcion.getCodigoFuncion() << "\" -> \"" 
                << nodo->der->funcion.getCodigoFuncion() << "\";\n";
        generarDotAux(nodo->der, archivo);
    }
}

void ArbolAVLFunciones::generarReporte() {
    std::ofstream archivo("reporte_avl.dot");
    archivo << "digraph AVL {\n";
    generarDotAux(raiz, archivo);
    archivo << "}\n";
    archivo.close();
    system("dot -Tpng reporte_avl.dot -o reporte_avl.png");
}