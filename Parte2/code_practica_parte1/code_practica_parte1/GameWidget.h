#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QTimer>
#include "Simulador.h"

class GameWidget : public QWidget {
    Q_OBJECT
public:
    explicit GameWidget(QWidget *parent = nullptr);

    // lanzar proyectil desde la UI
    void lanzarProyectil(double angGrados, double velocidad, int jugador);

    void setScale(double s) { scale = s; update(); }

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onTick();

private:
    Simulador sim;
    QTimer timer;
    double scale;   // pixeles por unidad física
    double tiempo_dt;
    int frames_without_projectile;
    double cannon1X, cannon1Y;
    double cannon2X, cannon2Y;
};

#endif // GAMEWIDGET_H
