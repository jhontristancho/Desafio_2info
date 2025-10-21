#ifndef MANEJO_DATOS_H
#define MANEJO_DATOS_H
#include <string>
std::string* guardarDinamico(const std::string& nombreArchivo, int& numLineas, int& capacidadFinal, int capacidadInicial = 10);
std::string obtenerCampo(const std::string& linea, int campo);
std:: string* redimensionarArreglo(std::string* viejo, int capacidadVieja, int nuevaCapacidad);
std:: string buscarCancion(std::string* lineas, int num_lineas, std::string id_buscado_str, int campo);
#endif // MANEJO_DATOS_H
