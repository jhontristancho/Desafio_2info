#ifndef CANCION_H
#define CANCION_H
#include <string>
#include "Creditos.h" // esto contiene a los credtos
class Album;
class Artista;
class Cancion {
private:
    std::string idCompleto;
    std::string nombre;
    float duracion;
    int reproducciones;
    std::string ruta128;
    std::string ruta320;
    Artista* artista;           // ✅ Ya existe
    Album* album;
    Creditos creditos;

public:
    // Constructores
    Cancion(const std::string& id, const std::string& nom, float dur,
            const std::string& r128, const std::string& r320);

    Cancion(const std::string& id, const std::string& nom, float dur,
            const std::string& r128, const std::string& r320,
            Album* a, Artista* art, const Creditos& cred);

    Cancion();

    // Getters
    std::string getIdCompleto() const { return idCompleto; }
    std::string getNombre() const { return nombre; }           // ✅ Ya existe
    float getDuracion() const { return duracion; }
    int getReproducciones() const { return reproducciones; }
    std::string getRuta128() const { return ruta128; }
    std::string getRuta320() const { return ruta320; }

    // ✅ AGREGAR ESTOS GETTERS
    Artista* getArtista() const { return artista; }
    Album* getAlbum() const { return album; }
    Creditos& getCreditos() { return creditos; }
    const Creditos& getCreditos() const { return creditos; }

    // Setters
    void setIdCompleto(const std::string& id) { idCompleto = id; }
    void setArtista(Artista* art) { artista = art; }
    void setAlbum(Album* alb) { album = alb; }

    // Operadores y métodos
    bool operator==(const Cancion& otra) const;
    void reproducir(int calidad);
};
#endif // CANCION_H
