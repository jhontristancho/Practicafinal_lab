#include "Puntaje.h"
#include <QGraphicsTextItem>
#include <QFont>

Puntaje::Puntaje(QGraphicsItem *parent): QGraphicsTextItem(parent){
    puntos=0;
    setPlainText(QString("Puntaje:")+QString::number(puntos));
    setDefaultTextColor(Qt::blue);
    setFont(QFont("times",16)); //familia de letra, tamaño
}

void Puntaje::aumentar()
{
    puntos++;
    setPlainText(QString("Puntaje:")+QString::number(puntos));

}

int Puntaje::getPuntaje()
{
    return puntos;
}
