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
    // Arreglos principales
    Artista* artistas;
    int numArtistas;
    int capacidadArtistas;
    Colaborador* colaboradores;
    int numColaboradores;
    int capacidadColaboradores;
    // Buffers temporales para líneas de archivos
    std::string* lineasAlbumes;
    std::string* lineasCanciones;
    std::string* lineasCreditos;
    int numLineasAlbumes, capacidadLineasAlbumes;
    int numLineasCanciones, capacidadLineasCanciones;
    int numLineasCreditos, capacidadLineasCreditos;
    // Métodos de utilidad
    void resizeArtistas();
    void resizeColaboradores();
    void obtenerSubCampos(const std::string& campo, char delimitador, std::string*& resultados, int& numResultados) const;
    std::string obtenerCampo(const std::string& linea, int campo) const;
    std::string buscarLineaPorID(const std::string* lineas, int numLineas, const std::string& idBuscado, int campoID) const;
    // Métodos de carga específicos

    bool cargarColaboradores();
    bool cargarAlbumesYCanciones();
    bool procesarAlbum(const std::string& lineaAlbum, int indice);
    bool procesarCancionesParaAlbum(Album& album, Artista* artista, const std::string& idAlbumCompleto);
    bool procesarCreditosParaCancion(Cancion* cancion, const std::string& idCreditoStr);
    long* idsColaboradores;
    // Métodos de búsqueda
    Colaborador* encontrarColaborador(const std::string& idColaborador) const;
    float parseDuracion(const std::string& duracionStr) const;
    Usuarios* usuarios;
    int numUsuarios;
    int capacidadUsuarios;
    void resizeUsuarios();
    Publicidad* publicidades;
    int numPublicidades;
    int capacidadPublicidades;
    int ultimaPublicidadMostrada;
    bool cargarPublicidadesPorDefecto();
    void resizePublicidades();
public:
    UdeATunesDataset();
    ~UdeATunesDataset();
    bool cargarDatos();
    int getNumArtistas() const { return numArtistas; }
    Artista* getArtistaAt(int idx) const {
        if (idx >= 0 && idx < numArtistas)
            return &artistas[idx];
        return nullptr;
    }
    // ... (justo despues de getArtistaAt(int idx))

    int getNumUsuarios() const { return numUsuarios; }
    Usuarios* getUsuarioAt(int idx) const {
        if (idx >= 0 && idx < numUsuarios)
            return &usuarios[idx];
        return nullptr;
    }
    bool cargarArtistas();
    Cancion* reproducirCancionAleatoria(Usuarios* usuario);
    void menuFavoritosPremium(Usuarios* usuario);
    void reproducirListaFavoritos(Usuarios* usuario, bool aleatoria);
    void iniciarSesionYReproducir();
    // Getters para acceso externo
    Artista* getArtistas() const { return artistas; }
      int getNumPublicidades() const { return numPublicidades; }
    Colaborador* getColaboradores() const { return colaboradores; }
    int getNumColaboradores() const { return numColaboradores; }
    Artista* getArtista(long id) const;
    // Métodos de utilidad pública
    bool cargarPublicidades();
    Publicidad* obtenerPublicidadAleatoria();
    static std::string* guardarDinamico(const std::string& nombreArchivo, int& numLineas, int& capacidadFinal, int capacidadInicial = 10);
    bool cargarUsuarios();
    Usuarios* getUsuario(const std::string& nickname) const;
    bool cargarListasDeFavoritos(const std::string& rutaArchivo);
Cancion* buscarCancion(const std::string& id) const;
};

#endif // UDEATUNES_DATASET_H
