#ifndef LZW_H
#define LZW_H

#include <fstream>
#include <string>

using namespace std;

class NodoCodigo {
public:
    int codigo;
    NodoCodigo* siguiente;
    explicit NodoCodigo(int valor) : codigo(valor), siguiente(nullptr) {}
};

class ListaCodigos {
private:
    NodoCodigo* cabeza;
    NodoCodigo* cola;
    int cantidad;

public:
    ListaCodigos();
    ListaCodigos(const ListaCodigos& otra);
    ~ListaCodigos();
    ListaCodigos& operator=(const ListaCodigos& otra);

    void agregar(int codigo);
    bool estaVacia() const;
    int size() const;
    NodoCodigo* obtenerCabeza() const;
    void limpiar();
};

class EntradaDiccionario {
public:
    string secuencia;
    int codigo;
    EntradaDiccionario* siguiente;
    EntradaDiccionario(const string& valor, int codigo_)
        : secuencia(valor), codigo(codigo_), siguiente(nullptr) {}
};

class DiccionarioCompresion {
private:
    EntradaDiccionario** tabla;
    int capacidad;
    int siguienteCodigo;
    int indiceHash(const string& secuencia) const;
    void liberar();

public:
    explicit DiccionarioCompresion(int capacidadInicial = 2053);
    ~DiccionarioCompresion();
    void inicializarASCII();
    bool buscar(const string& secuencia, int& codigo) const;
    bool agregar(const string& secuencia, int codigo);
    int obtenerSiguienteCodigo() const;
};

class DiccionarioDescompresion {
private:
    string* datos;
    bool* ocupado;
    int capacidad;
    int siguienteCodigo;
    void ampliar(int codigo);

public:
    explicit DiccionarioDescompresion(int capacidadInicial = 512);
    ~DiccionarioDescompresion();
    void inicializarASCII();
    bool buscar(int codigo, string& secuencia) const;
    bool agregar(const string& secuencia, int codigo);
    int obtenerSiguienteCodigo() const;
};

class CompresorLZW {
public:
    ListaCodigos comprimir(const string& textoPlano) const;
    string descomprimir(const ListaCodigos& codigos) const;
};

void guardarArchivoEDD(const string& ruta, const ListaCodigos& codigos);
ListaCodigos leerArchivoEDD(const string& ruta);

#endif
