#include "Cancion.h"
#include <iostream>
using namespace std;

// Constructor 1
Cancion::Cancion(const std::string& id, const std::string& nom, float dur,
                 const std::string& r128, const std::string& r320)
    : idCompleto(id), nombre(nom), duracion(dur), reproducciones(0),
    ruta128(r128), ruta320(r320), artista(nullptr), album(nullptr) {}

// Constructor 2
Cancion::Cancion(const std::string& id, const std::string& nom, float dur,
                 const std::string& r128, const std::string& r320,
                 Album* a, Artista* art, const Creditos& cred)
    : idCompleto(id), nombre(nom), duracion(dur), reproducciones(0),
    ruta128(r128), ruta320(r320), artista(art), album(a), creditos(cred) {}

// Constructor por defecto
Cancion::Cancion()
    : idCompleto(""), nombre(""), duracion(0.0f), reproducciones(0),
    ruta128(""), ruta320(""), artista(nullptr), album(nullptr) {}

// Operador de comparación
bool Cancion::operator==(const Cancion& otra) const {
    return this->idCompleto == otra.idCompleto;
}

void Cancion::reproducir(int calidad) {
    string ruta = (calidad == 320) ? ruta320 : ruta128;
    cout << "reproduciendo " << nombre << " a "<<calidad<<"kbps desde: "<<ruta<<endl;
    reproducciones++;
}

// ✅ Los getters ya están implementados en el header
// No necesitan implementación adicional en el .cpp
