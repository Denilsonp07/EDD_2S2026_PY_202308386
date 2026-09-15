#ifndef TABLAHASH_H
#define TABLAHASH_H
#include "reserva.h"
#include <string>

class TablaHash {
private:
    NodoHash** tabla; // Arreglo dinámico de punteros a nodos
    int capacidad;
    int cantidadElementos;

    // Funcion Hash para convertir el string (ej. "R001") a un índice un numerico
    int funcionHash(const string& clave);
    
    // para expandir la tabla si se llena demasiado (mantener eficiencia)
    void rehash();

public:
    TablaHash(int cap = 11); // Capacidad inicial preferiblemente número primo
    ~TablaHash();

    void insertar(const Reserva& reserva);
    Reserva* buscar(const string& codigo_reserva);
    bool eliminar(const string& codigo_reserva);

    void generarReporte();
    
};
#endif