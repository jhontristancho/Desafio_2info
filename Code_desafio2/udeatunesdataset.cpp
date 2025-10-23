#include "UdeATunesDataset.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <thread>
#include <chrono>
using namespace std::chrono_literals;
using namespace std;
const int GROW_FACTOR = 2;
const int DATASET_CAPACIDAD_INICIAL = 10;
UdeATunesDataset::UdeATunesDataset()
    : artistas(new Artista[DATASET_CAPACIDAD_INICIAL]),
    numArtistas(0),
    capacidadArtistas(DATASET_CAPACIDAD_INICIAL),publicidades(new Publicidad[10]),  // ✅ INICIALIZAR
    numPublicidades(0), capacidadPublicidades(10),
    ultimaPublicidadMostrada(-1),
    colaboradores(new Colaborador[DATASET_CAPACIDAD_INICIAL]),
    numColaboradores(0),
    capacidadColaboradores(DATASET_CAPACIDAD_INICIAL),
    lineasAlbumes(nullptr), numLineasAlbumes(0), capacidadLineasAlbumes(0),
    lineasCanciones(nullptr), numLineasCanciones(0), capacidadLineasCanciones(0),
    lineasCreditos(nullptr), numLineasCreditos(0), capacidadLineasCreditos(0),
    idsColaboradores(new long[DATASET_CAPACIDAD_INICIAL]),
    // --- INICIO DE LA CORRECCIÓN ---
    usuarios(new Usuarios[DATASET_CAPACIDAD_INICIAL]), // Asigna memoria
    numUsuarios(0),                                  // Inicia en 0
    capacidadUsuarios(DATASET_CAPACIDAD_INICIAL)     // Define capacidad
