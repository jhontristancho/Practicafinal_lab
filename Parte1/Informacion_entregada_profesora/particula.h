#ifndef PARTICULA_H
#define PARTICULA_H

#include <cmath>
#include <fstream>
using namespace std;

class Particula {
private:
    double x, y;
    double velX, velY;
    double g;
    double tiempoX, tiempoY;
    double ang;
    int dir;
    bool colX;

public:
    Particula(double xIn, double yIn, double velIn, double angIn, double gravedad);
    void actualizarPosicion(double dt, double ancho, double alto,ofstream &archivo);
    void actualizarValores(double xIn, double yIn);
    bool estaQuieto() const;
};

#endif // PARTICULA_H
