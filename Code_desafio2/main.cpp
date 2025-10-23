#include "UdeATunesDataset.h"
#include <iostream>
using namespace std;

int main() {
    UdeATunesDataset app;

    if (!app.cargarDatos()) {
        cout << "❌ Error al cargar los datos del sistema.\n";
        return 1;
    }

    app.iniciarSesionYReproducir();
    return 0;
}
