#include "UdeATunesDataset.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <random>
#include <ctime>
using namespace std::chrono_literals;
using namespace std;
long* UdeATunesDataset::iteraciones = new long(0);
const int GROW_FACTOR = 2;
const int DATASET_CAPACIDAD_INICIAL = 10;
UdeATunesDataset::UdeATunesDataset()
    : artistas(new Artista[DATASET_CAPACIDAD_INICIAL]),
    numArtistas(0),
    capacidadArtistas(DATASET_CAPACIDAD_INICIAL),
    publicidades(new Publicidad[10]),
    numPublicidades(0),
    capacidadPublicidades(10),
    ultimaPublicidadMostrada(-1),
    colaboradores(new Colaborador[DATASET_CAPACIDAD_INICIAL]),
    numColaboradores(0),
    capacidadColaboradores(DATASET_CAPACIDAD_INICIAL),
    lineasAlbumes(nullptr),
    numLineasAlbumes(0),
    capacidadLineasAlbumes(0),
    lineasCanciones(nullptr),
    numLineasCanciones(0),
    capacidadLineasCanciones(0),
    lineasCreditos(nullptr),
    numLineasCreditos(0),
    capacidadLineasCreditos(0),
    idsColaboradores(new long[DATASET_CAPACIDAD_INICIAL]),
    usuarios(new Usuarios[DATASET_CAPACIDAD_INICIAL]),
    numUsuarios(0),
    capacidadUsuarios(DATASET_CAPACIDAD_INICIAL)
{
    srand(static_cast<unsigned>(time(nullptr)));
}
UdeATunesDataset::~UdeATunesDataset() {
    delete[] artistas;
    delete[] colaboradores;
    delete[] lineasAlbumes;
    delete[] lineasCanciones;
    delete[] lineasCreditos;
    delete[] idsColaboradores;
    delete[] publicidades;
    delete iteraciones;
}
void UdeATunesDataset::resizeArtistas() {//aplicamos aca para redimensionar artista
    int contador=0;
    int nuevaCapacidad = capacidadArtistas * GROW_FACTOR;
    Artista* nuevo = new Artista[nuevaCapacidad];
    for (int i = 0; i < numArtistas; ++i) nuevo[i] = artistas[i];
    delete[] artistas;
    artistas = nuevo;
    capacidadArtistas = nuevaCapacidad;
}
void UdeATunesDataset::resizeColaboradores() {//esto para un futuro, ya que se hablo con el profesor y hay que dejar listos los metodos para un futuro
    int nuevaCapacidad = capacidadColaboradores * GROW_FACTOR;
    Colaborador* nuevo = new Colaborador[nuevaCapacidad];
    for (int i = 0; i < numColaboradores; ++i) nuevo[i] = colaboradores[i];
    delete[] colaboradores;
    colaboradores = nuevo;
    capacidadColaboradores = nuevaCapacidad;
}
void UdeATunesDataset::resizeUsuarios() {//este tambine va manejar los usuarios
    int nuevaCapacidad = capacidadUsuarios * GROW_FACTOR;
    Usuarios* nuevo = new Usuarios[nuevaCapacidad];
    for (int i = 0; i < numUsuarios; ++i) nuevo[i] = usuarios[i];
    delete[] usuarios;
    usuarios = nuevo;
    capacidadUsuarios = nuevaCapacidad;
}
void UdeATunesDataset::resizePublicidades() {//lo mism con la publicidad
    int nuevaCapacidad = capacidadPublicidades * GROW_FACTOR;
    Publicidad* nuevo = new Publicidad[nuevaCapacidad];
    for (int i = 0; i < numPublicidades; ++i) nuevo[i] = publicidades[i];
    delete[] publicidades;
    publicidades = nuevo;
    capacidadPublicidades = nuevaCapacidad;
}
void UdeATunesDataset::obtenerSubCampos(const string& campo, char delimitador, string*& resultados, int& numResultados) const {
    delete[] resultados;
    resultados = nullptr;
    numResultados = 0;
    if (campo.empty()) return;
    int count = 1;
    for (char c : campo) if (c == delimitador) count++;
    resultados = new string[count];
    size_t start = 0;
    size_t end = campo.find(delimitador);
    while (end != string::npos) {
        resultados[numResultados++] = campo.substr(start, end - start);
        start = end + 1;
        end = campo.find(delimitador, start);
    }
    resultados[numResultados++] = campo.substr(start);
}

