#include "UdeATunesDataset.h"
#include <iostream>
using namespace std;
int main() {
    UdeATunesDataset dataset;

    if (!dataset.cargarDatos()) {
        cerr << "hubo un error en la carga de los datos" << endl;
    }
    dataset.iniciarSesionYReproducir();
    dataset.guardarListasDeFavoritos("listadefavoritos.txt");
    cout << "numero de iteraciones total: " << *UdeATunesDataset::iteraciones << endl;
   cout << "total de memoria utilizada: " << UdeATunesDataset::getPeakMemoryUsageKB() << " KB\n";
    return 0;
}
