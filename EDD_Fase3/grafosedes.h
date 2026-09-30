#ifndef GRAFOSEDES_H
#define GRAFOSEDES_H

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

//  Estructura para la Sede
class Sede {
public:
    string codigo;
    string nombre;
    string direccion;
    int cantidadSalas;
    string telefono;

    Sede(string cod = "", string nom = "", string dir = "", int salas = 0, string tel = "")
        : codigo(cod), nombre(nom), direccion(dir), cantidadSalas(salas), telefono(tel) {}
};

// Nodo Adyacencia (La Arista / Conexión)
class Arco {
public:
    string codigoDestino; // Código de la sede con la que conecta
    int peso;             // Cantidad de películas en común
    Arco* siguiente;      // Siguiente conexión en la lista de adyacencia

    Arco(string destino, int p) : codigoDestino(destino), peso(p), siguiente(nullptr) {}
};

// Nodo del Grafo (Vertice)
class VerticeSede {
public:
    Sede sede;
    VerticeSede* siguiente; // Siguiente vértice en la lista principal
    Arco* listaAdyacencia;  // Cabeza de la lista de conexiones de este vértice

    VerticeSede(const Sede& s) : sede(s), siguiente(nullptr), listaAdyacencia(nullptr) {}
};

// Estructura Principal del Grafo No Dirigido Ponderado
class GrafoSedes {
private:
    VerticeSede* cabeza; // Inicio de la lista de vértices

    // Métodos auxiliares
    VerticeSede* buscarVertice(const string& codigo) const;
    void agregarArcoDirigido(const string& origen, const string& destino, int peso);
    void eliminarArcosDeVertice(VerticeSede* vertice);

public:
    GrafoSedes();
    ~GrafoSedes();

    // Requisitos Fase 3
    void insertarVertice(const Sede& sede);
    void agregarAristaNoDirigida(const string& origen, const string& destino, int peso);
    
    // Consultas y Reportes
    void mostrarGrafoConsola() const; 
    bool generarReporteGrafo(const string& rutaDot, const string& rutaPng) const;
    bool generarReporteAdyacencia(const string& rutaDot, const string& rutaPng) const;


};

#endif // GRAFOSEDES_H