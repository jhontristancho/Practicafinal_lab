#ifndef ENEMIGO_H
#define ENEMIGO_H

#include <QGraphicsPixmapItem>
#include <QObject>

class Enemigo:public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
public:
    Enemigo();
public slots:
    void mover();
};

#endif // ENEMIGO_H
