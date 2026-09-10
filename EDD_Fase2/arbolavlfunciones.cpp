
#include "arbolavlfunciones.h"
#include <iostream>
#include <cstdlib> // Para atoi

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

int ArbolAVLFunciones::extraerNumero(const std::string& codigo) {
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

Funcion* ArbolAVLFunciones::buscar(const std::string& codigo) {
    NodoAVL* actual = raiz;
    int codBuscado = extraerNumero(codigo);

    // La busqueda se realiza utilizando el codigo_funcion como clave[cite: 1]
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
    return nullptr; // Si el catalogo crece mucho, la busqueda sigue siendo optima gracias al AVL[cite: 1]
}