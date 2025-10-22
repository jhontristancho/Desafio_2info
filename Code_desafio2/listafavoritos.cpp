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
        std::cout << "   " << (i + 1) << ". ID: " << cancionesIds[i] << std::endl;
    }
    if (numCanciones == 0) {
        std::cout << "   (Lista vacia)" << std::endl;
    }
}

// Actualizar mostrarLista con dataset
void ListaFavoritos::mostrarLista(const UdeATunesDataset* dataset) const {
    const ListaFavoritos* listaActual = obtenerListaExponer();

    cout << "LISTA DE FAVORITOS (" << listaActual->getNumCanciones() << " canciones)";
    if (listaActual != this) {
        cout << " (DELEGADA - Accediendo a la lista de otro usuario)";
    }
    cout << endl;

    if (listaActual->getNumCanciones() == 0) {
        cout << "    (Lista vacia)" << endl;
        return;
    }

    const std::string* canciones = listaActual->getCancionesIds();

    for (int i = 0; i < listaActual->getNumCanciones(); ++i) {
        std::string idCancion = canciones[i];

        // ✅ BUSCAR LA CANCIÓN EN EL DATASET PARA OBTENER SU NOMBRE
        Cancion* cancion = nullptr;
        if (dataset) {
            cancion = dataset->buscarCancion(idCancion);
        }

        cout << "    " << (i + 1) << ". ";
        if (cancion) {
            // ✅ MOSTRAR NOMBRE DE LA CANCIÓN Y ARTISTA
            cout << cancion->getNombre();

            // Mostrar artista si está disponible
            if (cancion->getArtista()) {
                cout << " - " << cancion->getArtista()->getNombre();
            }

            cout << " (ID: " << idCancion << ")";
        } else {
            // Si no se encuentra la canción, mostrar solo el ID
            cout << "Canción no encontrada (ID: " << idCancion << ")";
        }
        cout << endl;
    }
}
// Actualizar operador +
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
