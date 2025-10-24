#ifndef UDEATUNES_DATASET_H
#define UDEATUNES_DATASET_H

#include "Artista.h"
#include "Colaborador.h"
#include "Album.h"
#include "Cancion.h"
#include "Creditos.h"
#include "Usuarios.h"
#include "Publicidad.h"
#include <string>

class UdeATunesDataset {
private:
    // ==============================
    // 🔹 Atributos principales
    // ==============================
    Artista* artistas;
    int numArtistas;
    int capacidadArtistas;

    Colaborador* colaboradores;
    int numColaboradores;
    int capacidadColaboradores;

    Usuarios* usuarios;
    int numUsuarios;
    int capacidadUsuarios;

    Publicidad* publicidades;
    int numPublicidades;
    int capacidadPublicidades;
    int ultimaPublicidadMostrada;

    // ==============================
    // 🔹 Archivos de datos cargados
    // ==============================
    std::string* lineasAlbumes;
    std::string* lineasCanciones;
    std::string* lineasCreditos;

    int numLineasAlbumes;
    int capacidadLineasAlbumes;

    int numLineasCanciones;
    int capacidadLineasCanciones;

    int numLineasCreditos;
    int capacidadLineasCreditos;

    // ==============================
    // 🔹 Utilidades internas
    // ==============================
    long* idsColaboradores;

    void resizeArtistas();
    void resizeColaboradores();
    void resizeUsuarios();
    void resizePublicidades();

    void obtenerSubCampos(const std::string& campo, char delimitador, std::string*& resultados, int& numResultados) const;
    float parseDuracion(const std::string& duracionStr) const;

    // ==============================
    // 🔹 Carga de datos
    // ==============================
    bool cargarArtistas();
    bool cargarColaboradores();
    bool cargarUsuarios();
    bool cargarAlbumesYCanciones();
    bool cargarPublicidades();
    bool cargarPublicidadesPorDefecto();
    bool cargarListasDeFavoritos(const std::string& rutaArchivo);

    // ==============================
    // 🔹 Procesamiento de datos
    // ==============================
    bool procesarAlbum(const std::string& lineaAlbum, int indice);
    bool procesarCancionesParaAlbum(Album& album, Artista* artista, const std::string& idAlbumCompleto);
    bool procesarCreditosParaCancion(Cancion* cancion, const std::string& idCreditoStr);

    // ==============================
    // 🔹 Búsquedas internas
    // ==============================
    Artista* getArtista(std::string id) const;
    Colaborador* encontrarColaborador(const std::string& idColaborador) const;

public:
    static long* iteraciones;
    // ==============================
    // 🔹 Constructor y destructor
    // ==============================
    UdeATunesDataset();
    ~UdeATunesDataset();

    // ==============================
    // 🔹 Métodos de carga y guardado
    // ==============================
    bool cargarDatos();
    static std::string* guardarDinamico(const std::string& nombreArchivo, int& numLineas, int& capacidadFinal, int capacidadInicial = 10);
    bool guardarListasDeFavoritos(const std::string& rutaArchivo) const;

    // ==============================
    // 🔹 Acceso general
    // ==============================
    int getNumArtistas() const { return numArtistas; }
    Artista* getArtistaAt(int idx) const {
        return (idx >= 0 && idx < numArtistas) ? &artistas[idx] : nullptr;
    }

    int getNumColaboradores() const { return numColaboradores; }
    Colaborador* getColaboradores() const { return colaboradores; }

    int getNumUsuarios() const { return numUsuarios; }
    Usuarios* getUsuarioAt(int idx) const {
        return (idx >= 0 && idx < numUsuarios) ? &usuarios[idx] : nullptr;
    }

    int getNumPublicidades() const { return numPublicidades; }

    // ==============================
    // 🔹 Nuevos getters solicitados
    // ==============================
    const std::string* getLineasCanciones() const { return lineasCanciones; }
    int getNumLineasCanciones() const { return numLineasCanciones; }

    // ==============================
    // 🔹 Funciones de negocio
    // ==============================
    Cancion* buscarCancion(const std::string& id) const;
    Cancion* reproducirCancionAleatoria(Usuarios* usuario);

    void reproducirListaFavoritos(Usuarios* usuario, bool aleatoria);
    void menuFavoritosPremium(Usuarios* usuario);
    void iniciarSesionYReproducir();

    // ==============================
    // 🔹 Publicidad
    // ==============================
    Publicidad* obtenerPublicidadAleatoria();

    // ==============================
    // 🔹 Utilidades de impresión
    // ==============================
    void mostrarReproduccion(const std::string& idCancion, Usuarios* usuario, bool mostrarPublicidad);
    std::string obtenerCampo(const std::string& linea, int campo) const;
    std::string buscarLineaPorID(const std::string* lineas, int numLineas, const std::string& idBuscado, int campoID) const;
};

#endif // UDEATUNES_DATASET_H
