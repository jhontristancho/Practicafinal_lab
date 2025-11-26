#include "GameWidget.h"
#include <QPainter>
#include <QDebug>
#include <cmath>

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent), sim(500, 350, 0.6), scale(1.0), tiempo_dt(0.016), frames_without_projectile(0)
{
    setMinimumSize(600, 450);

    connect(&timer, &QTimer::timeout, this, &GameWidget::onTick);
    timer.start(int(tiempo_dt * 1000));

    // -------------------------------------------------
    // Escena tipo "casita" para cada jugador
    // -------------------------------------------------
    sim.obstaculos.clear();

    double marginBottom = 10.0;   // separación del piso
    double colSize      = 40.0;   // lado de las columnas (cuadrado)
    double topSize      = 100.0;  // lado del bloque superior

    double baseY = sim.alto - colSize - marginBottom; // y de las columnas
    double topY  = baseY - topSize;                   // y del bloque superior

    // Ancla del techo izquierdo
    double leftTopX  = 80.0;
    // Ancla del techo derecho (simétrico)
    double rightTopX = sim.ancho - leftTopX - topSize;

    // ------- Fortaleza izquierda -------
    // columnas del mismo tamaño, pegadas a los lados del bloque superior
    sim.agregarObstaculo(Obstaculo(leftTopX,                         baseY, colSize, 200)); // columna izq
    sim.agregarObstaculo(Obstaculo(leftTopX + topSize - colSize,     baseY, colSize, 200)); // columna der
    sim.agregarObstaculo(Obstaculo(leftTopX,                         topY,  topSize, 100)); // techo

    // ------- Fortaleza derecha -------
    sim.agregarObstaculo(Obstaculo(rightTopX,                        baseY, colSize, 200)); // columna izq
    sim.agregarObstaculo(Obstaculo(rightTopX + topSize - colSize,    baseY, colSize, 200)); // columna der
    sim.agregarObstaculo(Obstaculo(rightTopX,                        topY,  topSize, 100)); // techo

    // -------------------------------------------------
    // Posiciones de los cañones (más arriba que la casita)
    // -------------------------------------------------
    double cannonOffsetY = 20.0;  // cuánto por encima del techo
    cannon1Y = topY - cannonOffsetY;
    cannon2Y = cannon1Y;

    cannon1X = leftTopX - 25.0;          // un poco a la izquierda de la casita
    cannon2X = rightTopX + topSize + 25.0; // un poco a la derecha de la casita
}

void GameWidget::lanzarProyectil(double angGrados, double velocidad, int jugador) {
    double rad = angGrados * M_PI / 180.0;

    // Origen según el jugador (cañón)
    double inicioX = (jugador == 1) ? cannon1X : cannon2X;
    double inicioY = (jugador == 1) ? cannon1Y : cannon2Y;

    double dir = (jugador == 1) ? 1.0 : -1.0;  // jugador 2 dispara hacia la izquierda

    double vx = velocidad * std::cos(rad) * dir;
    double vy = -velocidad * std::sin(rad); // y crece hacia abajo

    sim.limpiarParticulas();
    Particula p(inicioX, inicioY, vx, vy, 4.0, 6.0);
    p.gravedad = 9.0;
    sim.agregarParticula(p);

    frames_without_projectile = 0;
}

void GameWidget::onTick() {
    sim.step(tiempo_dt);

    // cuenta frames para cuando no hay partículas activas
    bool anyActive = false;
    for (auto &p : sim.particulas) { if (p.activa) { anyActive = true; break; } }
    if (!anyActive) frames_without_projectile++;
    else frames_without_projectile = 0;

    // si no hay partículas activas por muchos frames, consideramos terminado el tiro
    // (esto se usa por la UI si necesitara reaccionar)
    if (frames_without_projectile > 200) {
        // no hacemos nada por ahora, sólo seguimos dibujando
    }

    update();
}

void GameWidget::paintEvent(QPaintEvent * /*event*/) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // calcular escala automática para ajustar widget al "mundo"
    double sx = width() / sim.ancho;
    double sy = height() / sim.alto;
    double S = std::min(sx, sy);

    // fondo
    painter.fillRect(rect(), Qt::white);

    // dibujar borde de la caja (mundo)
    painter.setPen(Qt::black);
    QRectF caja(0,0, sim.ancho*S, sim.alto*S);
    painter.drawRect(caja);

    // ------- Cañones (placeholder gráfico) -------
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::darkGray);

    double cannonSize = 15.0;  // en unidades del mundo
    QRectF c1((cannon1X - cannonSize/2) * S,
              (cannon1Y - cannonSize/2) * S,
              cannonSize * S, cannonSize * S);

    QRectF c2((cannon2X - cannonSize/2) * S,
              (cannon2Y - cannonSize/2) * S,
              cannonSize * S, cannonSize * S);

    painter.drawRect(c1);
    painter.drawRect(c2);

    // ------- Obstáculos -------
    for (auto &o : sim.obstaculos) {
        if (!o.estaVivo()) continue;
        QRectF r(o.x*S, o.y*S, o.lado*S, o.lado*S);
        double vida = o.resistencia;
        QColor color = (vida > 60) ? QColor(100,180,100)
                                   : ((vida>25)? QColor(230,180,80) : QColor(200,80,80));
        painter.fillRect(r, color);
        painter.setPen(Qt::black);
        painter.drawRect(r);
        painter.drawText(r.left()+2, r.top()+12, QString::number(int(o.resistencia)));
    }


    // dibujar particulas
    painter.setPen(Qt::NoPen);
    for (auto &p : sim.particulas) {
        if (!p.activa) continue;
        double px = p.pos.x * S;
        double py = p.pos.y * S;
        double rad = p.radio * S;
        QRectF circ(px - rad, py - rad, rad*2, rad*2);
        painter.setBrush(Qt::blue);
        painter.drawEllipse(circ);
    }
}
