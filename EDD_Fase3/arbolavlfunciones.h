#ifndef ARBOLAVLFUNCIONES_H
#define ARBOLAVLFUNCIONES_H

#include "funcion.h"
#include <new>

using namespace std;

class NodoAVL {
public:
    Funcion funcion;
    NodoAVL* izq;
    NodoAVL* der;
    int altura;

    NodoAVL(const Funcion& fun) : funcion(fun), izq(nullptr), der(nullptr), altura(1) {}
};

class ListaFunciones {
private:    
    Funcion* datos;
    int tamanio;
    int capacidad;

    void redimensionar(int nuevaCapacidad) {
        Funcion* nuevo = static_cast<Funcion*>(::operator new(sizeof(Funcion) * nuevaCapacidad));
        for (int i = 0; i < tamanio; ++i) {
            new (&nuevo[i]) Funcion(datos[i]);
            datos[i].~Funcion();
        }
        ::operator delete(datos);
        datos = nuevo;
        capacidad = nuevaCapacidad;
    }   

public:
    ListaFunciones() : datos(nullptr), tamanio(0), capacidad(0) {}
    ~ListaFunciones() {
        for (int i = 0; i < tamanio; ++i) {
            datos[i].~Funcion();
        }
        ::operator delete(datos);
    }

    void agregar(const Funcion& funcion) {
        if (tamanio >= capacidad) {
            redimensionar(capacidad == 0 ? 1 : capacidad * 2);
        }
        new (&datos[tamanio]) Funcion(funcion);
        ++tamanio;
    }

    Funcion& operator[](int index) {
        return datos[index];
    }

    int size() const {
        return tamanio;
    }
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
    
    void recolectarInorden(NodoAVL* nodo, ListaFunciones& funciones) const;


public:
    ArbolAVLFunciones();
    ~ArbolAVLFunciones();

    ListaFunciones obtenerFuncionesInorden() const;
    string serializarFunciones() const;

    void insertar(const Funcion& funcion);
    bool eliminarPorCodigo(const string& codigo);
    Funcion* buscar(const string& codigo);

    void generarDotAux(NodoAVL* nodo, std::ofstream& archivo);
    void generarReporte();


    
    
    // despueslos recorridos Graphviz 
};
#endif