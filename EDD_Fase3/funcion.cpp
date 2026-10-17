#include "funcion.h"

Funcion::Funcion()
    : filas(0), columnas(0) {}

Funcion::Funcion(string codF, string codP, string hor,
                 string sal, int f, int c, string fe, string se)
    : codigo_funcion(codF), codigo_pelicula(codP), horario(hor), sala(sal),
      filas(f), columnas(c), fecha(fe), sede(se) {}

string Funcion::getCodigoFuncion() const { return codigo_funcion; }
string Funcion::getCodigoPelicula() const { return codigo_pelicula; }
string Funcion::getHorario() const { return horario; }
string Funcion::getSala() const { return sala; }
int Funcion::getFilas() const { return filas; }
int Funcion::getColumnas() const { return columnas; }
string Funcion::getArchivoAsientos() const { return archivo_asientos; }
string Funcion::getFecha() const { return fecha; }
string Funcion::getSede() const { return sede; }

void Funcion::setCodigoFuncion(const string& codigo) { codigo_funcion = codigo; }
void Funcion::setCodigoPelicula(const string& codigo) { codigo_pelicula = codigo; }
void Funcion::setHorario(const string& valor) { horario = valor; }
void Funcion::setSala(const string& valor) { sala = valor; }
void Funcion::setFilas(int valor) { filas = valor; }
void Funcion::setColumnas(int valor) { columnas = valor; }
void Funcion::setArchivoAsientos(const string& archivo) { archivo_asientos = archivo; }
void Funcion::setFecha(const string& valor) { fecha = valor; }
void Funcion::setSede(const string& valor) { sede = valor; }