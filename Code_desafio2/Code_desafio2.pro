TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        album.cpp \
        artista.cpp \
        cancion.cpp \
        creditos.cpp \
        main.cpp \
        udeatunesdataset.cpp

HEADERS += \
    Colaborador.h \
    album.h \
    artista.h \
    cancion.h \
    creditos.h \
    udeatunesdataset.h
