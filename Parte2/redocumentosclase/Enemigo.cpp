#include "Enemigo.h"
#include "Game.h"
#include <stdlib.h> //rand()
#include <QTimer>
#include <QGraphicsScene>
#include <QDebug>

extern Game * game;

Enemigo::Enemigo() {
    int rand_num = rand() % 700;
    setPos(rand_num,x());
    setPixmap(QPixmap(":/imagenes/enemigo3.png"));
    setTransformOriginPoint(50,50);
    setRotation(180);
    QTimer * timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(mover()));
    timer ->start(50);
}

void Enemigo::mover(){
    setPos(x(),y()+5);
    if (pos().y()>800){
        game->vida->perdervida();
        scene()-> removeItem(this);
        delete this;
        qDebug()<<"Enemigo Eliminado";

    }
}
