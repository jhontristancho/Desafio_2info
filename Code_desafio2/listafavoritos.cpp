#include "ListaFavoritos.h"
#include <iostream>
#include "UdeATunesDataset.h"
#include "Cancion.h"
#include <cstdlib>
#include <ctime>
using namespace std;

// Constructor por defecto - cambiar long* a string*
ListaFavoritos::ListaFavoritos()
    : cancionesIds(new std::string[10]), numCanciones(0), capacidad(10), listaSeguida(nullptr) {}

// Constructor con capacidad - cambiar long* a string*
ListaFavoritos::ListaFavoritos(int capacidad)
    : cancionesIds(new std::string[capacidad]), numCanciones(0), capacidad(capacidad),listaSeguida(nullptr) {}

// Constructor de copia - cambiar long* a string*
ListaFavoritos::ListaFavoritos(const ListaFavoritos &l)
    : cancionesIds(new std::string[l.capacidad]), numCanciones(l.numCanciones), capacidad(l.capacidad), listaSeguida(nullptr) {
    for (int i = 0; i < numCanciones; i++) {
        cancionesIds[i] = l.cancionesIds[i];
    }
}

// Destructor
ListaFavoritos::~ListaFavoritos() {
    delete[] cancionesIds;
}

// Contiene canción - cambiar parámetro a string
bool ListaFavoritos::contieneCancion(const std::string& id) const {
    for (int i = 0; i < numCanciones; i++) {
        if (cancionesIds[i] == id) {
            return true;
        }
    }
    return false;
}

// Agregar canción - ahora funciona correctamente
bool ListaFavoritos::agregarCancion(const std::string& idCancion) {
    if (contieneCancion(idCancion)) {
        std::cout << "La cancion ya esta en la lista" << std::endl;
        return false;
    }
    if (numCanciones >= MAX_CANCIONES) {
        std::cout << "Limite maximo de canciones alcanzado (" << MAX_CANCIONES << ")" << std::endl;
        return false;
    }

    if (numCanciones >= capacidad) {
        redimensionar(capacidad * 2);
    }

    cancionesIds[numCanciones] = idCancion;
    numCanciones++;

    std::cout << "Cancion " << idCancion << " agregada a favoritos" << std::endl;
    return true;
}

// Eliminar canción - cambiar parámetro a string
bool ListaFavoritos::eliminarCancion(const std::string& id) {
    for (int i = 0; i < numCanciones; i++) {
        if (cancionesIds[i] == id) {
            for (int j = i; j < numCanciones - 1; j++) {
                cancionesIds[j] = cancionesIds[j + 1];
            }
            numCanciones--;
            std::cout << "Cancion " << id << " eliminada de favoritos" << std::endl;
            return true;
        }
    }
    std::cout << "Cancion " << id << " no encontrada en la lista" << std::endl;
    return false;
}

// Redimensionar - cambiar long* a string*
void ListaFavoritos::redimensionar(int nuevaCapacidad) {
    std::string* nuevoArray = new std::string[nuevaCapacidad];  // Cambiar a string*

    for (int i = 0; i < numCanciones; i++) {
        nuevoArray[i] = cancionesIds[i];
    }

    delete[] cancionesIds;
    cancionesIds = nuevoArray;
    capacidad = nuevaCapacidad;
}

// Actualizar otros métodos que usen cancionesIds
void ListaFavoritos::mostrarLista() const {
    std::cout << "LISTA DE FAVORITOS (" << numCanciones << " canciones)" << std::endl;
    for (int i = 0; i < numCanciones; i++) {
        std::cout << "nombre: "<<(i + 1) << ". ID: " << cancionesIds[i] << std::endl;
    }
    if (numCanciones == 0) {
        std::cout << "   (Lista vacia)" << std::endl;
    }
}

// Actualizar mostrarLista con dataset
void ListaFavoritos::mostrarLista(const UdeATunesDataset* dataset) const {
    const ListaFavoritos* listaPropia = this;
    const ListaFavoritos* listaSeg = listaSeguida;

    // Combinar si sigue a alguien
    ListaFavoritos listaFinal = *listaPropia;
    if (listaSeg) {
        listaFinal = listaFinal + *listaSeg;
        cout << "LISTA DE FAVORITOS COMBINADA ("
             << listaFinal.getNumCanciones() << " canciones, incluyendo seguidas)"
             << endl;
    } else {
        cout << "LISTA DE FAVORITOS (" << listaFinal.getNumCanciones() << " canciones)"
             << endl;
    }

    if (listaFinal.getNumCanciones() == 0) {
        cout << "    (Lista vacía)" << endl;
        return;
    }

    const std::string* canciones = listaFinal.getCancionesIds();

    for (int i = 0; i < listaFinal.getNumCanciones(); ++i) {
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
             << " (ID: " << idCancion << ")" << endl;
    }
}
ListaFavoritos ListaFavoritos::operator+(const ListaFavoritos &otra) const {
    ListaFavoritos nuevaLista(this->numCanciones + otra.numCanciones);

    for (int i = 0; i < this->numCanciones; i++) {
        nuevaLista.agregarCancion(this->cancionesIds[i]);
    }

    for (int i = 0; i < otra.numCanciones; i++) {
        nuevaLista.agregarCancion(otra.cancionesIds[i]);
    }

    return nuevaLista;
}

// Los demás métodos permanecen igual...
const ListaFavoritos* ListaFavoritos::obtenerListaExponer() const {
    if (listaSeguida) {
        return listaSeguida;
    }
    return this;
}
// Operador de asignación
ListaFavoritos& ListaFavoritos::operator=(const ListaFavoritos &l) {
    if (this == &l) {
        return *this;
    }

    // Liberar memoria antigua
    delete[] cancionesIds;

    // Asignar nuevos valores
    capacidad = l.capacidad;
    numCanciones = l.numCanciones;
    listaSeguida = l.listaSeguida;

    // Reservar nueva memoria
    cancionesIds = new std::string[capacidad];

    // Copiar datos
    for (int i = 0; i < numCanciones; i++) {
        cancionesIds[i] = l.cancionesIds[i];
    }

    return *this;
}
int ListaFavoritos::getNumCanciones() const {
    const ListaFavoritos* listaActual = obtenerListaExponer();
    return listaActual->numCanciones;
}

void ListaFavoritos::reproducir(bool aleatoria) const {
    const ListaFavoritos* listaActual = obtenerListaExponer();
    // ... implementación
}
