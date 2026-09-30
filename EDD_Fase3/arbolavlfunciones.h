#ifndef ARBOLAVLFUNCIONES_H
#define ARBOLAVLFUNCIONES_H

#include "funcion.h"
#include "arbolavlfunciones.h"

using namespace std;

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

    // Metodos matematicos para el balanceo
    int obtenerAltura(NodoAVL* nodo);
    int obtenerEquilibrio(NodoAVL* nodo);
    int extraerNumero(const string& codigo);

    NodoAVL* obtenerNodoMinimo(NodoAVL* nodo);

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
    bool eliminarPorCodigo(const string& codigo);
    Funcion* buscar(const string& codigo);

    void generarDotAux(NodoAVL* nodo, std::ofstream& archivo);
    void generarReporte();


    
    
    // despueslos recorridos Graphviz 
};
#endif