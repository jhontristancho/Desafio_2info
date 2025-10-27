#include "Artista.h"
#include "Album.h"
#include <iostream>
#include <udeatunesdataset.h>
using namespace std;
const int ARTISTA_CAPACIDAD_INICIAL = 5;
const int GROW_FACTOR = 2;
void Artista::resizeAlbumes() {
    int contador=0;
    int nuevaCapacidad = capacidadAlbumes * GROW_FACTOR;
    Album** nuevoArray = new Album*[nuevaCapacidad];
        UdeATunesDataset::actualizarPeakMemory();
    for (int i = 0; i < numAlbumes; ++i) {
        ++contador;
        nuevoArray[i] = albumes[i];
    }
    delete[] albumes;
    albumes = nuevoArray;
    capacidadAlbumes = nuevaCapacidad;
    *UdeATunesDataset::iteraciones += contador;
}
Artista::Artista()
    : idArtista(""), nombre(""), edad(0), paisOrigen(""),
    seguidores(0), posicionTendencia(999999),
    albumes(nullptr), numAlbumes(0), capacidadAlbumes(ARTISTA_CAPACIDAD_INICIAL) {
    albumes = new Album*[capacidadAlbumes];
        UdeATunesDataset::actualizarPeakMemory();
}
Artista::Artista(string id, const std::string& nom)
    : idArtista(id), nombre(nom), edad(0), paisOrigen("desconocido"),
    seguidores(0), posicionTendencia(999999),
    albumes(nullptr), numAlbumes(0), capacidadAlbumes(ARTISTA_CAPACIDAD_INICIAL) {
    albumes = new Album*[capacidadAlbumes];
        UdeATunesDataset::actualizarPeakMemory();
}
Artista::~Artista() {
    delete[] albumes;
}
Artista::Artista(const Artista& otra)
    : idArtista(otra.idArtista), nombre(otra.nombre), edad(otra.edad),
    paisOrigen(otra.paisOrigen), seguidores(otra.seguidores),
    posicionTendencia(otra.posicionTendencia),
    numAlbumes(otra.numAlbumes), capacidadAlbumes(otra.capacidadAlbumes) {
    albumes = new Album*[capacidadAlbumes];
        UdeATunesDataset::actualizarPeakMemory();
    for (int i = 0; i < numAlbumes; ++i) {
        albumes[i] = otra.albumes[i];//este copia los puntero
    }
}
Artista& Artista::operator=(const Artista& otra) {//esto es para defininir el operador de igualar
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
            UdeATunesDataset::actualizarPeakMemory();
        for (int i = 0; i < numAlbumes; ++i) {
            ++contador;
            albumes[i] = otra.albumes[i];
        }
    }
    *UdeATunesDataset::iteraciones += contador;
    return *this;
}
bool Artista::agregarAlbum(Album* a) {
    int contador=0;
    if (!a) return false;
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
Album* Artista::buscarAlbum(string id) const {//este lo que va a hacer es buscar el album por el id, que en este caso nos toco con string para que si lo reconozca
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
    cout << "informacion del artista:" << endl; //esto por si en un futuro se debe implementar, ais que por ahora no re
    cout << "id: " << idArtista << endl;
    cout << "nombre: " << nombre << endl;
    cout << "pais: " << paisOrigen << endl;
    cout << "albumes (" << numAlbumes << "):" << endl;
    if (numAlbumes > 0) {
        for (int i = 0; i < numAlbumes; ++i) {
            ++contador;
            if (albumes[i]) {
                cout << "  " << (i+1) << ". " << albumes[i]->getNombre()
                << " (ID: " << albumes[i]->getIdAlbum() << ")" << endl;
            }
        }
    } else {
        cout << " no tiene albumunes en el dataset" << endl;
    }
    *UdeATunesDataset::iteraciones += contador;
}
