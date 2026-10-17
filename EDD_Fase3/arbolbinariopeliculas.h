#ifndef ARBOLBINARIOPELICULAS_H
#define ARBOLBINARIOPELICULAS_H

#include "Pelicula.h"
#include <fstream>
#include <string>

#include "arbolavlfunciones.h"

class GrafoSedes;

class ListaMensajes {
private:
    std::string* datos;
    int tamanio;
    int capacidad;

public:
    ListaMensajes() : datos(nullptr), tamanio(0), capacidad(0) {}
    ListaMensajes(const ListaMensajes& otra)
        : datos(nullptr), tamanio(0), capacidad(0) {
        for (int i = 0; i < otra.tamanio; ++i) push_back(otra.datos[i]);
    }
    ~ListaMensajes() { delete[] datos; }

    ListaMensajes& operator=(const ListaMensajes& otra) {
        if (this != &otra) {
            delete[] datos;
            datos = nullptr;
            tamanio = 0;
            capacidad = 0;
            for (int i = 0; i < otra.tamanio; ++i) push_back(otra.datos[i]);
        }
        return *this;
    }

    void push_back(const std::string& mensaje) {
        if (tamanio == capacidad) {
            int nuevaCapacidad = capacidad == 0 ? 4 : capacidad * 2;
            std::string* nuevo = new std::string[nuevaCapacidad];
            for (int i = 0; i < tamanio; ++i) nuevo[i] = datos[i];
            delete[] datos;
            datos = nuevo;
            capacidad = nuevaCapacidad;
        }
        datos[tamanio++] = mensaje;
    }

    bool empty() const { return tamanio == 0; }
    int size() const { return tamanio; }
    const std::string& operator[](int indice) const { return datos[indice]; }
};

struct ResultadoCargaPeliculas {
    int peliculasInsertadas = 0;
    int funcionesInsertadas = 0;
    ListaMensajes peliculasRechazadas;
    std::string error;
};

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

        Pelicula* getPunteroPelicula() { return &pelicula; }

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
    NodoArbolPeliculas* eliminarNodo(NodoArbolPeliculas* nodo, int codigoNumerico, bool& eliminado);
    NodoArbolPeliculas* obtenerNodoMinimo(NodoArbolPeliculas* nodo) const;
    
    void generarDotAux(NodoArbolPeliculas* nodo, ofstream& archivo);
    void generarReporteAux(NodoArbolPeliculas* nodo, ofstream& archivo,
                           const string& fechaActual);
    void eliminarNodos(NodoArbolPeliculas* nodo);
    int numeroCodigo(const string& codigo) const;
    void recorridoInorden(NodoArbolPeliculas* nodo);
    void recolectarInorden(NodoArbolPeliculas* nodo, ListaPeliculas& peliculas) const;
    

    //METODOS FASEEEE 2
    
    void recolectarPreorden(NodoArbolPeliculas* nodo, ListaPeliculas& peliculas) const;
    void recolectarPostorden(NodoArbolPeliculas* nodo, ListaPeliculas& peliculas) const;

    



    public: 

    ArbolBinarioDeBusquedaPeliculas();
    ~ArbolBinarioDeBusquedaPeliculas();

    bool estaVacio();
    void insertar(const Pelicula& pelicula);
    void limpiar();
    void generarDot();
    bool generarReporteCartelera(const string& rutaDot,
                                 const string& rutaPng,
                                 const string& fechaActual) const;
    void mostrarInorden();
    ListaPeliculas obtenerPeliculasInorden() const;
    bool eliminarPorCodigo(const string& codigo);

    //METODOS FASEEEE 2

    ListaPeliculas obtenerPeliculasPreorden() const;
    ListaPeliculas obtenerPeliculasPostorden() const;

    Pelicula* buscarPorCodigo(const string& codigo); 

    ResultadoCargaPeliculas cargarJSON(const std::string& ruta, ArbolAVLFunciones& arbolFunciones, GrafoSedes& grafo);
    
    
};

#endif // ARBOLBINARIOPELICULAS_H
