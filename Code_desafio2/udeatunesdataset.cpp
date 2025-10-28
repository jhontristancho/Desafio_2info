#include "UdeATunesDataset.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <random>
#include <ctime>
#include <windows.h>
#include <psapi.h>//este es para la que llame PROCESS_MEMORY_COUNTERS
using namespace std::chrono_literals;
using namespace std;
long* UdeATunesDataset::iteraciones = new long(0);
size_t UdeATunesDataset::peakMemoryUsage = 0;
size_t UdeATunesDataset::getPeakMemoryUsageKB() {
    return peakMemoryUsage;
}
size_t UdeATunesDataset::getMemoryUsageKB() {
    PROCESS_MEMORY_COUNTERS info;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info))) {
        return info.WorkingSetSize / 1024;
    }
    return 0;
}
void UdeATunesDataset::actualizarPeakMemory() {
    size_t actual = getMemoryUsageKB();
    if (actual > peakMemoryUsage) {
        peakMemoryUsage = actual;
    }
}
const int GROW_FACTOR = 2;
const int DATASET_CAPACIDAD_INICIAL = 10;
UdeATunesDataset::UdeATunesDataset()
    : artistas(new Artista[DATASET_CAPACIDAD_INICIAL]),
    numArtistas(0),
    capacidadArtistas(DATASET_CAPACIDAD_INICIAL),
    colaboradores(new Colaborador[DATASET_CAPACIDAD_INICIAL]),
    numColaboradores(0),
    capacidadColaboradores(DATASET_CAPACIDAD_INICIAL),
    usuarios(new Usuarios[DATASET_CAPACIDAD_INICIAL]),
    numUsuarios(0),
    capacidadUsuarios(DATASET_CAPACIDAD_INICIAL),
    publicidades(new Publicidad[10]),
    numPublicidades(0),
    capacidadPublicidades(10),
    ultimaPublicidadMostrada(-1),
    lineasAlbumes(nullptr),
    numLineasAlbumes(0),
    capacidadLineasAlbumes(0),
    lineasCanciones(nullptr),
    numLineasCanciones(0),
    capacidadLineasCanciones(0),
    lineasCreditos(nullptr),
    numLineasCreditos(0),
    capacidadLineasCreditos(0),
    idsColaboradores(new long[DATASET_CAPACIDAD_INICIAL])
{
    srand(static_cast<unsigned>(time(nullptr)));//para lo aleatorio
    actualizarPeakMemory();//ahi pra cuando utilicemos el construcror mide el uso maximo incliyedo todo
}
UdeATunesDataset::~UdeATunesDataset() {
    delete[] artistas;
    delete[] colaboradores;
    delete[] usuarios;
    delete[] publicidades;
    delete[] lineasAlbumes;
    delete[] lineasCanciones;
    delete[] lineasCreditos;
    delete[] idsColaboradores;
    delete iteraciones;
}//en cada resize vamos a ver si se esta incrementando el uso de memori
void UdeATunesDataset::resizeArtistas() {
    int contador = 0;
    int nuevaCapacidad = capacidadArtistas * GROW_FACTOR;
    Artista* nuevo = new Artista[nuevaCapacidad];
    for (int i = 0; i < numArtistas; ++i) {
        ++contador;
        nuevo[i] = artistas[i];
    }
    delete[] artistas;
    artistas = nuevo;
    capacidadArtistas = nuevaCapacidad;
    actualizarPeakMemory();
    *iteraciones += contador;
}
void UdeATunesDataset::resizeColaboradores() {
    int contador = 0;
    int nuevaCapacidad = capacidadColaboradores * GROW_FACTOR;
    Colaborador* nuevo = new Colaborador[nuevaCapacidad];
    for (int i = 0; i < numColaboradores; ++i) {
        ++contador;
        nuevo[i] = colaboradores[i];
    }
    delete[] colaboradores;
    colaboradores = nuevo;
    capacidadColaboradores = nuevaCapacidad;
    actualizarPeakMemory();
    *iteraciones += contador;
}
void UdeATunesDataset::resizeUsuarios() {
    int contador = 0;
    int nuevaCapacidad = capacidadUsuarios * GROW_FACTOR;
    Usuarios* nuevo = new Usuarios[nuevaCapacidad];
    for (int i = 0; i < numUsuarios; ++i) {
        ++contador;
        nuevo[i] = usuarios[i];
    }
    delete[] usuarios;
    usuarios = nuevo;
    capacidadUsuarios = nuevaCapacidad;
    actualizarPeakMemory();
    *iteraciones += contador;
}
void UdeATunesDataset::resizePublicidades() {
    int contador = 0;
    int nuevaCapacidad = capacidadPublicidades * GROW_FACTOR;
    Publicidad* nuevo = new Publicidad[nuevaCapacidad];
    for (int i = 0; i < numPublicidades; ++i) {
        ++contador;
        nuevo[i] = publicidades[i];
    }
    delete[] publicidades;
    publicidades = nuevo;
    capacidadPublicidades = nuevaCapacidad;
    actualizarPeakMemory();
    *iteraciones += contador;
}
void UdeATunesDataset::obtenerSubCampos(const string& campo, char delimitador, string*& resultados, int& numResultados) const {
    int contador = 0;
    delete[] resultados;
    resultados = nullptr;
    numResultados = 0;
    if (campo.empty()) {
        *iteraciones += contador;
        return;
    }
    int count = 1;
    for (char c : campo) {
        ++contador;
        if (c == delimitador) count++;
    }
    resultados = new string[count];
    actualizarPeakMemory();
    size_t start = 0;
    size_t end = campo.find(delimitador);
    while (end != string::npos) {
        ++contador;
        resultados[numResultados++] = campo.substr(start, end - start);
        start = end + 1;
        end = campo.find(delimitador, start);
    }
    resultados[numResultados++] = campo.substr(start);
    *iteraciones += contador;
}
string UdeATunesDataset::obtenerCampo(const string& linea, int campo) const {
    int contador = 0;
    int currentField = 0;
    size_t start = 0;
    size_t end = linea.find(',');

    while (end != string::npos) {
        ++contador;
        if (currentField == campo) {
            *iteraciones += contador;
            return linea.substr(start, end - start);
        }
        start = end + 1;
        end = linea.find(',', start);
        currentField++;
    }
    if (currentField == campo) {
        *iteraciones += contador;
        return linea.substr(start);
    }
    *iteraciones += contador;
    return "";
}
string UdeATunesDataset::buscarLineaPorID(const string* lineas, int numLineas, const string& idBuscado, int campoID) const {
    int contador = 0;
    for (int i = 0; i < numLineas; ++i) {
        ++contador;
        string idActual = obtenerCampo(lineas[i], campoID);
        if (idBuscado == idActual) {
            *iteraciones += contador;
            return lineas[i];
        }
    }
    *iteraciones += contador;
    return "";
}
string* UdeATunesDataset::guardarDinamico(const string& nombreArchivo, int& numLineas, int& capacidadFinal, int capacidadInicial) {    //esto se hace varias veces en el program
    int contador = 0;
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        *iteraciones += contador;
        return nullptr;
    }
    string* lineas = new string[capacidadInicial];
    actualizarPeakMemory();
    string linea;
    numLineas = 0;
    int capacidad = capacidadInicial;
    while (getline(archivo, linea)) {
        ++contador;
        if (!linea.empty()) {
            if (numLineas >= capacidad) {
                capacidad *= GROW_FACTOR;
                string* nuevasLineas = new string[capacidad];
                for (int i = 0; i < numLineas; ++i) {
                    ++contador;
                    nuevasLineas[i] = lineas[i];
                }
                delete[] lineas;
                lineas = nuevasLineas;
                actualizarPeakMemory();//vuelve a medir si se redimenciona
            }
            lineas[numLineas++] = linea;
        }
    }
    archivo.close();
    capacidadFinal = capacidad;
    *iteraciones += contador;
    cout << "***** numero de iteraciones por ahora para guardar en los arreglos dinamicos, cargar archivos, y poder acomodar todo en los arreglos:   " << *UdeATunesDataset::iteraciones << endl;
    cout << "***** memoria usada para guardar los arreglos por ahora: " << UdeATunesDataset::getPeakMemoryUsageKB() << " KB\n";
    return lineas;
}
bool UdeATunesDataset::cargarPublicidades() {
    int contador = 0;
    numPublicidades = 0;
    int numLineasLeidas = 0, capacidadLineas = 0;
    string* lineasPublicidad = guardarDinamico("publicidad.txt", numLineasLeidas, capacidadLineas);
    actualizarPeakMemory();//por la reserva
    if (!lineasPublicidad) {
        cout<<"no se pudo cargar la publicidad, revisar el txt"<<endl;
    }
    for (int i = 0; i < numLineasLeidas; ++i) {
        ++contador;
        string linea = lineasPublicidad[i];
        size_t p1 = linea.find(',');
        if (p1 == string::npos) continue;
        size_t p2 = linea.find(',', p1 + 1);
        if (p2 == string::npos) continue;
        string idStr = linea.substr(0, p1);
        string mensaje = linea.substr(p1 + 1, p2 - p1 - 1);
        string categoriaStr = linea.substr(p2 + 1);
        auto trim = [](string &s) {
            size_t a = s.find_first_not_of(" \t");
            size_t b = s.find_last_not_of(" \t");
            if (a == string::npos) { s = ""; return; }
            s = s.substr(a, b - a + 1);
        };
        trim(idStr); trim(mensaje); trim(categoriaStr);
        if (idStr.empty() || mensaje.empty() || categoriaStr.empty()) continue;
        char categoria = categoriaStr[0];
        int id = 0;
        try { id = stoi(idStr); }
        catch (...) { continue; }
        if (numPublicidades >= capacidadPublicidades) resizePublicidades();
        publicidades[numPublicidades++] = Publicidad(id, mensaje.c_str(), categoria);
    }
    delete[] lineasPublicidad;
    *iteraciones += contador;
    cout << "numero de iteraciones sumadas, pero ahora para poder cargar la publicidad:" << *UdeATunesDataset::iteraciones << endl;
    return true;
}
Publicidad* UdeATunesDataset::obtenerPublicidadAleatoria() {
    int contador = 0;
    if (numPublicidades == 0) {
        *iteraciones += contador;
        return nullptr;
    }
    int pesoTotal = 0;//esto es por la prioridad para ver como se van a mostrar
    for (int i = 0; i < numPublicidades; ++i) {
        ++contador;
        pesoTotal += publicidades[i].getPrioridad();
    }
    if (pesoTotal <= 0) {
        *iteraciones += contador;
        return &publicidades[0];
    }
    int rnd = rand() % pesoTotal;
    int acumulado = 0;
    for (int i = 0; i < numPublicidades; ++i) {
        ++contador;
        acumulado += publicidades[i].getPrioridad();
        if (rnd < acumulado) {
            if (i == ultimaPublicidadMostrada && numPublicidades > 1) {//esto evita que sea consecutiva
                int idx = (i + 1) % numPublicidades;
                ultimaPublicidadMostrada = idx;
                *iteraciones += contador;
                return &publicidades[idx];
            }
            ultimaPublicidadMostrada = i;
            *iteraciones += contador;
            return &publicidades[i];
        }
    }
    *iteraciones += contador;
    cout << "***** ahora sumando con todo lo demas anterior esto fue lo que gaste para poner la publicidad aleatoria " << *UdeATunesDataset::iteraciones << endl;
    return &publicidades[0];
}
bool UdeATunesDataset::cargarArtistas() {
    int contador = 0;
    string* lineasArtistas = guardarDinamico("artistas.txt", numArtistas, capacidadArtistas);
    actualizarPeakMemory();//por la reserva
    if (!lineasArtistas) {
        *iteraciones += contador;
        return false;
    }
    for (int i = 0; i < numArtistas; ++i) {
        ++contador;
        string linea = lineasArtistas[i];
        string idStr = obtenerCampo(linea, 0);//campo ya abes
        string nombre = obtenerCampo(linea, 1);
        artistas[i] = Artista(idStr, nombre);
    }
    delete[] lineasArtistas;
    *iteraciones += contador;
    cout << "sumado a lo anterior, esto costo cargar artistas:" << *UdeATunesDataset::iteraciones << endl;
    return true;
}
bool UdeATunesDataset::cargarColaboradores(){//este metodo recordemos que es pensado a futuro, aunque se carga pero si no se necesita por ahora
    string* lineasColaboradores = guardarDinamico("colaboradores.txt", numColaboradores, capacidadColaboradores);
    if (!lineasColaboradores) {
        return false;
    }
    for (int i = 0; i < numColaboradores; ++i) {
        string linea = lineasColaboradores[i];
        string idStr = obtenerCampo(linea, 0);
        string nombreCompleto = obtenerCampo(linea, 1);
        string rol = obtenerCampo(linea, 2);
        idsColaboradores[i] = stol(idStr);
        colaboradores[i] = Colaborador(nombreCompleto, rol, idStr);
    }
    delete[] lineasColaboradores;
    return true;
}
Colaborador* UdeATunesDataset::encontrarColaborador(const std::string& idColaborador) const{//un poco de lo mismo con el   anterior
    long idNum = 0;
    try {
        idNum = std::stol(idColaborador);
    } catch (...) {
        return nullptr;
    }
    for (int i = 0; i < numColaboradores; ++i) {
        if (idsColaboradores[i] == idNum) {
            return &colaboradores[i];
        }
    }
    return nullptr;
}
bool UdeATunesDataset::procesarCreditosParaCancion(Cancion* cancion, const string& idCreditoStr){//recordemos lo mismo esto pr ahora no se usa
    if (!cancion) {
        return false;
    }
    string lineaCredito = buscarLineaPorID(lineasCreditos, numLineasCreditos, idCreditoStr, 0);
    if (lineaCredito.empty()) {
        return false;
    }
    Creditos creditosCancion;
    auto procesarColaboradores = [&](int campo, char delimitador) {
        string campoData = obtenerCampo(lineaCredito, campo);
        if (campoData.empty()) return;
        string* ids = nullptr;
        int numIds = 0;
        obtenerSubCampos(campoData, delimitador, ids, numIds);
        if (numIds > 0) {
            Colaborador* arr = new Colaborador[numIds];
            int validCount = 0;
            for (int i = 0; i < numIds; ++i) {
                Colaborador* c = encontrarColaborador(ids[i]);
                if (c) arr[validCount++] = *c;
            }
            if (campo == 1) creditosCancion.setProductores(arr, validCount);
            else if (campo == 2) creditosCancion.setMusicos(arr, validCount);
            else if (campo == 3) creditosCancion.setCompositores(arr, validCount);
            delete[] arr;
        }
        delete[] ids;
    };
    procesarColaboradores(1, '#'); // productores
    procesarColaboradores(2, ','); // musicos
    procesarColaboradores(3, ','); // compositores
    cancion->getCreditos() = creditosCancion;
    return true;
}
float UdeATunesDataset::parseDuracion(const string& duracionStr) const {//para la duracion de la cancion
    int contador = 0;
    size_t pos = duracionStr.find(':');
    if (pos == string::npos) {
        *iteraciones += contador;
        return 0.0f;
    }
    try {
        int minutos = stoi(duracionStr.substr(0, pos));
        int segundos = stoi(duracionStr.substr(pos + 1));
        *iteraciones += contador;
        return minutos + (segundos / 60.0f);
    } catch (...) {
        *iteraciones += contador;
        return 0.0f;
    }
}
bool UdeATunesDataset::procesarCancionesParaAlbum(Album& album, Artista* artista, const string& idAlbumCompleto) {
    int contador = 0;
    if (!artista) {
        *iteraciones += contador;
        return false;
    }
    bool tieneCanciones = false;
    for (int j = 0; j < numLineasCanciones; ++j) {
        ++contador;
        string lineaCancion = lineasCanciones[j];
        string idCancionCompleto = obtenerCampo(lineaCancion, 0);
        if (idCancionCompleto.size() >= 7 && idCancionCompleto.substr(0, 7) == idAlbumCompleto) {
            try {
                string nombreCancion = obtenerCampo(lineaCancion, 1);
                string duracionStr = obtenerCampo(lineaCancion, 2);
                float duracion = parseDuracion(duracionStr);
                string ruta128 = obtenerCampo(lineaCancion, 3);
                string ruta320 = obtenerCampo(lineaCancion, 4);
                string idCreditoStr = obtenerCampo(lineaCancion, 6);
                Cancion* nuevaCancion = new Cancion(idCancionCompleto, nombreCancion, duracion, ruta128, ruta320);
                actualizarPeakMemory();
                nuevaCancion->setAlbum(&album);
                nuevaCancion->setArtista(artista);
                if (!idCreditoStr.empty()) procesarCreditosParaCancion(nuevaCancion, idCreditoStr);
                if (album.agregarCancion(nuevaCancion)) {
                    tieneCanciones = true;
                } else {
                    delete nuevaCancion;
                }
            } catch (...) {
                continue;
            }
        }
    }
    *iteraciones += contador;
    cout << "***** esto nos cuesta en iteraciones poder relacionar las canciones con los albumnes: " << *UdeATunesDataset::iteraciones << endl;
    cout << "***** memoria usada para relacionar canciones con albumnes: " << UdeATunesDataset::getPeakMemoryUsageKB() << " KB\n";
    return tieneCanciones;
}
Artista* UdeATunesDataset::getArtista(const std::string& id) const {
    int contador = 0;
    for (int i = 0; i < numArtistas; ++i) {
        ++contador;
        if (artistas[i].getIdArtista() == id) {
            *iteraciones += contador;
            return &artistas[i];
        }
    }
    *iteraciones += contador;
    return nullptr;
}
bool UdeATunesDataset::procesarAlbum(const string& lineaAlbum, int indice) {
    int contador = 0;
    if (lineaAlbum.empty()) {
        *iteraciones += contador;
        return false;
    }
    string idAlbumCompleto = obtenerCampo(lineaAlbum, 0);
    string idArtistaStr = idAlbumCompleto.substr(0, 5);
    Artista* artista = getArtista(idArtistaStr);
    if (!artista) {
        *iteraciones += contador;
        return false;
    }
    string nombreAlbum = obtenerCampo(lineaAlbum, 1);
    string anio = obtenerCampo(lineaAlbum, 2);
    string rutaPortada = obtenerCampo(lineaAlbum, 3);

    Album* nuevoAlbum = new Album(idAlbumCompleto, nombreAlbum, anio, rutaPortada);
    actualizarPeakMemory();//medir despues de hacer la reserva para el album
    nuevoAlbum->setArtista(artista);
    if (procesarCancionesParaAlbum(*nuevoAlbum, artista, idAlbumCompleto)) {
        bool agregado = artista->agregarAlbum(nuevoAlbum);
        if (!agregado) delete nuevoAlbum;
        *iteraciones += contador;
        return agregado;
    } else {
        delete nuevoAlbum;
        *iteraciones += contador;
        cout << "***** esto nos cuesta en iteraciones poder procesar los album: " << *UdeATunesDataset::iteraciones << endl;
        cout << "***** memoria usada para poder procesar los albumnes : " << UdeATunesDataset::getPeakMemoryUsageKB() << " KB\n";
        return false;
    }
}
bool UdeATunesDataset::cargarAlbumesYCanciones() {
    int contador = 0;
    lineasAlbumes = guardarDinamico("albumes.txt", numLineasAlbumes, capacidadLineasAlbumes);
    lineasCanciones = guardarDinamico("canciones.txt", numLineasCanciones, capacidadLineasCanciones);
    actualizarPeakMemory();//medir la memoria para ver con la reserva
    lineasCreditos = guardarDinamico("creditos.txt", numLineasCreditos, capacidadLineasCreditos);//acordemonos que creditos no se va tener en cuenta
    if (!lineasAlbumes || !lineasCanciones) {
        *iteraciones += contador;
        return false;
    }
    int albumesCargados = 0;
    for (int i = 0; i < numLineasAlbumes; ++i) {
        ++contador;
        if (procesarAlbum(lineasAlbumes[i], i)) albumesCargados++;
    }
    *iteraciones += contador;
    cout << "***** esto nos cuesta en iteraciones cargar album: " << *UdeATunesDataset::iteraciones << endl;
    cout << "***** memoria usada para poder guardar los albumnes : " << UdeATunesDataset::getPeakMemoryUsageKB() << " KB\n";
    return albumesCargados > 0;
}
Cancion* UdeATunesDataset::buscarCancion(const std::string& id) const {//busca la cancion por id
    int contador = 0;
    string lineaCancion = buscarLineaPorID(lineasCanciones, numLineasCanciones, id, 0);
    if (lineaCancion.empty()) {
        *iteraciones += contador;
        return nullptr;
    }
    string nom = obtenerCampo(lineaCancion, 1);
    string durStr = obtenerCampo(lineaCancion, 2);
    string r128 = obtenerCampo(lineaCancion, 3);
    string r320 = obtenerCampo(lineaCancion, 4);
    float duracion = parseDuracion(durStr);
    Cancion* c = new Cancion(id, nom, duracion, r128, r320);
    actualizarPeakMemory();//mide a ver cuanto se le asigna a cancion
    *iteraciones += contador;
    cout << "***** esto nos cuesta en iteraciones poder buscar una cancion y guardarla: " << *UdeATunesDataset::iteraciones << endl;
    cout << "***** memoria usada para poder guardar una cancion : " << UdeATunesDataset::getPeakMemoryUsageKB() << " KB\n";
    return c;
}
bool UdeATunesDataset::cargarUsuarios() {
    int contador = 0;
    int numLineasLeidas = 0, capacidadLineas = 0;
    string* lineasUsuarios = guardarDinamico("usuarios.txt", numLineasLeidas, capacidadLineas);
    actualizarPeakMemory();
    if (!lineasUsuarios || numLineasLeidas == 0) {
        *iteraciones += contador;
        return false;
    }
    for (int i = 0; i < numLineasLeidas; ++i) {
        ++contador;
        if (numUsuarios >= capacidadUsuarios) resizeUsuarios();
        string linea = lineasUsuarios[i];
        try {
            string nick   = obtenerCampo(linea, 0);
            string tipoS  = obtenerCampo(linea, 1);
            string ciudad = obtenerCampo(linea, 2);
            string pais   = obtenerCampo(linea, 3);
            string fecha  = obtenerCampo(linea, 4);
            int tipo = stoi(tipoS);
            usuarios[numUsuarios] = Usuarios(nick, tipo, ciudad, pais, fecha);
            numUsuarios++;
        } catch (...) {
            continue;
        }
    }
    delete[] lineasUsuarios;
    *iteraciones += contador;
    cout << "***** esto nos cuesta en iteraciones poder cargar los usuarios: " << *UdeATunesDataset::iteraciones << endl;
    cout << "***** memoria usada para poder cargar los usuarios : " << UdeATunesDataset::getPeakMemoryUsageKB() << " KB\n";
    return numUsuarios > 0;
}
bool UdeATunesDataset::cargarListasDeFavoritos(const string& rutaArchivo2) {
    ifstream archivo(rutaArchivo2);
    if (!archivo.is_open()) {
        cout << "no hay archivo de favoritos"<<endl;
        return false;
    }
    string linea;//y aca para cargar las canciones propias de cada usuario
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        size_t primerDosPuntos = linea.find(':');
        if (primerDosPuntos == string::npos) continue;
 string nickname = linea.substr(0, primerDosPuntos);
        size_t segundoDosPuntos = linea.find(':', primerDosPuntos + 1);
        if (segundoDosPuntos == string::npos) continue;
        string cancionesPropias = linea.substr(
            primerDosPuntos + 1,
            segundoDosPuntos - primerDosPuntos - 1
            );
        std::string nicknameSeguido = linea.substr(segundoDosPuntos + 1);
        Usuarios* usuario = buscarUsuario(nickname);//y aca busccamos al otro usaurio
        if (usuario == nullptr || usuario->getTipoMembresia() != 1) {
            continue;
        }
        usuario->cargarFavoritosDesdeString(cancionesPropias);
    }
    archivo.close();
    archivo.open(rutaArchivo2);//y aca se establecen las conexiones de seguir
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        size_t primerDosPuntos = linea.find(':');
        if (primerDosPuntos == string::npos) continue;
  string nickname = linea.substr(0, primerDosPuntos);
        size_t segundoDosPuntos = linea.find(':', primerDosPuntos + 1);
        if (segundoDosPuntos == string::npos) continue;
       string nicknameSeguido = linea.substr(segundoDosPuntos + 1);
        if (nicknameSeguido.empty()) {
            continue; //por si no sigue a nadi
        }
        Usuarios* usuario = buscarUsuario(nickname);//y se establece el seguimiento
        Usuarios* seguido = buscarUsuario(nicknameSeguido);
        if (usuario && seguido && usuario != seguido) {
            usuario->establecerSeguimiento(seguido);
        }
    }
    archivo.close();
    return true;
}
bool UdeATunesDataset::guardarListasDeFavoritos(const string& rutaArchivo2) {
    ofstream archivo(rutaArchivo2);
    if (!archivo.is_open()) {
        cout << "no se pudo abrir el archivo para guardar la lista de favoritos"
             << rutaArchivo2 << std::endl;
        return false;
    }
    for (int i = 0; i < numUsuarios; ++i) {
        Usuarios* usuario = &usuarios[i];
        if (usuario->getTipoMembresia() != 1) {//por si se llega a infliltar un estandar jajaj
            continue;
        }
        archivo << usuario->getNickname() << ":";//id1,id2,id3:nicknameSeguido
        archivo << usuario->getCancionesPropiasComoString() << ":";
        archivo << usuario->getNicknameSeguido();//por si no sigue a nadie
        archivo << endl;
    }
    archivo.close();
    return true;
}
Usuarios* UdeATunesDataset::buscarUsuario(const std::string& nickname) {
    int contador = 0;
    for (int i = 0; i < numUsuarios; ++i) {
        ++contador;
        if (usuarios[i].getNickname() == nickname) {
            *iteraciones += contador;
            return &usuarios[i];
        }
    }
    *iteraciones += contador;
    return nullptr;
}
void UdeATunesDataset::mostrarReproduccion(const std::string& idCancion, Usuarios* usuario, bool mostrarPublicidad) {
    int contador = 0;
    string lineaCancion = buscarLineaPorID(lineasCanciones, numLineasCanciones, idCancion, 0);
    if (lineaCancion.empty()) {
        cout << "no se encontro la cancion  " << idCancion << endl;
        *iteraciones += contador;
        return;
    }
    string nombreCancion = obtenerCampo(lineaCancion, 1);
    string duracionStr = obtenerCampo(lineaCancion, 2);
    string rutaArchivo = obtenerCampo(lineaCancion, 3);
    string rutaArchivo2 = obtenerCampo(lineaCancion, 4);
    string idAlbumCompleto = idCancion.size() >= 7 ? idCancion.substr(0, 7) : "";
    string idArtistaStr = idCancion.size() >= 5 ? idCancion.substr(0, 5) : "";
    Artista* artista = getArtista(idArtistaStr);
    string rutaReproduccion;
    if (usuario && usuario->getTipoMembresia() == 1) {
        rutaReproduccion = rutaArchivo2;//la de alta calidad
    } else {
        rutaReproduccion = rutaArchivo;
    }
    string nombreArtista = (artista ? artista->getNombre() : "Desconocido");
    string nombreAlbum = "Desconocido";
    string rutaPortada = "Desconocida";
    if (artista) {
        for (int j = 0; j < artista->getNumAlbumes(); ++j) {
            ++contador;
            Album* a = artista->getAlbumAt(j);
            if (a && a->getIdAlbum() == idAlbumCompleto) {
                nombreAlbum = a->getNombre();
                rutaPortada = a->getPortadaRuta();
                break;
            }
        }
    }
    if (mostrarPublicidad && usuario && usuario->debeMostrarPublicidad()){
        Publicidad* pub = obtenerPublicidadAleatoria();
        if (pub) {
            cout << "\nPUBLICIDAD:     " << endl;
            cout << pub->getMensaje() << endl;
            cout<< "prioridad de la publicidad: "<<pub->getPrioridad()<<endl;
        }
    }
    *iteraciones += contador;
    cout << "\nReproducciendo la cancion: :" << endl;
    cout << " Artista : " << nombreArtista << endl;
    cout << " album   : " << nombreAlbum << endl;
    cout << " ruta de la portada del album  : " << rutaPortada << endl;
    cout << " cancion : " << nombreCancion << endl;
    cout << " ruta de la cancion : " << rutaReproduccion << " (" << (usuario && usuario->getTipoMembresia() == 1 ? "premium" : "estandar") << ")" << endl;
    cout << " Duracion : " << duracionStr << endl;
    cout << "***** memoria usada para poder mostrar lo que se reproduce: " << UdeATunesDataset::getPeakMemoryUsageKB() << " KB\n";
    cout << "***** esto nos cuesta en iteraciones poder mostrar lo que se reproduce: " << *UdeATunesDataset::iteraciones << endl;
    cout << "--------------------------------------" << endl;
}
Cancion* UdeATunesDataset::reproducirCancionAleatoria(Usuarios* usuario){//esto retorna la cancion que se va reproduccir
    int contador = 0;
    if (numLineasCanciones == 0) {
        cout << "no hay canciones cargadas." << endl;
        *iteraciones += contador;
        return nullptr;
    }
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, numLineasCanciones - 1);
    int idx = distrib(gen);
    string lineaCancion = lineasCanciones[idx];
    string idCancion = obtenerCampo(lineaCancion, 0);
    string nombreCancion = obtenerCampo(lineaCancion, 1);
    string duracionStr = obtenerCampo(lineaCancion, 2);
    string ruta128 = obtenerCampo(lineaCancion, 3);
    mostrarReproduccion(idCancion, usuario, true);
    *iteraciones += contador;
    actualizarPeakMemory();
    return new Cancion(idCancion, nombreCancion, parseDuracion(duracionStr), ruta128, "");
}
void UdeATunesDataset::reproducirListaFavoritos(Usuarios* usuario, bool aleatoria) {
    int contador = 0;
    ListaFavoritos* lista = &usuario->getListaFavoritos();
    int totalCanciones = 0;
    std::string* listaTotalIds = lista->getListaCompletaDeIds(totalCanciones);//este metodo las unia
    if (totalCanciones == 0 || listaTotalIds == nullptr) {
        cout << "La lista de favoritos esta vacia, agregue canciones." << endl;
        *iteraciones += contador;
        return;
    }
    cout << "\nreproduciendo " << totalCanciones << " canciones de la lista de favoritos ";
    cout << (aleatoria ? "en modo aleatorio." : "en orden.") << endl;
    int* ordenReproduccion = new int[totalCanciones];
    for (int i = 0; i < totalCanciones; ++i) {
        ordenReproduccion[i] = i;
    }
    if (aleatoria) {
        for (int i = totalCanciones - 1; i > 0; --i) {
            ++contador;
            int j = rand() % (i + 1);//por si es aleatoria
            std::swap(ordenReproduccion[i], ordenReproduccion[j]);
        }
    }
    bool reproduciendo = true;
    int indiceLista = 0;
    const int M = 6;
    string historial[M];//para la anterior
    int historialSize = 0;
    while (reproduciendo && indiceLista < totalCanciones) {
        ++contador;
        string id = listaTotalIds[ordenReproduccion[indiceLista]];
        mostrarReproduccion(id, usuario, false);
        if (historialSize < M) historial[historialSize++] = id;
        else { // si el historial esta lleno, desplazar
            for (int i = 0; i < M - 1; ++i) historial[i] = historial[i + 1];
            historial[M - 1] = id;
        }
        cout << endl << "elije : 1) siguiente  2) anterior  3) detener" << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));//ahi si le permitimos al que este esuchando que se dmore lo que quiera
        int op = 0;
        cout << "ingrese opcion: ";
        if (!(std::cin >> op)) {//por si no ingresa nada en ese tiempo
            std::cin.clear();
            string tmp;
            getline(std::cin, tmp);
            op = 1;//este por defecto porque si
        }
        switch (op) {
        case 1:
            indiceLista++;
            break;
        case 2: {
            if (historialSize <= 1) {
                cout << "no hay cancion anterior en el historial. ya andas en la primera." << endl;
            } else {
                string idAnterior = historial[historialSize - 2];
                historialSize--;
                mostrarReproduccion(idAnterior, usuario, false);
                cout << "volviendo a la cancion anterior que habias escuchado, estuvo buena" << endl;
                indiceLista--;
                if (indiceLista < 0) {
                    indiceLista = 0;//esto nos ayuda para el error que estaba accediendo a negativo
                }
            }
            break;
        }
        case 3://Detener
            reproduciendo = false;
            break;
        default:
            cout << "opcion invalida. reproduzco la siguiente cancion." << endl;
            indiceLista++;
            break;
        }
    }
    cout << "\nreproduccion de la lista de favoritos finalizada." << endl;
    delete[] listaTotalIds;
    delete[] ordenReproduccion;
    *iteraciones += contador;
    cout << "***** memoria usada para reproduccir lista de favoritos: " << UdeATunesDataset::getPeakMemoryUsageKB() << " KB\n";
    cout << "***** esto nos cuesta en iteraciones poder reproducir lista de favoritos: " << *UdeATunesDataset::iteraciones << endl;
}
void UdeATunesDataset::menuFavoritosPremium(Usuarios* usuario) {
    int contador = 0;
    const std::string RUTA_FAVORITOS = "listadefavoritos.txt";
    if (usuario->getTipoMembresia() != 1) {
        cout << "opcion solo para premium" << endl;
        *iteraciones += contador;
        return;
    }
    bool continuar = true;
    while (continuar) {
        ++contador;
        cout << "\n=== menu para usted premium ===" << endl;
        cout << "1) mostrar mi lista" << endl;
        cout << "2) agregar cancion" << endl;
        cout << "3) eliminar cancion" << endl;
        cout << "4) seguir a otro usuario" << endl;
        cout << "5) dejar de seguir usuario" << endl;
        cout << "6) reproducir mi lista" << endl;
        cout << "7) salir" << endl;
        int opcion;
        if (!(cin >> opcion)) {
            cin.clear();
            string tmp;
            getline(cin, tmp);
            continue;
        }
        switch (opcion) {
        case 1:
            usuario->getListaFavoritos().mostrarLista(this);
            break;
        case 2: {
            string id;
            cout << "id de la cancion: ";
            cin >> id;
            if (usuario->getListaFavoritos().agregarCancion(id)) {
                if (guardarListasDeFavoritos(RUTA_FAVORITOS)) {
                    cout << "cancion agregada. cambios guardados en el archivo." << endl;
                } else {
                    cout << "la cancion se agrego, pero no se pudo guardar el archivo.intenta ahora despues" << endl;
                }
            } else {
                cout << "o esa cancion ya la agregaste o la digitaste mal senor usuario" << endl;
            }
            break;
        }
        case 3: {
            string id;
            cout << "id de la cancion: ";
            cin >> id;
            if (usuario->getListaFavoritos().eliminarCancion(id)) {
                if (guardarListasDeFavoritos(RUTA_FAVORITOS)) {
                    cout << "se borro efectivo la cancion de su lista" << endl;
                } else {
                    cout << "se borro" << endl;
                }
            } else {
                cout << "la cancion no estaba en su lista, mire si digito bien" << endl;
            }
            break;
        }
        case 4: {
            string otroNick;
            cout << "diganos el nombre del usuario que quiere seguir: ";
            cin >> otroNick;
            Usuarios* otro = buscarUsuario(otroNick);
            if (otro) {
                if (usuario->seguirUsuario(otro)) {
                    if (guardarListasDeFavoritos(RUTA_FAVORITOS)) {
                        cout << "ahora estas siguiendo a " << otroNick << ". y ya lo actualizamos en el sistema" << endl;
                    } else {
                        cout << "lo estas siguiendo por ahora, pero no lo pudimos actualizar en el sistema" << endl;
                    }
                } else {
                    cout << "no se pudo iniciar el seguimiento. revisa si ya sigues a alguien." << endl;
                }
            } else {
                cout << "no encontramos ese usuario, revisa si escribiste bien." << endl;
            }
            break;
        }
        case 5: {
            if (usuario->dejarDeSeguir()) {
                if (guardarListasDeFavoritos(RUTA_FAVORITOS)) {
                    cout << "dejaste de seguir al usuario y lo guardamos en el sistema" << endl;
                } else {
                    cout << "dejate de seguir al usuario" << endl;
                }
            } else {
                cout << "no estas siguiendo a ningun usuario." << endl;
            }
            break;
        }
        case 6: {
            int a;
            cout << "reproduccion aleatoria, 1 para si 0 para no "<<endl;
            cin >> a;
            reproducirListaFavoritos(usuario, a == 1);
            break;
        }
        case 7:
            continuar = false;
            break;
        default:
            cout << "opcion mala, solo las que aparecen" << endl;
        }
    }
    *iteraciones += contador;
}
void UdeATunesDataset::iniciarSesionYReproducir() {
    int contador = 0;
    cout <<endl << "bienvenido a la mejor plataforma del mundo para esuchar musica" << endl;
    cout<< "usuario recuerde que en nombre de su creador, jhon tristancho le quiero decir que espero que se sienta a gusto"<<endl;
    cout << "ingrese su nickname: ";
    string nickname;
    cin >> nickname;
    Usuarios* usuario = nullptr;
    for (int i = 0; i < numUsuarios; ++i) {
        ++contador;
        if (usuarios[i].getNickname() == nickname) { usuario = &usuarios[i]; break; }
    }
    if (!usuario) {
        cout << "no encontramos su usuario, intente de nuevo" << endl;
        *iteraciones += contador;
        return;
    }
    cout << "sesion iniciada: " << usuario->getNickname()
         << " - " << (usuario->getTipoMembresia() == 0 ? "estandar" : "premium") << endl;
    cout << "desea reproducir canciones aleatoriamente? presione s para si o n para no, si presiona otra cosa se asume que no: ";
    char respuesta; cin >> respuesta;
    if (respuesta != 's' && respuesta != 'S') {
        cout << "se cancelo la reproduccion" << endl;
        *iteraciones += contador;
        return;
    }
    const int K = 5;//canciones a reproduccir principalmente
    if (usuario->getTipoMembresia() == 0) {
        int contadorPublicidad = 0;
        int ultimaPublicidadID = -1;
        Cancion* ultimaCancion = nullptr;
        for (int i = 0; i < K; ++i) {
            ++contador;
            if (contadorPublicidad == 2) {
                contadorPublicidad = 0;
                Publicidad* pub = obtenerPublicidadAleatoria();
                if (pub && pub->getId() != ultimaPublicidadID) {
                    cout << "\nPUBLICIDAD: " << pub->getMensaje() <<endl<< " con una prioridad de "<< pub->getPrioridad()<< endl;
                    ultimaPublicidadID = pub->getId();
                }
            }
            Cancion* c = reproducirCancionAleatoria(usuario);
            if (!c) break;
            if (ultimaCancion) delete ultimaCancion;
            ultimaCancion = c;
            contadorPublicidad++;
            this_thread::sleep_for(chrono::seconds(3));
        }
        cout << "\nestas escuchando: " << (ultimaCancion ? ultimaCancion->getNombre() : "ninguna") << endl;
        bool reproduciendo = true;
        while (reproduciendo) {
            ++contador;
            cout << "desea detener la reproduccion, s si la quiere detener aclaro que se acaba la reproduccion o n si no quiere y quiere seguir escuchando musica: ";
            char op; cin >> op;
            if (op == 's' || op == 'S') {
                cout << "reproduccion detenida." << endl;
                reproduciendo = false;
            }
            else if (op == 'n' || op == 'N') {
                if (contadorPublicidad == 2) {
                    contadorPublicidad = 0;//se reinicia el contador de la publicidad, ya que se debe ir mostrando la publicidad
                    Publicidad* pub = obtenerPublicidadAleatoria();
                    if (pub && pub->getId() != ultimaPublicidadID) {
                        cout << "\nPUBLICIDAD: " << pub->getMensaje() <<endl<< " con una prioridad de "<< pub->getPrioridad()<< endl;
                        ultimaPublicidadID = pub->getId();
                    }
                }
                if (ultimaCancion) delete ultimaCancion;
                Cancion* nuevaCancion = reproducirCancionAleatoria(usuario);//y se reproduce la cancion aleatorria
                if (nuevaCancion) {
                    ultimaCancion = nuevaCancion;
                    contadorPublicidad++;
                    cout << "continua la reproduccion con una cancion nueva..." << endl;
                    this_thread::sleep_for(std::chrono::seconds(3));
                } else {
                    cout << "no hay mas canciones disponibles. pasate a premium solo por  19900 pesos mensuales y disfruta del todo el catalogo." << endl;
                    reproduciendo = false;
                }
            }
            else {
                cout << "opcion invalida." << endl;
            }
        }
        delete ultimaCancion;
        *iteraciones += contador;
        return;
    }
    //para los premium
    bool repetir = false;
    const int MAX_HIST = 4;
    string historial[MAX_HIST];
    int histCount = 0;
    Cancion* actual = nullptr;
    for (int i = 0; i < K; ++i) {
        ++contador;
        if (actual) {
            if (histCount < MAX_HIST) {
                historial[histCount++] = actual->getIdCompleto();
            } else {
                for (int j = 0; j < MAX_HIST - 1; ++j) {
                    historial[j] = historial[j + 1];
                }
                historial[MAX_HIST - 1] = actual->getIdCompleto();
            }
        }
        delete actual;
        actual = reproducirCancionAleatoria(usuario);
        if (!actual) break;
        this_thread::sleep_for(chrono::seconds(3));
    }
    if (actual) cout << "\nreproduccion automatica completada. actualmente escuchas: " << actual->getNombre() << endl;
    bool enMenu = true;
    while (enMenu) {
        ++contador;
        cout << "\nopciones premium:"<<endl;
        cout << "1) Siguiente"<<endl;
        cout << "2) Previa"<<endl;
        cout << "3) Activar o Desactivar repetir\n";
        cout << "4) menu de favoritos\n";
        cout << "5) Detener\n";
        cout << "6) Salir\n";
        int opcion; if (!(cin >> opcion)) { cin.clear(); string tmp; getline(cin, tmp); continue; }
        switch (opcion) {
        case 1: { // Siguiente
            if (actual) {
                if (histCount < MAX_HIST) {
                    historial[histCount++] = actual->getIdCompleto();
                } else {
                    for (int j = 0; j < MAX_HIST - 1; ++j) {
                        historial[j] = historial[j + 1];
                    }
                    historial[MAX_HIST - 1] = actual->getIdCompleto();
                }
            }
            delete actual;
            actual = reproducirCancionAleatoria(usuario);
            if (actual) this_thread::sleep_for(chrono::seconds(3));
            break;
        }
        case 2: { // Previa
            if (histCount == 0) {
                cout << "no hay cancion previa." << endl;
            } else {
                string idAnterior = historial[histCount - 1];
                delete actual;
                actual = buscarCancion(idAnterior);
                if (actual) {
                    mostrarReproduccion(idAnterior, usuario, false);
                    std::this_thread::sleep_for(std::chrono::seconds(3));
                } else {
                    cout << "no se encontro la cancion previa." << endl;
                }
                histCount--;
            }
            break;
        }
        case 3:
            repetir = !repetir;
            cout << (repetir ? "repetir activado." : "repetir desactivado.") << endl;
            if (repetir && actual) { mostrarReproduccion(actual->getIdCompleto(), usuario, false); std::this_thread::sleep_for(std::chrono::seconds(3)); }
            break;
        case 4:
            menuFavoritosPremium(usuario);
            break;
        case 5:
            cout << "reproduccion detenida." << endl;
            delete actual; actual = nullptr;
            break;
        case 6:
            enMenu = false;
            break;
        default:
            cout << "opcion invalida." << endl;
        }
    }
    if (usuario->getUsuarioSeguido() != nullptr) {
    }
    if (actual) delete actual; // Limpieza final
    *iteraciones += contador;
}
bool UdeATunesDataset::cargarDatos() {
    bool exito = true;
    if (!cargarPublicidades()) exito = false;
    if (!cargarArtistas()) exito = false;
    if (!cargarColaboradores()) exito = false;
    if (!cargarAlbumesYCanciones()) exito = false;
    if (!cargarUsuarios()) exito = false;
    cargarListasDeFavoritos("listadefavoritos.txt");
    actualizarPeakMemory();
    return exito;
}
