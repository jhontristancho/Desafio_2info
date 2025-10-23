#include "usuarios.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

Usuarios::Usuarios()
    : nickname(""), tipoMembresia(TIPO_STANDARD), ciudad(""), pais(""),
    fechaInscripcion(""), contadorReproducciones(0),
    posHistorial(0), numHistorial(0),
    listaFavoritos()  // ✅ INICIALIZAR listaFavoritos
{
    historialReproduccion = new std::string[MAX_HISTORIAL];
    for(int i=0; i < MAX_HISTORIAL; ++i) {
        historialReproduccion[i] = "";
    }
}

Usuarios::Usuarios(const std::string& nick, int tipo, const std::string& ciu,
                   const std::string& p, const std::string& fecha)
    : nickname(nick), tipoMembresia(tipo), ciudad(ciu), pais(p),
    fechaInscripcion(fecha), contadorReproducciones(0),
    posHistorial(0), numHistorial(0),
    listaFavoritos()  // ✅ INICIALIZAR listaFavoritos
{
    historialReproduccion = new std::string[MAX_HISTORIAL];
    for(int i=0; i < MAX_HISTORIAL; ++i) {
        historialReproduccion[i] = "";
    }
}

// Constructor de copia - ya está bien
// En usuarios.cpp - Constructor de copia
Usuarios::Usuarios(const Usuarios& u)
    : nickname(u.nickname), tipoMembresia(u.tipoMembresia),
    ciudad(u.ciudad), pais(u.pais),
    fechaInscripcion(u.fechaInscripcion),
    contadorReproducciones(u.contadorReproducciones),
    listaFavoritos(u.listaFavoritos),
    posHistorial(u.posHistorial), numHistorial(u.numHistorial) {

    cout << "[DEBUG Usuarios] Constructor de copia para: " << nickname << endl;

    historialReproduccion = new std::string[MAX_HISTORIAL];
    for (int i = 0; i < MAX_HISTORIAL; ++i) {
        historialReproduccion[i] = u.historialReproduccion[i];
    }
}
// Operador de asignación
Usuarios& Usuarios::operator=(const Usuarios& u) {
    if (this == &u) {
        return *this;
    }

    nickname = u.nickname;
    tipoMembresia = u.tipoMembresia;
    ciudad = u.ciudad;
    pais = u.pais;
    fechaInscripcion = u.fechaInscripcion;
    contadorReproducciones = u.contadorReproducciones;
    posHistorial = u.posHistorial;
    numHistorial = u.numHistorial;

    listaFavoritos = u.listaFavoritos;

    // ✅ CAMBIAR: re-asignar string*
    delete[] historialReproduccion;
    historialReproduccion = new std::string[MAX_HISTORIAL];
    for (int i = 0; i < MAX_HISTORIAL; ++i) {
        historialReproduccion[i] = u.historialReproduccion[i];
    }

    return *this;
}

// Destructor
Usuarios::~Usuarios() {
    delete[] historialReproduccion;
}

// Método registrarReproduccion actualizado
void Usuarios::registrarReproduccion(Cancion* c) {
    if (!c) return;

    contadorReproducciones++;

    if (tipoMembresia == TIPO_PREMIUM) {
        int posAnterior = (posHistorial - 1 + MAX_HISTORIAL) % MAX_HISTORIAL;

        // ✅ CAMBIAR: comparar strings
        if (numHistorial > 0 && historialReproduccion[posAnterior] == c->getIdCompleto()) {
            return;
        }

        // ✅ CAMBIAR: asignar string
        historialReproduccion[posHistorial] = c->getIdCompleto();

        posHistorial = (posHistorial + 1) % MAX_HISTORIAL;

        if (numHistorial < MAX_HISTORIAL) {
            numHistorial++;
        }
    }
}

// ✅ CAMBIAR: getCancionAnterior devuelve string*
std::string* Usuarios::getCancionAnterior() {
    if (tipoMembresia == TIPO_STANDARD) {
        cout << "Usuarios Standard no pueden retroceder canciones." << endl;
        return nullptr;
    }
    if (numHistorial == 0) {
        cout << "No hay canciones previas en el historial." << endl;
        return nullptr;
    }

    posHistorial = (posHistorial - 1 + MAX_HISTORIAL) % MAX_HISTORIAL;
    numHistorial--;

    return &historialReproduccion[posHistorial];
}

