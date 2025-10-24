#include "Artista.h"
#include "Album.h"
#include <iostream>
#include <udeatunesdataset.h>
using namespace std;

const int ARTISTA_CAPACIDAD_INICIAL = 5;
const int GROW_FACTOR = 2;

// =====================================================
//  Redimensionamiento dinámico del arreglo de álbumes
// =====================================================
void Artista::resizeAlbumes() {
    int contador=0;
    int nuevaCapacidad = capacidadAlbumes * GROW_FACTOR;
    Album** nuevoArray = new Album*[nuevaCapacidad];

    for (int i = 0; i < numAlbumes; ++i) {
        ++contador;
        nuevoArray[i] = albumes[i];
    }

    delete[] albumes;
    albumes = nuevoArray;
    capacidadAlbumes = nuevaCapacidad;

    cout << "[INFO_ARTISTA]: arreglo de álbumes redimensionado a "
         << capacidadAlbumes << endl;
    *UdeATunesDataset::iteraciones += contador;
}

// =====================================================
//  Constructores y Destructor
// =====================================================
Artista::Artista()
    : idArtista(""), nombre(""), edad(0), paisOrigen("N/A"),
    seguidores(0), posicionTendencia(999999),
    albumes(nullptr), numAlbumes(0), capacidadAlbumes(ARTISTA_CAPACIDAD_INICIAL) {
    albumes = new Album*[capacidadAlbumes];
}

Artista::Artista(string id, const std::string& nom)
    : idArtista(id), nombre(nom), edad(0), paisOrigen("Desconocido"),
    seguidores(0), posicionTendencia(999999),
    albumes(nullptr), numAlbumes(0), capacidadAlbumes(ARTISTA_CAPACIDAD_INICIAL) {
    albumes = new Album*[capacidadAlbumes];
}

Artista::~Artista() {
    // Importante: no borrar los álbumes (el dataset los gestiona)
    delete[] albumes;
}

// =====================================================
//  Constructor de copia
// =====================================================
Artista::Artista(const Artista& otra)
    : idArtista(otra.idArtista), nombre(otra.nombre), edad(otra.edad),
    paisOrigen(otra.paisOrigen), seguidores(otra.seguidores),
    posicionTendencia(otra.posicionTendencia),
    numAlbumes(otra.numAlbumes), capacidadAlbumes(otra.capacidadAlbumes) {

    albumes = new Album*[capacidadAlbumes];
    for (int i = 0; i < numAlbumes; ++i) {
        albumes[i] = otra.albumes[i]; // Copia los punteros, no los objetos
    }
}

// =====================================================
//  Operador de asignación
// =====================================================
Artista& Artista::operator=(const Artista& otra) {
    int contador=0;
    if (this != &otra) {
        delete[] albumes;

        idArtista = otra.idArtista;
        nombre = otra.nombre;
        edad = otra.edad;
        paisOrigen = otra.paisOrigen;
        seguidores = otra.seguidores;
        posicionTendencia = otra.posicionTendencia;
        numAlbumes = otra.numAlbumes;
        capacidadAlbumes = otra.capacidadAlbumes;

        albumes = new Album*[capacidadAlbumes];
        for (int i = 0; i < numAlbumes; ++i) {
            ++contador;
            albumes[i] = otra.albumes[i]; // Copia punteros
        }
    }
    *UdeATunesDataset::iteraciones += contador;
    return *this;
}

// =====================================================
//  Agregar álbum (mantiene punteros válidos)
// =====================================================
bool Artista::agregarAlbum(Album* a) {
    int contador=0;
    if (!a) return false;

    // Evitar duplicados
    for (int i = 0; i < numAlbumes; ++i){
        ++contador;
        if (albumes[i] == a) return false;
    }


    if (numAlbumes >= capacidadAlbumes)
        resizeAlbumes();

    albumes[numAlbumes++] = a;
    a->setArtista(this);
    *UdeATunesDataset::iteraciones += contador;
    return true;
}

// =====================================================
//  Buscar álbum por ID
// =====================================================
Album* Artista::buscarAlbum(string id) const {
    int contador=0;
    for (int i = 0; i < numAlbumes; ++i) {
        ++contador;
        if (albumes[i] && albumes[i]->getIdAlbum() == id)
            return albumes[i];
    }
    *UdeATunesDataset::iteraciones += contador;
    return nullptr;
}
void Artista::mostrarInfo() const {
    int contador=0;
    cout << "--- Info Artista ---" << endl;
    cout << "ID:       " << idArtista << endl;
    cout << "Nombre:   " << nombre << endl;
    cout << "Pais:     " << paisOrigen << endl;
    cout << "Albumes (" << numAlbumes << "):" << endl;

    if (numAlbumes > 0) {
        for (int i = 0; i < numAlbumes; ++i) {
            ++contador;
            if (albumes[i]) {
                cout << "  " << (i+1) << ". " << albumes[i]->getNombre()
                << " (ID: " << albumes[i]->getIdAlbum() << ")" << endl;
            }
        }
    } else {
        cout << "  (Sin albumes registrados)" << endl;
    }
    *UdeATunesDataset::iteraciones += contador;
    cout << "----------------------" << endl;
}
