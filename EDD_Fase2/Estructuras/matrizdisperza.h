#ifndef MATRIZDISPERSA_H
#define MATRIZDISPERSA_H
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

// NodoAsiento representa un asiento ocupado
// Si un asiento no tiene nodo, se asume que esta libre
class NodoAsiento {
private:
    int fila;
    int columna;
    string cliente;
    NodoAsiento* derecha; // siguiente asiento ocupado en la misma fila
    NodoAsiento* abajo;   // siguiente asiento ocupado en la misma columna

public:
    NodoAsiento(int fila, int columna, const string& cliente)
        : fila(fila), columna(columna), cliente(cliente), derecha(nullptr), abajo(nullptr) {}

    int getFila() const { return fila; }
    int getColumna() const { return columna; }
    std::string getCliente() const { return cliente; }
    NodoAsiento* getDerecha() const { return derecha; }
    NodoAsiento* getAbajo() const { return abajo; }

    void setDerecha(NodoAsiento* n) { derecha = n; }
    void setAbajo(NodoAsiento* n) { abajo = n; }
};

// NodoCabecera representa la entrada a una fila o columna que tiene
// al menos un asiento ocupado
class NodoCabecera {
private:
    int indice;              // numero de fila o de columna
    NodoAsiento* primero;    // primer asiento ocupado de esa fila/columna
    NodoCabecera* siguiente; // siguiente cabecera de fila/columna

public:
    NodoCabecera(int indice)
        : indice(indice), primero(nullptr), siguiente(nullptr) {}

    int getIndice() const { return indice; }
    NodoAsiento* getPrimero() const { return primero; }
    NodoCabecera* getSiguiente() const { return siguiente; }

    void setPrimero(NodoAsiento* n) { primero = n; }
    void setSiguiente(NodoCabecera* c) { siguiente = c; }
};

class MatrizDispersa {
private:
    int totalFilas;
    int totalColumnas;
    NodoCabecera* cabecerasFila;    // lista de cabeceras de fila
    NodoCabecera* cabecerasColumna; // lista de cabeceras de columna

    // busca (o crea si no existe) la cabecera de una fila/columna
    NodoCabecera* obtenerOCrearCabeceraFila(int fila);
    NodoCabecera* obtenerOCrearCabeceraColumna(int columna);

public:
    MatrizDispersa(int filas, int columnas);
    ~MatrizDispersa();

    void limpiar();

    void cargarDesdeJSON(const string& rutaArchivo, int filas, int columnas);
    void guardarAJSON(const string& rutaArchivo, const string& codigoFuncion);

    bool reservar(int fila, int columna, const string& codigo_reserva); 
    bool cancelar(int fila, int columna);
    bool estaOcupado(int fila, int columna);
    string getClienteAsiento(int fila, int columna); // "" si libre

    int contarOcupados();
    int contarLibres();

    void generarDot(const string& tituloFuncion);
    bool generarReporteAsientos(const string& tituloFuncion,
                                const string& rutaDot,
                                const string& rutaPng);
};

#endif