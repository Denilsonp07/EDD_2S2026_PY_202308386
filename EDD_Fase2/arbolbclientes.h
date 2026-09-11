#ifndef ARBOLBCLIENTES_H
#define ARBOLBCLIENTES_H
#include "cliente.h"

class NodoB {
public:
    Cliente* clientes[3]; // Max 3 claves por nodo para orden 4
    NodoB* hijos[4];      // Max 4 punteros a hijos
    int numClaves;        // Cuántos clientes hay actualmente en este nodo
    bool esHoja;          // Verdadero si no tiene hijos

    NodoB(bool hoja) {
        esHoja = hoja;
        numClaves = 0;
        for (int i = 0; i < 4; i++) {
            hijos[i] = nullptr;
        }
        for (int i = 0; i < 3; i++) {
            clientes[i] = nullptr;
        }
    }
};

class ArbolBClientes {
private:
    NodoB* raiz;

    // Metodos core del arbol B
    void dividirHijo(NodoB* padre, int i, NodoB* hijoLleno);
    void insertarNoLleno(NodoB* nodo, const Cliente& cliente);
    
    // Metodo recursivo para listar (Inorden adaptado para multiples claves)
    void mostrarInordenRecursivo(NodoB* nodo);

public:
    ArbolBClientes();
    
    void insertar(const Cliente& cliente);
    Cliente* buscar(const std::string& id);
    void mostrarClientes(); // Lista todos los clientes ordenados por Id
};
#endif