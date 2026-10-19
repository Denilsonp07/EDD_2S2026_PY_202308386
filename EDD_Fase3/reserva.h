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
    string id_cliente;
    float monto;
    string metodo_pago;
    string codigo_transaccion;

    Reserva(string codR = "", string codF = "", int f = 0, int c = 0,
            string fecha = "", string idCliente = "", float monto_ = 0.0f,
            string metodoPago = "", string codigoTransaccion = "")
        : codigo_reserva(codR), codigo_funcion(codF), fila(f), columna(c),
          fecha_reserva(fecha), id_cliente(idCliente), monto(monto_),
          metodo_pago(metodoPago), codigo_transaccion(codigoTransaccion) {}
};

// Nodo para manejar colisiones mediante listas enlazadas
struct NodoHash {
    Reserva reserva;
    NodoHash* siguiente;

    NodoHash(const Reserva& res) : reserva(res), siguiente(nullptr) {}
};
#endif