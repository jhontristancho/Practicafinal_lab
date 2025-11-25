#ifndef VIDA_H
#define VIDA_H

#include <QGraphicsTextItem>
#include <QGraphicsItem>

class Vida: public QGraphicsTextItem{
public:
    Vida(QGraphicsItem * patent=0);
    void perdervida();
    int getVida();
private:
    int vidas;
};

#endif // VIDA_H
