#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

string* redimensionarArreglo(string* viejo, int capacidadVieja, int nuevaCapacidad) {
    string* nuevo = new string[nuevaCapacidad];
    for (int i = 0; i < capacidadVieja; i++) {
        nuevo[i] = viejo[i];
    }

    delete[] viejo;
    return nuevo;
}

string* guardarDinamico(const string& nombreArchivo, int& numLineas, int& capacidadFinal, int capacidadInicial = 10) {
    ifstream archivo(nombreArchivo);
    string linea;
    int capacidad = capacidadInicial;
    numLineas = 0;
    string* lineas = new string[capacidad];

    while (getline(archivo, linea)) {
        if (numLineas >= capacidad) {
            lineas = redimensionarArreglo(lineas, capacidad, capacidad * 2);
            capacidad *= 2;
        }

        lineas[numLineas++] = linea; // ✅ Guardar la línea leída
    }

    archivo.close();
    capacidadFinal = capacidad; // 📦 Guardamos la capacidad final para referencia
    return lineas;
}


string obtenerCampo(const string& linea, int campo) {
    string resultado = "";
    int columna = 0;

    for (size_t i = 0; i < linea.length(); i++) {
        if (linea[i] == ',') {
            columna++;
            continue;
        }
        if (columna == campo) {
            resultado += linea[i];
        }
    }

    return resultado;
}

string buscarCancion(string* lineas, int num_lineas, string id_buscado_str, int campo) {
    long id_buscado = stol(id_buscado_str);
    int izquierda = 0;
    int derecha = num_lineas - 1;

    while (izquierda <= derecha) {
        int medio = (izquierda + derecha) / 2;
        long id_actual = stol(obtenerCampo(lineas[medio], campo));

        if (id_actual == id_buscado) {
            return lineas[medio]; // Devuelve toda la línea
        }
        else if (id_actual < id_buscado) {
            izquierda = medio + 1; // Buscar derecha
        } else {
            derecha = medio - 1; // Buscar izquierda
        }
    }

    return ""; // No se encontró
}

