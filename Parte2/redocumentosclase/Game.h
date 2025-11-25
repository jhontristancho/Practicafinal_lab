#ifndef GAME_H
#define GAME_H

#include <QGraphicsScene>
#include "Jugador.h"
#include "Puntaje.h"
#include "Vida.h"
#include <QGraphicsView>
#include <QWidget>

class Game:public QGraphicsView{
public:
    Game(QWidget * parent=0);
    QGraphicsScene * scene;
    Jugador * jugador;
    Puntaje * puntaje;
    Vida * vida;
};

#endif // GAME_H
