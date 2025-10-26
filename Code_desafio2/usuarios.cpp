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
    : nickname(u.nickname), tipoMembresia(u.tipoMembresia), ciudad(u.ciudad), pais(u.pais),
    fechaInscripcion(u.fechaInscripcion), contadorReproducciones(u.contadorReproducciones),
    posHistorial(u.posHistorial), numHistorial(u.numHistorial),
    listaFavoritos(u.listaFavoritos), // Llama al constructor de copia de ListaFavoritos (DEEP COPY)

    // 🏆 CORRECCIÓN CLAVE 1: Inicializar el puntero delegado a nullptr
    usuarioSeguido(nullptr) // El seguimiento NO se copia; se resuelve en la carga
{
    // 🏆 CORRECCIÓN CLAVE 2: Copia profunda de historialReproduccion
    this->historialReproduccion = new std::string[MAX_HISTORIAL];
    for(int i = 0; i < MAX_HISTORIAL; ++i) {
        this->historialReproduccion[i] = u.historialReproduccion[i];
    }
}
Usuarios& Usuarios::operator=(const Usuarios& u) {
    int contador = 0;
    if (this == &u) {
        return *this;
    }

    // 1. Copiar miembros primitivos y strings
    nickname = u.nickname;
    tipoMembresia = u.tipoMembresia;
    ciudad = u.ciudad;
    pais = u.pais;
    fechaInscripcion = u.fechaInscripcion;
    contadorReproducciones = u.contadorReproducciones;
    posHistorial = u.posHistorial;
    numHistorial = u.numHistorial;

    // 2. ¡CORRECCIÓN CLAVE! Neutralizar el puntero de objeto.
    // El seguimiento DEBE ser resuelto SÓLO durante la carga de datos (Doble Pase).
    // Evita que el objeto copiado apunte a la dirección antigua del seguido.
    this->usuarioSeguido = nullptr;

    // 3. Copia profunda de ListaFavoritos (llama a ListaFavoritos::operator=)
    listaFavoritos = u.listaFavoritos;

    // 4. Copia profunda del Historial
    delete[] historialReproduccion;
    historialReproduccion = new std::string[MAX_HISTORIAL];
    for (int i = 0; i < MAX_HISTORIAL; ++i) {
        ++contador;
        historialReproduccion[i] = u.historialReproduccion[i];
    }

    *UdeATunesDataset::iteraciones += contador;
    return *this;
}
// En el destructor de Usuarios
Usuarios::~Usuarios() {
    // Asegúrate de que solo liberas el array de strings
    if (historialReproduccion != nullptr) {
        delete[] historialReproduccion;
    }
    // No debes liberar listaFavoritos (es miembro, no puntero)
    // No debes liberar usuarioSeguido (es puntero de objeto externo)
}
void Usuarios::registrarReproduccion(Cancion* c) {
    if (!c) return;
    contadorReproducciones++;
    if (tipoMembresia == TIPO_PREMIUM) {
        int posAnterior = (posHistorial - 1 + MAX_HISTORIAL) % MAX_HISTORIAL;
        if (numHistorial > 0 && historialReproduccion[posAnterior] == c->getIdCompleto()) {
            return;
        }
        historialReproduccion[posHistorial] = c->getIdCompleto();

        posHistorial = (posHistorial + 1) % MAX_HISTORIAL;

        if (numHistorial < MAX_HISTORIAL) {
            numHistorial++;
        }
    }
}
std::string* Usuarios::getCancionAnterior() {
    if (tipoMembresia == TIPO_STANDARD) {
        cout << "usuarios estandar no pueden retroceder canciones." << endl;
        return nullptr;
    }
    if (numHistorial == 0) {
        cout << "no hay canciones previas en el historial." << endl;
        return nullptr;
    }
    posHistorial = (posHistorial - 1 + MAX_HISTORIAL) % MAX_HISTORIAL;
    numHistorial--;
    return &historialReproduccion[posHistorial];
}
int Usuarios::getCalidadAudio() const {
    if (tipoMembresia == TIPO_PREMIUM) {//para ver si se le mueestra al usuario cual
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
        std::cout << "[ERROR] Solo usuarios Premium pueden seguir listas de otros usuarios Premium." << std::endl;
        return false;
    }
    if (this == otroUsuario) {
        std::cout << "[ERROR] No puedes seguir tu propia lista." << std::endl;
        return false;
    }
    if (this->usuarioSeguido != nullptr) {
        std::cout << "[ERROR] Ya estás siguiendo la lista de '" << this->usuarioSeguido->nickname << "'. Debes dejar de seguirlo primero." << std::endl;
        return false;
    }
    // 3. Establecer el seguimiento
    this->usuarioSeguido = otroUsuario; // Almacenar el puntero del usuario
    this->listaFavoritos.setListaSeguida(&(otroUsuario->listaFavoritos)); // Delegar la visualización/reproducción
    std::cout << "[INFO] El usuario '" << this->nickname << "' ahora sigue la lista de '"
              << otroUsuario->nickname << "'." << std::endl;
    return true;
}
bool Usuarios::cargarFavoritosDesdeString(const std::string& idsCadena) {
    int contador = 0;
    if (idsCadena.empty()) {
        // Llama a un método para vaciar la lista de favoritos propia
        listaFavoritos.establecerCancionesPropias(nullptr, 0);
        return false;
    }

    // --- 1. Contar IDs para asignar memoria ---
    int numIds = 1; // Asume al menos un ID si la cadena no está vacía
    for (char c : idsCadena) {
        if (c == ',') {
            numIds++;
        }
    }

    // --- 2. Asignar memoria dinámica para IDs temporales ---
    std::string* idsCargados = new std::string[numIds];
    int idx = 0;

    // --- 3. Parseo manual sin STL containers (usando find/substr) ---
    size_t inicio = 0;
    size_t fin = idsCadena.find(',');

    while (fin != std::string::npos) {
        if (idx < numIds) {
            idsCargados[idx++] = idsCadena.substr(inicio, fin - inicio);
        }
        inicio = fin + 1;
        fin = idsCadena.find(',', inicio);
        ++contador;
    }
    // Añadir el último ID
    if (idx < numIds) {
        idsCargados[idx++] = idsCadena.substr(inicio);
    }

    // Ajustar numIds al conteo real (si había comas al final o vacíos)
    numIds = idx;

    // --- 4. Llamada al nuevo método de carga masiva ---
    // Este método es crucial para reemplazar el array interno de la lista.
    listaFavoritos.establecerCancionesPropias(idsCargados, numIds);

    // --- 5. Liberar memoria temporal ---
    delete[] idsCargados;

    *UdeATunesDataset::iteraciones += contador;
    return numIds > 0;
}
const ListaFavoritos* Usuarios::getListaSeguida() const {
    return listaFavoritos.obtenerListaExponer();
}
ListaFavoritos& Usuarios::getListaFavoritos() {
    return listaFavoritos;
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

    // 1. Validar el estado de seguimiento usando la nueva metadata (más robusto)
    if (this->usuarioSeguido == nullptr) {
        cout << "[INFO] No estás siguiendo a ningún usuario." << endl;
        return false;
    }

    // Guardar el nombre antes de borrar el puntero para el mensaje de confirmación
    std::string nicknameSeguido = this->usuarioSeguido->getNickname();

    // 2. ¡CORRECCIÓN CLAVE! Borrar el puntero de la metadata del usuario.
    // Esto asegura que guardarListasDeFavoritos() escriba un campo vacío en el TXT.
    this->usuarioSeguido = nullptr;

    // 3. Borrar la delegación de la lista (esto es lo que ya tenías)
    listaFavoritos.setListaSeguida(nullptr);

    cout << "[INFO] Has dejado de seguir la lista de '" << nicknameSeguido << "'." << endl;

    return true;
}