// Los demás métodos permanecen igual...
int Usuarios::getCalidadAudio() const {
    if (tipoMembresia == TIPO_PREMIUM) {
        return 320;
    } else {
        return 128;
    }
}

bool Usuarios::debeMostrarPublicidad() {
    return (tipoMembresia == TIPO_STANDARD &&
            contadorReproducciones > 0 &&
            (contadorReproducciones % 2) == 0);
}

void Usuarios::reproducirLista(bool aleatoria) {
    if (tipoMembresia != TIPO_PREMIUM) {
        cout << "[ERROR] La lista de favoritos es solo para Premium." << endl;
        return;
    }
    cout << "Iniciando reproduccion de la lista de favoritos." << endl;
    cout << "Modo: " << (aleatoria ? "Aleatorio" : "Orden original") << endl;
}

bool Usuarios::seguirUsuario(Usuarios* otroUsuario) {
    if (this->tipoMembresia != TIPO_PREMIUM || otroUsuario->tipoMembresia != TIPO_PREMIUM) {
        std::cout << "[ERROR] Solo usuarios Premium pueden seguir listas." << std::endl;
        return false;
    }
    if (this == otroUsuario) {
        std::cout << "[ERROR] No puedes seguir tu propia lista." << std::endl;
        return false;
    }
    this->listaFavoritos.setListaSeguida(&(otroUsuario->listaFavoritos));
    std::cout << "[INFO] El usuario '" << this->nickname << "' ahora sigue la lista de '"
              << otroUsuario->nickname << "'." << std::endl;
    return true;
}

// Los métodos cargarFavoritos... y mostrarInfo permanecen igual
bool Usuarios::cargarFavoritosDesdeString(const std::string& idsCadena) {
    std::cout << "[DEBUG] Iniciando carga de favoritos para: " << nickname << std::endl;
    if (idsCadena.empty()) {
        std::cout << "[DEBUG] Cadena vacía" << std::endl;
        return false;
    }
    std::stringstream ss(idsCadena);
    std::string idStr;
    int agregadas = 0;
    std::cout << "[DEBUG] IDs a procesar: " << idsCadena << std::endl;
    while (std::getline(ss, idStr, ',')) {
        std::cout << "[DEBUG] Procesando ID: '" << idStr << "'" << std::endl;
        if (!idStr.empty()) {
            std::cout << "[DEBUG] Llamando a agregarCancion..." << std::endl;
            if (listaFavoritos.agregarCancion(idStr)) {
                agregadas++;
                std::cout << "[DEBUG] Canción agregada exitosamente" << std::endl;
            } else {
                std::cout << "[DEBUG] No se pudo agregar la canción" << std::endl;
            }
        }
    }

    if (agregadas > 0) {
        std::cout << "[INFO] " << agregadas << " canciones cargadas para '" << nickname << "'." << std::endl;
    } else {
        std::cout << "[DEBUG] No se agregaron canciones" << std::endl;
    }

    return agregadas > 0;
}
ListaFavoritos& Usuarios::getListaFavoritos() {
    return listaFavoritos;
}
const ListaFavoritos* Usuarios::getListaSeguida() const {
    return listaFavoritos.obtenerListaExponer();
}
bool Usuarios::seguirListaFavoritos(ListaFavoritos* listaOtra) {
    if (this->tipoMembresia != TIPO_PREMIUM) {
        cout << "[ERROR] Solo los usuarios Premium pueden seguir listas." << endl;
        return false;
    }
    if (!listaOtra) {
        cout << "[ERROR] Lista de favoritos inválida." << endl;
        return false;
    }
    if (&this->listaFavoritos == listaOtra) {
        cout << "[ERROR] No puedes seguir tu propia lista." << endl;
        return false;
    }

    this->listaFavoritos.setListaSeguida(listaOtra);
    cout << "[INFO] " << nickname << " ahora sigue la lista de otro usuario." << endl;
    return true;
}
void Usuarios::mostrarInfo(const UdeATunesDataset* dataset) const {
    cout << "--- Perfil de Usuario ---\n";
    cout << "Reproducciones: " << contadorReproducciones << "\n";
    listaFavoritos.mostrarLista(dataset);
    cout << "-------------------------\n";
}
