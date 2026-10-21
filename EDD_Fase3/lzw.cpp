#include "lzw.h"

#include <iostream>

ListaCodigos::ListaCodigos() : cabeza(nullptr), cola(nullptr), cantidad(0) {}

ListaCodigos::ListaCodigos(const ListaCodigos& otra)
    : cabeza(nullptr), cola(nullptr), cantidad(0) {
    for (NodoCodigo* actual = otra.cabeza; actual != nullptr;
         actual = actual->siguiente) {
        agregar(actual->codigo);
    }
}

ListaCodigos::~ListaCodigos() {
    limpiar();
}

ListaCodigos& ListaCodigos::operator=(const ListaCodigos& otra) {
    if (this != &otra) {
        limpiar();
        for (NodoCodigo* actual = otra.cabeza; actual != nullptr;
             actual = actual->siguiente) {
            agregar(actual->codigo);
        }
    }
    return *this;
}

void ListaCodigos::agregar(int codigo) {
    NodoCodigo* nuevo = new NodoCodigo(codigo);
    if (cola == nullptr) {
        cabeza = nuevo;
    } else {
        cola->siguiente = nuevo;
    }
    cola = nuevo;
    ++cantidad;
}

bool ListaCodigos::estaVacia() const {
    return cabeza == nullptr;
}

int ListaCodigos::size() const {
    return cantidad;
}

NodoCodigo* ListaCodigos::obtenerCabeza() const {
    return cabeza;
}

void ListaCodigos::limpiar() {
    while (cabeza != nullptr) {
        NodoCodigo* siguiente = cabeza->siguiente;
        delete cabeza;
        cabeza = siguiente;
    }
    cola = nullptr;
    cantidad = 0;
}

DiccionarioCompresion::DiccionarioCompresion(int capacidadInicial)
    : tabla(new EntradaDiccionario*[capacidadInicial]),
      capacidad(capacidadInicial), siguienteCodigo(256) {
    for (int i = 0; i < capacidad; ++i) tabla[i] = nullptr;
    inicializarASCII();
}

DiccionarioCompresion::~DiccionarioCompresion() {
    liberar();
    delete[] tabla;
}

int DiccionarioCompresion::indiceHash(const string& secuencia) const {
    unsigned long hash = 5381;
    for (size_t i = 0; i < secuencia.size(); ++i) {
        hash = ((hash << 5) + hash) ^
               static_cast<unsigned char>(secuencia[i]);
    }
    return static_cast<int>(hash % static_cast<unsigned long>(capacidad));
}

void DiccionarioCompresion::liberar() {
    for (int i = 0; i < capacidad; ++i) {
        while (tabla[i] != nullptr) {
            EntradaDiccionario* siguiente = tabla[i]->siguiente;
            delete tabla[i];
            tabla[i] = siguiente;
        }
    }
}

void DiccionarioCompresion::inicializarASCII() {
    liberar();
    siguienteCodigo = 256;
    for (int i = 0; i < 256; ++i) {
        string caracter(1, static_cast<char>(i));
        agregar(caracter, i);
    }
}

bool DiccionarioCompresion::buscar(const string& secuencia, int& codigo) const {
    int indice = indiceHash(secuencia);
    for (EntradaDiccionario* actual = tabla[indice]; actual != nullptr;
         actual = actual->siguiente) {
        if (actual->secuencia == secuencia) {
            codigo = actual->codigo;
            return true;
        }
    }
    return false;
}

bool DiccionarioCompresion::agregar(const string& secuencia, int codigo) {
    int existente = 0;
    if (buscar(secuencia, existente)) return false;
    int indice = indiceHash(secuencia);
    EntradaDiccionario* nuevo = new EntradaDiccionario(secuencia, codigo);
    nuevo->siguiente = tabla[indice];
    tabla[indice] = nuevo;
    if (codigo >= siguienteCodigo) siguienteCodigo = codigo + 1;
    return true;
}

int DiccionarioCompresion::obtenerSiguienteCodigo() const {
    return siguienteCodigo;
}

DiccionarioDescompresion::DiccionarioDescompresion(int capacidadInicial)
    : datos(new string[capacidadInicial]), ocupado(new bool[capacidadInicial]),
      capacidad(capacidadInicial), siguienteCodigo(256) {
    for (int i = 0; i < capacidad; ++i) ocupado[i] = false;
    inicializarASCII();
}

DiccionarioDescompresion::~DiccionarioDescompresion() {
    delete[] datos;
    delete[] ocupado;
}

