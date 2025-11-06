#include <cmath>
#include "particula.h"
#include <fstream>

Particula::Particula(double xIn, double yIn, double velIn, double angIn, double gravedad){
    x=xIn, y=yIn, g=gravedad, tiempoX=0, tiempoY=0,ang=angIn, dir=1, colX=true;
    velX = velIn * cos(ang * (M_PI / 180));
    velY = velIn * sin(ang * (M_PI / 180));
}

void Particula::actualizarPosicion(double dt, double ancho, double alto, ofstream &archivo){
    // Calcular nuevas posiciones
    double xNuevo = x + velX * tiempoX * dir;
    double yNuevo = y - (velY * tiempoY - 0.5 * g * tiempoY * tiempoY);

    // Guardar en archivo
    archivo << xNuevo << " " << yNuevo << "\n";

    // Verificar colisiones horizontales
    if (xNuevo > ancho) {
        colX = true;
        actualizarValores(xNuevo, yNuevo);
    }

    // Verificar colisiones verticales (rebote en suelo)
    if (yNuevo > alto) {
        yNuevo = alto;
        colX = false;
        actualizarValores(xNuevo, yNuevo);

        // Reducir energía del rebote
        velY *= -0.5;
        if (fabs(velY) < 1.0)
            velY = 0;
    }

    // Actualizar tiempo
    tiempoX += dt;
    tiempoY += dt;
}

void Particula::actualizarValores(double xIn, double yIn) {
    if (colX) {
        dir *= -1;
        x = xIn;
        tiempoX = 0;
    } else {
        x = xIn;
        y = yIn;
        ang = -atan2(velY, velX) * 180 / M_PI;
        tiempoY = 0;
        tiempoX = 0;
    }
}

bool Particula::estaQuieto() const{
    return (velY == 0);
}
