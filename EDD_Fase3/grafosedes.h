#ifndef GRAFOSEDES_H
#define GRAFOSEDES_H

#include <fstream>
#include <iostream>
#include <string>

#include "arbolavlfunciones.h"

using namespace std;

class Sede {
public:
    string codigo;
    string nombre;
    string direccion;
    int cantidad_salas;
    string telefono;

    Sede(string cod = "", string nom = "", string dir = "", int salas = 0,
         string tel = "")
        : codigo(cod), nombre(nom), direccion(dir), cantidad_salas(salas),
          telefono(tel) {}
};

class Arco {
public:
    string codigoDestino;
    int peso;
    Arco* siguiente;

    Arco(const string& destino, int valor)
        : codigoDestino(destino), peso(valor), siguiente(nullptr) {}
};

class VerticeSede {
public:
    Sede sede;
    VerticeSede* siguiente;
    Arco* listaAdyacencia;

    explicit VerticeSede(const Sede& valor)
        : sede(valor), siguiente(nullptr), listaAdyacencia(nullptr) {}
};

class GrafoSedes {
private:
    VerticeSede* cabeza;

    void agregarArcoDirigido(const string& origen, const string& destino,
                             int peso);
    void eliminarArcosDeVertice(VerticeSede* vertice);
    int cantidadVertices() const;
    bool existeArco(const VerticeSede* vertice, const string& destino) const;

public:
    GrafoSedes();
    ~GrafoSedes();

    VerticeSede* buscarVertice(const string& codigo) const;
    void limpiar();
    string serializarSedes() const;
    void insertarVertice(const Sede& sede);
    void agregarAristaNoDirigida(const string& origen, const string& destino,
                                 int peso);

    void calcularAristas(ArbolAVLFunciones& arbolFunciones);
    void verPeliculasPorSede(const string& codigoSede,
                             ArbolAVLFunciones& arbolFunciones) const;
    void buscarCaminoMasCorto(const string& origen,
                              const string& destino) const;
    void verRankingSedesSimilares() const;

    void mostrarGrafoConsola() const;
    bool generarReporteGrafo(
        const string& rutaDot = "reporte_grafo_sedes.dot",
        const string& rutaPng = "reporte_grafo_sedes.png") const;
    bool generarReporteListaAdyacencia(
        const string& rutaDot = "reporte_lista_adyacencia.dot",
        const string& rutaPng = "reporte_lista_adyacencia.png") const;
    bool generarReporteAdyacencia(
        const string& rutaDot = "reporte_lista_adyacencia.dot",
        const string& rutaPng = "reporte_lista_adyacencia.png") const;
};

#endif
