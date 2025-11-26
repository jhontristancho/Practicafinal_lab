#include "GameWidget.h"
#include <QPainter>
#include <QDebug>
#include <cmath>
#include <QPixmap>
#include <algorithm>

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent), sim(500, 350, 0.6), scale(1.0), tiempo_dt(0.016), frames_without_projectile(0)
    ,currentAngle(30.0), currentSpeed(120.0), currentPlayer(1)
{
    setMinimumSize(600, 450);

    connect(&timer, &QTimer::timeout, this, &GameWidget::onTick);
    timer.start(int(tiempo_dt * 1000));

    //fondo
    backgroundTexture = QPixmap(":/imagenes/multimedia/imagenes/fondo.png" );


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


    obstacleTexture = QPixmap(":/imagenes/multimedia/imagenes/obstaculo.png");

    // -------------------------------------------------
    // Posiciones de los cañones
    // -------------------------------------------------

    cannonRTexture.load(":/imagenes/multimedia/imagenes/canon1.png");
    cannonLTexture.load(":/imagenes/multimedia/imagenes/canon2.png");
    projectileTexture.load(":/imagenes/multimedia/imagenes/bala.png");

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

    double dir = (jugador == 1) ? 1.0 : -1.0;

    double vx = velocidad * std::cos(rad) * dir;
    double vy = -velocidad * std::sin(rad); // y crece hacia abajo

    sim.limpiarParticulas();
    Particula p(inicioX, inicioY, vx, vy, 4.0, 6.0);
    p.gravedad = 9.0;

    p.activa = true;
    sim.agregarParticula(p);

    if (!timer.isActive()) {
        timer.start(int(tiempo_dt * 1000));
    }

    frames_without_projectile = 0;
}

void GameWidget::setShotParameters(double angle, double speed, int player) {
    currentAngle = angle;
    currentSpeed = speed;
    currentPlayer = player;
    update();
}

void GameWidget::onTick() {
    sim.step(tiempo_dt);

    // 2. VERIFICACIÓN DE VICTORIA
    if (checkWinCondition()) {
        timer.stop();
        update();
        return;
    }

    // DETECCIÓN DE FIN DE Turno
    bool anyActive = false;
    for (auto &p : sim.particulas) {
        if (p.activa) {
            anyActive = true;
            break;
        }
    }

    if (!anyActive) {
        frames_without_projectile++;
    } else {
        frames_without_projectile = 0;
    }


    if (frames_without_projectile > 5) {
        emit projectileFinished();
        frames_without_projectile = -100;
    }

    update();
}

bool GameWidget::checkWinCondition() {
    int aliveLeft = 0;   // Obstáculos del jugador 2 (objetivo del Jugador 2)
    int aliveRight = 0;  // Obstáculos del jugador 1 (objetivo del Jugador 1)

    double midX = sim.ancho / 2.0;

    for (const auto &o : sim.obstaculos) {
        if (o.estaVivo()) {
            // Clasifica el obstáculo basado en la posición de su centro (o.x + o.lado/2.0)
            if (o.x + o.lado/2.0 < midX) {
                aliveLeft++;
            }
            else {
                aliveRight++;
            }
        }
    }

    if (aliveRight == 0) {
        emit gameEnded(1); // Gana el Jugador 1 si destruye todos los obstáculos de la DERECHA
        return true;
    }
    if (aliveLeft == 0) {
        emit gameEnded(2); // Gana el Jugador 2 si destruye todos los obstáculos de la IZQUIERDA
        return true;
    }

    return false;
}

// Implementación del método de reinicio

void GameWidget::reiniciarSimulacion() {
    sim.limpiarParticulas();
    frames_without_projectile = 0;

    // 2. RECREAR OBSTÁCULOS
    sim.obstaculos.clear();

    double marginBottom = 10.0;
    double colSize      = 40.0;
    double topSize      = 100.0;

    double baseY = sim.alto - colSize - marginBottom;
    double topY  = baseY - topSize;

    double leftTopX  = 80.0;
    double rightTopX = sim.ancho - leftTopX - topSize;

    // ------- Fortaleza izquierda -------
    sim.agregarObstaculo(Obstaculo(leftTopX,                      baseY, colSize, 200));
    sim.agregarObstaculo(Obstaculo(leftTopX + topSize - colSize,  baseY, colSize, 200));
    sim.agregarObstaculo(Obstaculo(leftTopX,                      topY,  topSize, 100));

    // ------- Fortaleza derecha -------
    sim.agregarObstaculo(Obstaculo(rightTopX,                     baseY, colSize, 200));
    sim.agregarObstaculo(Obstaculo(rightTopX + topSize - colSize, baseY, colSize, 200));
    sim.agregarObstaculo(Obstaculo(rightTopX,                     topY,  topSize, 100));

    // 3. Reiniciar el temporizador
    if (!timer.isActive()) {
        timer.start(int(tiempo_dt * 1000));
    }

    update();
}

void GameWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // calcular escala automática
    double sx = width() / sim.ancho;
    double sy = height() / sim.alto;
    double S = std::min(sx, sy);

    // 1. DIBUJAR FONDO
    painter.fillRect(rect(), Qt::black);
    QRect caja(0, 0, sim.ancho * S, sim.alto * S);

    // DIBUJAR LA TEXTURA DE FONDO
    painter.drawPixmap(caja, backgroundTexture);

    // Dibujar borde de la caja
    painter.setPen(Qt::green);
    painter.drawRect(caja);


    // ------- PREDICCIÓN DE TRAYECTORIA -------


    const double G = 9.0;
    double angGrados = currentAngle;
    double velocidad = currentSpeed;
    int jugadorActual = currentPlayer;

    double x0 = (jugadorActual == 1) ? cannon1X : cannon2X;
    double y0 = (jugadorActual == 1) ? cannon1Y : cannon2Y;

    double angRad = angGrados * M_PI / 180.0;
    double dir = (jugadorActual == 1) ? 1.0 : -1.0;

    double vx0 = velocidad * std::cos(angRad) * dir;
    double vy0 = -velocidad * std::sin(angRad);

    // Dibuja los puntos (Arco amarillo)
    painter.setBrush(Qt::yellow);
    painter.setPen(Qt::yellow);


    for (double t = 0.0; t < 2.0; t += 0.02) {

        // FÓRMULAS DE CINEMÁTICA: Posición en el tiempo t
        double x_pred = x0 + vx0 * t;
        double y_pred = y0 + vy0 * t + 0.5 * G * t * t;

        if (x_pred < 0 || x_pred > sim.ancho || y_pred > sim.alto) {
            break;
        }

        bool choca = false;
        for (const auto &o : sim.obstaculos) {

            if (o.estaVivo() && x_pred >= o.x && x_pred <= o.x + o.lado && y_pred >= o.y && y_pred <= o.y + o.lado) {
                choca = true;
                break;
            }
        }
        if (choca) break;

        double px = x_pred * S;
        double py = y_pred * S;
        painter.drawEllipse(QPointF(px, py), 2, 2);
    }



    // ------- Cañones -------
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::NoBrush);

    double cannonSize = 95.0;  // en unidades del mundo<
    double cannonWidth = cannonSize * S;
    double cannonHeight = cannonSize * S;

    // 1. Cañón del Jugador 1 (Usa la textura derecha)
    QRectF c1((cannon1X - cannonSize/2) * S,
              (cannon1Y - cannonSize/2) * S,
              cannonWidth, cannonHeight);

    painter.drawPixmap(c1.toRect(), cannonRTexture);


    // 2. Cañón del Jugador 2 (Usa la textura izquierda)
    QRectF c2((cannon2X - cannonSize/2) * S,
              (cannon2Y - cannonSize/2) * S,
              cannonWidth, cannonHeight);

    painter.drawPixmap(c2.toRect(), cannonLTexture);

    // ------- Obstáculos -------
    for (auto &o : sim.obstaculos) {
        if (!o.estaVivo()) continue;

        // 1. Definir el área del obstáculo en píxeles
        QRectF r(o.x*S, o.y*S, o.lado*S, o.lado*S);


        QBrush tileBrush(obstacleTexture);
        tileBrush.setStyle(Qt::TexturePattern);

        painter.setBrush(tileBrush);
        painter.setPen(Qt::NoPen);
        painter.drawRect(r);

        // 3. Dibujar Overlay de Daño
        double vida = o.resistencia;
        if (vida < 100) {
            double damageFactor = 1.0 - (vida / 100.0);


            painter.setBrush(QColor(255, 0, 0, int(damageFactor * 100)));
            painter.drawRect(r);
        }

        // 4. Configurar Borde y Texto

        QPen borderPen(Qt::black);
        borderPen.setWidth(9);
        painter.setPen(borderPen);
        painter.setBrush(Qt::NoBrush);

        QFont currentFont = painter.font();
        currentFont.setPointSize(17);
        painter.setFont(currentFont);

        painter.drawRect(r);

        painter.drawText(r.left()+2, r.top()+23, QString::number(int(o.resistencia)));

        currentFont.setPointSize(10);
        painter.setFont(currentFont);
    }


    // ---------------------------------------------------
    // Dibujo del Proyectil

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::NoBrush);

    // 1. Verificar si hay alguna bala activa en el simulador
    bool anyActiveProjectile = false;
    for (const auto &p : sim.particulas) {
        if (p.activa) {
            anyActiveProjectile = true;
            break;
        }
    }

    // Si no hay proyectiles activos, dibujamos una bala en el cañón del jugador actual.
    if (!anyActiveProjectile) {
        const double DEFAULT_PARTICLE_RADIUS = 6.0;


        double x_cannon = (currentPlayer == 1) ? cannon1X : cannon2X;
        double y_cannon = (currentPlayer == 1) ? cannon1Y : cannon2Y;

        double displayScaleFactor = 2.7;
        double visualRad = (DEFAULT_PARTICLE_RADIUS * S) * displayScaleFactor;

        QRectF projectileRect((x_cannon * S) - visualRad, (y_cannon * S) - visualRad, visualRad * 2, visualRad * 2);
        painter.drawPixmap(projectileRect.toRect(), projectileTexture);
    }

    for (auto &p : sim.particulas) {
        if (!p.activa) continue;

        double px = p.pos.x * S;
        double py = p.pos.y * S;

        double displayScaleFactor = 2.7;
        double visualRad = (p.radio * S) * displayScaleFactor;
        QRectF projectileRect(px - visualRad, py - visualRad, visualRad * 2, visualRad * 2);

        painter.drawPixmap(projectileRect.toRect(), projectileTexture);
    }
}
