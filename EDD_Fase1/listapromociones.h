#ifndef LISTAPROMOCIONES_H
#define LISTAPROMOCIONES_H

#include "listabeneficios.h"

#include <iostream>
#include <fstream>
#include <string>


using namespace std;

class Promocion{
public:
    string codigo;
    string nombre;
    string vigencia;
    string diasAplicables;

    Promocion(std::string c = "", std::string n = "", std::string v = "", std::string d = "")
        : codigo(c), nombre(n), vigencia(v), diasAplicables(d) {}

};

class NodoPromocion{
public: 

    Promocion promocion;
    ListaDobleBeneficios listaBeneficios; // Lista de beneficios asociada a la promocion;
    NodoPromocion* siguiente;

    NodoPromocion(const Promocion& p) : promocion(p), siguiente(nullptr) {}

};


class ListaCircularPromociones {
private:
    NodoPromocion* cabeza;
    NodoPromocion* ultimo; // puntero al ultimo nodo 

public:
    ListaCircularPromociones();
    ~ListaCircularPromociones();

    bool estaVacia() const;
    void insertarPromocion(const Promocion& promo);
    bool eliminarPromocion(const std::string& codigo);
    NodoPromocion* buscarPromocion(const std::string& codigo);
    void mostrarPromocionesYBeneficios() const;
    bool generarReportePromociones(const std::string& rutaDot,
                                   const std::string& rutaPng) const;

    // Devuelve el puntero al inicio de la lista circular
    NodoPromocion* getCabeza() const { return cabeza; }


};




#endif // LISTAPROMOCIONES_H
