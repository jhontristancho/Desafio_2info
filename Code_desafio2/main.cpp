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
        cerr << "hubo un error en la carga de los datos" << endl;
    }
    dataset.iniciarSesionYReproducir();
    dataset.guardarListasDeFavoritos("listadefavoritos.txt");//al cerrarse guarda la lista en el txt
cout << "memoria usada: " << getMemoryUsageKB() << " KB\n";
    cout<<"numero de iteraciones: "<< *UdeATunesDataset::iteraciones<<endl;
    return 0;

}
