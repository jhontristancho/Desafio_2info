#include "UdeATunesDataset.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <algorithm>

using namespace std;

const int GROW_FACTOR = 2;
const int DATASET_CAPACIDAD_INICIAL = 10;

// CONSTRUCTOR / DESTRUCTOR
UdeATunesDataset::UdeATunesDataset()
    : artistas(new Artista[DATASET_CAPACIDAD_INICIAL]),
      numArtistas(0),
      capacidadArtistas(DATASET_CAPACIDAD_INICIAL),
      colaboradores(new Colaborador[DATASET_CAPACIDAD_INICIAL]),
      numColaboradores(0),
      capacidadColaboradores(DATASET_CAPACIDAD_INICIAL),
      lineasAlbumes(nullptr), numLineasAlbumes(0), capacidadLineasAlbumes(0),
      lineasCanciones(nullptr), numLineasCanciones(0), capacidadLineasCanciones(0),
      lineasCreditos(nullptr), numLineasCreditos(0), capacidadLineasCreditos(0) {
}

UdeATunesDataset::~UdeATunesDataset() {
    delete[] artistas;
    delete[] colaboradores;
    delete[] lineasAlbumes;
    delete[] lineasCanciones;
    delete[] lineasCreditos;
}

// REDIMENSIONAMIENTO
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

// MÉTODOS DE UTILIDAD
void UdeATunesDataset::obtenerSubCampos(const string& campo, char delimitador, string*& resultados, int& numResultados) const {
    numResultados = 0;
    delete[] resultados;
    resultados = nullptr;

    if (campo.empty()) return;

    // Contar elementos
    int count = 1;
    for (char c : campo) {
        if (c == delimitador) count++;
    }

    resultados = new string[count];
    stringstream ss(campo);
    string item;

    while (getline(ss, item, delimitador)) {
        if (!item.empty()) {
            resultados[numResultados++] = item;
        }
    }
}

string UdeATunesDataset::obtenerCampo(const string& linea, int campo) const {
    stringstream ss(linea);
    string item;
    int currentField = 0;

    while (getline(ss, item, ',')) {
        if (currentField == campo) {
            return item;
        }
        currentField++;
    }
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

// MÉTODOS DE CARGA ESPECÍFICOS
bool UdeATunesDataset::cargarArtistas() {
    cout << "  > Cargando Artistas..." << endl;

    string* lineasArtistas = guardarDinamico("artistas.txt", numArtistas, capacidadArtistas);
    if (!lineasArtistas) return false;

    // Redimensionar si es necesario
    while (numArtistas > capacidadArtistas) {
        resizeArtistas();
    }

    for (int i = 0; i < numArtistas; ++i) {
        string linea = lineasArtistas[i];
        string idStr = obtenerCampo(linea, 0);
        long id = stol(idStr);  // CAMBIADO: stol en vez de stoi
        string nombre = obtenerCampo(linea, 1);

        cout << "    [DEBUG] Cargando artista - ID: " << id << " (" << idStr << ") - Nombre: " << nombre << endl;

        artistas[i] = Artista(id, nombre);
    }

    delete[] lineasArtistas;
    return true;
}

bool UdeATunesDataset::cargarColaboradores() {
    cout << "  > Cargando Colaboradores..." << endl;

    string* lineasColaboradores = guardarDinamico("colaboradores.txt", numColaboradores, capacidadColaboradores);
    if (!lineasColaboradores) return false;

    while (numColaboradores > capacidadColaboradores) {
        resizeColaboradores();
    }

    for (int i = 0; i < numColaboradores; ++i) {
        string linea = lineasColaboradores[i];
        string idStr = obtenerCampo(linea, 0);
        string nombreCompleto = obtenerCampo(linea, 1);

        // Separar nombre y apellido
        size_t pos = nombreCompleto.find('_');
        string nombre = (pos == string::npos) ? nombreCompleto : nombreCompleto.substr(0, pos);
        string apellido = (pos == string::npos) ? "" : nombreCompleto.substr(pos + 1);

        colaboradores[i] = Colaborador(nombre, apellido, idStr);
    }

    delete[] lineasColaboradores;
    return true;
}

bool UdeATunesDataset::procesarCreditosParaCancion(Cancion* cancion, const string& idCreditoStr) {
    if (!cancion) return false;

    string lineaCredito = buscarLineaPorID(lineasCreditos, numLineasCreditos, idCreditoStr, 0);
    if (lineaCredito.empty()) return false;

    Creditos creditosCancion;

    // Procesar productores
    auto procesarColaboradores = [&](int campo, char delimitador) {
        string campoData = obtenerCampo(lineaCredito, campo);
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

            // Aplicar al crédito correspondiente
            if (campo == 1) creditosCancion.setProductores(arr, validCount);
            else if (campo == 2) creditosCancion.setMusicos(arr, validCount);
            else if (campo == 3) creditosCancion.setCompositores(arr, validCount);

            delete[] arr;
        }
        delete[] ids;
    };

    procesarColaboradores(1, '#'); // Productores
    procesarColaboradores(2, ','); // Músicos
    procesarColaboradores(3, ','); // Compositores

    cancion->getCreditos() = creditosCancion;
    return true;
}

