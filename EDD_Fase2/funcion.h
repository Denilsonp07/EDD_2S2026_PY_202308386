#ifndef FUNCION_H
#define FUNCION_H

#include <string>
using namespace std;    

class Funcion {

private:
    string codigo_funcion;
    string codigo_pelicula;
    string horario;
    string sala;
    int filas;
    int columnas;
    string archivo_asientos;
    
public:
    Funcion();

    Funcion(std::string codF = "", std::string codP = "", std::string hor = "", 
            std::string sal = "", int f = 0, int c = 0);

    // Getters
    string getCodigoFuncion() const;
    string getCodigoPelicula() const;
    string getHorario() const;
    string getSala() const;
    int getFilas() const;
    int getColumnas() const;
    string getArchivoAsientos() const;

    // Setters
    void setCodigoFuncion(const string& codigo_funcion);
    void setCodigoPelicula(const string& codigo_pelicula);
    void setHorario(const string& horario);
    void setSala(const string& sala);
    void setFilas(int filas);
    void setColumnas(int columnas);
    void setArchivoAsientos(const string& archivo_asientos);


};
#endif // FUNCION_H