#ifndef ARBOLBCLIENTES_H
#define ARBOLBCLIENTES_H
#include "cliente.h"
#include "tablahashreservas.h"
#include <vector>

class NodoB {
public:
    Cliente* clientes[3]; // Max 3 claves por nodo para orden 4
    NodoB* hijos[4];      // Max 4 punteros a hijos
    int numClaves;        // Cuantos clientes hay actualmente en este nodo
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

    Cliente* buscarCorreoRecursivo(NodoB* nodo, const std::string& correo);
    void recolectarInorden(NodoB* nodo, std::vector<Cliente*>& clientes);

    // Metodos core del arbol B
    void dividirHijo(NodoB* padre, int i, NodoB* hijoLleno);
    void insertarNoLleno(NodoB* nodo, const Cliente& cliente);
    
    // Metodo recursivo para listar (Inorden adaptado para multiples claves)
    void mostrarInordenRecursivo(NodoB* nodo);

    
    //LOGICA DE ELIMINACION

    void eliminarRecursivo(NodoB* nodo, int idNumerico);
    int encontrarClave(NodoB* nodo, int idNumerico);
    void removerDeHoja(NodoB* nodo, int indice);
    void removerDeNoHoja(NodoB* nodo, int indice);
    void llenar(NodoB* nodo, int indice);
    void pedirPrestadoAnterior(NodoB* nodo, int indice);
    void pedirPrestadoSiguiente(NodoB* nodo, int indice);
    void fusionar(NodoB* nodo, int indice);



public:
    ArbolBClientes();
    
    void insertar(const Cliente& cliente);
    Cliente* buscar(const std::string& id);
    Cliente* buscarPorCorreo(const std::string& correo);
    std::vector<Cliente*> obtenerClientesInorden();
    std::vector<Cliente*> obtenerClientesPreorden();
    std::vector<Cliente*> obtenerClientesPostorden();
    void mostrarClientes(); // Lista todos los clientes ordenados por Id

    //LOGICA DE ELIMINACION
    bool eliminarCliente(const string& id, TablaHash& tablaReservas);

    void cargarJSON(const string& ruta, TablaHash& tablaReservas);

    void generarReporte();
    void generarDotAux(NodoB* nodo, std::ofstream& archivo, int& contador);

};
#endif