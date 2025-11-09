TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        Obstaculo.cpp \
        Simulador.cpp \
        main.cpp

HEADERS += \
    Obstaculo.h \
    Simulador.h

DISTFILES += \
    salida.txt
