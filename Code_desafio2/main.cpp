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
   cout << "este fue lo maximo que alcanzo, pero hay que tener en cuenta que incluye todo : " << UdeATunesDataset::getPeakMemoryUsageKB() << " KB\n";
    return 0;
}
