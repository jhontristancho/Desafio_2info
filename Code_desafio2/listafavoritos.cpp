#include "ListaFavoritos.h"
#include <iostream>
#include "UdeATunesDataset.h"
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
//destrto
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
// En ListaFavoritos.cpp
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
            cout << "la cancion" << id << " eliminada de favoritos" << std::endl;
            *UdeATunesDataset::iteraciones += contador;
            return true;
        }
    }
    cout << "la cancion  " << id << " no fue encontrada en su lista de favorito" << std::endl;
    *UdeATunesDataset::iteraciones += contador;
    return false;
}
void ListaFavoritos::redimensionar(int nuevaCapacidad) {
    int contador=0;
    std::string* nuevoArray = new std::string[nuevaCapacidad];  // Cambiar a string*
    for (int i = 0; i < numCanciones; i++) {
        ++contador;
        nuevoArray[i] = cancionesIds[i];
    }
    delete[] cancionesIds;//para evitar fuga
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

// Actualizar mostrarLista con dataset
void ListaFavoritos::mostrarLista(const UdeATunesDataset* dataset) const {//ARREGLARRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR
    int contador=0;
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
             << " (ID: " << idCancion << ")" << endl;
    }
    *UdeATunesDataset::iteraciones += contador;
}
ListaFavoritos ListaFavoritos::operator+(const ListaFavoritos &otra) const {
    int contador=0;
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

    // 1. Liberar lo antiguo
    if (cancionesIds != nullptr) { // Siempre verifica antes de liberar
        delete[] cancionesIds;
    }

    // 2. Copiar metadata
    capacidad = l.capacidad;
    numCanciones = l.numCanciones;

    // 3. ¡CORRECCIÓN CLAVE! Neutralizar la delegación.
    // El objeto recién asignado debe empezar sin delegación activa.
    this->listaSeguida = nullptr;

    // 4. Reservar nueva memoria
    if (numCanciones > 0) {
        cancionesIds = new std::string[capacidad];

        // 5. Copiar los datos (Deep Copy)
        for (int i = 0; i < numCanciones; i++) {
            ++contador;
            cancionesIds[i] = l.cancionesIds[i];
        }
    } else {
        cancionesIds = nullptr;
    }

    *UdeATunesDataset::iteraciones += contador;
    return *this;
}
int ListaFavoritos::getNumCanciones() const {
    const ListaFavoritos* listaActual = obtenerListaExponer();
    return listaActual->numCanciones;
}

void ListaFavoritos::reproducir(bool aleatoria) const {
    const ListaFavoritos* listaActual = obtenerListaExponer();
}
// En ListaFavoritos.cpp (o en ListaFavoritos.h si es inline)
void ListaFavoritos::establecerCancionesPropias(std::string* nuevosIds, int numNuevos) {
    // 1. Liberar la memoria antigua para evitar fugas (CRÍTICO)
    if (this->cancionesIds != nullptr) {
        delete[] this->cancionesIds;
    }

    // 2. Establecer el nuevo tamaño
    this->numCanciones = numNuevos;

    if (numNuevos > 0) {
        // 3. Asignar nueva memoria
        this->cancionesIds = new std::string[this->numCanciones];

        // 4. Copiar los IDs
        for (int i = 0; i < this->numCanciones; ++i) {
            this->cancionesIds[i] = nuevosIds[i]; // Copia el string (NO punteros)
        }
    } else {
        // 5. Si está vacía, asegurar que el puntero sea nullptr
        this->cancionesIds = nullptr;
    }

    // NOTA: El puntero this->listaSeguida no se toca aquí.
}
std::string* ListaFavoritos::getListaCompletaDeIds(int& totalCanciones) const {

    // 1. Obtener canciones propias
    int propiasCount = this->numCanciones;       // Usa el contador de la lista actual
    const std::string* propiasIds = this->cancionesIds; // Usa el array de IDs de la lista actual

    // 2. Obtener canciones seguidas (con validación de puntero)
    int seguidasCount = 0;
    const std::string* seguidasIds = nullptr;

    if (this->listaSeguida != nullptr) {
        // **IMPORTANTE:** Aquí asumimos que getNumCanciones() y getCancionesIds() en
        // la lista seguida SÓLO devuelven sus propias canciones, evitando delegación recursiva.
        seguidasCount = this->listaSeguida->getNumCanciones();
        seguidasIds = this->listaSeguida->getCancionesIds();
    }

    totalCanciones = propiasCount + seguidasCount;

    if (totalCanciones == 0) {
        return nullptr; // No hay canciones en total
    }

    // 3. Crear el array combinado (DEBE SER LIBERADO por el método que lo llama)
    std::string* listaCombinada = new std::string[totalCanciones];
    int k = 0; // Índice para el nuevo array

    // 4. Copiar IDs propios
    for (int i = 0; i < propiasCount; ++i) {
        listaCombinada[k++] = propiasIds[i];
    }

    // 5. Copiar IDs seguidos (si existen)
    if (seguidasIds != nullptr && seguidasCount > 0) {
        for (int i = 0; i < seguidasCount; ++i) {
            listaCombinada[k++] = seguidasIds[i];
        }
    }

    return listaCombinada;
}