// --- FIN DE LA CORRECCIÓN ---
{
    // El constructor ahora esta completo
}
UdeATunesDataset::~UdeATunesDataset() {
    delete[] artistas;
    delete[] colaboradores;
    delete[] lineasAlbumes;
    delete[] lineasCanciones;
    delete[] lineasCreditos;
    delete[] idsColaboradores;
        delete[] publicidades;
}
//por si se ocupa toda la capacidad
void UdeATunesDataset::resizeArtistas() {
    int nuevaCapacidad = capacidadArtistas * GROW_FACTOR;
    Artista* nuevo = new Artista[nuevaCapacidad];
    for (int i = 0; i < numArtistas; ++i) {
        nuevo[i] = artistas[i];
    }
    delete[] artistas;
    artistas = nuevo;
    capacidadArtistas = nuevaCapacidad;
}
void UdeATunesDataset::resizeColaboradores() {
    int nuevaCapacidad = capacidadColaboradores * GROW_FACTOR;
    Colaborador* nuevo = new Colaborador[nuevaCapacidad];
    for (int i = 0; i < numColaboradores; ++i) {
        nuevo[i] = colaboradores[i];
    }
    delete[] colaboradores;
    colaboradores = nuevo;
    capacidadColaboradores = nuevaCapacidad;
}
void UdeATunesDataset::obtenerSubCampos(const string& campo, char delimitador, string*& resultados, int& numResultados) const {
    delete[] resultados;
    resultados = nullptr;
    numResultados = 0;
    if (campo.empty()) return;
    int count = 1;
    for (char c : campo) {
        if (c == delimitador) count++;
    }
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
        if (currentField == campo)
            return linea.substr(start, end - start);
        start = end + 1;
        end = linea.find(',', start);
        currentField++;
    }

    if (currentField == campo)
        return linea.substr(start);
    return "";
}
string UdeATunesDataset::buscarLineaPorID(const string* lineas, int numLineas, const string& idBuscado, int campoID) const {
    for (int i = 0; i < numLineas; ++i) {
        if (obtenerCampo(lineas[i], campoID) == idBuscado) {
            return lineas[i];
        }
    }
    return "";
}
bool UdeATunesDataset::cargarArtistas() {
    cout << "cargando artista para probar" << endl;
    string* lineasArtistas = guardarDinamico("artistas.txt", numArtistas, capacidadArtistas);
    if (!lineasArtistas) return false;
    for (int i = 0; i < numArtistas; ++i) {
        string linea = lineasArtistas[i];
        string idStr = obtenerCampo(linea, 0);
        long id = stol(idStr);
        string nombre = obtenerCampo(linea, 1);
        cout << " artista prueba:  " << id << " " << idStr << " nombre: " << nombre << endl;
        artistas[i] = Artista(id, nombre);
    }
    delete[] lineasArtistas;
    return true;
}
bool UdeATunesDataset::cargarColaboradores() {
    cout << " para ver si esta cargando los colaboradores" << endl;
    string* lineasColaboradores = guardarDinamico("colaboradores.txt", numColaboradores, capacidadColaboradores);
    if (!lineasColaboradores) return false;
    for (int i = 0; i < numColaboradores; ++i) {
        string linea = lineasColaboradores[i];
        string idStr = obtenerCampo(linea, 0);
        string nombreCompleto = obtenerCampo(linea, 1);
        string rol = obtenerCampo(linea, 2);
        idsColaboradores[i] = stol(idStr); // ✅ NUEVA LÍNEA: guarda el ID numérico
        colaboradores[i] = Colaborador(nombreCompleto, rol, idStr);
         cout<<nombreCompleto<<rol<<idStr;
    }
    delete[] lineasColaboradores;
    return true;
}

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
                if (c) {
                    arr[validCount++] = *c;
                }
            }

            // funcionalidad para fututo
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
    string idAlbumPrefijo = idAlbumCompleto;
    bool tieneCanciones = false;
    cout << "buscando las canciones: " << idAlbumPrefijo << endl;
    for (int j = 0; j < numLineasCanciones; ++j) {
        string lineaCancion = lineasCanciones[j];
        string idCancionCompleto = obtenerCampo(lineaCancion, 0);
        if (idCancionCompleto.substr(0, 7) == idAlbumPrefijo) {
            try {
                // ✅ ELIMINAR: long idCancion = stol(idCancionCompleto);
                string nombreCancion = obtenerCampo(lineaCancion, 1);
                string duracionStr = obtenerCampo(lineaCancion, 2);
                float duracion = parseDuracion(duracionStr);
                string ruta128 = obtenerCampo(lineaCancion, 3);
                string ruta320 = obtenerCampo(lineaCancion, 4);
                string idCreditoStr = obtenerCampo(lineaCancion, 6);
                cout << "nombre de cancion: " << nombreCancion
                     << "id: "<< idCancionCompleto << endl;

                // ✅ CAMBIAR: usar idCancionCompleto (string) en lugar de idCancion (long)
                Cancion* nuevaCancion = new Cancion(idCancionCompleto, nombreCancion, duracion, ruta128, ruta320);
                nuevaCancion->setAlbum(&album);
                nuevaCancion->setArtista(artista);
                if (!idCreditoStr.empty()) {
                    procesarCreditosParaCancion(nuevaCancion, idCreditoStr);
                }
                if (album.agregarCancion(nuevaCancion)) {
                    tieneCanciones = true;
                    cout << "se guardo la cancion en el album: " << nombreCancion << endl;
                } else {
                    cout << " no se agrego al album " << nombreCancion << endl;
                    delete nuevaCancion;
                }
            } catch (const exception& e) {
                cerr << " hubo un error en la carga de la cancion" << e.what() << endl;
                continue;
            }
        }
    }
    return tieneCanciones;
}
bool UdeATunesDataset::procesarAlbum(const string& lineaAlbum, int indice) {
    if (lineaAlbum.empty()) {
        cerr << "hay una linea vacia en el txt de album" << indice << endl;
        return false;
    }
    string idAlbumCompleto = obtenerCampo(lineaAlbum, 0);
    cout << "miremos a ver si procesa album" << idAlbumCompleto << endl;
    if (idAlbumCompleto.length() < 7) {
        cerr << " el id del album no tiene los 7 digitos" << idAlbumCompleto << "pasele 7 digitos" << endl;
        return false;
    }
    try {
        string idArtistaStr = idAlbumCompleto.substr(0, 5);//los primeros 5 son del artista
        long idArtista = stol(idArtistaStr);
        long idAlbum = stol(idAlbumCompleto);
        cout << "el id del artista es: " << idArtista <<"y todo el id que recibio fue" << idArtistaStr << endl;
        Artista* artista = getArtista(idArtista);
        if (!artista) {
            cerr << " no hay un artista registrado con este id: " << idArtista;
            cout << " los artistas que hay son: ";
            for (int i = 0; i < numArtistas; ++i)
                cout << artistas[i].getIdArtista() << " ";
            cout << endl;
            return false;
        }
        cout << " se encontro el artista " << artista->getNombre() << endl;
        string nombreAlbum = obtenerCampo(lineaAlbum, 1);//y lo guardamos
        string anio = obtenerCampo(lineaAlbum, 2);
        string rutaPortada = obtenerCampo(lineaAlbum, 3);
        if (nombreAlbum.empty()) {
            cerr << "el album no tiene nombre, error" << endl;
            return false;
        }
        Album* nuevoAlbum = new Album(idAlbum, nombreAlbum, anio, rutaPortada);//se crea si todo esta correcto
        nuevoAlbum->setArtista(artista);
        if (procesarCancionesParaAlbum(*nuevoAlbum, artista, idAlbumCompleto)) {
            bool agregado = artista->agregarAlbum(nuevoAlbum);
            cout << " album agregado al artista  " << agregado << endl;
            if (!agregado) {
                delete nuevoAlbum;//para evitar una fuga
            }
            return agregado;
        } else {
            cout << "como el album no tiene canciones, lo descartamos" << endl;
            delete nuevoAlbum;
            return false;
        }

    } catch (const exception& e) {
        cerr << "no se pudo crear el album" << e.what() << endl;
        return false;
    }
}

