#include "Cancion.h"
#include <iostream>
using namespace std;
Cancion::Cancion(const std::string& id, const std::string& nom, float dur,
                 const std::string& r128, const std::string& r320)
    : idCompleto(id), nombre(nom), duracion(dur), reproducciones(0),
    ruta128(r128), ruta320(r320), artista(nullptr), album(nullptr) {}

Cancion::Cancion(const std::string& id, const std::string& nom, float dur,
                 const std::string& r128, const std::string& r320,
                 Album* a, Artista* art, const Creditos& cred)
    : idCompleto(id), nombre(nom), duracion(dur), reproducciones(0),
    ruta128(r128), ruta320(r320), artista(art), album(a), creditos(cred) {}

Cancion::Cancion()//comstructor por defecto
    : idCompleto(""), nombre(""), duracion(0.0f), reproducciones(0),
    ruta128(""), ruta320(""), artista(nullptr), album(nullptr) {}
//operador de comparación
bool Cancion::operator==(const Cancion& otra) const {
    return this->idCompleto == otra.idCompleto;
}
void Cancion::reproducir(int calidad) {
    string ruta = (calidad == 320) ? ruta320 : ruta128;
    cout << "reproduciendo " << nombre << " a "<<calidad<<"kbps desde: "<<ruta<<endl;
    reproducciones++;
}
//los getters ya están implementados en el el .h
//se determina como una clase para futuras implememtaciones yq esto es pnsado a futuro
