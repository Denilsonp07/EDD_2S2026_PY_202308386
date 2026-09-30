#include "Pelicula.h"

Pelicula::Pelicula()
    : codigo(""), titulo(""), genero(""), duracion(""), clasificacion(""),
      idioma(""), fecha_streno(""), fecha_fin("") {}

Pelicula::Pelicula(const string& codigo, const string& titulo,
                   const string& genero, const string& duracion, const string& clasificacion,
                   const string& idioma, const string& fecha_streno, const string& fecha_fin)
    : codigo(codigo), titulo(titulo), genero(genero), duracion(duracion),
      clasificacion(clasificacion), idioma(idioma), fecha_streno(fecha_streno), fecha_fin(fecha_fin) {}


std::ostream& operator<<(std::ostream& os, const Pelicula& pelicula){
    os << "Codigo: " << pelicula.codigo << ", Titulo:" << pelicula.titulo;
    return os;
}      
