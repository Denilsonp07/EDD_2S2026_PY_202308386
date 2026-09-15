#include "listabeneficios.h"


ListaDobleBeneficios::ListaDobleBeneficios() {
    cabeza = nullptr;
    cola = nullptr;
}

// Destructor
ListaDobleBeneficios::~ListaDobleBeneficios() {
    NodoBeneficio* actual = cabeza;
    while (actual != nullptr) {
        NodoBeneficio* nodoABorrar = actual;
        actual = actual->siguiente;
        delete nodoABorrar;
    }
}

bool ListaDobleBeneficios::estaVacia() const {
    return cabeza == nullptr;
}

// insercion doblemente enlazada
void ListaDobleBeneficios::insertarFinal(const Beneficio& beneficio) {
    NodoBeneficio* nuevoNodo = new NodoBeneficio(beneficio);

    // Caso 1: Si la lista está vacía, el nuevo nodo es tanto la cabeza como la cola
    if (estaVacia()) {
        cabeza = nuevoNodo;
        cola = nuevoNodo;
    } 
    // Cuando ya hay elementos, conectamos la cola actual con el nuevo nodo
    else {
        cola->siguiente = nuevoNodo;    // El de atras apunta hacia adelante (al nuevo)
        nuevoNodo->anterior = cola;     // El nuevo apunta hacia atras (a la cola vieja)
        cola = nuevoNodo;               // El nuevo nodo se convierte en la nueva cola
    }
}

// Funcion auxiliar para imprimir en consola y verificar que funciona
void ListaDobleBeneficios::mostrarBeneficios() const {
    NodoBeneficio* actual = cabeza;
    while (actual != nullptr) {
        std::cout << "  - Tipo: " << actual->beneficio.tipo 
                  << " | Desc: " << actual->beneficio.descripcion 
                  << " | Valor: " << actual->beneficio.valor << std::endl;
        actual = actual->siguiente;
    }
}