string UdeATunesDataset::obtenerCampo(const string& linea, int campo) const {
    int currentField = 0;
    size_t start = 0;
    size_t end = linea.find(',');

    while (end != string::npos) {
        if (currentField == campo) return linea.substr(start, end - start);
        start = end + 1;
        end = linea.find(',', start);
        currentField++;
    }

    if (currentField == campo) return linea.substr(start);
    return "";
}

string UdeATunesDataset::buscarLineaPorID(const string* lineas, int numLineas, const string& idBuscado, int campoID) const {
    for (int i = 0; i < numLineas; ++i) {
        std::string idActual = obtenerCampo(lineas[i], campoID);
        if (idBuscado == idActual) return lineas[i];
    }
    return "";
}


// Lectura dinámica de archivos (ignora líneas vacías)
string* UdeATunesDataset::guardarDinamico(const string& nombreArchivo, int& numLineas, int& capacidadFinal, int capacidadInicial) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) return nullptr;

    string* lineas = new string[capacidadInicial];
    string linea;
    numLineas = 0;
    int capacidad = capacidadInicial;

    while (getline(archivo, linea)) {
        if (!linea.empty()) {
            if (numLineas >= capacidad) {
                capacidad *= GROW_FACTOR;
                string* nuevasLineas = new string[capacidad];
                for (int i = 0; i < numLineas; ++i) nuevasLineas[i] = lineas[i];
                delete[] lineas;
                lineas = nuevasLineas;
            }
            lineas[numLineas++] = linea;
        }
    }

    archivo.close();
    capacidadFinal = capacidad;
    return lineas;
}

