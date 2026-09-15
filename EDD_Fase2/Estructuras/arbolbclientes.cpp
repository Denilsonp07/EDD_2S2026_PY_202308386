#include "arbolbclientes.h"
#include <iostream>
#include <cstdlib>

#include "json.hpp"
#include <fstream>

// Función auxiliar para comparar los IDs numéricamente
int extraerNumeroID(const string& id) {
    string digitos = "";
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

Cliente* ArbolBClientes::buscar(const string& id) {
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
    // Si la raíz está completamente llena (3 claves para orden 4)
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
        // Intercalamos visitas a los hijos y lecturas de claves
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




// ------------------------- LOGICA DE ELIMINACION -----------------------------------------



bool ArbolBClientes::eliminarCliente(const string& id, TablaHash& tablaReservas) {
    if (raiz == nullptr || raiz->numClaves == 0) return false;

    // 1. Buscar al cliente para extraer sus reservas antes de borrarlo
    Cliente* cliente = buscar(id);
    if (cliente == nullptr) return false; // El cliente no existe

    // 2. Borrado en Cascada: Eliminar todas sus reservas de la Tabla Hash
    NodoReservaRef* actualReserva = cliente->getCabezaReservas();
    while (actualReserva != nullptr) {
        tablaReservas.eliminar(actualReserva->codigo_reserva);
        actualReserva = actualReserva->siguiente;
    }

    // 3. Ejecutar la eliminación estructural del Árbol B
    int idNumerico = extraerNumeroID(id);
    eliminarRecursivo(raiz, idNumerico);

    // 4. Si la raíz se quedó vacía tras una fusión, la reducimos
    if (raiz->numClaves == 0) {
        NodoB* temporal = raiz;
        if (raiz->esHoja) {
            raiz = nullptr;
        } else {
            raiz = raiz->hijos[0];
        }
        delete temporal;
    }

    return true;
}

int ArbolBClientes::encontrarClave(NodoB* nodo, int idNumerico) {
    int idx = 0;
    while (idx < nodo->numClaves && extraerNumeroID(nodo->clientes[idx]->getId()) < idNumerico) {
        ++idx;
    }
    return idx;
}

void ArbolBClientes::eliminarRecursivo(NodoB* nodo, int idNumerico) {
    int idx = encontrarClave(nodo, idNumerico);

    // Si la clave está en este nodo
    if (idx < nodo->numClaves && extraerNumeroID(nodo->clientes[idx]->getId()) == idNumerico) {
        if (nodo->esHoja) {
            removerDeHoja(nodo, idx);
        } else {
            removerDeNoHoja(nodo, idx);
        }
    } else {
        // Si llegamos a una hoja y no está, terminamos
        if (nodo->esHoja) return;

        // Bandera para saber si el hijo donde bajaremos está en el límite
        bool esUltimoHijo = (idx == nodo->numClaves);

        // Si el hijo por donde debemos bajar tiene menos de 2 claves (underflow), lo llenamos
        if (nodo->hijos[idx]->numClaves < 2) {
            llenar(nodo, idx);
        }

        // Tras el rebalanceo, el hijo pudo haberse fusionado con el anterior
        if (esUltimoHijo && idx > nodo->numClaves) {
            eliminarRecursivo(nodo->hijos[idx - 1], idNumerico);
        } else {
            eliminarRecursivo(nodo->hijos[idx], idNumerico);
        }
    }
}

void ArbolBClientes::removerDeHoja(NodoB* nodo, int indice) {
    // Liberar la memoria del cliente
    delete nodo->clientes[indice];

    // Desplazar las claves restantes un espacio hacia la izquierda
    for (int i = indice + 1; i < nodo->numClaves; ++i) {
        nodo->clientes[i - 1] = nodo->clientes[i];
    }
    nodo->numClaves--;
}



// ------------------------ Eliminacion y BALANCEO --------------------------------



void ArbolBClientes::removerDeNoHoja(NodoB* nodo, int indice) {
    Cliente* clienteAEliminar = nodo->clientes[indice];

    // Caso A: El hijo izquierdo tiene al menos 2 claves
    if (nodo->hijos[indice]->numClaves >= 2) {
        NodoB* actual = nodo->hijos[indice];
        while (!actual->esHoja) {
            actual = actual->hijos[actual->numClaves];
        }
        Cliente* predecesor = actual->clientes[actual->numClaves - 1];
        nodo->clientes[indice] = new Cliente(*predecesor);
        eliminarRecursivo(nodo->hijos[indice], extraerNumeroID(predecesor->getId()));
    }
    // Caso B: El hijo derecho tiene al menos 2 claves
    else if (nodo->hijos[indice + 1]->numClaves >= 2) {
        NodoB* actual = nodo->hijos[indice + 1];
        while (!actual->esHoja) {
            actual = actual->hijos[0];
        }
        Cliente* sucesor = actual->clientes[0];
        nodo->clientes[indice] = new Cliente(*sucesor);
        eliminarRecursivo(nodo->hijos[indice + 1], extraerNumeroID(sucesor->getId()));
    }
    // Caso C: Ambos hijos tienen solo 1 clave, los fusionamos
    else {
        fusionar(nodo, indice);
        eliminarRecursivo(nodo->hijos[indice], extraerNumeroID(clienteAEliminar->getId()));
    }
}




// ---------------------------- LLENAR PRESTAR Y FUSIONARR ----------------------------------



void ArbolBClientes::llenar(NodoB* nodo, int indice) {
    if (indice != 0 && nodo->hijos[indice - 1]->numClaves >= 2) {
        pedirPrestadoAnterior(nodo, indice);
    }
    else if (indice != nodo->numClaves && nodo->hijos[indice + 1]->numClaves >= 2) {
        pedirPrestadoSiguiente(nodo, indice);
    }
    else {
        if (indice != nodo->numClaves) {
            fusionar(nodo, indice);
        } else {
            fusionar(nodo, indice - 1);
        }
    }
}

void ArbolBClientes::pedirPrestadoAnterior(NodoB* nodo, int indice) {
    NodoB* hijo = nodo->hijos[indice];
    NodoB* hermano = nodo->hijos[indice - 1];

    for (int i = hijo->numClaves - 1; i >= 0; --i) {
        hijo->clientes[i + 1] = hijo->clientes[i];
    }
    if (!hijo->esHoja) {
        for (int i = hijo->numClaves; i >= 0; --i) {
            hijo->hijos[i + 1] = hijo->hijos[i];
        }
    }
    hijo->clientes[0] = nodo->clientes[indice - 1];
    if (!hijo->esHoja) {
        hijo->hijos[0] = hermano->hijos[hermano->numClaves];
    }
    nodo->clientes[indice - 1] = hermano->clientes[hermano->numClaves - 1];
    
    hijo->numClaves += 1;
    hermano->numClaves -= 1;
}

void ArbolBClientes::pedirPrestadoSiguiente(NodoB* nodo, int indice) {
    NodoB* hijo = nodo->hijos[indice];
    NodoB* hermano = nodo->hijos[indice + 1];

    hijo->clientes[hijo->numClaves] = nodo->clientes[indice];
    if (!hijo->esHoja) {
        hijo->hijos[hijo->numClaves + 1] = hermano->hijos[0];
    }
    nodo->clientes[indice] = hermano->clientes[0];
    for (int i = 1; i < hermano->numClaves; ++i) {
        hermano->clientes[i - 1] = hermano->clientes[i];
    }
    if (!hermano->esHoja) {
        for (int i = 1; i <= hermano->numClaves; ++i) {
            hermano->hijos[i - 1] = hermano->hijos[i];
        }
    }
    
    hijo->numClaves += 1;
    hermano->numClaves -= 1;
}

void ArbolBClientes::fusionar(NodoB* nodo, int indice) {
    NodoB* hijo = nodo->hijos[indice];
    NodoB* hermano = nodo->hijos[indice + 1];

    // Bajamos la clave del padre e insertamos el hermano en el hijo
    hijo->clientes[1] = nodo->clientes[indice];

    for (int i = 0; i < hermano->numClaves; ++i) {
        hijo->clientes[i + 2] = hermano->clientes[i];
    }
    if (!hijo->esHoja) {
        for (int i = 0; i <= hermano->numClaves; ++i) {
            hijo->hijos[i + 2] = hermano->hijos[i];
        }
    }
    for (int i = indice + 1; i < nodo->numClaves; ++i) {
        nodo->clientes[i - 1] = nodo->clientes[i];
    }
    for (int i = indice + 2; i <= nodo->numClaves; ++i) {
        nodo->hijos[i - 1] = nodo->hijos[i];
    }
    
    hijo->numClaves += hermano->numClaves + 1;
    nodo->numClaves--;
    
    // Evitamos fuga de memoria
    for(int i = 0; i < 3; i++) hermano->clientes[i] = nullptr;
    delete hermano;
}



// --------------------------------------- CARGAR JSON-----------------------------------------


using json = nlohmann::json;

void ArbolBClientes::cargarJSON(const std::string& ruta, TablaHash& tablaReservas) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo JSON de clientes: '" << ruta << "'." << std::endl;
        return;
    }

    try {
        json datosJson;
        archivo >> datosJson;

        int clientesInsertados = 0;
        int reservasInsertadas = 0;

        // Iterar sobre el arreglo principal de clientes
        for (const auto& itemCliente : datosJson["clientes"]) {
            
            std::string id = itemCliente["id"];
            std::string nombre = itemCliente["nombre"];
            std::string correo = itemCliente["correo"];
            std::string telefono = itemCliente["telefono"];
            std::string password = itemCliente["password"];
            
            // Crear la instancia del cliente
            Cliente nuevoCliente(id, nombre, correo, telefono, password);

            // Validar si el cliente trae un arreglo de reservas
            if (itemCliente.contains("reservas")) {
                for (const auto& itemReserva : itemCliente["reservas"]) {
                    // Cuidado aquí: Las llaves en este JSON tienen espacios según el PDF
                    std::string codReserva = itemReserva["codigo reserva"];
                    std::string codFuncion = itemReserva["codigo funcion"];
                    int fila = itemReserva["fila"];
                    int columna = itemReserva["columna"];
                    std::string fecha = itemReserva["fecha reserva"];

                    // 1. Crear la reserva y guardarla en la Tabla Hash general
                    Reserva nuevaReserva(codReserva, codFuncion, fila, columna, fecha);
                    tablaReservas.insertar(nuevaReserva);
                    reservasInsertadas++;

                    // 2. Guardar únicamente el codigo_reserva dentro del perfil del cliente
                    nuevoCliente.agregarReserva(codReserva);
                }
            }

            // Finalmente, insertar al cliente (con sus referencias de reserva listas) en el Árbol B
            insertar(nuevoCliente);
            clientesInsertados++;
        }

        std::cout << "Carga masiva exitosa. Clientes: " << clientesInsertados 
                  << " | Reservas: " << reservasInsertadas << std::endl;

    } catch (json::parse_error& e) {
        std::cout << "Error al parsear el JSON de clientes: " << e.what() << std::endl;
    }

    archivo.close();

}




