#ifndef UDEATUNES_DATASET_H
#define UDEATUNES_DATASET_H

#include "Artista.h"
#include "Colaborador.h"
#include "Album.h"
#include "Cancion.h"
#include "Creditos.h"
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
    bool cargarArtistas();
    bool cargarColaboradores();
    bool cargarAlbumesYCanciones();
    bool procesarAlbum(const std::string& lineaAlbum, int indice);
    bool procesarCancionesParaAlbum(Album& album, Artista* artista, const std::string& idAlbumCompleto);
    bool procesarCreditosParaCancion(Cancion* cancion, const std::string& idCreditoStr);

    // Métodos de búsqueda

    Colaborador* encontrarColaborador(const std::string& idColaborador) const;
    float parseDuracion(const std::string& duracionStr) const;

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
    // Getters para acceso externo
    Artista* getArtistas() const { return artistas; }
    Colaborador* getColaboradores() const { return colaboradores; }
    int getNumColaboradores() const { return numColaboradores; }
    Artista* getArtista(long id) const;
    // Métodos de utilidad pública
    static std::string* guardarDinamico(const std::string& nombreArchivo, int& numLineas, int& capacidadFinal, int capacidadInicial = 10);
};

#endif // UDEATUNES_DATASET_H
