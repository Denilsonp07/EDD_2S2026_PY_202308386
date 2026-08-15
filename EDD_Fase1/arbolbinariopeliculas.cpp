#include "arbolbinariopeliculas.h"

ArbolBinarioDeBusquedaPeliculas::ArbolBinarioDeBusquedaPeliculas() {
    raiz = nullptr;
}

ArbolBinarioDeBusquedaPeliculas::~ArbolBinarioDeBusquedaPeliculas() {
    eliminarNodos(raiz);
}

bool ArbolBinarioDeBusquedaPeliculas::estaVacio() {
    return raiz == nullptr;
}

void ArbolBinarioDeBusquedaPeliculas::limpiar() {
    eliminarNodos(raiz);
    raiz = nullptr;
}

void ArbolBinarioDeBusquedaPeliculas::insertar(const Pelicula& pelicula) {

    if(estaVacio()) {
        raiz = new NodoArbolPeliculas(pelicula);
    } else {
        raiz = insertarNodo(raiz, pelicula);
    }

}

NodoArbolPeliculas* ArbolBinarioDeBusquedaPeliculas::insertarNodo(NodoArbolPeliculas* nodo, const Pelicula& pelicula) {
    if (nodo == nullptr) {
        return new NodoArbolPeliculas(pelicula);
    }

    if (numeroCodigo(pelicula.getCodigo()) == numeroCodigo(nodo->getPelicula().getCodigo())) {
        return nodo;
    }
    else if (numeroCodigo(pelicula.getCodigo()) < numeroCodigo(nodo->getPelicula().getCodigo())) {
        nodo->setIzq(insertarNodo(nodo->getIzq(), pelicula));
    }
    else {
        nodo->setDer(insertarNodo(nodo->getDer(), pelicula));
    }

    return nodo;    
}

int ArbolBinarioDeBusquedaPeliculas::numeroCodigo(const string& codigo) const {
    string digitos;
    for(char c : codigo){
        if(isdigit(c)){
            digitos += c;
        }

    }
    return  digitos.empty() ? 0 : atoi(digitos.c_str());
    
}


std::string ArbolBinarioDeBusquedaPeliculas::quitarComillas(const std::string& campo) {
    std::string resultado = campo;
    if (resultado.size() >= 2 && resultado.front() == '"' && resultado.back() == '"') {
        resultado = resultado.substr(1, resultado.size() - 2);
    }
    return resultado;

}


void ArbolBinarioDeBusquedaPeliculas::cargarCSV(const std::string& ruta) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo '" << ruta << "'." << std::endl;
        return;
    }

    std::string linea;
    bool encabezado = true;
    int insertados = 0;

    while (getline(archivo, linea)) {
        if (linea.empty()) continue;

        if (encabezado) {
            encabezado = false;
            continue;
        }

        size_t inicio = 0;
        std::vector<std::string> campos;
        for (size_t i = 0; i <= linea.size(); ++i) {
            if (i == linea.size() || linea[i] == ',') {
                campos.push_back(quitarComillas(linea.substr(inicio, i - inicio)));
                inicio = i + 1;
            }
        }

        if (campos.size() >= 8) {
            Pelicula nuevaPelicula(campos[0], campos[1], campos[2], campos[3], campos[4], campos[5], campos[6], campos[7]);
            insertar(nuevaPelicula);
            insertados++;
        }
    }

    archivo.close();
    std::cout << "Se cargaron " << insertados << " peliculas desde '" << ruta << "'." << std::endl;
}

