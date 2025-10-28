#include "creditos.h"
#include <iostream>
#include <udeatunesdataset.h>
using namespace std;
//recordemos que esto es mas pesado en futuro
Creditos::Creditos()
    : productores(nullptr), numProductores(0),
    musicos(nullptr), numMusicos(0),
    compositores(nullptr), numCompositores(0) {}
Creditos::~Creditos() {
    delete[] productores;
    delete[] musicos;
    delete[] compositores;
}
Creditos::Creditos(const Creditos& otra)
    : numProductores(otra.numProductores), numMusicos(otra.numMusicos),
    numCompositores(otra.numCompositores) {
    auto deepCopy = [](Colaborador*& dest, const Colaborador* src, int num) {
        if (num > 0 && src != nullptr) {
            dest = new Colaborador[num];
            for (int i = 0; i < num; ++i) {
                dest[i] = src[i];
            }
        } else {
            dest = nullptr;
        }
    };
    deepCopy(productores, otra.productores, numProductores);
    deepCopy(musicos, otra.musicos, numMusicos);
    deepCopy(compositores, otra.compositores, numCompositores);
}
void Creditos::setProductores(const Colaborador* prod, int num) {
    delete[] productores;
    productores = nullptr;
    if (num > 0 && prod != nullptr) {
        productores = new Colaborador[num];
        for (int i = 0; i < num; ++i) {
            productores[i] = prod[i];
        }
    }
    numProductores = num;
}
void Creditos::setMusicos(const Colaborador* mus, int num) {
    delete[] musicos;
    musicos = nullptr;
    if (num > 0 && mus != nullptr) {
        musicos = new Colaborador[num];
        for (int i = 0; i < num; ++i) {
            musicos[i] = mus[i];
        }
    }
    numMusicos = num;
}
void Creditos::setCompositores(const Colaborador* comp, int num) {
    delete[] compositores;
    compositores = nullptr;
    if (num > 0 && comp != nullptr) {
        compositores = new Colaborador[num];
        for (int i = 0; i < num; ++i) {
            compositores[i] = comp[i];
        }
    }
    numCompositores = num;
}
void Creditos::mostrarCreditos() const {//en la hoja todavia no se habla de mostrar creditos pero si nos piden la funcionalidad aqui estaria
    auto imprimirLista = [](const Colaborador* lista, int num, const std::string& titulo) {
    cout<< titulo << " " << num << ":";
        if (num == 0) {
        cout << "ninguno"<<endl;
            return;
        }
        for (int i = 0; i < num; ++i) {
        cout << lista[i].getNombre() << " " << lista[i].getApellido()
            << "cod: " << lista[i].getCodigoAfiliacion()
            << (i < num - 1 ? " | " : "");
        }
        cout <<endl;
    };
    cout << "creditos"<< endl;
    imprimirLista(productores, numProductores, "productores");
    imprimirLista(musicos, numMusicos, "musicos");
    imprimirLista(compositores, numCompositores, "compositores");
}
