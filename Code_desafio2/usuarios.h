#ifndef USUARIOS_H
#define USUARIOS_H

#include <string>
#include "ListaFavoritos.h"
#include "Cancion.h"

const int TIPO_STANDARD = 0;
const int TIPO_PREMIUM = 1;
const int MAX_HISTORIAL = 6;

class UdeATunesDataset;

class Usuarios {
private:
    std::string nickname;
    int tipoMembresia;
    std::string ciudad;
    std::string pais;
    std::string fechaInscripcion;
    int contadorReproducciones;

    // ✅ CAMBIAR: int* a std::string*
    std::string* historialReproduccion;
    int posHistorial;
    int numHistorial;

public:
    // Constructores y Destructor
    Usuarios();
    Usuarios(const std::string& nick, int tipo, const std::string& ciu,
             const std::string& p, const std::string& fecha);
    Usuarios(const Usuarios& u);
    Usuarios& operator=(const Usuarios& u);
    ~Usuarios();
    int getContadorReproducciones() const { return contadorReproducciones; }
    ListaFavoritos& getListaFavoritos();
    const ListaFavoritos* getListaSeguida() const;
    bool seguirListaFavoritos(ListaFavoritos* listaOtra);

    bool debeMostrarPublicidad() const {
        return (tipoMembresia == TIPO_STANDARD &&
                contadorReproducciones > 0 &&
                (contadorReproducciones % 2) == 0);
    }
    ListaFavoritos listaFavoritos;
    // Getters
    std::string getNickname() const { return nickname; }
    int getTipoMembresia() const { return tipoMembresia; }
    std::string getCiudad() const { return ciudad; }
    std::string getPais() const { return pais; }
    std::string getFechaInscripcion() const { return fechaInscripcion; }
    const ListaFavoritos& getListaFavoritos() const { return listaFavoritos; }

    // Setters
    void setTipoMembresia(int tipo) { tipoMembresia = tipo; }

    // Métodos de Funcionalidad
    void registrarReproduccion(Cancion* c);
    int getCalidadAudio() const;
    bool debeMostrarPublicidad();
    bool seguirUsuario(Usuarios* otroUsuario);
    void reproducirLista(bool aleatoria);

    // ✅ CAMBIAR: int* a std::string*
    std::string* getCancionAnterior();
    bool dejarDeSeguir();
    bool cargarFavoritosDesdeString(const std::string& idsCadena);
    bool cargarFavoritosDesdeArchivo(const std::string& rutaArchivo);
    void mostrarInfo(const UdeATunesDataset* dataset) const;
};

#endif // USUARIOS_H
