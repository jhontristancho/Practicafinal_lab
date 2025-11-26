#include "MainWindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
: QWidget(parent), currentPlayer(1), turno(1)
{
    game = new GameWidget(this);

    lblPlayer = new QLabel("Current Player: 1", this);
    lblTurno = new QLabel("Turno: 1", this);
    spinAngle = new QDoubleSpinBox(this);
    spinAngle->setRange(0, 90);
    spinAngle->setValue(30);
    spinAngle->setSuffix("°");
    spinSpeed = new QDoubleSpinBox(this);
    spinSpeed->setRange(1, 500);
    spinSpeed->setValue(120);


    connect(spinAngle, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onShotParameterChanged);

    connect(spinSpeed, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onShotParameterChanged);

    game->setShotParameters(spinAngle->value(), spinSpeed->value(), currentPlayer);


    btnFire = new QPushButton("Fuego!", this);
    connect(btnFire, &QPushButton::clicked, this, &MainWindow::onFireClicked);


    connect(game, &GameWidget::gameEnded, this, &MainWindow::onGameEnded);

    QVBoxLayout *controlLayout = new QVBoxLayout;
    controlLayout->addWidget(lblPlayer);

    QGroupBox *gb = new QGroupBox("Shot parametros");
    QVBoxLayout *gbLayout = new QVBoxLayout;
    gbLayout->addWidget(new QLabel("Angulo:"));
    gbLayout->addWidget(spinAngle);
    gbLayout->addWidget(new QLabel("Velocidad:"));
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


void MainWindow::onGameEnded(int winnerPlayer) {
    btnFire->setEnabled(false);

    // 2. Mostrar la ventana de victoria con la opción de reiniciar
    QMessageBox msgBox;
    msgBox.setWindowTitle("¡Juego Terminado!");
    msgBox.setText(QString("¡El Jugador %1 ha ganado la partida en el Turno %2!").arg(winnerPlayer).arg(turno));

    msgBox.setInformativeText("¿Quieres volver a jugar?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);

    int ret = msgBox.exec();

    if (ret == QMessageBox::Yes) {
        resetGame();
    } else {

        QCoreApplication::quit();
    }
}


void MainWindow::resetGame() {

    currentPlayer = 1;
    turno = 1;

    // Actualizar
    lblPlayer->setText("Turno del jugador: 1");
    lblTurno->setText("Turno: 1");
    btnFire->setEnabled(true);
    game->reiniciarSimulacion();
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


void MainWindow::onShotParameterChanged() {
    // 1. Obtiene los valores actuales
    double angle = spinAngle->value();
    double speed = spinSpeed->value();
    game->setShotParameters(angle, speed, currentPlayer);
}
