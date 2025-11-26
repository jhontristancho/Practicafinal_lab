#include "MainWindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>

MainWindow::MainWindow(QWidget *parent)
: QWidget(parent), currentPlayer(1), turno(1)
{
    game = new GameWidget(this);

    lblPlayer = new QLabel("Current Player: 1", this);
    lblTurno = new QLabel("Turn: 1", this);
    spinAngle = new QDoubleSpinBox(this);
    spinAngle->setRange(0, 90);
    spinAngle->setValue(30);
    spinAngle->setSuffix("°");
    spinSpeed = new QDoubleSpinBox(this);
    spinSpeed->setRange(1, 500);
    spinSpeed->setValue(120);

    btnFire = new QPushButton("Fire!", this);
    connect(btnFire, &QPushButton::clicked, this, &MainWindow::onFireClicked);

    QVBoxLayout *controlLayout = new QVBoxLayout;
    controlLayout->addWidget(lblPlayer);

    QGroupBox *gb = new QGroupBox("Shot parameters");
    QVBoxLayout *gbLayout = new QVBoxLayout;
    gbLayout->addWidget(new QLabel("Angle:"));
    gbLayout->addWidget(spinAngle);
    gbLayout->addWidget(new QLabel("Speed:"));
    gbLayout->addWidget(spinSpeed);
    gbLayout->addWidget(btnFire);
    gb->setLayout(gbLayout);
    controlLayout->addWidget(gb);
    controlLayout->addStretch();
    controlLayout->addWidget(lblTurno);

    QHBoxLayout *main = new QHBoxLayout;
    main->addWidget(game, 1);
    main->addLayout(controlLayout);

    setLayout(main);
    setWindowTitle("Turn-based Projectile Game - Qt");
    resize(900, 600);
}

void MainWindow::onFireClicked() {
    double ang = spinAngle->value();
    double sp = spinSpeed->value();

    game->lanzarProyectil(ang, sp, currentPlayer);

    // alternar jugador
    currentPlayer = 3 - currentPlayer;
    turno++;

    lblPlayer->setText(QString("Current Player: %1").arg(currentPlayer));
    lblTurno->setText(QString("Turn: %1").arg(turno));
}
