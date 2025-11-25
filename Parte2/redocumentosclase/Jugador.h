#ifndef JUGADOR_H
#define JUGADOR_H

#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QObject>

/*Eventos keyPressEvent -> (QKeyEvent)
 * Eventos sistema de propagación (En que orden se perciben los eventos)
*/

class Jugador:public QObject,public QGraphicsPixmapItem{
    Q_OBJECT
public:
    void keyPressEvent(QKeyEvent * event);
public slots:
    void aparecer();
};

#endif // JUGADOR_H
