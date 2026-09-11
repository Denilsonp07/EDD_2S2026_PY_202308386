#include "arbolbclientes.h"
#include <iostream>
#include <cstdlib>

// Función auxiliar para comparar los IDs numéricamente
int extraerNumeroID(const std::string& id) {
    std::string digitos = "";
    for (char c : id) {
        if (isdigit(c)) digitos += c;
    }
    return digitos.empty() ? 0 : atoi(digitos.c_str());
}

ArbolBClientes::ArbolBClientes() {
    // El árbol siempre nace con una raíz que es hoja
    raiz = new NodoB(true);
}

// === BÚSQUEDA ===

Cliente* ArbolBClientes::buscar(const std::string& id) {
    NodoB* actual = raiz;
    int idBuscado = extraerNumeroID(id);

    while (actual != nullptr) {
        int i = 0;
        // Avanzamos en las claves del nodo mientras el ID buscado sea mayor
        while (i < actual->numClaves && idBuscado > extraerNumeroID(actual->clientes[i]->getId())) {
            i++;
        }

        // Si encontramos la clave exacta, retornamos el cliente
        if (i < actual->numClaves && idBuscado == extraerNumeroID(actual->clientes[i]->getId())) {
            return actual->clientes[i];
        }

        // Si llegamos a una hoja y no lo encontramos, el cliente no existe
        if (actual->esHoja) {
            return nullptr;
        }

        // Bajamos al hijo correspondiente
        actual = actual->hijos[i];
    }
    return nullptr;
}

// === INSERCIÓN ===

void ArbolBClientes::insertar(const Cliente& cliente) {
    // Si la raíz está completamente llena (3 claves para orden 4)[cite: 1]
    if (raiz->numClaves == 3) {
        // Creamos una nueva raíz vacía
        NodoB* nuevaRaiz = new NodoB(false);
        nuevaRaiz->hijos[0] = raiz;
        
        // Dividimos la raíz antigua, empujando la clave central hacia arriba
        dividirHijo(nuevaRaiz, 0, raiz);
        
        // Insertamos el cliente en el árbol ahora que la raíz tiene espacio
        insertarNoLleno(nuevaRaiz, cliente);
        
        // Actualizamos el puntero principal
        raiz = nuevaRaiz;
    } else {
        insertarNoLleno(raiz, cliente);
    }
}

void ArbolBClientes::insertarNoLleno(NodoB* nodo, const Cliente& cliente) {
    int i = nodo->numClaves - 1;
    int idNuevo = extraerNumeroID(cliente.getId());

    if (nodo->esHoja) {
        // Desplazamos las claves mayores hacia la derecha para hacer espacio
        while (i >= 0 && idNuevo < extraerNumeroID(nodo->clientes[i]->getId())) {
            nodo->clientes[i + 1] = nodo->clientes[i];
            i--;
        }
        // Insertamos el nuevo cliente
        nodo->clientes[i + 1] = new Cliente(cliente);
        nodo->numClaves++;
    } else {
        // Buscamos el hijo en el que debemos descender
        while (i >= 0 && idNuevo < extraerNumeroID(nodo->clientes[i]->getId())) {
            i--;
        }
        i++; // Ajustamos el índice al hijo correcto

        // Si el hijo donde vamos a bajar está lleno, lo dividimos primero
        if (nodo->hijos[i]->numClaves == 3) {
            dividirHijo(nodo, i, nodo->hijos[i]);

            // Después de dividir, la clave del medio subió. 
            // Evaluamos si debemos bajar por el hijo izquierdo o el derecho.
            if (idNuevo > extraerNumeroID(nodo->clientes[i]->getId())) {
                i++;
            }
        }
        insertarNoLleno(nodo->hijos[i], cliente);
    }
}

void ArbolBClientes::dividirHijo(NodoB* padre, int i, NodoB* hijoLleno) {
    // Creamos un nuevo nodo hermano que absorberá la mitad derecha
    NodoB* nuevoHijo = new NodoB(hijoLleno->esHoja);
    nuevoHijo->numClaves = 1;

    // Pasamos la clave 2 al nuevo nodo (la clave 1 subirá al padre)
    nuevoHijo->clientes[0] = hijoLleno->clientes[2];

    // Si no son hojas, también trasladamos los dos últimos hijos
    if (!hijoLleno->esHoja) {
        nuevoHijo->hijos[0] = hijoLleno->hijos[2];
        nuevoHijo->hijos[1] = hijoLleno->hijos[3];
    }
    hijoLleno->numClaves = 1; // El hijo lleno se queda solo con la clave 0

    // Hacemos espacio en el padre para el nuevo hijo
    for (int j = padre->numClaves; j >= i + 1; j--) {
        padre->hijos[j + 1] = padre->hijos[j];
    }
    padre->hijos[i + 1] = nuevoHijo;

    // Hacemos espacio en el padre para la clave que va a subir (la clave 1)
    for (int j = padre->numClaves - 1; j >= i; j--) {
        padre->clientes[j + 1] = padre->clientes[j];
    }
    padre->clientes[i] = hijoLleno->clientes[1];
    padre->numClaves++;
}

// === RECORRIDO ===

void ArbolBClientes::mostrarClientes() {
    std::cout << "--- Listado de Clientes ---" << std::endl;
    mostrarInordenRecursivo(raiz);
}

void ArbolBClientes::mostrarInordenRecursivo(NodoB* nodo) {
    if (nodo != nullptr) {
        int i;
        // Intercalamos visitas a los hijos y lecturas de claves[cite: 1]
        for (i = 0; i < nodo->numClaves; i++) {
            if (!nodo->esHoja) {
                mostrarInordenRecursivo(nodo->hijos[i]);
            }
            std::cout << "ID: " << nodo->clientes[i]->getId() 
                      << " | Nombre: " << nodo->clientes[i]->getNombre() << std::endl;
        }
        // Visitamos el último hijo
        if (!nodo->esHoja) {
            mostrarInordenRecursivo(nodo->hijos[i]);
        }
    }
}