#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <string>
#include <fstream>

#include "json.hpp"
#include "tablahashreservas.h"

using namespace std;

string sha256(const string& input);

class Transaccion {
public:
    string codigo_transaccion;
    string codigo_reserva;
    string id_cliente;
    string codigo_funcion;
    int fila;
    int columna;
    float monto;
    string metodo_pago;
    string fecha_pago;

    Transaccion(string codigoTransaccion = "", string codigoReserva = "",
                string idCliente = "", string codigoFuncion = "", int fila_ = 0,
                int columna_ = 0, float monto_ = 0.0f,
                string metodoPago = "", string fechaPago = "");

    string aCadena() const;
    nlohmann::json aJson() const;
    static Transaccion desdeJson(const nlohmann::json& objeto);
};

class NodoTransaccion {
public:
    Transaccion transaccion;
    NodoTransaccion* siguiente;

    explicit NodoTransaccion(const Transaccion& valor)
        : transaccion(valor), siguiente(nullptr) {}
};

class ListaTransacciones {
private:
    NodoTransaccion* cabeza;
    int cantidad;

public:
    ListaTransacciones();
    ListaTransacciones(const ListaTransacciones& otra);
    ~ListaTransacciones();

    ListaTransacciones& operator=(const ListaTransacciones& otra);
    void insertar(const Transaccion& transaccion);
    void insertarAlFinal(const Transaccion& transaccion);
    bool estaVacia() const;
    int size() const;
    NodoTransaccion* obtenerCabeza() const;
    const Transaccion* buscarPorCodigo(const string& codigo) const;
    void limpiar();
};

class NodoMerkle {
public:
    string hash;
    NodoMerkle* izq;
    NodoMerkle* der;

    explicit NodoMerkle(const string& valor)
        : hash(valor), izq(nullptr), der(nullptr) {}
};

class ArbolMerkle {
private:
    NodoMerkle* raiz;
    void liberar(NodoMerkle* nodo);
    NodoMerkle* construirNivel(NodoMerkle** nodos, int cantidad);
    void escribirDot(NodoMerkle* nodo, ofstream& archivo, int& contador) const;

public:
    ArbolMerkle();
    explicit ArbolMerkle(const ListaTransacciones& transacciones);
    ~ArbolMerkle();

    void construir(const ListaTransacciones& transacciones);
    string obtenerRoot() const;
    NodoMerkle* obtenerRaiz() const;
    bool generarReporte(const string& rutaDot, const string& rutaPng) const;
};

class Bloque {
public:
    int INDEX;
    string TIMESTAMP;
    int NONCE;
    ListaTransacciones* DATA;
    string ROOTMERKLE;
    string PREVIOUSHASH;
    string HASH;
    Bloque* siguiente;

    Bloque();
    Bloque(int index, const string& timestamp, int nonce,
           ListaTransacciones* data, const string& rootMerkle,
           const string& previousHash, const string& hash);
    ~Bloque();
};

class Blockchain {
private:
    Bloque* cabeza;
    ListaTransacciones pendientes;
    int cantidadPendientes;

    string obtenerTimestampActual() const;
    string calcularHash(const Bloque& bloque) const;
    bool hashIntegro(const Bloque& bloque, const string& hashAnterior) const;
    void insertarBloqueAdelante(Bloque* bloque);
    Bloque* buscarBloque(int index) const;
    bool guardarBloqueJSONInterno(const Bloque* bloque) const;

public:
    Blockchain();
    ~Blockchain();

    void agregarTransaccion(const Transaccion& transaccion);
    void registrarTransaccion(const Transaccion& transaccion);
    bool generarBloque();
    bool guardarBloqueJSON(Bloque* bloque);
    bool cargarYVerificarCadena(TablaHash& tablaReservas);

    void generarReporteBlockchain() const;
    bool generarReporteMerkle(int indexBloque) const;
    Bloque* obtenerCabeza() const;
    int obtenerCantidadPendientes() const;
};

#endif
