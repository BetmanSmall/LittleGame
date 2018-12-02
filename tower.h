#ifndef TOWER_H
#define TOWER_H

#include <QDebug>
#include <QPixmap>

#include "bullet.h"
#include "templatefortower.h"
#include "circle.h"
#include "cameracontroller.h"
#include "unit.h"

class Cell;
class Unit;
class Bullet;

class Tower
{
public:
    Cell* cell;
    float elapsedReloadTime;
    TemplateForTower* templateForTower;

    int player;
    int capacity;
    std::vector<Bullet*> bullets;
    Vector2* centerGraphicCoord;
    Circle* radiusDetectionCircle;
    Circle* radiusFlyShellCircle;

public:
    Tower(Cell *cell, TemplateForTower *templateForTower, int player);
    ~Tower();

    void updateGraphicCoordinates(CameraController *cameraController);
    bool recharge(float delta);
    bool shotFireBall(CameraController *cameraController);
    bool shoot(Unit *unit, CameraController *cameraController);
    void moveAllShells(float delta, CameraController *cameraController);
    void moveShell(float delta, Bullet *bullet, CameraController *cameraController);

    QString toString();
    QString toString(bool full);
};

#endif // TOWER_H