Cancion* UdeATunesDataset::buscarCancion(const std::string& id) const {  // Cambiar parámetro a string
    // Ya no necesitas convertir a string ni agregar ceros
    std::string lineaCancion = buscarLineaPorID(lineasCanciones, numLineasCanciones, id, 0);

    if (lineaCancion.empty()) {
        return nullptr;
    }

    std::string nom = obtenerCampo(lineaCancion, 1);
    std::string durStr = obtenerCampo(lineaCancion, 2);
    std::string r128 = obtenerCampo(lineaCancion, 3);
    std::string r320 = obtenerCampo(lineaCancion, 4);

    float duracion = parseDuracion(durStr);

    // Usar el ID string directamente
    Cancion* nuevaCancion = new Cancion(id, nom, duracion, r128, r320);
    return nuevaCancion;
}
bool UdeATunesDataset::cargarAlbumesYCanciones() {
    cout << "ahora cargamos album y canciones: " << endl;
    lineasAlbumes = guardarDinamico("albumes.txt", numLineasAlbumes, capacidadLineasAlbumes);
    lineasCanciones = guardarDinamico("canciones.txt", numLineasCanciones, capacidadLineasCanciones);
    lineasCreditos = guardarDinamico("creditos.txt", numLineasCreditos, capacidadLineasCreditos);
    if (!lineasAlbumes) {
        cerr << "hubo un error al cargar album" << endl;
        return false;
    }
    if (!lineasCanciones) {
        cerr << "hubo un error al cargar canciones " << endl;
        return false;
    }
    if (!lineasCreditos) {
        cerr << " hubo un error al cargar album, pero continuamos sin creditos " << endl;
    }
    cout << " lo que se leyo: " << endl;
    cout << " album:  " << numLineasAlbumes << endl;
    cout << " canciones:  " << numLineasCanciones << endl;
    cout << " creditos:  " << numLineasCreditos << endl;
    int albumesCargados = 0;
    for (int i = 0; i < numLineasAlbumes; ++i) {
        cout << "cargando album: " << (i+1) << "/" << numLineasAlbumes<< endl;
        if (procesarAlbum(lineasAlbumes[i], i)) {
            albumesCargados++;
        }
    }
    cout << "  > " << albumesCargados << " álbumes cargados exitosamente" << endl;
    return albumesCargados > 0;
}

void UdeATunesDataset::resizeUsuarios() {
    int nuevaCapacidad = capacidadUsuarios * GROW_FACTOR;
    Usuarios* nuevo = new Usuarios[nuevaCapacidad];

    // Copiamos los usuarios existentes al nuevo arreglo
    for (int i = 0; i < numUsuarios; ++i) {
        nuevo[i] = usuarios[i];
    }

    delete[] usuarios; // Liberamos el arreglo antiguo
    usuarios = nuevo;  // Apuntamos al nuevo arreglo
    capacidadUsuarios = nuevaCapacidad;

    cout << "[INFO_DATASET]: Arreglo de usuarios redimensionado a "
         << capacidadUsuarios << endl;
}
/**
 * @brief Carga los usuarios desde un archivo de texto.
 * Lee el archivo, parsea cada linea y crea los objetos Usuarios.
 * @param nombreArchivo El nombre del archivo (ej. "usuarios.txt").
 * @return true si la carga fue exitosa, false en caso contrario.
 */
bool UdeATunesDataset::cargarUsuarios() {
    cout << "Cargando usuarios "<< endl;

    int numLineasLeidas = 0;
    int capacidadLineas = 0;
    string* lineasUsuarios = guardarDinamico("usuarios.txt", numLineasLeidas, capacidadLineas);

    if (!lineasUsuarios) {
        cerr << "ERROR: No se pudo abrir o leer el archivo de usuarios." << endl;
        return false;
    }

    for (int i = 0; i < numLineasLeidas; ++i) {
        if (numUsuarios >= capacidadUsuarios) {
            resizeUsuarios();
        }

        string linea = lineasUsuarios[i];

        try {
            std::string nick = obtenerCampo(linea, 0);
            int tipo = std::stoi(obtenerCampo(linea, 1));
            std::string ciudad = obtenerCampo(linea, 2);
            std::string pais = obtenerCampo(linea, 3);
            std::string fecha = obtenerCampo(linea, 4);

            // ✅ CONSTRUCTOR CORRECTO
            usuarios[numUsuarios] = Usuarios(nick, tipo, ciudad, pais, fecha);
            numUsuarios++;

        } catch (const std::exception& e) {
            cerr << "ERROR: No se pudo procesar la linea " << i
                 << " del archivo de usuarios: " << e.what() << endl;
        }
    }

    delete[] lineasUsuarios;
    cout << " > Carga finalizada. " << numUsuarios << " usuarios cargados." << endl;
    return true;
}

