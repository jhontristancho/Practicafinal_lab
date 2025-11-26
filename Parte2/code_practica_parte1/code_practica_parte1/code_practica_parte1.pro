QT += widgets
CONFIG += c++17

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    GameWidget.cpp \
    Simulador.cpp \
    Particula.cpp \
    Obstaculo.cpp \
    Vector2.cpp

HEADERS += \
    MainWindow.h \
    GameWidget.h \
    Simulador.h \
    Particula.h \
    Obstaculo.h \
    Vector2.h

# Añade esto si usas Windows y necesitas el subsystem console para ver cout
# CONFIG += console

RESOURCES += \
    multimedia.qrc

