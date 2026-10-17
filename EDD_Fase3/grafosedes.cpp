#include "grafosedes.h"

#include <cstdlib>

namespace {
struct PeliculaSede {
    string codigo;
    PeliculaSede* siguiente;
    explicit PeliculaSede(const string& valor) : codigo(valor), siguiente(nullptr) {}
};

class CatalogoSede {
public:
    PeliculaSede* cabeza;
    CatalogoSede() : cabeza(nullptr) {}
    ~CatalogoSede() {
        while (cabeza != nullptr) {
            PeliculaSede* siguiente = cabeza->siguiente;
            delete cabeza;
            cabeza = siguiente;
        }
    }

    void agregarUnica(const string& codigo) {
        for (PeliculaSede* actual = cabeza; actual != nullptr;
             actual = actual->siguiente) {
            if (actual->codigo == codigo) return;
        }
        PeliculaSede* nuevo = new PeliculaSede(codigo);
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
    }

    bool contiene(const string& codigo) const {
        for (PeliculaSede* actual = cabeza; actual != nullptr;
             actual = actual->siguiente) {
            if (actual->codigo == codigo) return true;
        }
        return false;
    }
};

struct ConexionRanking {
    string origen;
    string destino;
    int peso;
};

void intercambiar(ConexionRanking& izquierda, ConexionRanking& derecha) {
    ConexionRanking temporal = izquierda;
    izquierda = derecha;
    derecha = temporal;
}
}

GrafoSedes::GrafoSedes() : cabeza(nullptr) {}

GrafoSedes::~GrafoSedes() {
    limpiar();
}

void GrafoSedes::limpiar() {
    while (cabeza != nullptr) {
        VerticeSede* siguiente = cabeza->siguiente;
        eliminarArcosDeVertice(cabeza);
        delete cabeza;
        cabeza = siguiente;
    }
}

void GrafoSedes::eliminarArcosDeVertice(VerticeSede* vertice) {
    while (vertice->listaAdyacencia != nullptr) {
        Arco* siguiente = vertice->listaAdyacencia->siguiente;
        delete vertice->listaAdyacencia;
        vertice->listaAdyacencia = siguiente;
    }
}

VerticeSede* GrafoSedes::buscarVertice(const string& codigo) const {
    for (VerticeSede* actual = cabeza; actual != nullptr;
         actual = actual->siguiente) {
        if (actual->sede.codigo == codigo) return actual;
    }
    return nullptr;
}

void GrafoSedes::insertarVertice(const Sede& sede) {
    if (sede.codigo.empty() || buscarVertice(sede.codigo) != nullptr) return;
    VerticeSede* nuevo = new VerticeSede(sede);
    if (cabeza == nullptr) {
        cabeza = nuevo;
        return;
    }
    VerticeSede* actual = cabeza;
    while (actual->siguiente != nullptr) actual = actual->siguiente;
    actual->siguiente = nuevo;
}

void GrafoSedes::agregarArcoDirigido(const string& origen,
                                     const string& destino, int peso) {
    VerticeSede* vertice = buscarVertice(origen);
    if (vertice == nullptr || buscarVertice(destino) == nullptr || peso <= 0) {
        return;
    }
    for (Arco* actual = vertice->listaAdyacencia; actual != nullptr;
         actual = actual->siguiente) {
        if (actual->codigoDestino == destino) {
            actual->peso = peso;
            return;
        }
    }
    Arco* nuevo = new Arco(destino, peso);
    nuevo->siguiente = vertice->listaAdyacencia;
    vertice->listaAdyacencia = nuevo;
}

void GrafoSedes::agregarAristaNoDirigida(const string& origen,
                                         const string& destino, int peso) {
    if (origen == destino) return;
    agregarArcoDirigido(origen, destino, peso);
    agregarArcoDirigido(destino, origen, peso);
}

int GrafoSedes::cantidadVertices() const {
    int cantidad = 0;
    for (VerticeSede* actual = cabeza; actual != nullptr;
         actual = actual->siguiente) ++cantidad;
    return cantidad;
}

bool GrafoSedes::existeArco(const VerticeSede* vertice,
                            const string& destino) const {
    for (Arco* actual = vertice->listaAdyacencia; actual != nullptr;
         actual = actual->siguiente) {
        if (actual->codigoDestino == destino) return true;
    }
    return false;
}

