#ifndef COLABORADOR_H
#define COLABORADOR_H
#include <string>
class Colaborador {
private:
    std::string nombre;
    std::string apellido;
    std::string codigoAfiliacion;
public:
    Colaborador(const std::string& nom = "", const std::string& ape = "", const std::string& cod = "")
        : nombre(nom), apellido(ape), codigoAfiliacion(cod) {}
    const std::string& getNombre() const { return nombre; }
    const std::string& getApellido() const { return apellido; }
    const std::string& getCodigoAfiliacion() const { return codigoAfiliacion; }
};
#endif // COLABORADOR_H
