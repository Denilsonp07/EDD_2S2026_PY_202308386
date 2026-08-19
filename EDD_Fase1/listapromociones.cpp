#include "listapromociones.h"
#include "listabeneficios.h"

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