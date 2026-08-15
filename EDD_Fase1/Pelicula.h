#ifndef PELICULA_H
#define PELICULA_H

#include <iostream>
#include <string>
using namespace std;


class Pelicula {

private:   

    string codigo;
    string titulo;
    string genero;
    string duracion;
    string clasificacion;
    string idioma;
    string fecha_streno;
    string fecha_fin;

public: 

    // Constructor
    Pelicula(const string& codigo, const string& titulo,
             const string& genero = "", const string& duracion = "", const string& clasificacion = "",
             const string& idioma = "", const string& fecha_streno = "", const string& fecha_fin = "");

    // Getters
    const string& getCodigo() const; { return codigo; }
    const string& getTitulo() const; { return titulo; }
    const string& getGenero() const; { return genero; }
    const string& getDuracion() const; { return duracion; }
    const string& getClasificacion() const; { return clasificacion; }
    const string& getIdioma() const; { return idioma; }
    const string& getFechaEstreno() const; { return fecha_streno; }
    const string& getFechaFin() const; { return fecha_fin; }

    friend ostream& operator<<(ostream& os, const Pelicula& pelicula);
        
    }
};




#endif // PELICULA_H