// metodo para la busqueda
Artista* UdeATunesDataset::getArtista(long id) const {
    cout << " buscar el artista: " << id << endl;
    for (int i = 0; i < numArtistas; ++i) {
        cout << " comparamos:  " << artistas[i].getIdArtista() << " - " << artistas[i].getNombre() << endl;
        if (artistas[i].getIdArtista() == id) {
            cout << " coincide " << endl;
            return &artistas[i];
        }
    }
    cout << " no coincide, ese artista no esta " << endl;
    return nullptr;
}
Colaborador* UdeATunesDataset::encontrarColaborador(const string& idColaborador) const {
    long idNum = stol(idColaborador); // ✅ convertir una sola vez
    for (int i = 0; i < numColaboradores; ++i) {
        if (idsColaboradores[i] == idNum) {
            return &colaboradores[i];
        }
    }
    return nullptr;
}
float UdeATunesDataset::parseDuracion(const string& duracionStr) const {//para encontrar la duracion
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
bool UdeATunesDataset::cargarDatos() {
    cout << "iniciamos la carga de archivos: " << endl;
    bool exito = true;
    if (!cargarPublicidades()) {
        cerr << "Advertencia: No se pudieron cargar las publicidades" << endl;
        // No marcamos exito=false porque es opcional
    }
    if (!cargarArtistas()) {
        cerr << "fallo la carga del txt de artista" << endl;
        exito = false;
    }
    if (!cargarColaboradores()) {
        cerr << "fallo la carga de colaboradores" << endl;
        exito = false;
    }
    if (!cargarAlbumesYCanciones()) {
        cerr << "fallo la carga de albumes y canciones" << endl;
        exito = false;
    }

    if (!cargarUsuarios()){
        cerr<< "fallo en cargar usuarios"<<endl;
        exito=false;
        }
    if (!cargarListasDeFavoritos("listadefavoritos.txt")) {
            cerr << "Advertencia: No se pudieron cargar las listas de favoritos" << endl;
    }
    if (exito) {
        cout << "Se cargo todo bien" << endl;
        cout << "artistas: " << numArtistas << endl;
        cout << "colaboradores: " << numColaboradores << endl;
        cout << "albumes : " << numLineasAlbumes << endl;
    } else {
        cout << "hubo un error en la carga" << endl;
    }
    return exito;
}
bool UdeATunesDataset::cargarListasDeFavoritos(const std::string& rutaArchivo) {
    std::ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        std::cerr << "[ERROR] No se pudo abrir el archivo: " << rutaArchivo << std::endl;
        return false;
    }

    std::cout << "[INFO] Cargando listas de favoritos desde: " << rutaArchivo << std::endl;
    std::string linea;
    int listasCargadas = 0;
    while (std::getline(archivo, linea)) {
        if (linea.empty()) continue;
        size_t pos_separador = linea.find(':');
        if (pos_separador == std::string::npos) continue;
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
            // Delega la carga de los IDs de canciones a la clase Usuarios
            if (usuario->cargarFavoritosDesdeString(canciones_str)) {
                listasCargadas++;
            }
        } else {
            std::cout << "[ADVERTENCIA] Usuario '" << nickname << "' no encontrado." << std::endl;
        }
    }

    archivo.close();
    std::cout << "[INFO] Listas de favoritos cargadas para " << listasCargadas << " usuarios." << std::endl;
    return true;
}
void UdeATunesDataset::resizePublicidades() {
    int nuevaCapacidad = capacidadPublicidades * GROW_FACTOR;
    Publicidad* nuevo = new Publicidad[nuevaCapacidad];
    for (int i = 0; i < numPublicidades; ++i) {
        nuevo[i] = publicidades[i];
    }
    delete[] publicidades;
    publicidades = nuevo;
    capacidadPublicidades = nuevaCapacidad;
}
bool UdeATunesDataset::cargarPublicidadesPorDefecto() {
    cout << "=== CARGANDO PUBLICIDADES POR DEFECTO ===" << endl;

    // Publicidades de ejemplo bien formadas y validadas
    struct PublicidadDefecto {
        int id;
        const char* mensaje;
        char categoria;
    };

    PublicidadDefecto publicidadesDefecto[] = {
        {1, "¡50% de descuento en tu primera suscripción premium!", 'A'},
        {2, "Descubre nuevos artistas cada semana en UdeATunes!", 'B'},
        {3, "Escucha música sin límites y sin anuncios con Premium!", 'A'},
        {4, "¿Sabías que puedes crear hasta 6 listas de reproducción?", 'C'},
        {5, "Comparte tu música favorita con amigos!", 'B'},
        {6, "¡Nuevos lanzamientos exclusivos para usuarios Premium!", 'A'},
        {7, "Descarga canciones para escuchar sin conexión", 'C'},
        {8, "Personaliza tu perfil con temas exclusivos!", 'B'},
        {9, "¡Vota por tus canciones favoritas!", 'C'},
        {10, "Obtén recomendaciones basadas en tu gusto musical!", 'A'}
    };

    int numDefecto = sizeof(publicidadesDefecto) / sizeof(publicidadesDefecto[0]);

    for (int i = 0; i < numDefecto; i++) {
        if (numPublicidades >= capacidadPublicidades) {
            resizePublicidades();
        }

        try {
            publicidades[numPublicidades] = Publicidad(
                publicidadesDefecto[i].id,
                publicidadesDefecto[i].mensaje,
                publicidadesDefecto[i].categoria
                );
            numPublicidades++;
            cout << "[DEBUG] ✅ Publicidad por defecto " << (i+1) << " agregada" << endl;
        } catch (const exception& e) {
            cout << "[DEBUG] ❌ Error creando publicidad por defecto " << (i+1) << ": " << e.what() << endl;
        }
    }

    cout << "✅ " << numPublicidades << " publicidades por defecto cargadas" << endl;
    cout << "=====================================" << endl;

    return numPublicidades > 0;
}
bool UdeATunesDataset::cargarPublicidades() {
    cout << "=== CARGANDO PUBLICIDADES ===" << endl;

    // ✅ LIMPIAR PUBLICIDADES EXISTENTES
    numPublicidades = 0;

    int numLineasLeidas = 0;
    int capacidadLineas = 0;
    string* lineasPublicidad = guardarDinamico("publicidad.txt", numLineasLeidas, capacidadLineas);

    if (!lineasPublicidad) {
        cout << "[ERROR] No se pudo abrir publicidad.txt" << endl;
        cout << "🔄 Cargando publicidades por defecto..." << endl;
        return cargarPublicidadesPorDefecto();
    }

    cout << "[INFO] Archivo publicidad.txt encontrado (" << numLineasLeidas << " líneas)" << endl;

    int lineasProcesadas = 0;
    int lineasOmitidas = 0;

    for (int i = 0; i < numLineasLeidas; ++i) {
        if (numPublicidades >= capacidadPublicidades) {
            resizePublicidades();
        }

        string linea = lineasPublicidad[i];
        lineasProcesadas++;

        // ✅ VERIFICACIONES BÁSICAS DE LA LÍNEA
        if (linea.empty()) {
            cout << "[DEBUG] ❌ Línea " << (i+1) << " vacía, omitiendo" << endl;
            lineasOmitidas++;
            continue;
        }

        // ✅ VERIFICAR QUE TENGA AL MENOS 2 COMAS (3 campos)
        size_t primeraComa = linea.find(',');
        if (primeraComa == string::npos) {
            cout << "[DEBUG] ❌ Línea " << (i+1) << " sin comas: " << linea << endl;
            lineasOmitidas++;
            continue;
        }

        size_t segundaComa = linea.find(',', primeraComa + 1);
        if (segundaComa == string::npos) {
            cout << "[DEBUG] ❌ Línea " << (i+1) << " solo tiene una coma: " << linea << endl;
            lineasOmitidas++;
            continue;
        }

        try {
            // ✅ EXTRAER CAMPOS MANUALMENTE PARA MÁS CONTROL
            string idStr = linea.substr(0, primeraComa);
            string mensaje = linea.substr(primeraComa + 1, segundaComa - primeraComa - 1);
            string categoriaStr = linea.substr(segundaComa + 1);

            // ✅ LIMPIAR ESPACIOS EN BLANCO
            idStr.erase(0, idStr.find_first_not_of(" \t"));
            idStr.erase(idStr.find_last_not_of(" \t") + 1);

            mensaje.erase(0, mensaje.find_first_not_of(" \t"));
            mensaje.erase(mensaje.find_last_not_of(" \t") + 1);

            categoriaStr.erase(0, categoriaStr.find_first_not_of(" \t"));
            categoriaStr.erase(categoriaStr.find_last_not_of(" \t") + 1);

            // ✅ VALIDACIONES ESTRICTAS
            if (idStr.empty()) {
                cout << "[DEBUG] ❌ ID vacío en línea " << (i+1) << endl;
                lineasOmitidas++;
                continue;
            }

            if (mensaje.empty()) {
                cout << "[DEBUG] ❌ Mensaje vacío en línea " << (i+1) << endl;
                lineasOmitidas++;
                continue;
            }

            if (categoriaStr.empty()) {
                cout << "[DEBUG] ❌ Categoría vacía en línea " << (i+1) << endl;
                lineasOmitidas++;
                continue;
            }

            if (categoriaStr.length() != 1) {
                cout << "[DEBUG] ❌ Categoría debe ser un solo carácter en línea " << (i+1) << ": " << categoriaStr << endl;
                lineasOmitidas++;
                continue;
            }

            char categoria = categoriaStr[0];
            if (categoria != 'A' && categoria != 'B' && categoria != 'C') {
                cout << "[DEBUG] ❌ Categoría inválida '" << categoria << "' en línea " << (i+1) << endl;
                lineasOmitidas++;
                continue;
            }

            // ✅ VERIFICAR QUE NO SEA UNA LÍNEA DE USUARIO CORRUPTA
            if (mensaje.find("user") != string::npos ||
                mensaje.find("_fan") != string::npos ||
                mensaje.find("_lover") != string::npos ||
                mensaje.find(",0,") != string::npos ||
                mensaje.find(",1,") != string::npos ||
                mensaje.find("Colombia") != string::npos ||
                mensaje.find("2025-") != string::npos) {
                cout << "[DEBUG] ❌ Línea contiene datos de usuario: " << mensaje << endl;
                lineasOmitidas++;
                continue;
            }

            // ✅ CONVERTIR ID
            int id = stoi(idStr);

            // ✅ VERIFICAR LONGITUD DEL MENSAJE
            if (mensaje.length() > 500) {
                cout << "[DEBUG] ⚠️  Mensaje muy largo en línea " << (i+1) << ", truncando..." << endl;
                mensaje = mensaje.substr(0, 500);
            }

            // ✅ CREAR PUBLICIDAD
            publicidades[numPublicidades] = Publicidad(id, mensaje.c_str(), categoria);
            numPublicidades++;

            cout << "[DEBUG] ✅ Línea " << (i+1) << " procesada: ID=" << id
                 << ", Mensaje='" << mensaje << "', Categoría=" << categoria << endl;

        } catch (const invalid_argument& e) {
            cout << "[DEBUG] ❌ Error numérico en línea " << (i+1) << ": " << e.what() << " - " << linea << endl;
            lineasOmitidas++;
            continue;
        } catch (const out_of_range& e) {
            cout << "[DEBUG] ❌ ID fuera de rango en línea " << (i+1) << ": " << e.what() << " - " << linea << endl;
            lineasOmitidas++;
            continue;
        } catch (const exception& e) {
            cout << "[DEBUG] ❌ Error general en línea " << (i+1) << ": " << e.what() << " - " << linea << endl;
            lineasOmitidas++;
            continue;
        }
    }

    delete[] lineasPublicidad;

    // ✅ RESUMEN DE CARGA
    cout << "--- RESUMEN CARGA PUBLICIDADES ---" << endl;
    cout << "Líneas procesadas: " << lineasProcesadas << endl;
    cout << "Líneas omitidas: " << lineasOmitidas << endl;
    cout << "Publicidades cargadas: " << numPublicidades << endl;

    // ✅ SI NO SE CARGÓ NADA, USAR PUBLICIDADES POR DEFECTO
    if (numPublicidades == 0) {
        cout << "🔄 No se cargaron publicidades válidas, usando por defecto..." << endl;
        return cargarPublicidadesPorDefecto();
    }

    cout << "✅ Carga de publicidades completada exitosamente" << endl;
    cout << "=====================================" << endl;

    return true;
}


