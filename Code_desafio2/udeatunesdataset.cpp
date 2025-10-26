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
#include <psapi.h>//para PROCESS_MEMORY_COUNTERS
using namespace std::chrono_literals;
using namespace std;
long* UdeATunesDataset::iteraciones = new long(0);
const int GROW_FACTOR = 2;
const int DATASET_CAPACIDAD_INICIAL = 10;
static size_t peakMemoryUsage = 0;
size_t getMemoryUsageKB() {
    PROCESS_MEMORY_COUNTERS info;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info))) {
        // WorkingSetSize es la memoria física (RAM) que usa el proceso.
        return info.WorkingSetSize / 1024;
    }
    return 0;
}
void actualizarPeakMemory() {
    size_t actual = getMemoryUsageKB();
    if (actual > peakMemoryUsage) {
        peakMemoryUsage = actual;
    }
}
size_t UdeATunesDataset::getPeakMemoryUsageKB() {
    return peakMemoryUsage;
}
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
    srand(static_cast<unsigned>(time(nullptr)));
    actualizarPeakMemory(); // Mide uso inicial después de las asignaciones del constructor.
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
}
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
    actualizarPeakMemory(); // Llama después de la nueva asignación de memoria
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
    actualizarPeakMemory();//Desps de la asignacin del array de resultados
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
string* UdeATunesDataset::guardarDinamico(const string& nombreArchivo, int& numLineas, int& capacidadFinal, int capacidadInicial) {
    int contador = 0;
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        *iteraciones += contador;
        return nullptr;
    }
    string* lineas = new string[capacidadInicial];
    actualizarPeakMemory();//se mide luego de la asignacion y asi pues tengo lo que en verdad se esta consumiendo
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
    //esto se hace varias veces en el program
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
            // evitar consecutivo
            if (i == ultimaPublicidadMostrada && numPublicidades > 1) {
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
float UdeATunesDataset::parseDuracion(const string& duracionStr) const {
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
        string idStr = obtenerCampo(linea, 0);
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
                actualizarPeakMemory();//mide despues de la reserva
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
// Asumiremos que has añadido 'string nicknameSeguidoTemp' a Usuarios.h para este fin.

bool UdeATunesDataset::cargarListasDeFavoritos(const std::string& rutaArchivo) {
    int contador = 0;

    // =======================================================
    // 💡 PASO 0: Usar guardarDinamico para cargar las líneas
    // =======================================================
    std::string* lineas = nullptr; // Array dinámico de líneas
    int numLineas = 0;             // Número de líneas cargadas
    int capacidadFinal = 0;        // Variable no usada aquí, pero requerida por guardarDinamico

    // Llama a tu función de carga, que asigna memoria para 'lineas'
    // y llena 'numLineas'. Asumo una capacidad inicial razonable (ej: 10).
    lineas = guardarDinamico(rutaArchivo, numLineas, capacidadFinal, 10);

    if (lineas == nullptr || numLineas == 0) {
        // La función guardarDinamico ya maneja el contador si falla al abrir
        if (*iteraciones == 0) {
            *iteraciones += contador;
        }
        std::cerr << "Advertencia: No se pudo cargar el archivo de favoritos o está vacío.\n";
        return false;
    }
    // =======================================================

    // --- PASO 1: Cargar canciones PROPIAS y el nicknameSeguidoTemp ---
    for (int i = 0; i < numLineas; ++i) {
        ++contador;
        std::string linea = lineas[i];

        // El nuevo formato esperado es: nickname:ids_propios:nickname_seguido
        size_t pos1 = linea.find(':');
        size_t pos2 = linea.find(':', pos1 + 1);

        if (pos1 == std::string::npos) continue;

        std::string nickname = linea.substr(0, pos1);
        std::string idsCadena = "";
        std::string seguidoNick = "";

        if (pos2 != std::string::npos) {
            // Formato completo: user:id1,id2:seguido
            idsCadena = linea.substr(pos1 + 1, pos2 - pos1 - 1);
            seguidoNick = linea.substr(pos2 + 1);
        } else {
            // Formato antiguo o solo canciones: user:id1,id2
            idsCadena = linea.substr(pos1 + 1);
        }

        Usuarios* u = buscarUsuario(nickname);
        if (u) {
            u->cargarFavoritosDesdeString(idsCadena);
            if (!seguidoNick.empty()) {
                // Almacenar temporalmente el nickname del seguido
                u->setNicknameSeguidoTemp(seguidoNick);
            }
        }
    }

    // --- PASO 2: Resolver los seguimientos (El "Doble Pase") ---
    for (int i = 0; i < numUsuarios; ++i) {
        ++contador;
        Usuarios& u = usuarios[i];

        // Se asume que getNicknameSeguidoTemp y clearNicknameSeguidoTemp existen en Usuarios
        if (!u.getNicknameSeguidoTemp().empty()) {
            Usuarios* seguido = buscarUsuario(u.getNicknameSeguidoTemp());
            if (seguido) {
                // Usamos seguirUsuario para establecer los punteros y la delegación de la lista
                u.seguirUsuario(seguido);
            }
            // Limpiar la variable temporal en cualquier caso
            u.clearNicknameSeguidoTemp();
        }
    }
    if (lineas) {
        delete[] lineas;
    }
    *iteraciones += contador;
    return true;
}
bool UdeATunesDataset::guardarListasDeFavoritos(const std::string& rutaArchivo2) const {
    int contador = 0;
    std::ofstream fs(rutaArchivo2);
    if (!fs.is_open()) {
        *iteraciones += contador;
        return false;
    }

    for (int i = 0; i < numUsuarios; ++i) {
        ++contador;
        Usuarios& u = usuarios[i];
        ListaFavoritos& lf = u.getListaFavoritos();

        // 1. Escribir el nickname
        fs << u.getNickname() << ":";

        // 2. Escribir las canciones PROPIAS (asumiendo que getCancionesIds() solo da las propias)
        const std::string* ids = lf.getCancionesIds();
        for (int j = 0; j < lf.getNumCanciones(); ++j) {
            ++contador;
            if (j > 0) fs << ",";
            fs << ids[j];
        }

        // 3. Escribir el separador para el campo de seguimiento
        fs << ":";

        // 4. Escribir el nickname del usuario seguido, si existe
        Usuarios* seguido = u.getUsuarioSeguido(); // Asume este getter existe
        if (seguido != nullptr) {
            fs << seguido->getNickname();
        }

        // 5. Nueva línea
        fs << "\n";
    }
    fs.close();
    *iteraciones += contador;
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
        // Usuario Premium (asumo tipo 1): Alta Calidad (rutaArchivo2)
        rutaReproduccion = rutaArchivo2;
    } else {
        // Usuario Estándar (asumo tipo 0): Calidad Estándar (rutaArchivo)
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
    if (mostrarPublicidad && usuario && usuario->debeMostrarPublicidad()){// Publicidad si corresponde
        Publicidad* pub = obtenerPublicidadAleatoria();
        if (pub) {
            cout << "\nPUBLICIDAD:     " << endl;
            cout << pub->getMensaje() << endl;
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

    // =========================================================================
    // 1. Obtener la lista COMBINADA (Propia + Seguida)
    // =========================================================================
    int totalCanciones = 0;
    // Llama al método que combina las IDs propias y las seguidas en un array dinámico
    std::string* listaTotalIds = lista->getListaCompletaDeIds(totalCanciones);

    if (totalCanciones == 0 || listaTotalIds == nullptr) {
        cout << "La lista de favoritos esta vacia, agregue canciones." << endl;
        *iteraciones += contador;
        return;
    }

    cout << "\nReproduciendo " << totalCanciones << " canciones de la lista de favoritos ";
    cout << (aleatoria ? "en modo aleatorio." : "en orden.") << endl;

    // Crear un array de índices para el orden de reproducción (original o aleatorio)
    int* ordenReproduccion = new int[totalCanciones];
    for (int i = 0; i < totalCanciones; ++i) {
        ordenReproduccion[i] = i;
    }

    if (aleatoria) {
        // Mezclar los índices (Fisher-Yates)
        for (int i = totalCanciones - 1; i > 0; --i) {
            ++contador;
            int j = rand() % (i + 1);
            std::swap(ordenReproduccion[i], ordenReproduccion[j]);
        }
    }

    // =========================================================================
    // 2. Bucle de Reproducción Controlado
    // =========================================================================
    bool reproduciendo = true;
    int indiceLista = 0; // Índice para iterar sobre el array 'ordenReproduccion'
    const int M = 6;
    std::string historial[M]; // Guarda los IDs para "Anterior"
    int historialSize = 0;

    while (reproduciendo && indiceLista < totalCanciones) {
        ++contador;

        // El ID es el valor en el índice del array de ordenación
        std::string id = listaTotalIds[ordenReproduccion[indiceLista]];

        // 2a. Reproducir la canción actual y medir el tiempo
        mostrarReproduccion(id, usuario, false); // Asumo que esto ya imprime info

        // Guardar en el historial antes de avanzar
        if (historialSize < M) historial[historialSize++] = id;
        else { // Si el historial está lleno, desplazar
            for (int i = 0; i < M - 1; ++i) historial[i] = historial[i + 1];
            historial[M - 1] = id;
        }

        cout << endl << "Opciones (Tiempo restante para siguiente: 3s): 1) Siguiente  2) Anterior  3) Detener" << endl;

        // Simulación de espera por interacción del usuario (no bloqueante idealmente, pero usamos un simple cin)
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        int op = 0;

        // Intentar leer la opción sin esperar infinitamente (opcional: mejor usar hilos o ncurses, pero simplificado aquí)
        // Por la limitación de consola simple, tendremos que forzar una pausa o pedir input.
        // Nos apegamos a tu lógica original de input forzado, pero lo reestructuramos.

        // Pedir input después de una pausa (simulando que el usuario puede elegir rápido)
        cout << "Ingrese opcion (o 0 para seguir): ";
        if (!(std::cin >> op)) {
            // Si el input falla o no se ingresa nada, limpiamos y asumimos 'Siguiente' si el bucle lo permite
            std::cin.clear();
            std::string tmp;
            std::getline(std::cin, tmp);
            op = 1; // Avanzar automáticamente a la siguiente (comportamiento por defecto)
        }

        switch (op) {
        case 1: // Siguiente (Avanzar al siguiente índice en el array de ordenación)
            indiceLista++;
            break;
        case 2: { // Anterior (Usar el historial)
            if (historialSize <= 1) {
                // Si hay 0 o 1 canciones, no hay una "anterior" disponible.
                cout << "No hay cancion anterior en el historial. Ya estás en la primera." << endl;
            } else {
                // La canción anterior es el penúltimo elemento. (Ej: size 3 -> índice 1)
                std::string idAnterior = historial[historialSize - 2];

                // 1. **IMPORTANTE:** Reducir el historial en 1.
                // Esto elimina la ID de la canción actual del historial (la última).
                historialSize--; // Ahora size es 2 (la canción anterior es el nuevo final)

                mostrarReproduccion(idAnterior, usuario, false);
                cout << "Volviendo a la cancion anterior del historial." << endl;

                // 2. Ajustar el índice de la lista.
                // Retrocedemos el índice de la lista combinada para que, en el siguiente loop,
                // se reproduzca el ID que acabamos de poner en el historial (idAnterior).
                // Si el bucle while reproduce indiceLista, debemos asegurar que indiceLista
                // apunte a la posición del idAnterior. Como no podemos mapear un ID aleatorio
                // de vuelta a su posición, la forma más simple es retroceder en 1 el puntero de la lista,
                // pero esto puede ser peligroso si el modo es aleatorio.

                // 🚨 PRECAUCIÓN: Si la lógica del bucle principal es correcta, esta línea puede ser el problema.
                // La eliminamos por ahora, ya que la reproducción principal del while(reproduciendo)
                // siempre debería avanzar.
                // Si la eliminas, el usuario deberá elegir '1) Siguiente' para obtener una nueva canción.

                // Vamos a mantener tu indiceLista--, pero sabiendo que es la fuente potencial del crash
                // si se vuelve negativo.
                indiceLista--;

                // **Verificación anti-crash para indiceLista**
                if (indiceLista < 0) {
                    indiceLista = 0; // Asegura que no se acceda a listaTotalIds[-1]
                }
            }
            break;
        }
        case 3: // Detener
            reproduciendo = false;
            break;
        default:
            cout << "Opcion invalida. Continuando con la siguiente cancion." << endl;
            indiceLista++;
            break;
        }

        // Si no es la última canción y el usuario no detuvo, avanza a la siguiente automáticamente.
        // Si el usuario eligió una opción válida, ya se actualizó indiceLista.
    }

    cout << "\nReproducción de la lista de favoritos finalizada." << endl;

    // =========================================================================
    // 3. Limpieza de memoria dinámica
    // =========================================================================
    delete[] listaTotalIds;
    delete[] ordenReproduccion;

    *iteraciones += contador;
    cout << "***** memoria usada para reproduccir lista de favoritos: " << UdeATunesDataset::getPeakMemoryUsageKB() << " KB\n";
    cout << "***** esto nos cuesta en iteraciones poder reproducir lista de favoritos: " << *UdeATunesDataset::iteraciones << endl;
}
void UdeATunesDataset::menuFavoritosPremium(Usuarios* usuario) {
    int contador = 0;
    // Asumo que tu archivo de persistencia se llama "listadefavoritos.txt"
    const std::string RUTA_FAVORITOS = "listadefavoritos.txt";

    if (usuario->getTipoMembresia() != 1) {
        cout << "opcion solo para premium" << endl;
        *iteraciones += contador;
        return;
    }
    bool continuar = true;
    while (continuar) {
        ++contador;

        // 1. Mostrar las opciones del menú actualizadas
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
            cout << "ID de la cancion: ";
            cin >> id;
            if (usuario->getListaFavoritos().agregarCancion(id)) {
                // 🚨 PUNTO DE GUARDADO 1: La lista propia ha cambiado (agregar)
                if (guardarListasDeFavoritos(RUTA_FAVORITOS)) {
                    cout << "[INFO] Canción agregada. Cambios guardados en el archivo." << endl;
                } else {
                    cout << "[ERROR] La cancion se agrego, pero NO se pudo guardar el archivo." << endl;
                }
            } else {
                cout << "La cancion ya existe en tu lista o el ID es invalido." << endl;
            }
            break;
        }
        case 3: {
            string id;
            cout << "ID de la cancion: ";
            cin >> id;
            if (usuario->getListaFavoritos().eliminarCancion(id)) {
                // 🚨 PUNTO DE GUARDADO 2: La lista propia ha cambiado (eliminar)
                if (guardarListasDeFavoritos(RUTA_FAVORITOS)) {
                    cout << "[INFO] Canción eliminada. Cambios guardados en el archivo." << endl;
                } else {
                    cout << "[ERROR] La cancion se elimino, pero NO se pudo guardar el archivo." << endl;
                }
            } else {
                cout << "La cancion no se encontro en tu lista." << endl;
            }
            break;
        }
        case 4: { // Seguir a otro usuario
            string otroNick;
            cout << "diganos el nombre del usuario que quiere seguir: ";
            cin >> otroNick;

            Usuarios* otro = buscarUsuario(otroNick);

            if (otro) {
                // Asumo que seguirUsuario devuelve true si el seguimiento fue exitoso
                if (usuario->seguirUsuario(otro)) {
                    // 🚨 PUNTO DE GUARDADO 3: El estado de seguimiento ha cambiado
                    if (guardarListasDeFavoritos(RUTA_FAVORITOS)) {
                        cout << "[INFO] Ahora sigues a " << otroNick << ". Estado guardado en el archivo." << endl;
                    } else {
                        cout << "[ERROR] Se inicio el seguimiento, pero NO se pudo guardar el archivo." << endl;
                    }
                } else {
                    // El seguimiento pudo haber fallado si ya se seguía al mismo usuario o a otro
                    cout << "No se pudo iniciar el seguimiento. Revisa si ya sigues a alguien." << endl;
                }
            } else {
                cout << "No encontramos ese usuario, revisa si escribiste bien." << endl;
            }
            break;
        }
        case 5: { // Dejar de seguir usuario (soluciona la persistencia)
            // Asumo que dejarDeSeguir() devuelve true si había alguien a quien dejar de seguir.
            if (usuario->dejarDeSeguir()) {
                // 🚨 PUNTO DE GUARDADO 4: El estado de seguimiento ha cambiado (a vacío)
                if (guardarListasDeFavoritos(RUTA_FAVORITOS)) {
                    cout << "[INFO] Has dejado de seguir al usuario. Estado guardado en el archivo." << endl;
                } else {
                    cout << "[ERROR] Se dejo de seguir al usuario, pero NO se pudo guardar el archivo." << endl;
                }
            } else {
                cout << "Actualmente no estas siguiendo a ningun usuario." << endl;
            }
            break;
        }
        case 6: { // Reproducir mi lista
            int a;
            cout << "reproduccion aleatoria? (1=Si / 0=No): ";
            cin >> a;
            reproducirListaFavoritos(usuario, a == 1);
            break;
        }
        case 7: // Salir
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
                    cout << "\n[PUBLICIDAD] " << pub->getMensaje() << endl;
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
            cout << "desea detener la reproduccion? (s/n): ";
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
                        cout << "\n[PUBLICIDAD] " << pub->getMensaje() << endl;
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
    bool repetir = false;
    const int MAX_HIST = 4;
    string historial[MAX_HIST];
    int histCount = 0;
    Cancion* actual = nullptr;
    for (int i = 0; i < K; ++i) {
        ++contador;
        if (actual) {
            // Guardar la ID de la canción que acaba de terminar ANTES de reproducir la siguiente
            if (histCount < MAX_HIST) {
                historial[histCount++] = actual->getIdCompleto();
            } else {
                // Manejo de historial lleno (desplazar para dar espacio)
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
                // 1. Guardar la canción actual en el historial (si hay espacio)
                if (histCount < MAX_HIST) {
                    historial[histCount++] = actual->getIdCompleto();
                } else {
                    // Desplazar el historial para insertar la nueva canción
                    for (int j = 0; j < MAX_HIST - 1; ++j) {
                        historial[j] = historial[j + 1];
                    }
                    historial[MAX_HIST - 1] = actual->getIdCompleto();
                }
            }

            // 2. Reproducir la siguiente canción aleatoria
            delete actual;
            actual = reproducirCancionAleatoria(usuario);
            if (actual) this_thread::sleep_for(chrono::seconds(3));
            break;
        }
        case 2: { // Previa
            if (histCount == 0) {
                cout << "no hay cancion previa." << endl;
            } else {
                // El historial YA CONTIENE la canción actual si el historial se llenó,
                // o si acabamos de retroceder. Necesitamos el ID del elemento anterior
                // al último que se guardó.

                string idAnterior = historial[histCount - 1]; // Obtener el ID previo

                // Nota: La canción actual `actual` NO se guarda si se retrocede, solo se reemplaza.
                // Si la canción previa es la que queremos, no deberíamos guardarla de nuevo.

                delete actual;
                actual = buscarCancion(idAnterior);

                if (actual) {
                    mostrarReproduccion(idAnterior, usuario, false);
                    std::this_thread::sleep_for(std::chrono::seconds(3));
                } else {
                    cout << "no se encontro la cancion previa." << endl;
                }

                // 💡 SOLUCIÓN: El índice del historial NO DEBE AVANZAR al retroceder,
                // pero SÍ DEBE DISMINUIR después de obtener la canción anterior.
                // Esto permite múltiples retrocesos.
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
        } // Cierra el switch
    } // Cierra el while (enMenu) (¡Esto faltaba!)
    if (usuario->getUsuarioSeguido() != nullptr) {
        // Esta llamada elimina la delegación y limpia la metadata.
        usuario->dejarDeSeguir();
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
