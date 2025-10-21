// Cancion.cpp
#include "Cancion.h"
#include <iostream>
using namespace std;
Cancion::Cancion(int id, const std::string& nom, float dur, const std::string& r128, const std::string& r320)
    : idCompleto(id), nombre(nom), duracion(dur), reproducciones(0), ruta128(r128),
    ruta320(r320), artista(nullptr), album(nullptr) {}
Cancion::Cancion(int id, const std::string& nom, float dur, const std::string& r128, const std::string& r320,
                 Album* a, Artista* art, const Creditos& cred)
    : idCompleto(id), nombre(nom), duracion(dur), reproducciones(0), ruta128(r128),
    ruta320(r320), artista(art), album(a), creditos(cred) {}

Cancion::Cancion()
    : idCompleto(0), nombre(""), duracion(0.0f), reproducciones(0),
    ruta128(""), ruta320(""), artista(nullptr), album(nullptr){}
bool Cancion::operator==(const Cancion& otra) const {
    return this->idCompleto == otra.idCompleto;
}

void Cancion::reproducir(int calidad) {
    string ruta = (calidad == 320) ? ruta320 : ruta128;
    cout << "reproduciendo " << nombre << " a "<<calidad<<"kbps desde: "<<ruta<<endl;
    reproducciones++;
}

Creditos& Cancion::getCreditos() { return creditos; }
void Cancion::setArtista(Artista* art) { this->artista = art; }
void Cancion::setAlbum(Album* alb) { this->album = alb; }