bool UdeATunesDataset::cargarPublicidades() {
    numPublicidades = 0;
    int numLineasLeidas = 0, capacidadLineas = 0;
    string* lineasPublicidad = guardarDinamico("publicidad.txt", numLineasLeidas, capacidadLineas);
    if (!lineasPublicidad) {
        cout<<"Intente de nuevo cargar la publicidad"<<endl;
    }

    for (int i = 0; i < numLineasLeidas; ++i) {
        string linea = lineasPublicidad[i];
        size_t p1 = linea.find(',');
        if (p1 == string::npos) continue;
        size_t p2 = linea.find(',', p1 + 1);
        if (p2 == string::npos) continue;

        string idStr = linea.substr(0, p1);
        string mensaje = linea.substr(p1 + 1, p2 - p1 - 1);
        string categoriaStr = linea.substr(p2 + 1);
        // trim (simple)
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
    return true;
}

Publicidad* UdeATunesDataset::obtenerPublicidadAleatoria() {
    if (numPublicidades == 0) return nullptr;

    // Peso basado en prioridad
    int pesoTotal = 0;
    for (int i = 0; i < numPublicidades; ++i) pesoTotal += publicidades[i].getPrioridad();

    if (pesoTotal <= 0) return &publicidades[0];

    int rnd = rand() % pesoTotal;
    int acumulado = 0;
    for (int i = 0; i < numPublicidades; ++i) {
        acumulado += publicidades[i].getPrioridad();
        if (rnd < acumulado) {
            // evitar consecutivo
            if (i == ultimaPublicidadMostrada && numPublicidades > 1) {
                int idx = (i + 1) % numPublicidades;
                ultimaPublicidadMostrada = idx;
                return &publicidades[idx];
            }
            ultimaPublicidadMostrada = i;
            return &publicidades[i];
        }
    }
    return &publicidades[0];
}

// Duración parse
float UdeATunesDataset::parseDuracion(const string& duracionStr) const {
    size_t pos = duracionStr.find(':');
    if (pos == string::npos) return 0.0f;
    try {
        int minutos = stoi(duracionStr.substr(0, pos));
        int segundos = stoi(duracionStr.substr(pos + 1));
        return minutos + (segundos / 60.0f);
    } catch (...) {
        return 0.0f;
    }
}

// Carga de artistas y colaboradores y álbumes/canciones
bool UdeATunesDataset::cargarArtistas() {
    string* lineasArtistas = guardarDinamico("artistas.txt", numArtistas, capacidadArtistas);
    if (!lineasArtistas) return false;
    for (int i = 0; i < numArtistas; ++i) {
        string linea = lineasArtistas[i];
        string idStr = obtenerCampo(linea, 0);
        string nombre = obtenerCampo(linea, 1);
        artistas[i] = Artista(idStr, nombre);
    }
    delete[] lineasArtistas;
    return true;
}

bool UdeATunesDataset::cargarColaboradores() {
    string* lineasColaboradores = guardarDinamico("colaboradores.txt", numColaboradores, capacidadColaboradores);
    if (!lineasColaboradores) return false;
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
// definición corregida: encontrarColaborador
Colaborador* UdeATunesDataset::encontrarColaborador(const std::string& idColaborador) const {
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

// definición corregida: getArtista


bool UdeATunesDataset::procesarCreditosParaCancion(Cancion* cancion, const string& idCreditoStr) {
    if (!cancion) return false;
    string lineaCredito = buscarLineaPorID(lineasCreditos, numLineasCreditos, idCreditoStr, 0);
    if (lineaCredito.empty()) return false;
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
    if (!artista) return false;
    bool tieneCanciones = false;
    for (int j = 0; j < numLineasCanciones; ++j) {
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
    return tieneCanciones;
}
Artista* UdeATunesDataset::getArtista(const std::string& id) const {
    for (int i = 0; i < numArtistas; ++i) {
        if (artistas[i].getIdArtista() == id) {
            return &artistas[i];
        }
    }
    return nullptr;
}

bool UdeATunesDataset::procesarAlbum(const string& lineaAlbum, int indice) {
    if (lineaAlbum.empty()) return false;
    string idAlbumCompleto = obtenerCampo(lineaAlbum, 0);
    if (idAlbumCompleto.length() < 7) return false;
    string idArtistaStr = idAlbumCompleto.substr(0, 5);
    Artista* artista = getArtista(idArtistaStr);
    if (!artista) return false;
    string nombreAlbum = obtenerCampo(lineaAlbum, 1);
    string anio = obtenerCampo(lineaAlbum, 2);
    string rutaPortada = obtenerCampo(lineaAlbum, 3);
    if (nombreAlbum.empty()) return false;
    Album* nuevoAlbum = new Album(idAlbumCompleto, nombreAlbum, anio, rutaPortada);
    nuevoAlbum->setArtista(artista);
    if (procesarCancionesParaAlbum(*nuevoAlbum, artista, idAlbumCompleto)) {
        bool agregado = artista->agregarAlbum(nuevoAlbum);
        if (!agregado) delete nuevoAlbum;
        return agregado;
    } else {
        delete nuevoAlbum;
        return false;
    }
}

bool UdeATunesDataset::cargarAlbumesYCanciones() {
    lineasAlbumes = guardarDinamico("albumes.txt", numLineasAlbumes, capacidadLineasAlbumes);
    lineasCanciones = guardarDinamico("canciones.txt", numLineasCanciones, capacidadLineasCanciones);
    lineasCreditos = guardarDinamico("creditos.txt", numLineasCreditos, capacidadLineasCreditos);
    if (!lineasAlbumes || !lineasCanciones) return false;
    int albumesCargados = 0;
    for (int i = 0; i < numLineasAlbumes; ++i) {
        if (procesarAlbum(lineasAlbumes[i], i)) albumesCargados++;
    }
    return albumesCargados > 0;
}

// Buscar canción por id (crea objeto temporal)
Cancion* UdeATunesDataset::buscarCancion(const std::string& id) const {
    string lineaCancion = buscarLineaPorID(lineasCanciones, numLineasCanciones, id, 0);
    if (lineaCancion.empty()) return nullptr;
    string nom = obtenerCampo(lineaCancion, 1);
    string durStr = obtenerCampo(lineaCancion, 2);
    string r128 = obtenerCampo(lineaCancion, 3);
    string r320 = obtenerCampo(lineaCancion, 4);
    float duracion = parseDuracion(durStr);
    return new Cancion(id, nom, duracion, r128, r320);
}

// Cargar usuarios
bool UdeATunesDataset::cargarUsuarios() {
    int numLineasLeidas = 0, capacidadLineas = 0;
    string* lineasUsuarios = guardarDinamico("usuarios.txt", numLineasLeidas, capacidadLineas);
    if (!lineasUsuarios || numLineasLeidas == 0) return false;
    for (int i = 0; i < numLineasLeidas; ++i) {
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
            // ignorar línea corrupta
            continue;
        }
    }

    delete[] lineasUsuarios;
    return numUsuarios > 0;
}

// Cargar listas de favoritos (delegar a Usuarios)
bool UdeATunesDataset::cargarListasDeFavoritos(const std::string& rutaArchivo) {
    std::ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) return false;
    std::string linea;
    while (std::getline(archivo, linea)) {
        if (linea.empty()) continue;
        size_t pos_separador = linea.find(':');
        if (pos_separador == string::npos) continue;
        std::string nickname = linea.substr(0, pos_separador);
        std::string canciones_str = linea.substr(pos_separador + 1);
        Usuarios* usuario = nullptr;
        for (int i = 0; i < numUsuarios; ++i) {
            if (usuarios[i].getNickname() == nickname) {
                usuario = &usuarios[i];
                break;
            }
        }
        if (usuario) {
            usuario->cargarFavoritosDesdeString(canciones_str);
        }
    }
    archivo.close();
    return true;
}

// Guardar listas al archivo
bool UdeATunesDataset::guardarListasDeFavoritos(const std::string& rutaArchivo) const {
    std::ofstream fs(rutaArchivo);
    if (!fs.is_open()) return false;
    for (int i = 0; i < numUsuarios; ++i) {
        const Usuarios& u = usuarios[i];
        const ListaFavoritos& lf = u.getListaFavoritos();
        fs << u.getNickname() << ":";
        const std::string* ids = lf.getCancionesIds();
        for (int j = 0; j < lf.getNumCanciones(); ++j) {
            if (j > 0) fs << ",";
            fs << ids[j];
        }
        fs << "\n";
    }
    fs.close();
    return true;
}

// Mostrar reproducción (con ruta portada y archivo)
void UdeATunesDataset::mostrarReproduccion(const std::string& idCancion, Usuarios* usuario, bool mostrarPublicidad) {
    string lineaCancion = buscarLineaPorID(lineasCanciones, numLineasCanciones, idCancion, 0);
    if (lineaCancion.empty()) {
        cout << "Error: Cancion no encontrada: " << idCancion << endl;
        return;
    }
    string nombreCancion = obtenerCampo(lineaCancion, 1);
    string duracionStr = obtenerCampo(lineaCancion, 2);
    string rutaArchivo = obtenerCampo(lineaCancion, 3);

    // album / artista / portada
    string idAlbumCompleto = idCancion.size() >= 7 ? idCancion.substr(0, 7) : "";
    string idArtistaStr = idCancion.size() >= 5 ? idCancion.substr(0, 5) : "";
    Artista* artista = getArtista(idArtistaStr);
    string nombreArtista = (artista ? artista->getNombre() : "Desconocido");
    string nombreAlbum = "Desconocido";
    string rutaPortada = "Desconocida";
    if (artista) {
        for (int j = 0; j < artista->getNumAlbumes(); ++j) {
            Album* a = artista->getAlbumAt(j);
            if (a && a->getIdAlbum() == idAlbumCompleto) {
                nombreAlbum = a->getNombre();
                rutaPortada = a->getPortadaRuta();
                break;
            }
        }
    }

    // Publicidad si corresponde
    if (mostrarPublicidad && usuario && usuario->debeMostrarPublicidad()) {
        Publicidad* pub = obtenerPublicidadAleatoria();
        if (pub) {
            cout << "\n[PUBLICIDAD]" << endl;
            cout << pub->getMensaje() << endl;
        }
    }

    // Salida minimalista
    cout << "\nReproduciendo:" << endl;
    cout << " Artista : " << nombreArtista << endl;
    cout << " Álbum   : " << nombreAlbum << endl;
    cout << " Portada  : " << rutaPortada << endl;
    cout << " Canción : " << nombreCancion << endl;
    cout << " Archivo : " << rutaArchivo << endl;
    cout << " Duración : " << duracionStr << endl;
    cout << "--------------------------------------" << endl;
}

// Reproducir canción aleatoria (retorna objeto Cancion que el caller debe liberar)
Cancion* UdeATunesDataset::reproducirCancionAleatoria(Usuarios* usuario) {
    if (numLineasCanciones == 0) {
        cout << "No hay canciones cargadas." << endl;
        return nullptr;
    }

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, numLineasCanciones - 1);
    int idx = distrib(gen);
    string lineaCancion = lineasCanciones[idx];
    string idCancion = obtenerCampo(lineaCancion, 0);
    string nombreCancion = obtenerCampo(lineaCancion, 1);
    string duracionStr = obtenerCampo(lineaCancion, 2);
    string ruta128 = obtenerCampo(lineaCancion, 3);

    mostrarReproduccion(idCancion, usuario, true);
    return new Cancion(idCancion, nombreCancion, parseDuracion(duracionStr), ruta128, "");
}

// Reproducción de lista de favoritos (usa la lista combinada si se siguió otra)
void UdeATunesDataset::reproducirListaFavoritos(Usuarios* usuario, bool aleatoria) {
    const ListaFavoritos* lista = &usuario->getListaFavoritos();
    if (!lista || lista->getNumCanciones() == 0) {
        cout << "La lista de favoritos está vacía." << endl;
        return;
    }

    const std::string* canciones = lista->getCancionesIds();
    int numCanciones = lista->getNumCanciones();

    cout << "\nReproduciendo lista de favoritos (" << numCanciones << " canciones)" << endl;
    cout << (aleatoria ? "Modo: aleatorio" : "Modo: orden original") << endl;

    bool reproduciendo = true;
    int indiceActual = aleatoria ? (rand() % numCanciones) : 0;

    const int M = 6;
    string historial[M];
    int historialSize = 0;

    while (reproduciendo) {
        string id = canciones[indiceActual];
        mostrarReproduccion(id, usuario, false);

        cout << "\nOpciones: 1) Siguiente  2) Anterior  3) Detener\n> ";
        int op;
        if (!(cin >> op)) {
            cin.clear();
            string tmp;
            getline(cin, tmp);
            continue;
        }

        switch (op) {
        case 1:
            if (historialSize < M) historial[historialSize++] = id;
            if (aleatoria) indiceActual = rand() % numCanciones;
            else indiceActual = (indiceActual + 1) % numCanciones;
            break;
        case 2:
            if (historialSize == 0) {
                cout << "No hay canción previa disponible." << endl;
            } else {
                string idAnterior = historial[--historialSize];
                mostrarReproduccion(idAnterior, usuario, false);
            }
            break;
        case 3:
            reproduciendo = false;
            break;
        default:
            cout << "Opción inválida." << endl;
        }
    }
}

// Menú de favoritos (premium)
void UdeATunesDataset::menuFavoritosPremium(Usuarios* usuario) {
    if (usuario->getTipoMembresia() != 1) {
        cout << "Opción disponible solo para usuarios Premium." << endl;
        return;
    }

    bool continuar = true;
    while (continuar) {
        cout << "\n=== Menú de Favoritos ===" << endl;
        cout << "1) Mostrar mi lista" << endl;
        cout << "2) Agregar canción" << endl;
        cout << "3) Eliminar canción" << endl;
        cout << "4) Seguir lista de otro usuario" << endl;
        cout << "5) Reproducir mi lista" << endl;
        cout << "6) Salir" << endl;
        cout << "> ";
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
            cout << "ID de la canción: ";
            cin >> id;
            usuario->getListaFavoritos().agregarCancion(id);
            break;
        }
        case 3: {
            string id;
            cout << "ID de la canción: ";
            cin >> id;
            usuario->getListaFavoritos().eliminarCancion(id);
            break;
        }
        case 4: {
            string otroNick;
            cout << "Nickname a seguir: ";
            cin >> otroNick;
            Usuarios* otro = nullptr;
            for (int i = 0; i < numUsuarios; ++i) {
                if (usuarios[i].getNickname() == otroNick) { otro = &usuarios[i]; break; }
            }
            if (otro) {
                usuario->seguirListaFavoritos(&otro->getListaFavoritos());
                cout << "Lista seguida correctamente." << endl;
            } else {
                cout << "Usuario no encontrado." << endl;
            }
            break;
        }
        case 5: {
            int a;
            cout << "Reproducción aleatoria? (1=Sí / 0=No): ";
            cin >> a;
            reproducirListaFavoritos(usuario, a == 1);
            break;
        }
        case 6:
            continuar = false;
            break;
        default:
            cout << "Opción inválida." << endl;
        }
    }
}

