#ifndef LISTABENEFICIOS_H
#define LISTABENEFICIOS_H
#include <iostream>
#include <string>

using namespace std;



class Beneficio{
public:
    string tipo; 
    string descripcion;
    string valor;

    Beneficio(string t, string d, string v)
    : tipo(t), descripcion(d), valor(v) {}

};

class  NodoBeneficio{
public:

    Beneficio beneficio;
    NodoBeneficio* siguiente;
    NodoBeneficio* anterior;

    

    NodoBeneficio(const Beneficio& b) : beneficio(b), siguiente(nullptr), anterior(nullptr) {}
};

class ListaDobleBeneficios {
private:
    NodoBeneficio* cabeza;
    NodoBeneficio* cola; 

public:

    ListaDobleBeneficios();
    ~ListaDobleBeneficios();

    bool estaVacia() const;
    void insertarFinal(const Beneficio& beneficio);
    void mostrarBeneficios() const;

    // Devuelve el puntero al inicio de la lista
    NodoBeneficio* getCabeza() const { return cabeza; }
};


#endif // LISTABENEFICIOS_H
