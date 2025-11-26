#ifndef PUNTAJE_H
#define PUNTAJE_H

#include <QGraphicsTextItem>
#include <QGraphicsItem>

class Puntaje: public QGraphicsTextItem{
public:
    Puntaje(QGraphicsItem * patent=0);
    void aumentar();
    int getPuntaje();
private:
    int puntos;
};

#endif // PUNTAJE_H
