#ifndef RESERVA_H
#define RESERVA_H
#include <string>

using namespace std;

class Reserva {
public:
    string codigo_reserva;
    string codigo_funcion;
    int fila;
    int columna;
    string fecha_reserva;

    Reserva(string codR = "", string codF = "", int f = 0, int c = 0, string fecha = "")
        : codigo_reserva(codR), codigo_funcion(codF), fila(f), columna(c), fecha_reserva(fecha) {}
};

// Nodo para manejar colisiones mediante listas enlazadas
struct NodoHash {
    Reserva reserva;
    NodoHash* siguiente;

    NodoHash(const Reserva& res) : reserva(res), siguiente(nullptr) {}
};
#endif