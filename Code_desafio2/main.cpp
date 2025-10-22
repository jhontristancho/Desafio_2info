#include <iostream>
#include "UdeATunesDataset.h"
#include "Usuarios.h"
#include "Cancion.h"
#include "Artista.h"
#include "Album.h"
#include "Publicidad.h"
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

void mostrarMenuPrincipal();
void probarCargaDatos(UdeATunesDataset& dataset);
void probarUsuariosYFavoritos(UdeATunesDataset& dataset);
void probarReproduccionConPublicidad(UdeATunesDataset& dataset);
void probarBusquedaCanciones(UdeATunesDataset& dataset);
void probarPublicidades(UdeATunesDataset& dataset);
void probarArtistasYAlbumes(UdeATunesDataset& dataset);

int main() {
    cout << "============================================" << endl;
    cout << "       PRUEBA COMPLETA UDEATUNES DATASET   " << endl;
    cout << "============================================" << endl;

    // Inicializar semilla para números aleatorios
    srand(time(0));

    UdeATunesDataset dataset;  // UN SOLO OBJETO PARA TODAS LAS PRUEBAS

    // PRIMERO CARGAR TODOS LOS DATOS UNA SOLA VEZ
    cout << "\n⚠️  Cargando datos iniciales..." << endl;
    if (!dataset.cargarDatos()) {
        cout << "❌ Error al cargar datos iniciales. Algunas funciones pueden no trabajar." << endl;
    }
    cout << "✅ Datos iniciales cargados\n" << endl;

    int opcion;
    do {
        mostrarMenuPrincipal();
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore(); // Limpiar buffer

        switch(opcion) {
        case 1:
            probarCargaDatos(dataset);
            break;
        case 2:
            probarUsuariosYFavoritos(dataset);
            break;
        case 3:
            probarReproduccionConPublicidad(dataset);
            break;
        case 4:
            probarBusquedaCanciones(dataset);
            break;
        case 5:
            probarPublicidades(dataset);
            break;
        case 6:
            probarArtistasYAlbumes(dataset);
            break;
        case 0:
            cout << "Saliendo del programa..." << endl;
            break;
        default:
            cout << "Opcion no valida!" << endl;
        }

        if (opcion != 0) {
            cout << "\nPresione Enter para continuar...";
            cin.get();
        }

    } while (opcion != 0);

    return 0;
}

void mostrarMenuPrincipal() {
    cout << "\n========== MENU PRINCIPAL ==========" << endl;
    cout << "1. Probar carga completa de datos" << endl;
    cout << "2. Probar usuarios y listas de favoritos" << endl;
    cout << "3. Probar reproduccion con publicidad" << endl;
    cout << "4. Probar busqueda de canciones" << endl;
    cout << "5. Probar sistema de publicidades" << endl;
    cout << "6. Probar artistas y albumes" << endl;
    cout << "0. Salir" << endl;
    cout << "=====================================" << endl;
}

void probarCargaDatos(UdeATunesDataset& dataset) {
    cout << "\n=== PRUEBA DE CARGA DE DATOS ===" << endl;

    // Ya cargamos los datos al inicio, solo mostrar estado
    cout << "✅ Datos ya cargados previamente" << endl;
    cout << "Artistas: " << dataset.getNumArtistas() << endl;
    cout << "Usuarios: " << dataset.getNumUsuarios() << endl;
    cout << "Publicidades: " << dataset.getNumPublicidades() << endl;

    // Mostrar info del primer usuario
    if (dataset.getNumUsuarios() > 0) {
        cout << "\n--- INFO PRIMER USUARIO ---" << endl;
        dataset.getUsuarioAt(0)->mostrarInfo(&dataset);
    }
}

void probarUsuariosYFavoritos(UdeATunesDataset& dataset) {
    cout << "\n=== PRUEBA USUARIOS Y FAVORITOS ===" << endl;

    int numUsuarios = dataset.getNumUsuarios();
    if (numUsuarios == 0) {
        cout << "❌ No hay usuarios cargados" << endl;
        return;
    }

    cout << "Total de usuarios: " << numUsuarios << endl;

    // Mostrar información de todos los usuarios
    for (int i = 0; i < numUsuarios; i++) {
        Usuarios* usuario = dataset.getUsuarioAt(i);
        if (usuario) {
            cout << "\n--- Usuario " << (i+1) << " ---" << endl;
            cout << "Nickname: " << usuario->getNickname() << endl;
            cout << "Tipo: " << (usuario->getTipoMembresia() == TIPO_PREMIUM ? "Premium" : "Standard") << endl;
            cout << "Ciudad: " << usuario->getCiudad() << endl;
            usuario->mostrarInfo(&dataset);
        }
    }
}