Publicidad* UdeATunesDataset::obtenerPublicidadAleatoria() {
    if (numPublicidades == 0) {
        cout << "❌ ERROR: No hay publicidades disponibles" << endl;
        return nullptr;
    }

    cout << "[DEBUG] Obteniendo publicidad aleatoria de " << numPublicidades << " disponibles" << endl;

    // Calcular peso total basado en prioridades
    int pesoTotal = 0;
    for (int i = 0; i < numPublicidades; ++i) {
        pesoTotal += publicidades[i].getPrioridad();
        cout << "[DEBUG] Publicidad " << i << ": '" << publicidades[i].getMensaje()
             << "' - Cat: " << publicidades[i].getCategoria()
             << " - Prioridad: " << publicidades[i].getPrioridad() << endl;
    }

    cout << "[DEBUG] Peso total: " << pesoTotal << endl;

    // Generar número aleatorio considerando pesos
    int randomNum = rand() % pesoTotal;
    cout << "[DEBUG] Número aleatorio: " << randomNum << " de " << pesoTotal << endl;

    int acumulado = 0;
    for (int i = 0; i < numPublicidades; ++i) {
        acumulado += publicidades[i].getPrioridad();
        cout << "[DEBUG] Probando publicidad " << i << " - acumulado: " << acumulado << endl;

        if (randomNum < acumulado) {
            // Evitar mostrar la misma publicidad consecutivamente
            if (i == ultimaPublicidadMostrada && numPublicidades > 1) {
                int nextIndex = (i + 1) % numPublicidades;
                ultimaPublicidadMostrada = nextIndex;
                cout << "[DEBUG] ✅ Publicidad seleccionada (evitando consecutiva): " << nextIndex << endl;
                return &publicidades[nextIndex];
            }

            ultimaPublicidadMostrada = i;
            cout << "[DEBUG] ✅ Publicidad seleccionada: " << i << endl;
            return &publicidades[i];
        }
    }

    cout << "[DEBUG] ⚠️  Fallback a publicidad 0" << endl;
    return &publicidades[0]; // Fallback
}
string* UdeATunesDataset::guardarDinamico(const string& nombreArchivo, int& numLineas, int& capacidadFinal, int capacidadInicial) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "no se pudo abrir el archivo:  " << nombreArchivo << endl;
        return nullptr;
    }
    cout << "lectura del txt de " << nombreArchivo << endl;
    string* lineas = new string[capacidadInicial];
    string linea;
    numLineas = 0;
    int capacidad = capacidadInicial;
    while (getline(archivo, linea)) {
        if (!linea.empty()) {//esta es para ignorar las lineas que esten vacias
            if (numLineas >= capacidad) {
                capacidad *= GROW_FACTOR;
                string* nuevasLineas = new string[capacidad];
                for (int i = 0; i < numLineas; ++i) {
                    nuevasLineas[i] = lineas[i];
                }
                delete[] lineas;
                lineas = nuevasLineas;
            }
            lineas[numLineas++] = linea;
        }
    }

    archivo.close();
    capacidadFinal = capacidad;
    cout << " fue correcto la carga de " << nombreArchivo << " - " << numLineas << "numero de lineas leidas" << endl;
    return lineas;
}
Cancion* UdeATunesDataset::reproducirCancionAleatoria(Usuarios* usuario) {
    if (numLineasCanciones == 0) {
        cout << "❌ No hay canciones cargadas.\n";
        return nullptr;
    }

    int indiceCancion = rand() % numLineasCanciones;
    string lineaCancion = lineasCanciones[indiceCancion];

    string idCancion = obtenerCampo(lineaCancion, 0);
    string nombreCancion = obtenerCampo(lineaCancion, 1);
    string duracionStr = obtenerCampo(lineaCancion, 2);
    string ruta128 = obtenerCampo(lineaCancion, 3);
    string idAlbumCompleto = idCancion.substr(0, 7);
    string idArtistaStr = idCancion.substr(0, 5);

    Artista* artista = getArtista(stol(idArtistaStr));
    string nombreArtista = artista ? artista->getNombre() : "Desconocido";

    Album* albumEncontrado = nullptr;
    string rutaPortada = "Desconocida";
    if (artista) {
        for (int j = 0; j < artista->getNumAlbumes(); ++j) {
            Album* a = artista->getAlbumAt(j);
            if (a && to_string(a->getIdAlbum()) == idAlbumCompleto) {
                albumEncontrado = a;
                rutaPortada = a->getPortadaRuta();
                break;
            }
        }
    }

    if (usuario->getTipoMembresia() == 0) {
        Publicidad* pub = obtenerPublicidadAleatoria();
        if (pub) {
            cout << "\n📢 [PUBLICIDAD] " << pub->getMensaje()
                << " (" << pub->getCategoria() << ")" << endl;
        }
    }

    cout << "\n🎶 Reproduciendo canción:\n";
    cout << "--------------------------------------\n";
    cout << "Artista: " << nombreArtista << endl;
    cout << "Álbum: " << (albumEncontrado ? albumEncontrado->getNombre() : "Desconocido") << endl;
    cout << "Ruta portada: " << rutaPortada << endl;
    cout << "Canción: " << nombreCancion << endl;
    cout << "Ruta archivo: " << ruta128 << endl;
    cout << "Duración: " << duracionStr << " minutos\n";
    cout << "--------------------------------------\n";

    return new Cancion(idCancion, nombreCancion, parseDuracion(duracionStr), ruta128, "");
}
void UdeATunesDataset::reproducirListaFavoritos(Usuarios* usuario, bool aleatoria) {
    const ListaFavoritos* lista = usuario->getListaSeguida();
    if (!lista || lista->getNumCanciones() == 0) {
        cout << "⚠️ La lista de favoritos está vacía.\n";
        return;
    }

    cout << "\n🎧 Reproduciendo lista de favoritos (" << lista->getNumCanciones() << " canciones)\n";

    int M = 6;
    string historial[6];
    int totalHistorial = 0;
    bool reproduciendo = true;
    int indiceActual = aleatoria ? rand() % lista->getNumCanciones() : 0;

    while (reproduciendo) {
        string id = lista->getCancionesIds()[indiceActual];
        Cancion* c = buscarCancion(id);
        if (c) {
            cout << "\n▶️ Canción actual (" << (indiceActual + 1) << "/" << lista->getNumCanciones() << "):\n";
            cout << "   " << c->getNombre() << " (ID: " << id << ")\n";
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }

        cout << "\nOpciones: 1.Siguiente  2.Previo  3.Detener\n> ";
        int op;
        cin >> op;

        if (op == 1) {
            if (totalHistorial < M) historial[totalHistorial++] = id;
            indiceActual = aleatoria ? rand() % lista->getNumCanciones() : (indiceActual + 1) % lista->getNumCanciones();
        } else if (op == 2) {
            if (totalHistorial == 0)
                cout << "⚠️ No hay canciones previas.\n";
            else {
                indiceActual = rand() % lista->getNumCanciones();
                totalHistorial--;
            }
        } else {
            reproduciendo = false;
            cout << "🛑 Fin de reproducción.\n";
        }
        delete c;
    }
}

