#include "Vida.h"

#include <QGraphicsTextItem>
#include <QFont>

Vida::Vida(QGraphicsItem *parent): QGraphicsTextItem(parent){
    vidas=3;
    setPlainText(QString("Vida:")+QString::number(vidas));
    setDefaultTextColor(Qt::red);
    setFont(QFont("times",16)); //familia de letra, tamaño
}

void Vida::perdervida()
{
    if (vidas>0){
        vidas--;
        setPlainText(QString("Vida:")+QString::number(vidas));
    }
}

int Vida::getVida()
{
    return vidas;
}
