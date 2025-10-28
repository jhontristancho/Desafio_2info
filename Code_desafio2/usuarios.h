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
    std::string* historialReproduccion;
    int posHistorial;
    int numHistorial;
          ListaFavoritos listaFavoritos;
Usuarios* usuarioSeguido = nullptr;//el puntero pa al logica
std::string nicknameSeguidoTemp;
public:
    Usuarios();
    Usuarios(const std::string& nick, int tipo, const std::string& ciu,
             const std::string& p, const std::string& fecha);
    Usuarios(const Usuarios& u);
    Usuarios& operator=(const Usuarios& u);
    ~Usuarios();
    int getContadorReproducciones() const { return contadorReproducciones; }
    const ListaFavoritos* getListaSeguida() const;
    bool seguirListaFavoritos(const ListaFavoritos* listaOtra);
    bool debeMostrarPublicidad() const {
        return (tipoMembresia == TIPO_STANDARD &&
                contadorReproducciones > 0 &&
                (contadorReproducciones % 2) == 0);
    }
    std::string getNickname() const { return nickname; }
    int getTipoMembresia() const { return tipoMembresia; }
    std::string getCiudad() const { return ciudad; }
    std::string getPais() const { return pais; }
    std::string getFechaInscripcion() const { return fechaInscripcion; }
    ListaFavoritos& getListaFavoritos();
    Usuarios* getUsuarioSeguido() const {
        return usuarioSeguido;
    }
    const std::string& getNicknameSeguidoTemp() const {
        return nicknameSeguidoTemp;
    }
    void setTipoMembresia(int tipo) { tipoMembresia = tipo; }
    void setNicknameSeguidoTemp(const std::string& nickname) {
        this->nicknameSeguidoTemp = nickname;
    }
    void registrarReproduccion(Cancion* c);
    int getCalidadAudio() const;
    bool debeMostrarPublicidad();
    bool seguirUsuario(Usuarios* otroUsuario);
    void reproducirLista(bool aleatoria);
    std::string* getCancionAnterior();
    bool dejarDeSeguir();
    void clearNicknameSeguidoTemp() {//para limpiar
        this->nicknameSeguidoTemp.clear();
    }
    std::string getCancionesPropiasComoString() const;
    std::string getNicknameSeguido() const;
    void establecerSeguimiento(Usuarios* otroUsuario);
    bool cargarFavoritosDesdeString(const std::string& idsCadena);
    bool cargarFavoritosDesdeArchivo(const std::string& rutaArchivo);
    void mostrarInfo(const UdeATunesDataset* dataset) const;
};

#endif // USUARIOS_H
