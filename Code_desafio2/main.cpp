#include "UdeATunesDataset.h"
#include <iostream>
#include <windows.h>
#include <psapi.h>
using namespace std;



size_t getMemoryUsageKB() {
    PROCESS_MEMORY_COUNTERS info;
    GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info));
    return info.WorkingSetSize / 1024;
}


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
    std::cout << "Memoria usada: " << getMemoryUsageKB() << " KB\n";
    cout<<"NUMERO DE ITERACIONES"<< *UdeATunesDataset::iteraciones<<endl;
    return 0;

}
