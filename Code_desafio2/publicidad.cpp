#include "publicidad.h"
#include "udeatunesdataset.h"
#include <iostream>
#include <cstring>
using namespace std;
Publicidad::Publicidad()//este es por defecto, para probar y que no flle
    : id(0), mensaje(new char[1]), categoria('C'), prioridad(1) {
    mensaje[0] = '\0';
}
Publicidad::Publicidad(int id, const char* mensaje, char categoria)
    : id(id), categoria(categoria) {
    int longitud = strlen(mensaje);
    if (longitud > 500) {//solo valida que si los mensajes publicitarios que se metan en el dataset sean menores de 500 caracteres
        throw "el mensaje de publicidad no puede ser mayor de 500 caracteres";
    }
    this->mensaje = new char[longitud + 1];
    strncpy(this->mensaje, mensaje, strlen(mensaje));
    this->mensaje[strlen(mensaje)] = '\0';
    if (categoria == 'A') prioridad = 3;//esto es para ver que prioridad tienen los mensajes
    else if (categoria == 'B') prioridad = 2;
    else prioridad = 1;//aca toma el de c por defecto
}
Publicidad::Publicidad(const Publicidad &p)
    : id(p.id), categoria(p.categoria), prioridad(p.prioridad) {
    mensaje = new char[strlen(p.mensaje) + 1];
    strncpy(this->mensaje, mensaje, strlen(mensaje));
    this->mensaje[strlen(mensaje)] = '\0';
}
Publicidad& Publicidad::operator=(const Publicidad& otra) {
    if (this != &otra) {
        id = otra.id;
        categoria = otra.categoria;
        prioridad = otra.prioridad;
        delete[] mensaje;
        int longitud = strlen(otra.mensaje);
        mensaje = new char[longitud + 1];
        strncpy(mensaje, otra.mensaje, longitud);
        mensaje[longitud] = '\0';
    }
    return *this;
}
Publicidad::~Publicidad() {
    delete[] mensaje;
}
const char* Publicidad::getMensaje() const {
    return mensaje;
}
char Publicidad::getCategoria() const {
    return categoria;
}
int Publicidad::getPrioridad() const {
    return prioridad;
}
void Publicidad::setMensaje(const char* m) {
    int longitud = strlen(m);
    if (longitud > 500) {
        throw "el mensaje no puede pasar los 500 caracteres";
    }
    delete[] mensaje;
    mensaje = new char[longitud + 1];
    UdeATunesDataset::actualizarPeakMemory();
    strncpy(this->mensaje, mensaje, strlen(mensaje));
    this->mensaje[strlen(mensaje)] = '\0';
}
void Publicidad::setCategoria(char c) {
    categoria = c;
    if (c == 'A') prioridad = 3;
    else if (c == 'B') prioridad = 2;
    else prioridad = 1;
}
void Publicidad::setPrioridad(int p) {
    prioridad = p;
}
void Publicidad::mostrarPublicidad() const {
    cout << "publicidad " << categoria << " - Prioridad: " << prioridad << endl;
    cout << "   " << mensaje << endl;
   cout << "========================================" << std::endl;
}
char Publicidad::determinarCategoria(int numeroAleatorio) {
    if (numeroAleatorio == 0) {
        return 'A';
    } else if (numeroAleatorio == 1 || numeroAleatorio == 2) {
        return 'B';
    } else if (numeroAleatorio >= 3 && numeroAleatorio <= 5) {
        return 'C';
    } else {
        return 'C';
    }
}
