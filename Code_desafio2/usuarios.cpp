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
    historialReproduccion = new string[MAX_HISTORIAL];
    int contador=0;
    for(int i=0; i < MAX_HISTORIAL; ++i) {
        ++contador;
        historialReproduccion[i] = "";
    }
    *UdeATunesDataset::iteraciones += contador;

}
Usuarios::Usuarios(const string& nick, int tipo, const string& ciu,
                   const string& p, const string& fecha)
    : nickname(nick), tipoMembresia(tipo), ciudad(ciu), pais(p),
    fechaInscripcion(fecha), contadorReproducciones(0),
    posHistorial(0), numHistorial(0),
    listaFavoritos()
{
    historialReproduccion = new string[MAX_HISTORIAL];
    int contador= 0;
    for(int i=0; i < MAX_HISTORIAL; ++i) {
        ++contador;
        historialReproduccion[i] = "";
    }
    *UdeATunesDataset::iteraciones += contador;

}
Usuarios::Usuarios(const Usuarios& u)
    : nickname(u.nickname), tipoMembresia(u.tipoMembresia), ciudad(u.ciudad), pais(u.pais),
    fechaInscripcion(u.fechaInscripcion), contadorReproducciones(u.contadorReproducciones),
    posHistorial(u.posHistorial), numHistorial(u.numHistorial),
    listaFavoritos(u.listaFavoritos),//lama al constructor de copia de ListaFavoritos
    usuarioSeguido(nullptr)//en la carga es qu tomamos la lista del usuario, por eso empieza en nulloo
{
    this->historialReproduccion = new string[MAX_HISTORIAL];//para aseguranos que se copie bien
    UdeATunesDataset::actualizarPeakMemory();
    int contador = 0;
    for(int i = 0; i < MAX_HISTORIAL; ++i) {
        ++contador;
        this->historialReproduccion[i] = u.historialReproduccion[i];
    }
    *UdeATunesDataset::iteraciones += contador;

}
Usuarios& Usuarios::operator=(const Usuarios& u) {
    int contador = 0;
    if (this == &u) {
        return *this;
    }
    nickname = u.nickname;//miembros que todo usuario tien
    tipoMembresia = u.tipoMembresia;
    ciudad = u.ciudad;
    pais = u.pais;
    fechaInscripcion = u.fechaInscripcion;
    contadorReproducciones = u.contadorReproducciones;
    posHistorial = u.posHistorial;
    numHistorial = u.numHistorial;
    this->usuarioSeguido = nullptr;//solo en la carga va apuntar a la lista del usuario que va seguir
    listaFavoritos = u.listaFavoritos;
    delete[] historialReproduccion;
    historialReproduccion = new string[MAX_HISTORIAL];//copia dek historial
    UdeATunesDataset::actualizarPeakMemory();
    for (int i = 0; i < MAX_HISTORIAL; ++i) {
        ++contador;
        historialReproduccion[i] = u.historialReproduccion[i];
    }
    *UdeATunesDataset::iteraciones += contador;
    return *this;
}
Usuarios::~Usuarios() {
    if (historialReproduccion != nullptr) {
        delete[] historialReproduccion;
    }
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
string* Usuarios::getCancionAnterior() {
    if (tipoMembresia == TIPO_STANDARD) {
        cout << "usuarios estandar no pueden retroceder canciones." << endl;
        return nullptr;
    }
    if (numHistorial == 0) {
        cout << "no hay mas canciones anteriores" << endl;
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
    return (tipoMembresia == TIPO_STANDARD &&//validar que solo para el estandar
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
        cout << "solo usuarios Premium pueden seguir listas de otros usuarios Premium." << endl;
        return false;
    }
    if (this == otroUsuario) {
        cout << "no puedes seguir tu propia lista." << endl;
        return false;
    }
    if (this->usuarioSeguido != nullptr) {
        cout << "ya estas siguiendo la lista de '" << this->usuarioSeguido->nickname << "'. debes dejar de seguirlo primero." << std::endl;
        return false;
    }
    this->usuarioSeguido = otroUsuario;//ahora ese puntero va apuntar a la lista del usuario que sigue
    this->listaFavoritos.setListaSeguida(&(otroUsuario->listaFavoritos));
    cout << "El usuario '" << this->nickname << "' ahora sigue la lista de '"<< otroUsuario->nickname << "'." << endl;
    return true;
}
bool Usuarios::cargarFavoritosDesdeString(const std::string& idsCadena) {
    int contador = 0;
    if (idsCadena.empty()) {
        listaFavoritos.establecerCancionesPropias(nullptr, 0);
        return false;
    }
    int numIds = 1;//al menos un ID si no esta vacia
    for (char c : idsCadena) {
        if (c == ',') {
            numIds++;
        }
    }
    string* idsCargados = new string[numIds];
    UdeATunesDataset::actualizarPeakMemory();
    int idx = 0;
    size_t inicio = 0;
    size_t fin = idsCadena.find(',');
    while (fin != string::npos) {
        if (idx < numIds) {
            idsCargados[idx++] = idsCadena.substr(inicio, fin - inicio);
        }
        inicio = fin + 1;
        fin = idsCadena.find(',', inicio);
        ++contador;
    }
    if (idx < numIds) {
        idsCargados[idx++] = idsCadena.substr(inicio);
    }
    numIds = idx;
    listaFavoritos.establecerCancionesPropias(idsCargados, numIds);
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
    cout << "perfil de Usuario"<<endl;
    cout << "reproducciones: " << contadorReproducciones << "\n";
    cout << "Mi lista de favoritos:\n";
    listaFavoritos.mostrarLista(dataset);//muestra delegada si listaSeguida esta activa
    cout << "\nmostrando tambien mi lista propia si sigo otra :\n";
    ListaFavoritos copia = listaFavoritos;
    copia.setListaSeguida(nullptr);//para mostrar tambien la lista propia
    copia.mostrarLista(dataset);
    if (listaFavoritos.getListaSeguida() != nullptr) {
        cout << "\nactualmente estas siguiendo la lista de otro usuario. para dejar de seguir use la opcion correspondiente.\n";
    }
    cout << "-------------------------\n";
}
bool Usuarios::dejarDeSeguir() {
    if (this->usuarioSeguido == nullptr) {
        cout << "no estas siguiendo a ningun usuario. no hay necesidad de nada" << endl;
        return false;
    }
    string nicknameSeguido = this->usuarioSeguido->getNickname();//para ver el mensaje y guardar el nombre
    this->usuarioSeguido = nullptr;//y ya apuntamos a nulo
    listaFavoritos.setListaSeguida(nullptr);
    cout << "has dejado de seguir la lista de '" << nicknameSeguido << "'." << endl;
    return true;
}
string Usuarios::getCancionesPropiasComoString() const {
    int numPropias = listaFavoritos.getNumCancionesPropias();
    const string* idsPropias = listaFavoritos.getCancionesIdsPropias();
    if (numPropias == 0) {
        return "";//sin ninguna cancion
    }
    string resultado = "";
    for (int i = 0; i < numPropias; ++i) {
        if (i > 0) resultado += ",";
        resultado += idsPropias[i];
    }
    return resultado;
}
string Usuarios::getNicknameSeguido() const {
    if (usuarioSeguido != nullptr) {
        return usuarioSeguido->getNickname();
    }
    return "";//no sigue a nadie
}
void Usuarios::establecerSeguimiento(Usuarios* otroUsuario) {
    this->usuarioSeguido = otroUsuario;
    this->listaFavoritos.setListaSeguida(&(otroUsuario->listaFavoritos));
}
