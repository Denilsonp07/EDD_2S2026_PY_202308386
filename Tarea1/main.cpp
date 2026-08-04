#include <iostream>
#include <fstream>
#include <limits>

using namespace std;

// Nodo
struct Nodo {
    int dato;
    Nodo* siguiente;
};

// Clase lista Enlazada 
class ListaSimple {
private:
    Nodo* cabeza; 

public:
   
    ListaSimple() {
        cabeza = nullptr;
    }

    // nsertar un nodo 
    void insertar(int valor) {
        Nodo* nuevoNodo = new Nodo();
        nuevoNodo->dato = valor;
        nuevoNodo->siguiente = nullptr;

        if (cabeza == nullptr) {
            cabeza = nuevoNodo; 
        } else {
            Nodo* actual = cabeza;
            while (actual->siguiente != nullptr) {
                actual = actual->siguiente; 
            }
            actual->siguiente = nuevoNodo; 
        }
        cout << "Dato [" << valor << "] insertado correctamente.\n";
    }

    // para eliminar 
    void eliminar(int valor) {
        if (cabeza == nullptr) {
            cout << "La lista esta vacia. No se puede eliminar.\n";
            return;
        }

        Nodo* actual = cabeza;
        Nodo* anterior = nullptr;

        if (actual->dato == valor) {
            cabeza = actual->siguiente;
            delete actual;
            cout << "Dato [" << valor << "] eliminado (era el primer nodo).\n";
            return;
        }

        while (actual != nullptr && actual->dato != valor) {
            anterior = actual;
            actual = actual->siguiente;
        }

        if (actual == nullptr) {
            cout << "El dato [" << valor << "] no se encontro en la lista.\n";
            return;
        }

        anterior->siguiente = actual->siguiente;
        delete actual;
        cout << "Dato [" << valor << "] eliminado correctamente.\n";
    }

    // recorrer y mostrar
    void mostrarConsola() {
        if (cabeza == nullptr) {
            cout << "La lista esta vacia.\n";
            return;
        }

        Nodo* actual = cabeza;
        cout << "\n--- Recorrido de la Lista ---\n";
        while (actual != nullptr) {
            cout << "[" << actual->dato << "] -> ";
            actual = actual->siguiente;
        }
        cout << "NULL\n-----------------------------\n";
    }

    // Graphvuz 

    void generarReporteGraphviz() {
        ofstream archivo("reporte_lista.dot");
        
        if (!archivo.is_open()) {
            cout << "Error al crear el archivo Graphviz.\n";
            return;
        }

        archivo << "digraph ListaEnlazada {\n";
        archivo << "    rankdir=LR;\n"; // De izquierda a derecha
        archivo << "    node [shape=record, style=filled, fillcolor=lightblue];\n";

        Nodo* actual = cabeza;
        int i = 0;

        while (actual != nullptr) {
            archivo << "    nodo" << i << " [label=\"{ Dato: " << actual->dato << " | <ref> * }\"];\n";
            actual = actual->siguiente;
            i++;
        }

        // Crear flechas 
        actual = cabeza;
        i = 0;
        while (actual != nullptr && actual->siguiente != nullptr) {
            archivo << "    nodo" << i << ":ref -> nodo" << (i + 1) << ";\n";
            actual = actual->siguiente;
            i++;
        }

        if (cabeza != nullptr) {
            archivo << "    null [shape=none, fillcolor=white, label=\"NULL\"];\n";
            archivo << "    nodo" << i << ":ref -> null;\n";
        } else {
            archivo << "    null [shape=none, fillcolor=white, label=\"Lista Vacia\"];\n";
        }

        archivo << "}\n";
        archivo.close();
        cout << "Reporte generado exitosamente en 'reporte_lista.dot'.\n";
    }
};

// validacion de solo numeros
int leerEntero() {
    int numero;
    while (!(cin >> numero)) {
        cout << "Error: Entrada invalida. Por favor, ingrese un numero entero: ";
        cin.clear(); // Limpia el estado de error
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Descarta la entrada mala
    }
    return numero;
}

int main() {
    ListaSimple miLista;
    int opcion, valor;

    do {

        cout << "1. Insertar nuevo nodo\n";
        cout << "2. Mostrar lista en consola\n";
        cout << "3. Eliminar un nodo\n";
        cout << "4. Generar reporte en graphviz)\n";
        cout << "5. Salir\n";
        cout << "Seleccione una opcion: ";
        
        opcion = leerEntero();

        switch (opcion) {
            case 1:
                cout << "Ingrese un valor entero: ";
                valor = leerEntero();
                miLista.insertar(valor);
                break;
            case 2:
                miLista.mostrarConsola();
                break;
            case 3:
                cout << "Ingrese el valor del nodo que desea eliminar: ";
                valor = leerEntero();
                miLista.eliminar(valor);
                break;
            case 4:
                miLista.generarReporteGraphviz();
                break;
            case 5:
                cout << "Saliendo\n";
                break;
            default:
                cout << "Opcion no valida.\n";
        }
    } while (opcion != 5);

    return 0;
}