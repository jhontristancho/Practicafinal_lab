TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        Obstaculo.cpp \
        Simulador.cpp \
        main.cpp \
        particula.cpp \
        vector2.cpp

HEADERS += \
    Obstaculo.h \
    Simulador.h \
    particula.h \
    vector2.h

DISTFILES += \
    salida.txt