// Inicio de sesión y reproducción (pregunta inicial y reproducción automática K=5)
void UdeATunesDataset::iniciarSesionYReproducir() {
    cout << "\nBienvenido a UdeATunes" << endl;
    cout << "Ingrese su nickname: ";
    string nickname;
    cin >> nickname;

    Usuarios* usuario = nullptr;
    for (int i = 0; i < numUsuarios; ++i) {
        if (usuarios[i].getNickname() == nickname) { usuario = &usuarios[i]; break; }
    }

    if (!usuario) {
        cout << "Usuario no encontrado." << endl;
        return;
    }

    cout << "Sesión iniciada: " << usuario->getNickname()
         << " - " << (usuario->getTipoMembresia() == 0 ? "Estándar" : "Premium") << endl;

    cout << "¿Desea reproducir canciones aleatoriamente? (s/n): ";
    char respuesta; cin >> respuesta;
    if (respuesta != 's' && respuesta != 'S') {
        cout << "Reproducción cancelada." << endl;
        return;
    }

    const int K = 5;
    // Para usuario estándar: reproducción automática de K canciones, luego queda escuchando la última; puede detener.
    if (usuario->getTipoMembresia() == 0) {
        int contadorPublicidad = 0;
        int ultimaPublicidadID = -1;
        Cancion* ultimaCancion = nullptr;

        for (int i = 0; i < K; ++i) {
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
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }

        cout << "\nAhora estás escuchando: " << (ultimaCancion ? ultimaCancion->getNombre() : "ninguna") << endl;

        bool reproduciendo = true;
        while (reproduciendo) {
            cout << "Desea detener la reproducción? (s/n): ";
            char op; cin >> op;
            if (op == 's' || op == 'S') { cout << "Reproducción detenida." << endl; reproduciendo = false; }
            else if (op == 'n' || op == 'N') { cout << "Continúa la reproducción..." << endl; std::this_thread::sleep_for(std::chrono::seconds(3)); }
            else cout << "Opción inválida." << endl;
        }

        delete ultimaCancion;
        return;
    }

    // Para usuario premium: reproducir automáticamente K canciones (temporizador) y luego menú interactivo
    // Historial de hasta 4 canciones
    bool repetir = false;
    const int MAX_HIST = 4;
    string historial[MAX_HIST];
    int histCount = 0;
    Cancion* actual = nullptr;

    // Sesión automática inicial de K canciones
    for (int i = 0; i < K; ++i) {
        if (actual && histCount < MAX_HIST) historial[histCount++] = actual->getIdCompleto();
        delete actual;
        actual = reproducirCancionAleatoria(usuario);
        if (!actual) break;
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    if (actual) cout << "\nReproducción automática completada. Actualmente escuchas: " << actual->getNombre() << endl;

    bool enMenu = true;
    while (enMenu) {
        cout << "\nOpciones Premium:\n";
        cout << "1) Siguiente\n";
        cout << "2) Previa\n";
        cout << "3) Activar/Desactivar repetir\n";
        cout << "4) Menú de favoritos\n";
        cout << "5) Detener\n";
        cout << "6) Salir\n";
        cout << "> ";
        int opcion; if (!(cin >> opcion)) { cin.clear(); string tmp; getline(cin, tmp); continue; }

        switch (opcion) {
        case 1:
            if (actual && histCount < MAX_HIST) historial[histCount++] = actual->getIdCompleto();
            delete actual;
            actual = reproducirCancionAleatoria(usuario);
            if (actual) std::this_thread::sleep_for(std::chrono::seconds(3));
            break;
        case 2:
            if (histCount == 0) cout << "No hay canción previa." << endl;
            else {
                string idAnterior = historial[--histCount];
                delete actual;
                actual = buscarCancion(idAnterior);
                if (actual) { mostrarReproduccion(idAnterior, usuario, false); std::this_thread::sleep_for(std::chrono::seconds(3)); }
                else cout << "No se encontró la canción previa." << endl;
            }
            break;
        case 3:
            repetir = !repetir;
            cout << (repetir ? "Repetir activado." : "Repetir desactivado.") << endl;
            if (repetir && actual) { mostrarReproduccion(actual->getIdCompleto(), usuario, false); std::this_thread::sleep_for(std::chrono::seconds(3)); }
            break;
        case 4:
            menuFavoritosPremium(usuario);
            break;
        case 5:
            cout << "Reproducción detenida." << endl;
            delete actual; actual = nullptr;
            break;
        case 6:
            enMenu = false;
            break;
        default:
            cout << "Opción inválida." << endl;
        }
    }

    if (actual) delete actual;
}

// Cargar todos los datos
bool UdeATunesDataset::cargarDatos() {
    bool exito = true;
    if (!cargarPublicidades()) exito = false;
    if (!cargarArtistas()) exito = false;
    if (!cargarColaboradores()) exito = false;
    if (!cargarAlbumesYCanciones()) exito = false;
    if (!cargarUsuarios()) exito = false;
    cargarListasDeFavoritos("listadefavoritos.txt"); // no crítico
    return exito;
}