void DiccionarioDescompresion::ampliar(int codigo) {
    if (codigo < capacidad) return;
    int nuevaCapacidad = capacidad;
    while (codigo >= nuevaCapacidad) nuevaCapacidad *= 2;
    string* nuevosDatos = new string[nuevaCapacidad];
    bool* nuevosOcupados = new bool[nuevaCapacidad];
    for (int i = 0; i < nuevaCapacidad; ++i) nuevosOcupados[i] = false;
    for (int i = 0; i < capacidad; ++i) {
        nuevosDatos[i] = datos[i];
        nuevosOcupados[i] = ocupado[i];
    }
    delete[] datos;
    delete[] ocupado;
    datos = nuevosDatos;
    ocupado = nuevosOcupados;
    capacidad = nuevaCapacidad;
}

void DiccionarioDescompresion::inicializarASCII() {
    for (int i = 0; i < capacidad; ++i) ocupado[i] = false;
    siguienteCodigo = 256;
    for (int i = 0; i < 256; ++i) {
        ampliar(i);
        datos[i] = string(1, static_cast<char>(i));
        ocupado[i] = true;
    }
}

bool DiccionarioDescompresion::buscar(int codigo, string& secuencia) const {
    if (codigo < 0 || codigo >= capacidad || !ocupado[codigo]) return false;
    secuencia = datos[codigo];
    return true;
}

bool DiccionarioDescompresion::agregar(const string& secuencia, int codigo) {
    if (codigo < 0) return false;
    ampliar(codigo);
    if (ocupado[codigo]) return false;
    datos[codigo] = secuencia;
    ocupado[codigo] = true;
    if (codigo >= siguienteCodigo) siguienteCodigo = codigo + 1;
    return true;
}

int DiccionarioDescompresion::obtenerSiguienteCodigo() const {
    return siguienteCodigo;
}

ListaCodigos CompresorLZW::comprimir(const string& textoPlano) const {
    ListaCodigos salida;
    if (textoPlano.empty()) return salida;
    DiccionarioCompresion diccionario;
    string actual(1, textoPlano[0]);
    for (size_t i = 1; i < textoPlano.size(); ++i) {
        string candidato = actual + textoPlano[i];
        int codigo = 0;
        if (diccionario.buscar(candidato, codigo)) {
            actual = candidato;
        } else {
            diccionario.buscar(actual, codigo);
            salida.agregar(codigo);
            diccionario.agregar(candidato, diccionario.obtenerSiguienteCodigo());
            actual = string(1, textoPlano[i]);
        }
    }
    int codigoFinal = 0;
    diccionario.buscar(actual, codigoFinal);
    salida.agregar(codigoFinal);
    return salida;
}

string CompresorLZW::descomprimir(const ListaCodigos& codigos) const {
    if (codigos.estaVacia()) return "";
    DiccionarioDescompresion diccionario;
    NodoCodigo* actualCodigo = codigos.obtenerCabeza();
    string anterior;
    if (!diccionario.buscar(actualCodigo->codigo, anterior)) return "";
    string salida = anterior;
    actualCodigo = actualCodigo->siguiente;
    while (actualCodigo != nullptr) {
        string actual;
        if (diccionario.buscar(actualCodigo->codigo, actual)) {
            // La entrada normal ya está disponible.
        } else if (actualCodigo->codigo == diccionario.obtenerSiguienteCodigo()) {
            actual = anterior + anterior[0];
        } else {
            return "";
        }
        diccionario.agregar(anterior + actual[0],
                            diccionario.obtenerSiguienteCodigo());
        salida += actual;
        anterior = actual;
        actualCodigo = actualCodigo->siguiente;
    }
    return salida;
}

void guardarArchivoEDD(const string& ruta, const ListaCodigos& codigos) {
    ofstream archivo(ruta.c_str(), ios::binary);
    if (!archivo.is_open()) return;
    for (NodoCodigo* actual = codigos.obtenerCabeza(); actual != nullptr;
         actual = actual->siguiente) {
        archivo.write(reinterpret_cast<const char*>(&actual->codigo),
                      sizeof(actual->codigo));
    }
}

ListaCodigos leerArchivoEDD(const string& ruta) {
    ListaCodigos codigos;
    ifstream archivo(ruta.c_str(), ios::binary);
    if (!archivo.is_open()) return codigos;
    int codigo = 0;
    while (archivo.read(reinterpret_cast<char*>(&codigo), sizeof(codigo))) {
        codigos.agregar(codigo);
    }
    return codigos;
}