void GrafoSedes::calcularAristas(ArbolAVLFunciones& arbolFunciones) {
    int cantidad = cantidadVertices();
    VerticeSede* vertice = cabeza;
    while (vertice != nullptr) {
        eliminarArcosDeVertice(vertice);
        vertice = vertice->siguiente;
    }
    if (cantidad < 2) return;

    VerticeSede** vertices = new VerticeSede*[cantidad];
    CatalogoSede* catalogos = new CatalogoSede[cantidad];
    vertice = cabeza;
    for (int i = 0; i < cantidad; ++i) {
        vertices[i] = vertice;
        vertice = vertice->siguiente;
    }

    ListaFunciones funciones = arbolFunciones.obtenerFuncionesInorden();
    for (int i = 0; i < funciones.size(); ++i) {
        Funcion& funcion = funciones[i];
        for (int j = 0; j < cantidad; ++j) {
            if (vertices[j]->sede.codigo == funcion.getSede()) {
                catalogos[j].agregarUnica(funcion.getCodigoPelicula());
                break;
            }
        }
    }

    for (int i = 0; i < cantidad; ++i) {
        for (int j = i + 1; j < cantidad; ++j) {
            int comunes = 0;
            for (PeliculaSede* pelicula = catalogos[i].cabeza;
                 pelicula != nullptr; pelicula = pelicula->siguiente) {
                if (catalogos[j].contiene(pelicula->codigo)) ++comunes;
            }
            if (comunes > 0) {
                agregarAristaNoDirigida(vertices[i]->sede.codigo,
                                        vertices[j]->sede.codigo, comunes);
            }
        }
    }
    delete[] catalogos;
    delete[] vertices;
}

void GrafoSedes::verPeliculasPorSede(const string& codigoSede,
                                     ArbolAVLFunciones& arbolFunciones) const {
    if (buscarVertice(codigoSede) == nullptr) {
        cout << "La sede no existe: " << codigoSede << endl;
        return;
    }
    ListaFunciones funciones = arbolFunciones.obtenerFuncionesInorden();
    bool encontrada = false;
    for (int i = 0; i < funciones.size(); ++i) {
        Funcion& funcion = funciones[i];
        if (funcion.getSede() == codigoSede) {
            encontrada = true;
            cout << "Pelicula: " << funcion.getCodigoPelicula()
                 << " | Horario: " << funcion.getHorario()
                 << " | Sala: " << funcion.getSala() << endl;
        }
    }
    if (!encontrada) cout << "La sede no tiene funciones registradas." << endl;
}

void GrafoSedes::buscarCaminoMasCorto(const string& origen,
                                      const string& destino) const {
    int cantidad = cantidadVertices();
    VerticeSede* inicio = buscarVertice(origen);
    VerticeSede* fin = buscarVertice(destino);
    if (inicio == nullptr || fin == nullptr) {
        cout << "La sede de origen o destino no existe." << endl;
        return;
    }
    if (origen == destino) {
        cout << "Ruta: " << origen << endl;
        return;
    }

    VerticeSede** vertices = new VerticeSede*[cantidad];
    bool* visitado = new bool[cantidad];
    int* anterior = new int[cantidad];
    int indiceInicio = -1;
    int indiceFin = -1;
    VerticeSede* actual = cabeza;
    for (int i = 0; i < cantidad; ++i) {
        vertices[i] = actual;
        visitado[i] = false;
        anterior[i] = -1;
        if (actual == inicio) indiceInicio = i;
        if (actual == fin) indiceFin = i;
        actual = actual->siguiente;
    }

    int* cola = new int[cantidad];
    int frente = 0;
    int final = 0;
    cola[final++] = indiceInicio;
    visitado[indiceInicio] = true;
    while (frente < final) {
        int indice = cola[frente++];
        for (Arco* arco = vertices[indice]->listaAdyacencia; arco != nullptr;
             arco = arco->siguiente) {
            for (int i = 0; i < cantidad; ++i) {
                if (!visitado[i] && vertices[i]->sede.codigo == arco->codigoDestino) {
                    visitado[i] = true;
                    anterior[i] = indice;
                    cola[final++] = i;
                    break;
                }
            }
        }
    }

    if (!visitado[indiceFin]) {
        cout << "No existe una ruta entre " << origen << " y " << destino << "." << endl;
    } else {
        int* ruta = new int[cantidad];
        int longitud = 0;
        for (int i = indiceFin; i != -1; i = anterior[i]) ruta[longitud++] = i;
        cout << "Ruta: ";
        for (int i = longitud - 1; i >= 0; --i) {
            cout << vertices[ruta[i]]->sede.codigo;
            if (i > 0) cout << " -> ";
        }
        cout << " | Saltos: " << longitud - 1 << endl;
        delete[] ruta;
    }
    delete[] cola;
    delete[] anterior;
    delete[] visitado;
    delete[] vertices;
}