// --------------------------------- REPORTE DEL ARBOL BB ----------------------------------




void ArbolBClientes::generarDotAux(NodoB* nodo, std::ofstream& archivo, int& contador) {
    if (nodo == nullptr) return;

    int idActual = contador++;
    std::string label = "";
    
    // Colores distintos para distinguir la forma del árbol[cite: 1]
    std::string color = "lightblue"; 
    if (nodo == raiz) color = "lightcoral"; // Raíz
    else if (nodo->esHoja) color = "lightgreen"; // Hojas

    // Construir la estructura dividida (record)
    for (int i = 0; i < nodo->numClaves; i++) {
        // Formato: <puntero_izq> | Dato | ...
        label += "<f" + std::to_string(i) + "> | " + nodo->clientes[i]->getId() + " | ";
    }
    // Agregar el último puntero derecho
    label += "<f" + std::to_string(nodo->numClaves) + ">";

    archivo << "    nodo" << idActual << " [label=\"" << label << "\", shape=record, style=filled, fillcolor=" << color << "];\n";

    // Llamadas recursivas conectando a los punteros exactos
    if (!nodo->esHoja) {
        for (int i = 0; i <= nodo->numClaves; i++) {
            int idHijo = contador; 
            generarDotAux(nodo->hijos[i], archivo, contador);
            // Conectar desde la división exacta "<fi>"
            archivo << "    nodo" << idActual << ":f" << i << " -> nodo" << idHijo << ";\n";
        }
    }
}

void ArbolBClientes::generarReporte() {
    std::ofstream archivo("reporte_arbolb.dot");
    archivo << "digraph ArbolB {\n";
    int contador = 0;
    generarDotAux(raiz, archivo, contador);
    archivo << "}\n";
    archivo.close();
    system("dot -Tpng reporte_arbolb.dot -o reporte_arbolb.png");
}
