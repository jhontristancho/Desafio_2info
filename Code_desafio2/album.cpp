#include "Album.h"
#include <iostream>
#include <string>
#include <udeatunesdataset.h>
using namespace std;
const int ALBUM_CAPACIDAD_INICIAL = 10;
const int GROW_FACTOR = 2;
void Album::resizeCanciones() {
    int contador=0;
    int nuevaCapacidad = capacidadCanciones * GROW_FACTOR;
    Cancion** nuevoArray = new Cancion*[nuevaCapacidad];
    for (int i = 0; i < numCanciones; ++i) {
        ++contador;
        nuevoArray[i] = canciones[i];
    }
    delete[] canciones;
    canciones = nuevoArray;
    capacidadCanciones = nuevaCapacidad;
    *UdeATunesDataset::iteraciones += contador;
}
Album::Album()//constrctor de defecto
    : idAlbum(""), nombre(""), selloDisquero(""), fechaLanzamiento(""),
    duracionTotal(0.0f), puntuacion(0), portadaRuta(""),
    generos(nullptr), numGeneros(0),
    canciones(nullptr), numCanciones(0), capacidadCanciones(ALBUM_CAPACIDAD_INICIAL),
    artista(nullptr)
{
    canciones = new Cancion*[capacidadCanciones];
}
//este es el que se va usar
Album::Album(string id, const std::string& nom, const std::string& fecha, const std::string& port)
    : idAlbum(id), nombre(nom),
    selloDisquero("ninguno"), fechaLanzamiento(fecha),
    duracionTotal(0.0f), puntuacion(0), portadaRuta(port),
    generos(nullptr), numGeneros(0),
    canciones(nullptr), numCanciones(0), capacidadCanciones(ALBUM_CAPACIDAD_INICIAL),
    artista(nullptr)
{
canciones = new Cancion*[capacidadCanciones];//puntero a canciones
}
Album::~Album() {
    for (int i = 0; i < numCanciones; ++i) {
        delete canciones[i];
    }
    delete[] canciones;
    delete[] generos;//cuando lo usemos
}
Album::Album(const Album& otra)
    : idAlbum(otra.idAlbum), nombre(otra.nombre),
    selloDisquero(otra.selloDisquero), fechaLanzamiento(otra.fechaLanzamiento),
    duracionTotal(otra.duracionTotal), puntuacion(otra.puntuacion),
    portadaRuta(otra.portadaRuta),
    numGeneros(otra.numGeneros), numCanciones(otra.numCanciones),
    capacidadCanciones(otra.capacidadCanciones), artista(nullptr)
{
    if (numGeneros > 0) {
        generos = new std::string[numGeneros];
        for (int i = 0; i < numGeneros; ++i) {
            generos[i] = otra.generos[i];
        }
    } else {
        generos = nullptr;
    }
    canciones = new Cancion*[capacidadCanciones];
    for (int i = 0; i < numCanciones; ++i) {
        // este constructord de copia nos va servir para crear y asi evitar que el compilador se confunda
        canciones[i] = new Cancion(*(otra.canciones[i]));
        canciones[i]->setAlbum(this);
    }
}

Album& Album::operator=(const Album& otra) {//asignarle pa que copie
    int contador=0;
    if (this == &otra) {
        return *this;
    }
    for (int i = 0; i < numCanciones; ++i) {
        ++contador;
        delete canciones[i];//liberamos para no ocupar doble
    }
    delete[] canciones;
    delete[] generos;
idAlbum = otra.idAlbum;
nombre = otra.nombre;
    selloDisquero = otra.selloDisquero;
    fechaLanzamiento = otra.fechaLanzamiento;
    duracionTotal = otra.duracionTotal;
    puntuacion = otra.puntuacion;
    portadaRuta = otra.portadaRuta;
    numGeneros = otra.numGeneros;
    numCanciones = otra.numCanciones;
    capacidadCanciones = otra.capacidadCanciones;
    artista = nullptr;
    if (numGeneros > 0) {
        generos = new std::string[numGeneros];
        for (int i = 0; i < numGeneros; ++i) {
            ++contador;
            generos[i] = otra.generos[i];
        }
    } else {
        generos = nullptr;
    }
    canciones = new Cancion*[capacidadCanciones];
    for (int i = 0; i < numCanciones; ++i) {
        ++contador;
        // Crea un nuevo objeto Cancion
        canciones[i] = new Cancion(*(otra.canciones[i]));
        canciones[i]->setAlbum(this);
    }
    *UdeATunesDataset::iteraciones += contador;
    return *this;
}
bool Album::agregarCancion(Cancion* c) {
    int contador=0;
    if (!c) return false;

    // evitar duplicados
    for (int i = 0; i < numCanciones; ++i){
        ++contador;
        if (canciones[i] == c) return false;
    }
    if (numCanciones >= capacidadCanciones)
        resizeCanciones();

    canciones[numCanciones++] = c;
    duracionTotal += c->getDuracion();
    *UdeATunesDataset::iteraciones += contador;
    return true;
}


void Album::setGeneros(const std::string* gens, int num) {//pa el futuro o si se implementa

    delete[] generos;
    generos = nullptr;
    if (num > 0 && gens) {
        numGeneros = num;
        generos = new std::string[numGeneros];
        for (int i = 0; i < numGeneros; ++i)
            generos[i] = gens[i];
    } else numGeneros = 0;
}

void Album::mostrarInfo() const {
    int contador=0;
    cout << "album" << endl;
    cout << "id: " << idAlbum << "nombre: " << nombre << endl;//el id del album igual, pero por ahora dejemoslo y si no lo quitamos
    //cout << "Sello: " << selloDisquero << " | Puntuacion: " << puntuacion << endl;//esto lo podemos implementar si se nos da, pero por lo que hablamos con el profesor no se va a dar por ahora
   //cout << "Fecha: " << fechaLanzamiento << " | Duracion Total: " << duracionTotal << " min" << endl;
    cout << "Canciones (" << numCanciones << "):" << endl;
    if (numCanciones > 0) {
        for (int i = 0; i < numCanciones; ++i) {
            ++contador;
            cout << "  " << (i+1) << ". "
                 << "id: " << canciones[i]->getIdCompleto()<<endl//dejemoslo para ver si esta leyendo bien el id
                 << "titulo: " << canciones[i]->getNombre()<<endl
                 << "duración: " << canciones[i]->getDuracion() << endl;
        }
    } else {
        cout << "  no hay canciones " << endl;
    }
    cout << endl;
    *UdeATunesDataset::iteraciones += contador;
}
Cancion* Album::buscarCancion(const std::string& id) const {
    int contador=0;
    for (int i = 0; i < numCanciones; ++i) {
        ++contador;
        if (canciones[i]->getIdCompleto() == id) {
            return canciones[i];
        }
    }
    *UdeATunesDataset::iteraciones += contador;
    return nullptr;
}
