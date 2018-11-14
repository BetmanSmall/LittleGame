#ifndef TOWER_H
#define TOWER_H

#include <QDebug>
#include <QPixmap>

#include "src/head/bullet.h"
#include "src/head/templatefortower.h"
#include "circle.h"
#include "cameracontroller.h"

class Tower
{
public:
    QPoint* position;
//    int currX, currY;
    float elapsedReloadTime;
    TemplateForTower* templateForTower;

    int player;
    int capacity;
    std::vector<Bullet*> bullets;
    Circle* radiusDetectionCircle;
    Circle* radiusFlyShellCircle;

public:
    Tower(QPoint* position, TemplateForTower* templateForTower, int player);
    ~Tower();

    void updateGraphicCoordinates(CameraController *cameraController);
    bool recharge(float delta);
    bool shoot(Unit* unit, CameraController* cameraController);
    void createBullets(int difficultyLevel);
    void moveAllShells(float delta);
    void moveShell(float delta, Bullet* bullet);

    QString toString();
    QString toString(bool full);
};

#endif // TOWER_H