void probarReproduccionConPublicidad(UdeATunesDataset& dataset) {
    cout << "\n=== PRUEBA REPRODUCCION CON PUBLICIDAD ===" << endl;

    if (dataset.getNumUsuarios() == 0) {
        cout << "❌ No hay usuarios cargados" << endl;
        return;
    }

    // Buscar un usuario estándar para probar publicidad
    Usuarios* usuarioStandard = nullptr;
    for (int i = 0; i < dataset.getNumUsuarios(); i++) {
        if (dataset.getUsuarioAt(i)->getTipoMembresia() == TIPO_STANDARD) {
            usuarioStandard = dataset.getUsuarioAt(i);
            break;
        }
    }

    if (!usuarioStandard) {
        cout << "❌ No se encontro usuario standard para probar publicidad" << endl;
        return;
    }

    cout << "Usuario seleccionado: " << usuarioStandard->getNickname() << " (Standard)" << endl;

    // Buscar algunas canciones para reproducir
    vector<string> idsCanciones = {"000010101", "000010102", "000010103", "000020101"};

    cout << "\n--- SIMULANDO REPRODUCCION ---" << endl;
    for (int i = 0; i < idsCanciones.size(); i++) {
        Cancion* cancion = dataset.buscarCancion(idsCanciones[i]);
        if (cancion) {
            cout << "\n🎵 Reproduciendo cancion " << (i+1) << ": " << cancion->getNombre() << endl;

            int calidad = usuarioStandard->getCalidadAudio();
            cancion->reproducir(calidad);
            usuarioStandard->registrarReproduccion(cancion);

            // Verificar si debe mostrar publicidad
            if (usuarioStandard->debeMostrarPublicidad()) {
                cout << "🔔 Debe mostrar publicidad despues de " << usuarioStandard->getContadorReproducciones() << " reproducciones!" << endl;

                // Obtener y mostrar publicidad
                Publicidad* pub = dataset.obtenerPublicidadAleatoria();
                if (pub) {
                    cout << "\n=== PUBLICIDAD ===" << endl;
                    pub->mostrarPublicidad();
                }
            }
        } else {
            cout << "❌ Cancion no encontrada: " << idsCanciones[i] << endl;
        }
    }
}

void probarBusquedaCanciones(UdeATunesDataset& dataset) {
    cout << "\n=== PRUEBA BUSQUEDA DE CANCIONES ===" << endl;

    vector<string> idsPrueba = {"000010101", "000010102", "000020101", "999999999"};

    for (const string& id : idsPrueba) {
        Cancion* cancion = dataset.buscarCancion(id);
        if (cancion) {
            cout << "✅ CANCION ENCONTRADA:" << endl;
            cout << "   ID: " << cancion->getIdCompleto() << endl;
            cout << "   Nombre: " << cancion->getNombre() << endl;
            cout << "   Duracion: " << cancion->getDuracion() << " minutos" << endl;
            if (cancion->getArtista()) {
                cout << "   Artista: " << cancion->getArtista()->getNombre() << endl;
            }
            if (cancion->getAlbum()) {
                cout << "   Album: " << cancion->getAlbum()->getNombre() << endl;
            }
        } else {
            cout << "❌ Cancion NO encontrada: " << id << endl;
        }
        cout << endl;
    }
}

void probarPublicidades(UdeATunesDataset& dataset) {
    cout << "\n=== PRUEBA SISTEMA DE PUBLICIDADES ===" << endl;

    // Verificar que hay publicidades cargadas
    if (dataset.getNumPublicidades() == 0) {
        cout << "❌ No hay publicidades cargadas en el dataset" << endl;
        cout << "   Ejecuta primero 'Probar carga completa de datos' (opción 1)" << endl;
        return;
    }

    cout << "✅ Publicidades disponibles: " << dataset.getNumPublicidades() << endl;

    // Probar obtención de publicidades aleatorias
    cout << "Generando 10 publicidades aleatorias..." << endl;
    cout << "(Probabilidades: A=3x, B=2x, C=1x)" << endl << endl;

    int publicidadesMostradas = 0;
    for (int i = 0; i < 10; i++) {
        Publicidad* pub = dataset.obtenerPublicidadAleatoria();
        if (pub) {
            cout << "Publicidad " << (i+1) << ":" << endl;
            pub->mostrarPublicidad();
            publicidadesMostradas++;
        } else {
            cout << "❌ No se pudo obtener publicidad " << (i+1) << endl;
        }
    }

    cout << "✅ Se mostraron " << publicidadesMostradas << "/10 publicidades correctamente" << endl;
}

void probarArtistasYAlbumes(UdeATunesDataset& dataset) {
    cout << "\n=== PRUEBA ARTISTAS Y ALBUMES ===" << endl;

    int numArtistas = dataset.getNumArtistas();
    if (numArtistas == 0) {
        cout << "❌ No hay artistas cargados" << endl;
        return;
    }

    cout << "Total de artistas: " << numArtistas << endl;

    // Mostrar información de artistas y sus álbumes
    for (int i = 0; i < numArtistas; i++) {
        Artista* artista = dataset.getArtistaAt(i);
        if (artista) {
            cout << "\n🎤 ARTISTA: " << artista->getNombre() << " (ID: " << artista->getIdArtista() << ")" << endl;

            // Mostrar álbumes del artista
            int numAlbumes = artista->getNumAlbumes();
            cout << "   Albumes: " << numAlbumes << endl;

            for (int j = 0; j < numAlbumes; j++) {
                Album* album = artista->getAlbumAt(j);
                if (album) {
                    // Mostrar información básica del álbum
                    cout << "   📀 Album: " << album->getNombre();

                    // Mostrar año si está disponible (manejar posible error)
                    try {
                        cout << " (" << album->getAnio() << ")";
                    } catch (...) {
                        cout << " (Año no disponible)";
                    }
                    cout << endl;

                    // Mostrar canciones del álbum
                    int numCanciones = album->getNumCanciones();
                    cout << "      Canciones: " << numCanciones << endl;

                    for (int k = 0; k < numCanciones; k++) {
                        Cancion* cancion = album->getCancionAt(k);
                        if (cancion) {
                            cout << "      🎵 " << cancion->getNombre() << " (" << cancion->getDuracion() << " min)" << endl;
                        }
                    }
                }
            }
        }
    }
}
