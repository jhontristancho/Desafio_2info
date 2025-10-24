#include "UdeATunesDataset.h"
#include <iostream>
using namespace std;

int main() {
    UdeATunesDataset dataset;
    if (!dataset.cargarDatos()) {
        cerr << "Error cargando datos." << endl;
        // seguir o salir según quieras
    }

    // Inicia sesión y reproducción (tu método ya maneja la interacción)
    dataset.iniciarSesionYReproducir();

    // Al cerrar, guardar listas de favoritos
    dataset.guardarListasDeFavoritos("listadefavoritos.txt");

    cout << "Saliendo. Guardado completado." << endl;
    return 0;
}
