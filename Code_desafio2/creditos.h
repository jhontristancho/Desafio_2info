#ifndef CREDITOS_H
#define CREDITOS_H
#include <string>
#include "Colaborador.h"
class Creditos {
private:
    Colaborador* productores;
    int numProductores;
    Colaborador* musicos;
    int numMusicos;
    Colaborador* compositores;
    int numCompositores;
public:
    Creditos();
    ~Creditos();
    Creditos(const Creditos& otra);
    Creditos& operator=(const Creditos& otra);
    void setMusicos(const Colaborador* mus, int num);
    void setCompositores(const Colaborador* comp, int num);
    void setProductores(const Colaborador* prod, int num);
    void mostrarCreditos() const;
};

#endif // CREDITOS_H
