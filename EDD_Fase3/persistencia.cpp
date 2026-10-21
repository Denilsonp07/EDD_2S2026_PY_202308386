#include "persistencia.h"

#include <iostream>

#include "datoscine.h"
#include "lzw.h"

namespace {
void guardarTextoComprimido(const string& ruta, const string& texto) {
    CompresorLZW compresor;
    ListaCodigos codigos = compresor.comprimir(texto);
    guardarArchivoEDD(ruta, codigos);
}
}

void guardarEstadoSistema() {
    DatosCine& datos = datosCine();
    guardarTextoComprimido("sedes.edd", datos.grafoSedes.serializarSedes());
    guardarTextoComprimido("peliculas.edd",
                           datos.arbolPeliculas.serializarPeliculas());
    guardarTextoComprimido("funciones.edd",
                           datos.arbolFunciones.serializarFunciones());
}
