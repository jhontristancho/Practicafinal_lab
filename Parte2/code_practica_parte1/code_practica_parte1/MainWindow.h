#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QDoubleSpinBox>
#include "GameWidget.h"

class MainWindow : public QWidget {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void onFireClicked();
    void onGameEnded(int winnerPlayer); // NUEVO SLOT

private:
    GameWidget *game;
     int turno;
     QLabel *lblTurno;
    QLabel *lblPlayer;
    QDoubleSpinBox *spinAngle;
    QDoubleSpinBox *spinSpeed;
    QPushButton *btnFire;
    int currentPlayer;
    void resetGame();
};

#endif
