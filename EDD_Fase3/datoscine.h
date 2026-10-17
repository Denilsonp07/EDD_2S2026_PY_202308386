#ifndef DATOSCINE_H
#define DATOSCINE_H

#include <QString>

#include "arbolbinariopeliculas.h"
#include "arbolbclientes.h"
#include "listapromociones.h"
#include "matrizdisperza.h"
#include "grafosedes.h"

struct FuncionCine {
    QString pelicula;
    QString horario;
    QString sala;
    int filas;
    int columnas;
    MatrizDispersa* asientos;
    FuncionCine* siguiente;

    FuncionCine()
        : filas(0), columnas(0), asientos(nullptr), siguiente(nullptr) {}

    ~FuncionCine()
    {
        delete asientos;
    }

    FuncionCine(const FuncionCine&) = delete;
    FuncionCine& operator=(const FuncionCine&) = delete;
};

struct DatosCine {
    ArbolBinarioDeBusquedaPeliculas arbolPeliculas;
    ArbolAVLFunciones arbolFunciones;
    ArbolBClientes arbolClientes;
    TablaHash tablaReservas;
    ListaCircularPromociones promociones;
    GrafoSedes grafoSedes;
    FuncionCine* primeraFuncion;
    int totalFunciones;

    DatosCine()
        : primeraFuncion(nullptr), totalFunciones(0) {}

    ~DatosCine()
    {
        limpiarFunciones();
    }

    void agregarFuncion(FuncionCine* funcion)
    {
        if (funcion == nullptr) {
            return;
        }

        funcion->siguiente = nullptr;
        if (primeraFuncion == nullptr) {
            primeraFuncion = funcion;
        } else {
            FuncionCine* actual = primeraFuncion;
            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }
            actual->siguiente = funcion;
        }
        ++totalFunciones;
    }

    FuncionCine* obtenerFuncion(int indice) const
    {
        if (indice < 0 || indice >= totalFunciones) {
            return nullptr;
        }

        FuncionCine* actual = primeraFuncion;
        for (int posicion = 0; posicion < indice; ++posicion) {
            actual = actual->siguiente;
        }
        return actual;
    }

    void limpiarFunciones()
    {
        while (primeraFuncion != nullptr) {
            FuncionCine* siguiente = primeraFuncion->siguiente;
            delete primeraFuncion;
            primeraFuncion = siguiente;
        }
        totalFunciones = 0;
    }
};

inline DatosCine& datosCine()
{
    static DatosCine datos;
    return datos;
}

#endif // DATOSCINE_H