bool UdeATunesDataset::procesarCancionesParaAlbum(Album& album, Artista* artista, const string& idAlbumCompleto) {
    if (!artista) return false;

    string idAlbumPrefijo = idAlbumCompleto.substr(0, 7);
    bool tieneCanciones = false;

    cout << "    🔍 Buscando canciones para álbum: " << idAlbumPrefijo << endl;

    for (int j = 0; j < numLineasCanciones; ++j) {
        string lineaCancion = lineasCanciones[j];
        string idCancionCompleto = obtenerCampo(lineaCancion, 0);

        if (idCancionCompleto.substr(0, 7) == idAlbumPrefijo) {
            try {
                long idCancion = stol(idCancionCompleto);  // CAMBIADO: stol
                string nombreCancion = obtenerCampo(lineaCancion, 1);
                string duracionStr = obtenerCampo(lineaCancion, 2);
                float duracion = parseDuracion(duracionStr);
                string ruta128 = obtenerCampo(lineaCancion, 3);
                string ruta320 = obtenerCampo(lineaCancion, 4);
                string idCreditoStr = obtenerCampo(lineaCancion, 6);

                cout << "      ✅ Encontrada canción: " << nombreCancion
                     << " (ID: " << idCancionCompleto << ")" << endl;

                // Crear canción
                Cancion* nuevaCancion = new Cancion(idCancion, nombreCancion, duracion, ruta128, ruta320);
                nuevaCancion->setAlbum(&album);
                nuevaCancion->setArtista(artista);

                // Procesar créditos
                if (!idCreditoStr.empty()) {
                    procesarCreditosParaCancion(nuevaCancion, idCreditoStr);
                }

                // Agregar al álbum
                if (album.agregarCancion(nuevaCancion)) {
                    tieneCanciones = true;
                    cout << "      ✅ Canción agregada al álbum: " << nombreCancion << endl;
                } else {
                    cout << "      ❌ Error agregando canción: " << nombreCancion << endl;
                    delete nuevaCancion;
                }

            } catch (const exception& e) {
                cerr << "      ❌ Error procesando canción: " << e.what() << endl;
                continue;
            }
        }
    }

    return tieneCanciones;
}

bool UdeATunesDataset::procesarAlbum(const string& lineaAlbum, int indice) {
    if (lineaAlbum.empty()) {
        cerr << "[WARN] Línea de álbum vacía en índice " << indice << endl;
        return false;
    }

    string idAlbumCompleto = obtenerCampo(lineaAlbum, 0);
    cout << "  🎵 Procesando álbum ID: " << idAlbumCompleto << endl;

    if (idAlbumCompleto.length() < 7) {
        cerr << "  ❌ ID de álbum muy corto: " << idAlbumCompleto << " (necesita 7 dígitos)" << endl;
        return false;
    }

    try {
        // === 1️⃣ Obtener ID de artista y buscarlo ===
        string idArtistaStr = idAlbumCompleto.substr(0, 5);
        long idArtista = stol(idArtistaStr);
        long idAlbum = stol(idAlbumCompleto);

        cout << "    Artista ID: " << idArtista << " (de: " << idArtistaStr << ")" << endl;

        Artista* artista = getArtista(idArtista);
        if (!artista) {
            cerr << "    ❌ Artista " << idArtista << " no encontrado en la base de datos" << endl;
            cout << "    Artistas disponibles: ";
            for (int i = 0; i < numArtistas; ++i)
                cout << artistas[i].getIdArtista() << " ";
            cout << endl;
            return false;
        }

        cout << "    ✅ Artista encontrado: " << artista->getNombre() << endl;

        // === 2️⃣ Crear el álbum en el HEAP ===
        string nombreAlbum = obtenerCampo(lineaAlbum, 1);
        string anio = obtenerCampo(lineaAlbum, 2);
        string rutaPortada = obtenerCampo(lineaAlbum, 3);

        if (nombreAlbum.empty()) {
            cerr << "    ❌ Nombre de álbum vacío" << endl;
            return false;
        }

        Album* nuevoAlbum = new Album(idAlbum, nombreAlbum, anio, rutaPortada);
        nuevoAlbum->setArtista(artista);

        if (procesarCancionesParaAlbum(*nuevoAlbum, artista, idAlbumCompleto)) {
            bool agregado = artista->agregarAlbum(nuevoAlbum);

            cout << "    " << (agregado ? "✅" : "❌") << " Álbum agregado al artista" << endl;

            if (!agregado) {
                delete nuevoAlbum; // Evita fugas si ya existía
            }

            return agregado;
        } else {
            cout << "    ⚠ Álbum sin canciones válidas, se descarta." << endl;
            delete nuevoAlbum;
            return false;
        }

    } catch (const exception& e) {
        cerr << "    ❌ Error procesando álbum: " << e.what() << endl;
        return false;
    }
}


