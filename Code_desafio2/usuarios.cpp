#include "usuarios.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <udeatunesdataset.h>
using namespace std;
Usuarios::Usuarios()
    : nickname(""), tipoMembresia(TIPO_STANDARD), ciudad(""), pais(""),
    fechaInscripcion(""), contadorReproducciones(0),
    posHistorial(0), numHistorial(0),
    listaFavoritos()//inicializ la lista de favortio del usuario
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
    listaFavoritos()
{
    historialReproduccion = new std::string[MAX_HISTORIAL];
    for(int i=0; i < MAX_HISTORIAL; ++i) {
        historialReproduccion[i] = "";
    }
}
Usuarios::Usuarios(const Usuarios& u)
    : nickname(u.nickname), tipoMembresia(u.tipoMembresia),
    ciudad(u.ciudad), pais(u.pais),
    fechaInscripcion(u.fechaInscripcion),
    contadorReproducciones(u.contadorReproducciones),
    listaFavoritos(u.listaFavoritos),
    posHistorial(u.posHistorial), numHistorial(u.numHistorial) {
    historialReproduccion = new std::string[MAX_HISTORIAL];
    for (int i = 0; i < MAX_HISTORIAL; ++i) {
        historialReproduccion[i] = u.historialReproduccion[i];
    }
}
Usuarios& Usuarios::operator=(const Usuarios& u) {
    int contador=0;
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
    delete[] historialReproduccion;
    historialReproduccion = new std::string[MAX_HISTORIAL];
    for (int i = 0; i < MAX_HISTORIAL; ++i) {
        ++contador;
        historialReproduccion[i] = u.historialReproduccion[i];
    }
    *UdeATunesDataset::iteraciones +=contador;
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
        cout << "Usuarios estandar no pueden retroceder canciones." << endl;
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
        cout << "la lista de favoritos es solo para Premium." << endl;
        return;
    }
    cout << "iniciando reproduccion de la lista de favoritos." << endl;
    cout << "modo: " << (aleatoria ? "aleatorio" : "orden original") << endl;
}

bool Usuarios::seguirUsuario(Usuarios* otroUsuario) {
    if (this->tipoMembresia != TIPO_PREMIUM || otroUsuario->tipoMembresia != TIPO_PREMIUM) {
        std::cout << "solo usuarios Premium pueden seguir listas." << std::endl;
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
    int contador=0;
    if (idsCadena.empty()) {
        return false;
    }
    std::stringstream ss(idsCadena);
    std::string idStr;
    int agregadas = 0;
    while (std::getline(ss, idStr, ',')) {
        ++contador;
        if (!idStr.empty()) {
            if (listaFavoritos.agregarCancion(idStr)) {
                agregadas++;
            } else {
            }
        }
    }

    if (agregadas > 0) {
    } else {
    }
    *UdeATunesDataset::iteraciones += contador;
    return agregadas > 0;
}
ListaFavoritos& Usuarios::getListaFavoritos() {
    return listaFavoritos;
}
const ListaFavoritos* Usuarios::getListaSeguida() const {
    return listaFavoritos.obtenerListaExponer();
}
bool Usuarios::seguirListaFavoritos(const ListaFavoritos* listaOtra) {
    if (this->tipoMembresia != TIPO_PREMIUM) {
        cout << "Solo los usuarios Premium pueden seguir listas." << endl;
        return false;
    }

    if (!listaOtra) {
        cout << "La lista a seguir no existe.\n";
        return false;
    }

    // Evitar seguir la propia lista
    if (listaOtra == &this->listaFavoritos) {
        cout << "No puedes seguir tu propia lista." << endl;
        return false;
    }
    ListaFavoritos nuevaLista = this->listaFavoritos + *listaOtra;
    this->listaFavoritos = nuevaLista;
    cout << "Has seguido correctamente la lista. "
         << "Ahora tu lista contiene " << this->listaFavoritos.getNumCanciones()
         << " canciones (incluidas las de la lista seguida)." << endl;
    return true;
}
void Usuarios::mostrarInfo(const UdeATunesDataset* dataset) const {
    cout << "Perfil de Usuario"<<endl;
    cout << "Reproducciones: " << contadorReproducciones << "\n";
    cout << "Mi lista de favoritos:\n";
    listaFavoritos.mostrarLista(dataset); // muestra delegada si listaSeguida está activa
    // Mostrar explícitamente la lista propia aunque siga otra
    cout << "\n(Mostrando también mi lista propia si sigo otra):\n";
    // Forzar mostrar la lista propia sin delegación:
    ListaFavoritos copia = listaFavoritos; // usa operator= o constructor copia
    copia.setListaSeguida(nullptr); // asegurar que muestra su propia lista
    copia.mostrarLista(dataset);
    if (listaFavoritos.getListaSeguida() != nullptr) {
        cout << "\nActualmente siguiendo la lista de otro usuario. Para dejar de seguir use la opción correspondiente.\n";
    }
    cout << "-------------------------\n";
}
bool Usuarios::dejarDeSeguir() {
    if (listaFavoritos.getListaSeguida() == nullptr) {
        cout << "[INFO] No estás siguiendo ninguna lista." << endl;
        return false;
    }
    listaFavoritos.setListaSeguida(nullptr);
    cout << "[INFO] Has dejado de seguir la lista." << endl;
    return true;
}
