#ifndef PUBLICIDAD_H
#define PUBLICIDAD_H

class Publicidad {
private:
    int id;           // ✅ AGREGAR ID
    char* mensaje;
    char categoria;
    int prioridad;

public:
    // Constructores
    Publicidad();
    Publicidad(int id, const char* mensaje, char categoria); // ✅ AGREGAR ID
    Publicidad(const Publicidad &p);
    // Destructor
    ~Publicidad();
    Publicidad& operator=(const Publicidad& otra);

    // Getters
    int getId() const { return id; }  // ✅ AGREGAR
    const char* getMensaje() const;
    char getCategoria() const;
    int getPrioridad() const;

    // Setters
    void setId(int i) { id = i; }     // ✅ AGREGAR
    void setMensaje(const char* m);
    void setCategoria(char c);
    void setPrioridad(int p);

    // Métodos específicos
    void mostrarPublicidad() const;

    // Método estático para determinar categoría (OPCIONAL - según tu lógica)
    static char determinarCategoria(int numeroAleatorio);
};

#endif
