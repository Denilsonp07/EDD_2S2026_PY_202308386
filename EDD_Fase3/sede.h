#ifndef SEDE_H
#define SEDE_H

#include <string>

class Sede {
private:
    std::string nombre;
    std::string ubicacion;

public:
    Sede(const std::string& n, const std::string& u) : nombre(n), ubicacion(u) {}

    const std::string& getNombre() const { return nombre; }
    const std::string& getUbicacion() const { return ubicacion; }
};

#endif // SEDE_H