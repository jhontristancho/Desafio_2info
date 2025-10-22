#include "publicidad.h"
#include <iostream>
#include <cstring>
using namespace std;
// Constructor por defecto
Publicidad::Publicidad()
    : id(0), mensaje(new char[1]), categoria('C'), prioridad(1) {
    mensaje[0] = '\0';
}

// Constructor con parametros - ACTUALIZADO CON ID
Publicidad::Publicidad(int id, const char* mensaje, char categoria)
    : id(id), categoria(categoria) {

    int longitud = strlen(mensaje);
    if (longitud > 500) {
        throw "Error: El mensaje no puede exceder 500 caracteres";
    }

    this->mensaje = new char[longitud + 1];
    strcpy(this->mensaje, mensaje);

    // ✅ CALCULAR PRIORIDAD BASADA EN CATEGORÍA
    if (categoria == 'A') prioridad = 3;
    else if (categoria == 'B') prioridad = 2;
    else prioridad = 1; // 'C' por defecto

    cout << "[DEBUG Publicidad] Creada: ID=" << id << ", Mensaje='" << mensaje
         << "', Categoria=" << categoria << ", Prioridad=" << prioridad << endl;
}
Publicidad::Publicidad(const Publicidad &p)
    : id(p.id), categoria(p.categoria), prioridad(p.prioridad) {

    mensaje = new char[strlen(p.mensaje) + 1];
    strcpy(mensaje, p.mensaje);
}

// Destructor
Publicidad::~Publicidad() {
    delete[] mensaje;
}

// Getters
const char* Publicidad::getMensaje() const {
    return mensaje;
}

char Publicidad::getCategoria() const {
    return categoria;
}

int Publicidad::getPrioridad() const {
    return prioridad;
}

// Setters
void Publicidad::setMensaje(const char* m) {
    int longitud = strlen(m);
    if (longitud > 500) {
        throw "Error: El mensaje no puede exceder 500 caracteres";
    }

    delete[] mensaje;
    mensaje = new char[longitud + 1];
    strcpy(mensaje, m);
}

void Publicidad::setCategoria(char c) {
    categoria = c;
    // ✅ ACTUALIZAR PRIORIDAD AUTOMÁTICAMENTE
    if (c == 'A') prioridad = 3;
    else if (c == 'B') prioridad = 2;
    else prioridad = 1; // 'C' por defecto
}

void Publicidad::setPrioridad(int p) {
    prioridad = p;
}

// Mostrar publicidad - MEJORADO
void Publicidad::mostrarPublicidad() const {
    std::cout << "=== PUBLICIDAD [" << categoria << " - Prioridad: " << prioridad << "] ===" << std::endl;
    std::cout << "   " << mensaje << std::endl;
    std::cout << "========================================" << std::endl;
}

// Método estático para determinar categoría - SEGÚN TU LÓGICA
char Publicidad::determinarCategoria(int numeroAleatorio) {
    if (numeroAleatorio == 0) {
        return 'A';
    } else if (numeroAleatorio == 1 || numeroAleatorio == 2) {
        return 'B';
    } else if (numeroAleatorio >= 3 && numeroAleatorio <= 5) {
        return 'C';
    } else {
        return 'C'; // Por defecto
    }
}