void ArbolBinarioDeBusquedaPeliculas::generarDotAux(NodoArbolPeliculas* nodo, std::ofstream& archivo) {
    if (nodo != nullptr) {
        std::string infoNodo = "Codigo: " + nodo->getPelicula().getCodigo() + "\\n" +
                               "Titulo: " + nodo->getPelicula().getTitulo() + "\\n" +
                               "Duracion: " + nodo->getPelicula().getDuracion() + " min\\n" +
                               "Clasificacion: " + nodo->getPelicula().getClasificacion();

        std::string nombreNodo = "\"" + infoNodo + "\"";

        if (nodo->getIzq() != nullptr) {
            std::string infoIzq = "Codigo: " + nodo->getIzq()->getPelicula().getCodigo() + "\\n" +
                                  "Titulo: " + nodo->getIzq()->getPelicula().getTitulo() + "\\n" +
                                  "Duracion: " + nodo->getIzq()->getPelicula().getDuracion() + " min\\n" +
                                  "Clasificacion: " + nodo->getIzq()->getPelicula().getClasificacion();
            std::string nombreIzq = "\"" + infoIzq + "\"";
            archivo << nombreNodo << " -> " << nombreIzq << ";\n";
        }
        if (nodo->getDer() != nullptr) {
            std::string infoDer = "Codigo: " + nodo->getDer()->getPelicula().getCodigo() + "\\n" +
                                  "Titulo: " + nodo->getDer()->getPelicula().getTitulo() + "\\n" +
                                  "Duracion: " + nodo->getDer()->getPelicula().getDuracion() + " min\\n" +
                                  "Clasificacion: " + nodo->getDer()->getPelicula().getClasificacion();
            std::string nombreDer = "\"" + infoDer + "\"";
            archivo << nombreNodo << " -> " << nombreDer << ";\n";
        }
        generarDotAux(nodo->getIzq(), archivo);
        generarDotAux(nodo->getDer(), archivo);
    }
}

void ArbolBinarioDeBusquedaPeliculas::generarDot() {
    std::ofstream archivo("arbol_peliculas.dot");
    archivo << "digraph ArbolBinarioDeBusquedaPeliculas {\n";
    archivo << "bgcolor=lightblue;\n";
    archivo << "node [shape=box, style=\"rounded,filled\", fillcolor=lightgreen, color=black, penwidth=1, fontname=\"Arial\"];\n";
    archivo << "edge [splines=polyline];\n";

    generarDotAux(raiz, archivo);
    archivo << "}\n";
    archivo.close();
    
    // Se genera el reporte de manera silenciosa en el repositorio
    system("dot -Tpng arbol_peliculas.dot -o arbol_peliculas.png");
}

void ArbolBinarioDeBusquedaPeliculas::eliminarNodos(NodoArbolPeliculas* nodo) {
    if (nodo != nullptr) {
        eliminarNodos(nodo->getIzq());
        eliminarNodos(nodo->getDer());
        delete nodo;
    }
}

void ArbolBinarioDeBusquedaPeliculas::mostrarInorden() {
    if (estaVacio()) {
        std::cout << "Cartelera vacia. No hay películas para mostrar." << std::endl;
    } else {
        std::cout << "Cartelera de peliculas ordenada por codigo:" << std::endl;
        recorridoInorden(raiz);
        std::cout << "------------------------------------------" << std::endl;
    }
}

void ArbolBinarioDeBusquedaPeliculas::recorridoInorden(NodoArbolPeliculas* nodo) {
    if (nodo != nullptr) {

        //  Ir todo lo posible hacia la IZQUIERDA (codigos menores)
        recorridoInorden(nodo->getIzq());
        
        // Visitar/Imprimir la RAÍZ (el nodo actual), 
        // esto imprimira automáticamente el codigo y el titulo.
        cout << nodo->getPelicula() << endl;
        
        
        // 3. Ir hacia la DERECHA (codigos mayores)
        recorridoInorden(nodo->getDer());

    }


}

void ArbolBinarioDeBusquedaPeliculas::recolectarInorden(NodoArbolPeliculas* nodo, vector<Pelicula>& peliculas) const {
    if (nodo == nullptr) {
        return;
    }

    recolectarInorden(nodo->getIzq(), peliculas);
    peliculas.push_back(nodo->getPelicula());
    recolectarInorden(nodo->getDer(), peliculas);
}

vector<Pelicula> ArbolBinarioDeBusquedaPeliculas::obtenerPeliculasInorden() const {
    vector<Pelicula> peliculas;
    recolectarInorden(raiz, peliculas);
    return peliculas;
}