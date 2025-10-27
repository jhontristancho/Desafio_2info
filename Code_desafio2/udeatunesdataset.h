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
    Artista* artistas;//estos son los principales, los que van apuntr a cada uno
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
    std::string* lineasAlbumes;//para cargar los daticos
    std::string* lineasCanciones;
    std::string* lineasCreditos;
    int numLineasAlbumes;
    int capacidadLineasAlbumes;
    int numLineasCanciones;
    int capacidadLineasCanciones;
    int numLineasCreditos;
    int capacidadLineasCreditos;
    long* idsColaboradores;//futuro
    void resizeArtistas();//darle el doble de la reserva
    void resizeColaboradores();
    void resizeUsuarios();
    void resizePublicidades();
    void obtenerSubCampos(const std::string& campo, char delimitador, std::string*& resultados, int& numResultados) const;
    float parseDuracion(const std::string& duracionStr) const;
    bool cargarArtistas();//cargar datos
    bool cargarColaboradores();
    bool cargarUsuarios();
    bool cargarAlbumesYCanciones();
    bool cargarPublicidades();
    bool cargarPublicidadesPorDefecto();
    bool cargarListasDeFavoritos(const std::string& rutaArchivo2);
    bool procesarAlbum(const std::string& lineaAlbum, int indice);
    bool procesarCancionesParaAlbum(Album& album, Artista* artista, const std::string& idAlbumCompleto);
    bool procesarCreditosParaCancion(Cancion* cancion, const std::string& idCreditoStr);
public:
    static long* iteraciones;//esto es para la metrica
        static size_t peakMemoryUsage;
    UdeATunesDataset();
    ~UdeATunesDataset();
    bool cargarDatos();
    static std::string* guardarDinamico(const std::string& nombreArchivo, int& numLineas, int& capacidadFinal, int capacidadInicial = 10);
    bool guardarListasDeFavoritos(const std::string& rutaArchivo2);
    int getNumArtistas() const { return numArtistas; }
    Artista* getArtistaAt(int idx) const {
        return (idx >= 0 && idx < numArtistas) ? &artistas[idx] : nullptr;
    }
    Colaborador* encontrarColaborador(const std::string& idColaborador) const;
    Artista* getArtista(const std::string& id) const;
    int getNumColaboradores() const { return numColaboradores; }
    Colaborador* getColaboradores() const { return colaboradores; }
    int getNumUsuarios() const { return numUsuarios; }
    Usuarios* getUsuarioAt(int idx) const {
        return (idx >= 0 && idx < numUsuarios) ? &usuarios[idx] : nullptr;
    }
    int getNumPublicidades() const { return numPublicidades; }
    static void actualizarPeakMemory();
    static size_t getPeakMemoryUsageKB();
    static size_t getMemoryUsageKB();
    const std::string* getLineasCanciones() const { return lineasCanciones; }
    int getNumLineasCanciones() const { return numLineasCanciones; }
    Cancion* buscarCancion(const std::string& id) const;//funciones mas admi
    Cancion* reproducirCancionAleatoria(Usuarios* usuario);
Usuarios* buscarUsuario(const std::string& nickname);
    void reproducirListaFavoritos(Usuarios* usuario, bool aleatoria);
    void menuFavoritosPremium(Usuarios* usuario);
    void iniciarSesionYReproducir();
    Publicidad* obtenerPublicidadAleatoria();
    void mostrarReproduccion(const std::string& idCancion, Usuarios* usuario, bool mostrarPublicidad);//para imprimir, despligue
    std::string obtenerCampo(const std::string& linea, int campo) const;
    std::string buscarLineaPorID(const std::string* lineas, int numLineas, const std::string& idBuscado, int campoID) const;
};

#endif // UDEATUNES_DATASET_H
