#ifndef PUBLICIDAD_H
#define PUBLICIDAD_H

class Publicidad {
private:
    int id;
    char* mensaje;
    char categoria;
    int prioridad;
public:
    Publicidad();
    Publicidad(int id, const char* mensaje, char categoria);
    Publicidad(const Publicidad &p);
    ~Publicidad();
    Publicidad& operator=(const Publicidad& otra);
    int getId() const { return id; }
    const char* getMensaje() const;
    char getCategoria() const;
    int getPrioridad() const;
    void setId(int i) { id = i; }
    void setMensaje(const char* m);
    void setCategoria(char c);
    void setPrioridad(int p);
    void mostrarPublicidad() const;
    static char determinarCategoria(int numeroAleatorio);
};

#endif
