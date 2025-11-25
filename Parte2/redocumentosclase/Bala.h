#ifndef BALA_H
#define BALA_H

#include <QGraphicsPixmapItem>
#include <QObject>

class Bala: public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
public:
    Bala();
public slots:
    void mover();
};

#endif // BALA_H
