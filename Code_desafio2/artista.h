#ifndef ARTISTA_H
#define ARTISTA_H

#include <string>
#include "Album.h"

class Artista {
private:
    int idArtista;
    std::string nombre;
    int edad;
    std::string paisOrigen;
    int seguidores;
    int posicionTendencia;

    Album** albumes;     // arreglo de punteros
    int numAlbumes;
    int capacidadAlbumes;

    void resizeAlbumes();

public:
    Artista();
    Artista(int id, const std::string& nom);
    ~Artista();
    Artista(const Artista& otra);
    Artista& operator=(const Artista& otra);

bool agregarAlbum(Album* a);
    Album* buscarAlbum(int id) const;

    // Getters
    int getIdArtista() const { return idArtista; }
    const std::string& getNombre() const { return nombre; }

    int getNumAlbumes() const { return numAlbumes; }
    Album* getAlbumAt(int idx) const {
        if (idx >= 0 && idx < numAlbumes) return albumes[idx];
        return nullptr;
    }
};

#endif
