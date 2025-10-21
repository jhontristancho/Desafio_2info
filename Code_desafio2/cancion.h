#ifndef CANCION_H
#define CANCION_H
#include <string>
#include "Creditos.h" // esto contiene a los credtos
class Album;
class Artista;
class Cancion {
private:
    int idCompleto;
    std::string nombre;
    float duracion;
    int reproducciones;
    std::string ruta128; // cancion pa los estandar
    std::string ruta320; // y esta pa los premium
    Artista* artista;
    Album* album;
    Creditos creditos;

public:
    Cancion(int id, const std::string& nom, float dur, const std::string& r128, const std::string& r320);
    Cancion(int id, const std::string& nom, float dur, const std::string& r128, const std::string& r320,
            Album* a, Artista* art, const Creditos& cred);
    Cancion();
    bool operator==(const Cancion& otra) const;
    void reproducir(int calidad);
    float getDuracion() const { return duracion; }
    int getIdCompleto() const { return idCompleto; }
const std::string& getNombre() const { return nombre; }
    Creditos& getCreditos();
    void setArtista(Artista* art);
    void setAlbum(Album* alb);
    int getReproducciones() const { return reproducciones; }
    const Creditos& getCreditos() const { return creditos; }

};

#endif // CANCION_H
