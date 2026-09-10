#ifndef ARBOLAVLFUNCIONES_H
#define ARBOLAVLFUNCIONES_H

#include "funcion.h"

class NodoAVL {
public:
    Funcion funcion;
    NodoAVL* izq;
    NodoAVL* der;
    int altura;

    NodoAVL(const Funcion& fun) : funcion(fun), izq(nullptr), der(nullptr), altura(1) {}
};

class ArbolAVLFunciones {
private:
    NodoAVL* raiz;

    // Métodos matemáticos para el balanceo
    int obtenerAltura(NodoAVL* nodo);
    int obtenerEquilibrio(NodoAVL* nodo);
    int extraerNumero(const std::string& codigo);

    // Rotaciones
    NodoAVL* rotacionDerecha(NodoAVL* y);
    NodoAVL* rotacionIzquierda(NodoAVL* x);

    // Operaciones Core
    NodoAVL* insertarNodo(NodoAVL* nodo, const Funcion& funcion);
    NodoAVL* eliminarNodo(NodoAVL* nodo, int codigoNumerico, bool& eliminado);
    void eliminarNodos(NodoAVL* nodo);

public:
    ArbolAVLFunciones();
    ~ArbolAVLFunciones();

    void insertar(const Funcion& funcion);
    bool eliminarPorCodigo(const std::string& codigo);
    Funcion* buscar(const std::string& codigo);
    
    // despueslos recorridos y Graphviz aqui
};
#endif