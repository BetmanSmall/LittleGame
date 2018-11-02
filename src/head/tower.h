#ifndef TOWER_H
#define TOWER_H

#include <QDebug>
#include <QPixmap>

#include "src/head/bullet.h"
#include "src/head/templatefortower.h"
//#include "circle.h"

class Tower
{
public:
//    QPoint position;
    int currX, currY;
    int elapsedReloadTime;
    TemplateForTower* templateForTower;

//    int player;
//    int capacity;
    std::vector<Bullet*> bullets;
//    Circle* radiusDetectionСircle;
//    Circle* radiusFlyShellСircle;
//    int radius;
//    QPixmap pixmap;

    Tower(int x, int y, TemplateForTower* templateForTower);
    bool recharge(float delta);
    void createBullets(int difficultyLevel);
};

#endif // TOWER_H