void GrafoSedes::verRankingSedesSimilares() const {
    int cantidadAristas = 0;
    for (VerticeSede* vertice = cabeza; vertice != nullptr;
         vertice = vertice->siguiente) {
        for (Arco* arco = vertice->listaAdyacencia; arco != nullptr;
             arco = arco->siguiente) {
            if (vertice->sede.codigo < arco->codigoDestino) ++cantidadAristas;
        }
    }
    ConexionRanking* ranking = new ConexionRanking[cantidadAristas];
    int indice = 0;
    for (VerticeSede* vertice = cabeza; vertice != nullptr;
         vertice = vertice->siguiente) {
        for (Arco* arco = vertice->listaAdyacencia; arco != nullptr;
             arco = arco->siguiente) {
            if (vertice->sede.codigo < arco->codigoDestino) {
                ranking[indice].origen = vertice->sede.codigo;
                ranking[indice].destino = arco->codigoDestino;
                ranking[indice++].peso = arco->peso;
            }
        }
    }
    for (int i = 0; i < indice - 1; ++i) {
        for (int j = 0; j < indice - i - 1; ++j) {
            if (ranking[j].peso < ranking[j + 1].peso) {
                intercambiar(ranking[j], ranking[j + 1]);
            }
        }
    }
    cout << "Ranking de sedes similares:" << endl;
    for (int i = 0; i < indice; ++i) {
        cout << i + 1 << ". " << ranking[i].origen << " - "
             << ranking[i].destino << " | Peliculas en comun: "
             << ranking[i].peso << endl;
    }
    if (indice == 0) cout << "No hay sedes conectadas." << endl;
    delete[] ranking;
}

void GrafoSedes::mostrarGrafoConsola() const {
    for (VerticeSede* vertice = cabeza; vertice != nullptr;
         vertice = vertice->siguiente) {
        cout << "Sede: " << vertice->sede.codigo << " -> ";
        for (Arco* arco = vertice->listaAdyacencia; arco != nullptr;
             arco = arco->siguiente) {
            cout << "[" << arco->codigoDestino << " | Peso: "
                 << arco->peso << "] ";
        }
        cout << endl;
    }
}

bool GrafoSedes::generarReporteGrafo(const string& rutaDot,
                                     const string& rutaPng) const {
    ofstream archivo(rutaDot.c_str());
    if (!archivo.is_open()) return false;
    archivo << "graph GrafoSedes {\n  rankdir=LR;\n";
    for (VerticeSede* vertice = cabeza; vertice != nullptr;
         vertice = vertice->siguiente) {
        archivo << "  \"" << vertice->sede.codigo << "\" [shape=box,label=\""
                << vertice->sede.codigo << "\\n" << vertice->sede.nombre
                << "\\nSalas: " << vertice->sede.cantidad_salas << "\"];\n";
        for (Arco* arco = vertice->listaAdyacencia; arco != nullptr;
             arco = arco->siguiente) {
            if (vertice->sede.codigo < arco->codigoDestino) {
                archivo << "  \"" << vertice->sede.codigo << "\" -- \""
                        << arco->codigoDestino << "\" [label=\""
                        << arco->peso << "\"];\n";
            }
        }
    }
    archivo << "}\n";
    archivo.close();
    string comando = "dot -Tpng \"" + rutaDot + "\" -o \"" + rutaPng + "\"";
    return system(comando.c_str()) == 0;
}

bool GrafoSedes::generarReporteListaAdyacencia(const string& rutaDot,
                                               const string& rutaPng) const {
    ofstream archivo(rutaDot.c_str());
    if (!archivo.is_open()) return false;
    archivo << "digraph ListaAdyacencia {\n  rankdir=LR;\n";
    int contador = 0;
    for (VerticeSede* vertice = cabeza; vertice != nullptr;
         vertice = vertice->siguiente) {
        string anterior = "V" + to_string(contador++);
        archivo << "  \"" << anterior << "\" [shape=box,label=\""
                << vertice->sede.codigo << "\"];\n";
        for (Arco* arco = vertice->listaAdyacencia; arco != nullptr;
             arco = arco->siguiente) {
            string actual = "A" + to_string(contador++);
            archivo << "  \"" << actual << "\" [shape=ellipse,label=\""
                    << arco->codigoDestino << "\\nPeso: " << arco->peso
                    << "\"];\n";
            archivo << "  \"" << anterior << "\" -> \"" << actual << "\";\n";
            anterior = actual;
        }
    }
    archivo << "}\n";
    archivo.close();
    string comando = "dot -Tpng \"" + rutaDot + "\" -o \"" + rutaPng + "\"";
    return system(comando.c_str()) == 0;
}

bool GrafoSedes::generarReporteAdyacencia(const string& rutaDot,
                                          const string& rutaPng) const {
    return generarReporteListaAdyacencia(rutaDot, rutaPng);
}
