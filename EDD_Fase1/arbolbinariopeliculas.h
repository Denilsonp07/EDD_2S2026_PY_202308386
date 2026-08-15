#ifndef ARBOLBINARIOPELICULAS_H
#define ARBOLBINARIOPELICULAS_H

#include "Pelicula.h"
#include <fstream>
#include <vector>
#include <string>

//Nodo
class NodoArbolPeliculas {
private:

    Pelicula pelicula;
    NodoArbolPeliculas* izq;
    NodoArbolPeliculas* der;

public:
    NodoArbolPeliculas(const Pelicula& pelicula) 
        : pelicula(pelicula), izq(nullptr), der(nullptr) {}

        Pelicula getPelicula() const { return pelicula; }
        NodoArbolPeliculas* getIzq() const { return izq; }
        NodoArbolPeliculas* getDer() const { return der; }

        void setPelicula(const Pelicula& pelicula) { this->pelicula = pelicula; }
        void setIzq(NodoArbolPeliculas* izq) { this->izq = izq; }
        void setDer(NodoArbolPeliculas* der) { this->der = der; }   
};

//Arbol 
class ArbolBinarioDeBusquedaPeliculas {
private:
    NodoArbolPeliculas* raiz;

    NodoArbolPeliculas* insertarNodo(NodoArbolPeliculas* nodo, const Pelicula& pelicula);
    void generarDotAux(NodoArbolPeliculas* nodo, ofstream& archivo);
    void eliminarNodos(NodoArbolPeliculas* nodo);
    string quitarComillas(const string& campo);
    int numeroCodigo(const string& codigo) const;

    public: 

    ArbolBinarioDeBusquedaPeliculas();
    ~ArbolBinarioDeBusquedaPeliculas();

    bool estaVacio();
    void insertar(const Pelicula& pelicula);
    void cargarCSV(const string& ruta);
    void generarDot();
    
};

#endif // ARBOLBINARIOPELICULAS_H
