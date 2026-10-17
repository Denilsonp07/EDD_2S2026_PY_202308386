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

    string fecha; // Nueva variable para almacenar la fecha de la función   
    string sede;  // Nueva variable para almacenar la sede de la función    
    
public:
    Funcion();

    Funcion(string codF = "", string codP = "", string hor = "", 
            string sal = "", int f = 0, int c = 0, string fe = "", string se = "");

    // Getters
    string getCodigoFuncion() const;
    string getCodigoPelicula() const;
    string getHorario() const;
    string getSala() const;
    int getFilas() const;
    int getColumnas() const;
    string getArchivoAsientos() const;
    string getFecha() const;
    string getSede() const;

    // Setters
    void setCodigoFuncion(const string& codigo_funcion);
    void setCodigoPelicula(const string& codigo_pelicula);
    void setHorario(const string& horario);
    void setSala(const string& sala);
    void setFilas(int filas);
    void setColumnas(int columnas);
    void setArchivoAsientos(const string& archivo_asientos);
    void setFecha(const string& fecha);
    void setSede(const string& sede);


};
#endif // FUNCION_H