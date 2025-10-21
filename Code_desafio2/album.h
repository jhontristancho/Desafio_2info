#ifndef ALBUM_H
#define ALBUM_H
#include <string>
#include "Cancion.h"
class Artista;
class Album {
private:
    int idAlbum;
    std::string nombre;
    std::string selloDisquero;
    std::string fechaLanzamiento;
    float duracionTotal;
    int puntuacion;
    std::string portadaRuta;
    std::string* generos;
    int numGeneros;
    Cancion** canciones;
    int numCanciones;
    int capacidadCanciones;
    Artista* artista;
    void resizeCanciones();
public:
    Album();
    Album(int id, const std::string& nom, const std::string& fecha, const std::string& port);//este constructor es el que nos muestra la version que nos pide el profesor
    ~Album();
    Album(const Album& otra);
    Album& operator=(const Album& otra);
bool agregarCancion(Cancion* c);
    void setGeneros(const std::string* gens, int num);//est0 es para el futuro
    void mostrarInfo() const;
    int getIdAlbum() const { return idAlbum; }
    float getDuracionTotal() const { return duracionTotal; }
    Cancion* buscarCancion(int id) const;
    void setArtista(Artista* art) { artista = art; }
    const std::string& getNombre() const { return nombre; }
    const std::string& getPortadaRuta() const { return portadaRuta; }
    int getNumCanciones() const { return numCanciones; }
    Cancion* getCancionAt(int idx) {
        if (idx >= 0 && idx < numCanciones)
            return canciones[idx];
        return nullptr;
    }
};
#endif // ALBUM_H
