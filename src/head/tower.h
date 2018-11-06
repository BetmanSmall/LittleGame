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
//    int capacity;
    std::vector<Bullet*> bullets;
    Circle* radiusDetectionCircle;
//    Circle* radiusFlyShellСircle;
//    int radius;
//    QPixmap pixmap;

    Tower(QPoint* position, TemplateForTower* templateForTower, int player);
    bool recharge(float delta);
    void createBullets(int difficultyLevel);
};

#endif // TOWER_H
