#ifndef CLIENTE_H
#define CLIENTE_H
#include <string>

using namespace std;

// Nodo manual para guardar solo los codigos de reserva del cliente
struct NodoReservaRef {
    string codigo_reserva;
    NodoReservaRef* siguiente;
    NodoReservaRef(string cod) : codigo_reserva(cod), siguiente(nullptr) {}
};

class Cliente {
private:
    string id;
    string nombre;
    string correo; 
    string telefono;
    string password;
    string tipo;
    NodoReservaRef* cabezaReservas;

public:
    Cliente(string _id = "", string _nombre = "", string _correo = "", 
            string _telefono = "", string _password = "") 
        : id(_id), nombre(_nombre), correo(_correo), telefono(_telefono), 
          password(_password), tipo("cliente"), cabezaReservas(nullptr) {}

    ~Cliente() {
        // Liberacion de memoria de las reservas vinculadas
        NodoReservaRef* actual = cabezaReservas;
        while (actual != nullptr) {
            NodoReservaRef* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
    }

    // Getters principales
    string getId() const { return id; }
    string getNombre() const { return nombre; }
    string getCorreo() const { return correo; }
    string getTelefono() const { return telefono; }
    string getPassword() const { return password; }
    NodoReservaRef* getCabezaReservas() const {return cabezaReservas; }

    // Agregar referencia de reserva sin duplicar datos
    void agregarReserva(const string& codigo_reserva) {
        NodoReservaRef* nuevo = new NodoReservaRef(codigo_reserva);
        if (!cabezaReservas) {
            cabezaReservas = nuevo;
        } else {
            NodoReservaRef* actual = cabezaReservas;
            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevo;
        }
    }
};
#endif