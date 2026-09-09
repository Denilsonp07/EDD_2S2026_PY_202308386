#include "arbolbinariopeliculas.h"

namespace {
int fechaComoNumero(const std::string& fecha)
{
    std::string digitos;
    for (char caracter : fecha) {
        if (isdigit(static_cast<unsigned char>(caracter))) {
            digitos += caracter;
        }
    }
    if (digitos.size() != 8) {
        return 0;
    }

    const int primero = atoi(digitos.substr(0, 4).c_str());
    const int segundo = atoi(digitos.substr(4, 2).c_str());
    const int tercero = atoi(digitos.substr(6, 2).c_str());
    if (primero > 1900) {
        return primero * 10000 + segundo * 100 + tercero;
    }

    return tercero * 10000 + segundo * 100 + primero;
}

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

NodoArbolPeliculas* ArbolBinarioDeBusquedaPeliculas::obtenerNodoMinimo(NodoArbolPeliculas* nodo) const {
    NodoArbolPeliculas* actual = nodo;
    while (actual != nullptr && actual->getIzq() != nullptr) {
        actual = actual->getIzq();
    }
    return actual;
}

NodoArbolPeliculas* ArbolBinarioDeBusquedaPeliculas::eliminarNodo(NodoArbolPeliculas* nodo, int codigoNumerico, bool& eliminado) {
    if (nodo == nullptr) {
        return nullptr;
    }

    const int codigoActual = numeroCodigo(nodo->getPelicula().getCodigo());

    if (codigoNumerico < codigoActual) {
        nodo->setIzq(eliminarNodo(nodo->getIzq(), codigoNumerico, eliminado));
        return nodo;
    }

    if (codigoNumerico > codigoActual) {
        nodo->setDer(eliminarNodo(nodo->getDer(), codigoNumerico, eliminado));
        return nodo;
    }

    eliminado = true;

    if (nodo->getIzq() == nullptr) {
        NodoArbolPeliculas* derecha = nodo->getDer();
        delete nodo;
        return derecha;
    }

    if (nodo->getDer() == nullptr) {
        NodoArbolPeliculas* izquierda = nodo->getIzq();
        delete nodo;
        return izquierda;
    }

    NodoArbolPeliculas* sucesor = obtenerNodoMinimo(nodo->getDer());
    nodo->setPelicula(sucesor->getPelicula());

    bool eliminadoSucesor = false;
    const int codigoSucesor = numeroCodigo(sucesor->getPelicula().getCodigo());
    nodo->setDer(eliminarNodo(nodo->getDer(), codigoSucesor, eliminadoSucesor));

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

        std::string campos[8];
        int contadorCampos = 0;
        size_t inicio = 0;

        for (size_t i = 0; i <= linea.size(); ++i) {
            if (i == linea.size() || linea[i] == ',') {
                if (contadorCampos < 8) {
                    campos[contadorCampos++] = quitarComillas(linea.substr(inicio, i - inicio));
                }
                inicio = i + 1;
            }
        }

        if (contadorCampos >= 8) {
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

void ArbolBinarioDeBusquedaPeliculas::recolectarInorden(NodoArbolPeliculas* nodo, ListaPeliculas& peliculas) const {
    if (nodo == nullptr) {
        return;
    }

    recolectarInorden(nodo->getIzq(), peliculas);
    peliculas.push_back(nodo->getPelicula());
    recolectarInorden(nodo->getDer(), peliculas);
}

ListaPeliculas ArbolBinarioDeBusquedaPeliculas::obtenerPeliculasInorden() const {
    ListaPeliculas peliculas;
    recolectarInorden(raiz, peliculas);
    return peliculas;
}

bool ArbolBinarioDeBusquedaPeliculas::eliminarPorCodigo(const string& codigo) {
    bool eliminado = false;
    raiz = eliminarNodo(raiz, numeroCodigo(codigo), eliminado);
    return eliminado;
}

void ArbolBinarioDeBusquedaPeliculas::generarReporteAux(
    NodoArbolPeliculas* nodo, std::ofstream& archivo, const string& fechaActual)
{
    if (nodo == nullptr) {
        return;
    }

    const Pelicula& pelicula = nodo->getPelicula();
    const int hoy = fechaComoNumero(fechaActual);
    const int retiro = fechaComoNumero(pelicula.getFechaFin());
    const bool vencida = retiro > 0 && retiro < hoy;
    const bool proximaARetirar = !vencida && retiro > hoy && retiro <= hoy + 7;
    const string color = vencida ? "#D5D8DC" : (proximaARetirar ? "#F4D03F" : "#7DCEA0");
    const string codigo = escaparGraphviz(pelicula.getCodigo());
    const string etiqueta = "Codigo: " + escaparGraphviz(pelicula.getCodigo()) + "\\n"
        + "Titulo: " + escaparGraphviz(pelicula.getTitulo()) + "\\n"
        + "Duracion: " + escaparGraphviz(pelicula.getDuracion()) + " min\\n"
        + "Clasificacion: " + escaparGraphviz(pelicula.getClasificacion());

    archivo << "\"" << codigo << "\" [shape=box, style=filled, fillcolor=\""
            << color << "\", label=\"" << etiqueta << "\"];\n";
    if (nodo->getIzq() != nullptr) {
        archivo << "\"" << codigo << "\" -> \""
                << escaparGraphviz(nodo->getIzq()->getPelicula().getCodigo()) << "\";\n";
    }
    if (nodo->getDer() != nullptr) {
        archivo << "\"" << codigo << "\" -> \""
                << escaparGraphviz(nodo->getDer()->getPelicula().getCodigo()) << "\";\n";
    }

    generarReporteAux(nodo->getIzq(), archivo, fechaActual);
    generarReporteAux(nodo->getDer(), archivo, fechaActual);
}

bool ArbolBinarioDeBusquedaPeliculas::generarReporteCartelera(
    const string& rutaDot, const string& rutaPng, const string& fechaActual) const
{
    std::ofstream archivo(rutaDot);
    if (!archivo.is_open()) {
        return false;
    }

    archivo << "digraph CarteleraPeliculas {\n"
            << "rankdir=TB;\n"
            << "bgcolor=white;\n"
            << "node [fontname=Arial, color=black];\n"
            << "edge [color=\"#566573\", arrowsize=0.8];\n";
    const_cast<ArbolBinarioDeBusquedaPeliculas*>(this)->generarReporteAux(
        raiz, archivo, fechaActual);
    archivo << "}\n";
    archivo.close();

    const string comando = "dot -Tpng \"" + rutaDot + "\" -o \"" + rutaPng + "\"";
    return system(comando.c_str()) == 0;
}

void AbolBinarioDeBusquedaPeliculas::contarNodosDeBST