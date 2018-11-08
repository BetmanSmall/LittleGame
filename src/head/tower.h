#ifndef TOWER_H
#define TOWER_H

#include <QDebug>
#include <QPixmap>

#include "src/head/bullet.h"
#include "src/head/templatefortower.h"
#include "circle.h"

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

    bool recharge(float delta);
    bool shoot(Unit* unit, CameraController* cameraController);
    void createBullets(int difficultyLevel);
    void moveAllShells(float delta);
    void moveShell(float delta, Bullet* bullet);

    QPointF* getCenterGraphicCoord(CameraController* cameraController);
    QPointF* getCenterGraphicCoord(int map, CameraController* cameraController);
    QPointF* getCenterGraphicCoord(int cellX, int cellY, int map, CameraController* cameraController);

    QString toString();
    QString toString(bool full);
};

#endif // TOWER_H
