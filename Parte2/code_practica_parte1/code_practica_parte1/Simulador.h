#ifndef SIMULADOR_H
#define SIMULADOR_H

#include <vector>
#include "Particula.h"
#include "Obstaculo.h"

class Simulador {
public:
    double ancho, alto;   // tamaño del área en unidades lógicas
    double c;             // coeficiente de restitución para obstaculos
    double K_dano;        // constante para calcular daño = K * momento

    std::vector<Particula> particulas;
    std::vector<Obstaculo> obstaculos;

    Simulador(double ancho_ = 500, double alto_ = 350, double coef_rest = 0.6);

    void agregarParticula(const Particula& p);
    void limpiarParticulas();
    void agregarObstaculo(const Obstaculo& o);

    // step avanza la simulación dt segundos (actualiza posiciones, colisiones)
    void step(double dt);

private:
    void colisionesPared(Particula& p);
    void colisionesObstaculos(Particula& p);
    void colisionesParticulas();
};

#endif
