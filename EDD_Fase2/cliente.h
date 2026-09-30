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
            string _telefono = "", string _password = "", string _tipo = "cliente") 
        : id(_id), nombre(_nombre), correo(_correo), telefono(_telefono), 
          password(_password), tipo(_tipo), cabezaReservas(nullptr) {}

    Cliente(const Cliente& otro)
        : id(otro.id), nombre(otro.nombre), correo(otro.correo),
          telefono(otro.telefono), password(otro.password), tipo(otro.tipo),
          cabezaReservas(nullptr)
    {
        for (NodoReservaRef* reserva = otro.cabezaReservas;
             reserva != nullptr;
             reserva = reserva->siguiente) {
            agregarReserva(reserva->codigo_reserva);
        }
    }

    Cliente& operator=(const Cliente& otro)
    {
        if (this == &otro) {
            return *this;
        }

        NodoReservaRef* actual = cabezaReservas;
        while (actual != nullptr) {
            NodoReservaRef* siguiente = actual->siguiente;
            delete actual;
            actual = siguiente;
        }

        id = otro.id;
        nombre = otro.nombre;
        correo = otro.correo;
        telefono = otro.telefono;
        password = otro.password;
        tipo = otro.tipo;
        cabezaReservas = nullptr;

        for (NodoReservaRef* reserva = otro.cabezaReservas;
             reserva != nullptr;
             reserva = reserva->siguiente) {
            agregarReserva(reserva->codigo_reserva);
        }
        return *this;
    }

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
    string getTipo() const { return tipo; }
    NodoReservaRef* getCabezaReservas() const {return cabezaReservas; }

    void setNombre(const string& valor) { nombre = valor; }
    void setCorreo(const string& valor) { correo = valor; }
    void setTelefono(const string& valor) { telefono = valor; }
    void setPassword(const string& valor) { password = valor; }

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

    bool eliminarReserva(const string& codigo_reserva) {
        NodoReservaRef* anterior = nullptr;
        NodoReservaRef* actual = cabezaReservas;
        while (actual != nullptr) {
            if (actual->codigo_reserva == codigo_reserva) {
                if (anterior == nullptr) {
                    cabezaReservas = actual->siguiente;
                } else {
                    anterior->siguiente = actual->siguiente;
                }
                delete actual;
                return true;
            }
            anterior = actual;
            actual = actual->siguiente;
        }
        return false;
    }
};
#endif