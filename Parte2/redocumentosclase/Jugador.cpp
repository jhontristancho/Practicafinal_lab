#include "Jugador.h"
#include "Bala.h"
#include "Enemigo.h"
#include <QDebug>
#include <QGraphicsScene>


void Jugador::keyPressEvent(QKeyEvent *event)
{
    //qDebug() << "Jugador sabe que presionaste un boton";
    if (event ->key()==Qt::Key_Left){
        if (pos().x()>0){
            setPos(x()-10,y());
        }
    }
    else if (event ->key()==Qt::Key_Right){
        if (pos().x()-QPixmap().width()<700){
            setPos(x()+10,y());
        }
    }
    else if (event ->key()==Qt::Key_Space){
        Bala * bala = new Bala();
        bala ->setPos(x(),y());
        scene()->addItem(bala);

        //qDebug() << "Bala creada";

    }
}

void Jugador::aparecer(){
    Enemigo * enemigo =new Enemigo();
    scene()->addItem(enemigo);
}
