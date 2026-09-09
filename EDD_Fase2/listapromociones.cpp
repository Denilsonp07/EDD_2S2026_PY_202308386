#include "listapromociones.h"
#include "listabeneficios.h"

namespace {
std::string escaparGraphviz(const std::string& texto)
{
    std::string resultado;
    for (char caracter : texto) {
        if (caracter == '"' || caracter == '\\') {
            resultado += '\\';
        }
        resultado += caracter;
    }
    return resultado;
}
}

// Inicializamos la lista vacia
ListaCircularPromociones::ListaCircularPromociones() {
    cabeza = nullptr;
    ultimo = nullptr;
}

// Destructor para limpiar la memoria
ListaCircularPromociones::~ListaCircularPromociones() {
    if (estaVacia()) return;

    NodoPromocion* actual = cabeza;
    do {
        NodoPromocion* nodoABorrar = actual;
        actual = actual->siguiente;
        delete nodoABorrar; // el destructor de ListaDobleBeneficios se llama automaticamente aquí
    } while (actual != cabeza);
}

bool ListaCircularPromociones::estaVacia() const {
    return cabeza == nullptr;
}

// Insercion en Lista Circular Simple
void ListaCircularPromociones::insertarPromocion(const Promocion& promo) {
    NodoPromocion* nuevoNodo = new NodoPromocion(promo);

    if (estaVacia()) {
        cabeza = nuevoNodo;
        ultimo = nuevoNodo;
        ultimo->siguiente = cabeza; // Se apunta a sí mismo cerrando el circulo
    } else {
        ultimo->siguiente = nuevoNodo;
        ultimo = nuevoNodo;
        ultimo->siguiente = cabeza; // El nuevo ultimo apunta a la cabeza
    }
}

bool ListaCircularPromociones::eliminarPromocion(const std::string& codigo) {
    if (estaVacia()) return false;

    NodoPromocion* anterior = ultimo;
    NodoPromocion* actual = cabeza;

    do {
        if (actual->promocion.codigo == codigo) {
            if (actual == cabeza && actual == ultimo) {
                cabeza = nullptr;
                ultimo = nullptr;
            } else {
                anterior->siguiente = actual->siguiente;
                if (actual == cabeza) cabeza = actual->siguiente;
                if (actual == ultimo) ultimo = anterior;
                ultimo->siguiente = cabeza;
            }

            delete actual;
            return true;
        }

        anterior = actual;
        actual = actual->siguiente;
    } while (actual != cabeza);

    return false;
}

// Búsqueda por código para agregar beneficios a una promo específica
NodoPromocion* ListaCircularPromociones::buscarPromocion(const std::string& codigo) {
    if (estaVacia()) return nullptr;

    NodoPromocion* actual = cabeza;
    do {
        if (actual->promocion.codigo == codigo) {
            return actual;
        }
        actual = actual->siguiente;
    } while (actual != cabeza); // Recorremos hasta dar la vuelta completa

    return nullptr; // No se encontro
}

// Metodo de prueba por consola
void ListaCircularPromociones::mostrarPromocionesYBeneficios() const {
    if (estaVacia()) {
        std::cout << "No hay promociones registradas." << std::endl;
        return;
    }

    NodoPromocion* actual = cabeza;
    do {
        std::cout << "Promocion: " << actual->promocion.nombre 
                  << " (" << actual->promocion.codigo << ")" << std::endl;
        
        // Imprimimos la lista interna de beneficios[cite: 1]
        std::cout << "  Beneficios:" << std::endl;
        if (actual->listaBeneficios.estaVacia()) {
            std::cout << "    [Sin beneficios]" << std::endl;
        } else {
            actual->listaBeneficios.mostrarBeneficios();
        }
        std::cout << "----------------------" << std::endl;

        actual = actual->siguiente;
    } while (actual != cabeza);
}

bool ListaCircularPromociones::generarReportePromociones(
    const std::string& rutaDot, const std::string& rutaPng) const
{
    std::ofstream archivo(rutaDot);
    if (!archivo.is_open()) {
        return false;
    }

    archivo << "digraph PromocionesBeneficios {\n"
            << "rankdir=TB;\n"
            << "bgcolor=white;\n"
            << "node [fontname=Arial, color=black];\n"
            << "edge [color=\"#566573\", arrowsize=0.8];\n";

    if (cabeza == nullptr) {
        archivo << "vacio [shape=box, label=\"No hay promociones registradas\"];\n}\n";
        archivo.close();
        const std::string comando = "dot -Tpng \"" + rutaDot + "\" -o \"" + rutaPng + "\"";
        return system(comando.c_str()) == 0;
    }

    archivo << "subgraph promociones { rank=same; ";
    NodoPromocion* actual = cabeza;
    do {
        const std::string id = "promo_" + escaparGraphviz(actual->promocion.codigo);
        archivo << "\"" << id << "\"; ";
        actual = actual->siguiente;
    } while (actual != cabeza);
    archivo << "}\n";

    actual = cabeza;
    int numeroBeneficio = 0;
    do {
        const std::string codigo = escaparGraphviz(actual->promocion.codigo);
        const std::string idPromocion = "promo_" + codigo;
        const std::string etiquetaPromocion = "Codigo: " + codigo + "\\n"
            + "Nombre: " + escaparGraphviz(actual->promocion.nombre) + "\\n"
            + "Vigencia: " + escaparGraphviz(actual->promocion.vigencia);
        archivo << "\"" << idPromocion << "\" [shape=box, style=filled, "
                << "fillcolor=\"#AED6F1\", label=\"" << etiquetaPromocion << "\"];\n";

        NodoBeneficio* beneficio = actual->listaBeneficios.getCabeza();
        int beneficioAnterior = -1;
        while (beneficio != nullptr) {
            const int indiceActual = numeroBeneficio++;
            const std::string idBeneficio = "beneficio_" + std::to_string(indiceActual);
            const std::string etiquetaBeneficio = "Tipo: " + escaparGraphviz(beneficio->beneficio.tipo) + "\\n"
                + "Descripcion: " + escaparGraphviz(beneficio->beneficio.descripcion) + "\\n"
                + "Valor: " + escaparGraphviz(beneficio->beneficio.valor);
            archivo << "\"" << idBeneficio << "\" [shape=box, style=filled, "
                    << "fillcolor=\"#D5F5E3\", label=\"" << etiquetaBeneficio << "\"];\n";
            if (beneficioAnterior < 0) {
                archivo << "\"" << idPromocion << "\" -> \"" << idBeneficio
                        << "\" [color=\"#2E86C1\"];\n";
            } else {
                archivo << "\"beneficio_" << beneficioAnterior << "\" -> \""
                        << idBeneficio << "\";\n";
            }
            beneficioAnterior = indiceActual;
            beneficio = beneficio->siguiente;
        }
        actual = actual->siguiente;
    } while (actual != cabeza);

    actual = cabeza;
    do {
        const std::string origen = "promo_" + escaparGraphviz(actual->promocion.codigo);
        const std::string destino = "promo_" + escaparGraphviz(actual->siguiente->promocion.codigo);
        archivo << "\"" << origen << "\" -> \"" << destino
                << "\" [color=\"#1B4F72\", constraint=false];\n";
        actual = actual->siguiente;
    } while (actual != cabeza);

    archivo << "}\n";
    archivo.close();
    const std::string comando = "dot -Tpng \"" + rutaDot + "\" -o \"" + rutaPng + "\"";
    return system(comando.c_str()) == 0;
}