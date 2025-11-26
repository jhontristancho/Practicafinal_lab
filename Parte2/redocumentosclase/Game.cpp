#include "Game.h"
#include <QGraphicsScene>
#include "Jugador.h"
#include <QGraphicsView>
#include <QTimer>
#include <QPixmap>


Game::Game(QWidget *parent): QGraphicsView (parent){
    //crear escenario
    scene = new QGraphicsScene();
    QPixmap bgr(":/imagenes/fondo.jpg");

    //Crear un Item
    jugador = new Jugador();
    jugador ->setPixmap(QPixmap(":/imagenes/jugador3.png"));
    scene ->addItem(jugador);

    jugador->setFlag(QGraphicsItem::ItemIsFocusable);
    jugador->setFocus();

    QGraphicsView * view = new QGraphicsView(scene);

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    view ->setBackgroundBrush(bgr);
    view -> show();
    view -> setFixedSize(800,600);
    scene ->setSceneRect(0,0,800,600);
    jugador ->setPos(view->width()/2,view->height() - 100);

    puntaje= new Puntaje();
    scene ->addItem(puntaje);

    vida=new Vida();
    vida->setPos(x(),y()+25);
    scene ->addItem(vida);

    QTimer * timer= new QTimer();
    QObject::connect(timer,SIGNAL(timeout()),jugador,SLOT(aparecer()));
    timer ->start(2000);
}
