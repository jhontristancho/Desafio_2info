TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        album.cpp \
        artista.cpp \
        cancion.cpp \
        creditos.cpp \
        listafavoritos.cpp \
        main.cpp \
        publicidad.cpp \
        udeatunesdataset.cpp \
        usuarios.cpp

HEADERS += \
    Colaborador.h \
    album.h \
    artista.h \
    cancion.h \
    creditos.h \
    listafavoritos.h \
    publicidad.h \
    udeatunesdataset.h \
    usuarios.h