bool UdeATunesDataset::cargarAlbumesYCanciones() {
    cout << "  > Cargando Álbumes y Canciones..." << endl;

    // Cargar archivos
    lineasAlbumes = guardarDinamico("albumes.txt", numLineasAlbumes, capacidadLineasAlbumes);
    lineasCanciones = guardarDinamico("canciones.txt", numLineasCanciones, capacidadLineasCanciones);
    lineasCreditos = guardarDinamico("creditos.txt", numLineasCreditos, capacidadLineasCreditos);

    if (!lineasAlbumes) {
        cerr << "  ❌ No se pudo cargar albumes.txt" << endl;
        return false;
    }
    if (!lineasCanciones) {
        cerr << "  ❌ No se pudo cargar canciones.txt" << endl;
        return false;
    }
    if (!lineasCreditos) {
        cerr << "  ⚠  No se pudo cargar creditos.txt (continuando sin créditos)" << endl;
    }

    cout << "  📊 Estadísticas:" << endl;
    cout << "    - Álbumes: " << numLineasAlbumes << endl;
    cout << "    - Canciones: " << numLineasCanciones << endl;
    cout << "    - Créditos: " << numLineasCreditos << endl;

    int albumesCargados = 0;
    for (int i = 0; i < numLineasAlbumes; ++i) {
        cout << "\n  --- Procesando álbum " << (i+1) << "/" << numLineasAlbumes << " ---" << endl;
        if (procesarAlbum(lineasAlbumes[i], i)) {
            albumesCargados++;
        }
    }

    cout << "  > " << albumesCargados << " álbumes cargados exitosamente" << endl;
    return albumesCargados > 0;
}

// MÉTODOS DE BÚSQUEDA
Artista* UdeATunesDataset::getArtista(long id) const {  // CAMBIADO: long
    cout << "    [DEBUG] Buscando artista ID: " << id << endl;
    for (int i = 0; i < numArtistas; ++i) {
        cout << "    [DEBUG] Comparando con: " << artistas[i].getIdArtista() << " - " << artistas[i].getNombre() << endl;
        if (artistas[i].getIdArtista() == id) {
            cout << "    [DEBUG] ✅ Artista encontrado!" << endl;
            return &artistas[i];
        }
    }
    cout << "    [DEBUG] ❌ Artista NO encontrado" << endl;
    return nullptr;
}

Colaborador* UdeATunesDataset::encontrarColaborador(const string& idColaborador) const {
    for (int i = 0; i < numColaboradores; ++i) {
        if (colaboradores[i].getCodigoAfiliacion() == idColaborador) {
            return &colaboradores[i];
        }
    }
    return nullptr;
}

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

// CARGA PRINCIPAL
bool UdeATunesDataset::cargarDatos() {
    cout << "=== INICIO DE CARGA DE DATOS ===" << endl;

    bool exito = true;

    if (!cargarArtistas()) {
        cerr << "ERROR: Falló la carga de artistas" << endl;
        exito = false;
    }

    if (!cargarColaboradores()) {
        cerr << "ERROR: Falló la carga de colaboradores" << endl;
        exito = false;
    }

    if (!cargarAlbumesYCanciones()) {
        cerr << "ERROR: Falló la carga de álbumes y canciones" << endl;
        exito = false;
    }

    if (exito) {
        cout << "=== CARGA COMPLETADA EXITOSAMENTE ===" << endl;
        cout << "Artistas: " << numArtistas << endl;
        cout << "Colaboradores: " << numColaboradores << endl;
        cout << "Álbumes procesados: " << numLineasAlbumes << endl;
    } else {
        cout << "=== CARGA COMPLETADA CON ERRORES ===" << endl;
    }

    return exito;
}

// FUNCIÓN guardarDinamico (manteniendo tu implementación original)
string* UdeATunesDataset::guardarDinamico(const string& nombreArchivo, int& numLineas, int& capacidadFinal, int capacidadInicial) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "❌ ERROR: No se pudo abrir " << nombreArchivo << endl;
        return nullptr;
    }

    cout << "    📖 Leyendo archivo: " << nombreArchivo << endl;

    string* lineas = new string[capacidadInicial];
    string linea;
    numLineas = 0;
    int capacidad = capacidadInicial;

    while (getline(archivo, linea)) {
        if (!linea.empty()) { // Ignorar líneas vacías
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
    cout << "    ✅ " << nombreArchivo << " - " << numLineas << " líneas leídas" << endl;
    return lineas;
}
