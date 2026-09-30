#include "tablahashreservas.h"
#include <iostream>
#include <fstream>

TablaHash::TablaHash(int cap) {
    capacidad = cap;
    cantidadElementos = 0;
    tabla = new NodoHash*[capacidad];
    for (int i = 0; i < capacidad; i++) {
        tabla[i] = nullptr;
    }
}

TablaHash::~TablaHash() {
    for (int i = 0; i < capacidad; i++) {
        NodoHash* actual = tabla[i];
        while (actual != nullptr) {
            NodoHash* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
    }
    delete[] tabla;
}

int TablaHash::funcionHash(const string& clave) {
    int suma = 0;
    for (char c : clave) {
        suma += static_cast<int>(c);
    }
    return suma % capacidad;
}

void TablaHash::rehash() {
    int nuevaCapacidad = capacidad * 2 + 1;
    NodoHash** nuevaTabla = new NodoHash*[nuevaCapacidad];
    for (int i = 0; i < nuevaCapacidad; i++) {
        nuevaTabla[i] = nullptr;
    }

    // Rehashing: mover todos los nodos a la nueva tabla
    for (int i = 0; i < capacidad; i++) {
        NodoHash* actual = tabla[i];
        while (actual != nullptr) {
            NodoHash* siguienteTemp = actual->siguiente;
            
            // Recalcular índice con nueva capacidad
            int suma = 0;
            for (char c : actual->reserva.codigo_reserva) suma += static_cast<int>(c);
            int nuevoIndice = suma % nuevaCapacidad;

            // Insertar en la nueva tabla (al inicio de la lista)
            actual->siguiente = nuevaTabla[nuevoIndice];
            nuevaTabla[nuevoIndice] = actual;

            actual = siguienteTemp;
        }
    }
    delete[] tabla;
    tabla = nuevaTabla;
    capacidad = nuevaCapacidad;
}

void TablaHash::insertar(const Reserva& reserva) {
    // Si la tabla supera un factor de carga del 70%, expandimos
    if ((cantidadElementos * 100) / capacidad >= 70) {
        rehash();
    }

    int indice = funcionHash(reserva.codigo_reserva);
    NodoHash* nuevo = new NodoHash(reserva);

    // Inserción al inicio de la lista enlazada del bucket correspondiente
    if (tabla[indice] == nullptr) {
        tabla[indice] = nuevo;
    } else {
        nuevo->siguiente = tabla[indice];
        tabla[indice] = nuevo;
    }
    cantidadElementos++;
}

Reserva* TablaHash::buscar(const string& codigo_reserva) {
    int indice = funcionHash(codigo_reserva);
    NodoHash* actual = tabla[indice];

    // Recorrer la lista en caso de colisión
    while (actual != nullptr) {
        if (actual->reserva.codigo_reserva == codigo_reserva) {
            return &(actual->reserva);
        }
        actual = actual->siguiente;
    }
    return nullptr; // No encontrada
}

bool TablaHash::eliminar(const string& codigo_reserva) {
    int indice = funcionHash(codigo_reserva);
    NodoHash* actual = tabla[indice];
    NodoHash* anterior = nullptr;

    while (actual != nullptr) {
        if (actual->reserva.codigo_reserva == codigo_reserva) {
            if (anterior == nullptr) {
                // Es el primer elemento de la lista
                tabla[indice] = actual->siguiente;
            } else {
                // Está en medio o al final
                anterior->siguiente = actual->siguiente;
            }
            delete actual;
            cantidadElementos--;
            return true;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    return false; // No existía la reserva
}



//--------------------------- REPORTE DE LA TABLAS HASHH -----------------------------------




void TablaHash::generarReporte() {
    std::ofstream archivo("reporte_hash.dot");
    // rankdir=LR dibuja de izquierda a derecha
    archivo << "digraph Hash {\nrankdir=LR;\nnode [shape=box, style=filled];\n";
    
    for (int i = 0; i < capacidad; i++) {
        NodoHash* actual = tabla[i];
        std::string anterior = "bucket" + std::to_string(i);
        
        // Color gris si está vacío, naranja si está ocupado[cite: 1]
        std::string colorBucket = (actual == nullptr) ? "lightgrey" : "orange";
        archivo << "    " << anterior << " [label=\"Bucket " << i << "\", fillcolor=" << colorBucket << "];\n";
        
        // Imprimir la lista enlazada (colisiones)[cite: 1]
        while (actual != nullptr) {
            std::string idNodo = "nodo_" + actual->reserva.codigo_reserva;
            std::string label = actual->reserva.codigo_reserva + "\\nFunc: " + actual->reserva.codigo_funcion;
            
            archivo << "    " << idNodo << " [label=\"" << label << "\", fillcolor=white];\n";
            archivo << "    " << anterior << " -> " << idNodo << ";\n";
            
            anterior = idNodo;
            actual = actual->siguiente;
        }
    }
    archivo << "}\n";
    archivo.close();
    system("dot -Tpng reporte_hash.dot -o reporte_hash.png");
}