void UdeATunesDataset::menuFavoritosPremium(Usuarios* usuario) {
    if (usuario->getTipoMembresia() != 1) {
        cout << "⚠️ Solo disponible para usuarios Premium.\n";
        return;
    }

    bool continuar = true;
    while (continuar) {
        cout << "\n=== MENÚ DE FAVORITOS ===\n";
        cout << "1. Mostrar mi lista\n";
        cout << "2. Agregar canción\n";
        cout << "3. Eliminar canción\n";
        cout << "4. Seguir lista de otro usuario\n";
        cout << "5. Reproducir mi lista\n";
        cout << "6. Salir\n";
        cout << "> ";
        int opcion;
        cin >> opcion;

        switch (opcion) {
        case 1:
            usuario->getListaFavoritos().mostrarLista(this);
            break;
        case 2: {
            string id;
            cout << "Ingrese ID de la canción a agregar: ";
            cin >> id;
            usuario->getListaFavoritos().agregarCancion(id);
            break;
        }
        case 3: {
            string id;
            cout << "Ingrese ID de la canción a eliminar: ";
            cin >> id;
            usuario->getListaFavoritos().eliminarCancion(id);
            break;
        }
        case 4: {
            string otroNick;
            cout << "Ingrese nickname del usuario a seguir: ";
            cin >> otroNick;
            Usuarios* otro = nullptr;
            for (int i = 0; i < numUsuarios; ++i) {
                if (usuarios[i].getNickname() == otroNick) {
                    otro = &usuarios[i];
                    break;
                }
            }
            if (otro) {
                usuario->seguirListaFavoritos(&otro->getListaFavoritos());
                cout << "👥 Ahora sigues la lista de " << otroNick << endl;
            } else {
                cout << "❌ Usuario no encontrado.\n";
            }
            break;
        }
        case 5: {
            bool aleatoria;
            cout << "¿Reproducción aleatoria? (1=Sí / 0=No): ";
            cin >> aleatoria;
            reproducirListaFavoritos(usuario, aleatoria);
            break;
        }
        case 6:
            continuar = false;
            break;
        default:
            cout << "❌ Opción inválida.\n";
        }
    }
}
void UdeATunesDataset::iniciarSesionYReproducir() {
    cout << "\n========== BIENVENIDO A UDEATUNES ==========\n";
    cout << "Ingrese su nickname para iniciar sesión: ";
    string nickname;
    cin >> nickname;

    Usuarios* usuario = nullptr;
    for (int i = 0; i < numUsuarios; ++i) {
        if (usuarios[i].getNickname() == nickname) {
            usuario = &usuarios[i];
            break;
        }
    }

    if (!usuario) {
        cout << "❌ Usuario no encontrado.\n";
        return;
    }

    cout << "\n✅ Sesión iniciada como: " << usuario->getNickname()
         << " (" << (usuario->getTipoMembresia() == 0 ? "Estándar" : "Premium") << ")\n";

    // === USUARIO ESTÁNDAR ===
    if (usuario->getTipoMembresia() == 0) {
        cout << "\n🎧 Reproducción automática (máx 5 canciones)...\n";
        const int K = 5;
        for (int i = 0; i < K; ++i) {
            Cancion* c = reproducirCancionAleatoria(usuario);
            if (!c) break;
            std::this_thread::sleep_for(std::chrono::seconds(3));
            delete c;
        }
        cout << "\n🛑 Fin de la reproducción (5 canciones).\n";
        return;
    }

    // === USUARIO PREMIUM ===
    cout << "\n🎵 Bienvenido al modo Premium.\n";
    bool repetir = false;
    int totalHistorial = 0;
    string historial[4];
    bool reproduciendo = true;

    Cancion* actual = reproducirCancionAleatoria(usuario);

    while (reproduciendo) {
        cout << "\nOpciones:\n";
        cout << "1. Siguiente canción\n";
        cout << "2. Canción previa\n";
        cout << "3. Activar/desactivar repetir\n";
        cout << "4. Menú de favoritos\n";
        cout << "5. Detener\n";
        cout << "> ";
        int opcion;
        cin >> opcion;

        switch (opcion) {
        case 1: // siguiente
            if (actual && totalHistorial < 4) historial[totalHistorial++] = actual->getIdCompleto();
            delete actual;
            actual = reproducirCancionAleatoria(usuario);
            std::this_thread::sleep_for(std::chrono::seconds(3));
            break;
        case 2: // previa
            if (totalHistorial == 0) {
                cout << "⚠️ No hay canciones previas.\n";
            } else {
                string idAnterior = historial[--totalHistorial];
                delete actual;
                actual = buscarCancion(idAnterior);
                if (actual) {
                    cout << "\n⏪ Reproduciendo canción anterior: " << idAnterior << endl;
                    std::this_thread::sleep_for(std::chrono::seconds(3));
                }
            }
            break;
        case 3: // repetir
            repetir = !repetir;
            cout << (repetir ? "🔁 Repetir activado.\n" : "⏹️ Repetir desactivado.\n");
            if (repetir && actual) std::this_thread::sleep_for(std::chrono::seconds(3));
            break;
        case 4: // favoritos
            menuFavoritosPremium(usuario);
            break;
        case 5:
            reproduciendo = false;
            cout << "🛑 Reproducción detenida.\n";
            break;
        default:
            cout << "❌ Opción inválida.\n";
        }
    }

    delete actual;
}
