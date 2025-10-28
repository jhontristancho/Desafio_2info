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
    void redimensionar(int nuevaCapacidad);
public:
    ListaFavoritos();
    ListaFavoritos(int capacidad);
    ListaFavoritos(const ListaFavoritos &l);
    ~ListaFavoritos();
    bool agregarCancion(const std::string& idCancion);
    bool eliminarCancion(const std::string& id);
    void mostrarLista() const;
    void reproducir(bool aleatoria) const;
    bool agregarLista(ListaFavoritos* otra);
    const ListaFavoritos* obtenerListaExponer() const;
    int getCapacidad() const { return capacidad; }
    ListaFavoritos& operator=(const ListaFavoritos &l);
    ListaFavoritos operator+(const ListaFavoritos &otra) const;
       std::string* getListaCompletaDeIds(int& totalCanciones) const;
    void establecerCancionesPropias(std::string* nuevosIds, int numNuevos);
    void mostrarLista(const UdeATunesDataset* dataset) const;
    bool contieneCancion(const std::string& id) const;
    int getNumCanciones() const;
    void setListaSeguida(ListaFavoritos* lista) { listaSeguida = lista; }
    const ListaFavoritos* getListaSeguida() const { return listaSeguida; }
    int getNumCancionesPropias() const { return numCanciones; }
    const std::string* getCancionesIdsPropias() const { return cancionesIds; }
    void combinarListas(const ListaFavoritos* otra);
    void eliminarCancionesDe(const ListaFavoritos* otra);
    const std::string* getCancionesIds() const {
        const ListaFavoritos* lista = obtenerListaExponer();
        return lista->cancionesIds;
    }
};
#endif

