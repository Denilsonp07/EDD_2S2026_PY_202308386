#ifndef ARBOLBINARIOPELICULAS_H
#define ARBOLBINARIOPELICULAS_H

#include "Pelicula.h"
#include <fstream>
#include <string>

class ListaPeliculas {
private:
    Pelicula* datos;
    int tamanio;
    int capacidad;

    void redimensionar(int nuevaCapacidad) {
        Pelicula* nuevo = new Pelicula[nuevaCapacidad];
        for (int i = 0; i < tamanio; ++i) {
            nuevo[i] = datos[i];
        }
        delete[] datos;
        datos = nuevo;
        capacidad = nuevaCapacidad;
    }

public:
    ListaPeliculas() : datos(nullptr), tamanio(0), capacidad(0) {}

    ~ListaPeliculas() {
        delete[] datos;
    }

    ListaPeliculas(const ListaPeliculas& otra)
        : datos(nullptr), tamanio(otra.tamanio), capacidad(otra.capacidad) {
        if (capacidad > 0) {
            datos = new Pelicula[capacidad];
            for (int i = 0; i < tamanio; ++i) {
                datos[i] = otra.datos[i];
            }
        }
    }

    ListaPeliculas& operator=(const ListaPeliculas& otra) {
        if (this != &otra) {
            delete[] datos;
            tamanio = otra.tamanio;
            capacidad = otra.capacidad;
            datos = nullptr;

            if (capacidad > 0) {
                datos = new Pelicula[capacidad];
                for (int i = 0; i < tamanio; ++i) {
                    datos[i] = otra.datos[i];
                }
            }
        }
        return *this;
    }

    void push_back(const Pelicula& pelicula) {
        if (tamanio == capacidad) {
            int nuevaCapacidad = (capacidad == 0) ? 10 : capacidad * 2;
            redimensionar(nuevaCapacidad);
        }
        datos[tamanio++] = pelicula;
    }

    int size() const {
        return tamanio;
    }

    const Pelicula& operator[](int indice) const {
        return datos[indice];
    }

    Pelicula& operator[](int indice) {
        return datos[indice];
    }

    void clear() {
        delete[] datos;
        datos = nullptr;
        tamanio = 0;
        capacidad = 0;
    }
};

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
    void recorridoInorden(NodoArbolPeliculas* nodo);
    void recolectarInorden(NodoArbolPeliculas* nodo, ListaPeliculas& peliculas) const;

    public: 

    ArbolBinarioDeBusquedaPeliculas();
    ~ArbolBinarioDeBusquedaPeliculas();

    bool estaVacio();
    void insertar(const Pelicula& pelicula);
    void limpiar();
    void cargarCSV(const string& ruta);
    void generarDot();
    void mostrarInorden();
    ListaPeliculas obtenerPeliculasInorden() const;
    
};

#endif // ARBOLBINARIOPELICULAS_H
