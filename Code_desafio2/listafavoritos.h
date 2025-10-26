#ifndef LISTA_FAVORITOS_H
#define LISTA_FAVORITOS_H
#include "cancion.h"
class UdeATunesDataset;
class Cancion;
class ListaFavoritos {
private:
    std::string* cancionesIds;
    int numCanciones;
    int capacidad;
    static const int MAX_CANCIONES = 10000;
    ListaFavoritos* listaSeguida;
;
    void redimensionar(int nuevaCapacidad);
    bool contieneCancion(const std::string& id) const;  // Cambiar a string
public:
    // Constructores y destructor
    ListaFavoritos();
    ListaFavoritos(int capacidad);
    ListaFavoritos(const ListaFavoritos &l);
    ~ListaFavoritos();
    ListaFavoritos& operator=(const ListaFavoritos &l);
    // Métodos principales
    bool agregarCancion(const std::string& idCancion);
    bool eliminarCancion(const std::string& id);  // Cambiar a string
    void mostrarLista() const;
    void reproducir(bool aleatoria) const;
    void establecerCancionesPropias(std::string* nuevosIds, int numNuevos);
    bool agregarLista(ListaFavoritos* otra);
    // Sobrecarga de operadores
    ListaFavoritos operator+(const ListaFavoritos &otra) const;
    // Getters
    const ListaFavoritos* obtenerListaExponer() const;
    int getCapacidad() const { return capacidad; }
    void mostrarLista(const UdeATunesDataset* dataset) const;
    const ListaFavoritos* getListaSeguida() const { return listaSeguida; }
    void setListaSeguida(ListaFavoritos* otraLista) { listaSeguida = otraLista; }
    int getNumCanciones() const;
    std::string* getListaCompletaDeIds(int& totalCanciones) const;
    void combinarListas(const ListaFavoritos* otra);
    void eliminarCancionesDe(const ListaFavoritos* otra);
    const std::string* getCancionesIds() const {  // Cambiar a string*
        const ListaFavoritos* lista = obtenerListaExponer();
        return lista->cancionesIds;
    }
};
#endif

