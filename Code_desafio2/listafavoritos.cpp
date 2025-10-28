#include "ListaFavoritos.h"
#include <iostream>
#include <udeatunesdataset.h>
#include "Cancion.h"
#include <cstdlib>
#include <ctime>
using namespace std;
ListaFavoritos::ListaFavoritos()
    : cancionesIds(new string[10]), numCanciones(0), capacidad(10), listaSeguida(nullptr) {}
//este por si se conoce la capacidad si no se le asigna el por defecto con 10 canciones
ListaFavoritos::ListaFavoritos(int capacidad)
    : cancionesIds(new string[capacidad]), numCanciones(0), capacidad(capacidad),listaSeguida(nullptr) {}
//constructor de copia
ListaFavoritos::ListaFavoritos(const ListaFavoritos &l)
    : cancionesIds(new std::string[l.capacidad]), numCanciones(l.numCanciones), capacidad(l.capacidad), listaSeguida(nullptr) {
    for (int i = 0; i < numCanciones; i++) {
        cancionesIds[i] = l.cancionesIds[i];
    }
}
//destrtor
ListaFavoritos::~ListaFavoritos() {
    delete[] cancionesIds;
}
//para ver si ya esta esa cancion y lanzar un booleano a ver si si o no
bool ListaFavoritos::contieneCancion(const std::string& id) const {
    int contador=0;
    for (int i = 0; i < numCanciones; i++) {
        ++contador;
        if (cancionesIds[i] == id) {
            *UdeATunesDataset::iteraciones += contador;
            return true;
        }
    }
    *UdeATunesDataset::iteraciones += contador;
    return false;
}
bool ListaFavoritos::agregarCancion(const string& idCancion) {
    int contador=0;
    if (contieneCancion(idCancion)) {
        cout << "esa cancion ya la agregaste" << endl;
        return false;
    }
    if (numCanciones >= MAX_CANCIONES) {
        cout << "ya alcanzaste el maximo de canciones que es  (" << MAX_CANCIONES << ")" <<endl;
        return false;
    }
    if (numCanciones >= capacidad) {
        redimensionar(capacidad * 2);
    }
    cancionesIds[numCanciones] = idCancion;
    numCanciones++;
    *UdeATunesDataset::iteraciones+=contador;
    return true;
}
bool ListaFavoritos::eliminarCancion(const std::string& id) {
    int contador=0;
    for (int i = 0; i < numCanciones; i++) {
        ++contador;
        if (cancionesIds[i] == id) {
            for (int j = i; j < numCanciones - 1; j++) {
                ++contador;
                cancionesIds[j] = cancionesIds[j + 1];
            }
            numCanciones--;
            cout << "la cancion con el id de " << id << " fue eliminada de favoritos" << std::endl;
            *UdeATunesDataset::iteraciones += contador;
            return true;
        }
    }
    cout << "la cancion  " << id << " no fue encontrada en la lista de favorito" << std::endl;
    *UdeATunesDataset::iteraciones += contador;
    return false;
}
void ListaFavoritos::redimensionar(int nuevaCapacidad) {
    int contador=0;
    std::string* nuevoArray = new std::string[nuevaCapacidad];
UdeATunesDataset::actualizarPeakMemory();
    for (int i = 0; i < numCanciones; i++) {
        ++contador;
        nuevoArray[i] = cancionesIds[i];
    }
    delete[] cancionesIds;//para evitar lafuga
    cancionesIds = nuevoArray;
    capacidad = nuevaCapacidad;
    *UdeATunesDataset::iteraciones += contador;
}
void ListaFavoritos::mostrarLista() const {//desplieque
    int contador=0;
    cout << "tu lista de favoritos con (" << numCanciones << " canciones)" <<endl;
    for (int i = 0; i < numCanciones; i++) {
        ++contador;
        cout << "cancion: "<<(i + 1) << ". ID: " << cancionesIds[i] <<endl;
    }
    if (numCanciones == 0) {
        cout << "tu lista no tiene canciones, empieza a agregar" <<endl;
    }
    *UdeATunesDataset::iteraciones += contador;
}
void ListaFavoritos::mostrarLista(const UdeATunesDataset* dataset) const {//ARREGLARRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR
    int contador=0;
    const ListaFavoritos* listaPropia = this;
    const ListaFavoritos* listaSeg = listaSeguida;
    ListaFavoritos listaFinal = *listaPropia;//aca si sigue a alguien se combina utilizndo ese puntero
    if (listaSeg) {
        listaFinal = listaFinal + *listaSeg;
        cout << "tu lista de favoritos incluyendo la del usuario al que sigues  ("<< listaFinal.getNumCanciones() << " canciones, muy buen gusto.)"<< endl;
    } else {
        cout << "tu lista de favoritos (" << listaFinal.getNumCanciones() << " canciones, que buen gusto )"
             << endl;
    }
    if (listaFinal.getNumCanciones() == 0) {
        cout << "tienes la lista vacia, agrega canciones y escucha se que te van a gustar" << endl;
        return;
    }
    const std::string* canciones = listaFinal.getCancionesIds();
    for (int i = 0; i < listaFinal.getNumCanciones(); ++i) {
        ++contador;
        std::string idCancion = canciones[i];
        std::string nombreCancion = "Desconocida";
        if (dataset) {
            std::string linea = dataset->buscarLineaPorID(
                dataset->getLineasCanciones(),
                dataset->getNumLineasCanciones(),
                idCancion,
                0
                );
            if (!linea.empty()) {
                nombreCancion = dataset->obtenerCampo(linea, 1);
            }
        }
        cout << "    " << (i + 1) << ". " << nombreCancion
             << " ID: " << idCancion << ")" << endl;
    }
    *UdeATunesDataset::iteraciones += contador;
}
ListaFavoritos ListaFavoritos::operator+(const ListaFavoritos &otra) const {
    int contador=0;//esto principalmente se va utilizar pa la logica de la lista a otro usuario que sigue
    ListaFavoritos nuevaLista(this->numCanciones + otra.numCanciones);
    for (int i = 0; i < this->numCanciones; i++) {
        ++contador;
        nuevaLista.agregarCancion(this->cancionesIds[i]);
    }
    for (int i = 0; i < otra.numCanciones; i++) {
        ++contador;
        nuevaLista.agregarCancion(otra.cancionesIds[i]);
    }
    *UdeATunesDataset::iteraciones += contador;
    return nuevaLista;
}
const ListaFavoritos* ListaFavoritos::obtenerListaExponer() const {
    if (listaSeguida) {
        return listaSeguida;
    }
    return this;
}
ListaFavoritos& ListaFavoritos::operator=(const ListaFavoritos &l){
    int contador=0;
    if (this == &l) {
        return *this;
    }
    if (cancionesIds != nullptr) {
        delete[] cancionesIds;//por si acaso
    }
    capacidad = l.capacidad;
    numCanciones = l.numCanciones;
    this->listaSeguida = nullptr;//por si apuntaba a alguna basura
    if (capacidad > 0) { //siempre se reserva memoria, asa esa la lista est vacia
        cancionesIds = new std::string[capacidad];
UdeATunesDataset::actualizarPeakMemory();
        for (int i = 0; i < numCanciones; i++) {
            ++contador;
            cancionesIds[i] = l.cancionesIds[i];
        }
    } else {//si es 0 asigna 10
        capacidad = 10;
        cancionesIds = new std::string[capacidad];
        UdeATunesDataset::actualizarPeakMemory();
    }
    *UdeATunesDataset::iteraciones += contador;
    return *this;
}
int ListaFavoritos::getNumCanciones() const {
    int total = numCanciones;//esta siempre va contar es la propias de cada usuario
    if (listaSeguida != nullptr) {
        total += listaSeguida->getNumCancionesPropias();
    }
    return total;
}
void ListaFavoritos::reproducir(bool aleatoria) const {
    const ListaFavoritos* listaActual = obtenerListaExponer();
}
void ListaFavoritos::establecerCancionesPropias(std::string* nuevosIds, int numNuevos) {
    if (this->cancionesIds != nullptr) {//esto es para evita alguna fuga
        delete[] this->cancionesIds;
        this->cancionesIds = nullptr;
    }
    this->numCanciones = numNuevos;
    if (numNuevos > 0) {
        this->capacidad = numNuevos;
        this->cancionesIds = new std::string[this->capacidad];
UdeATunesDataset::actualizarPeakMemory();
        for (int i = 0; i < this->numCanciones; ++i) {
            this->cancionesIds[i] = nuevosIds[i];
        }
    } else {//por si la lista esta vacia
        this->capacidad = 10;
        this->numCanciones = 0;
        this->cancionesIds = new std::string[this->capacidad];//y que nunca apunte a nulo
        UdeATunesDataset::actualizarPeakMemory();
    }
}
std::string* ListaFavoritos::getListaCompletaDeIds(int& totalCanciones) const {//y ahora por si sigue a otro usuario mostrar la lista complet
    int propiasCount = this->numCanciones;
    const std::string* propiasIds = this->cancionesIds;
    int seguidasCount = 0;
    const std::string* seguidasIds = nullptr;
    if (this->listaSeguida != nullptr) {
        seguidasCount = this->listaSeguida->numCanciones;
        seguidasIds = this->listaSeguida->cancionesIds;
    }
    totalCanciones = propiasCount + seguidasCount;
    if (totalCanciones == 0) {
        return nullptr;//unico caso
    }
    std::string* listaCombinada = new std::string[totalCanciones];
    UdeATunesDataset::actualizarPeakMemory();
    int k = 0;
    for (int i = 0; i < propiasCount; ++i) {
        listaCombinada[k++] = propiasIds[i];
    }
    if (seguidasIds != nullptr && seguidasCount > 0) {//y esto para las que sigo
        for (int i = 0; i < seguidasCount; ++i) {
            listaCombinada[k++] = seguidasIds[i];
        }
    }
    return listaCombinada;
}
