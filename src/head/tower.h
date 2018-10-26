#ifndef TOWER_H
#define TOWER_H

#include <QDebug>
#include <QPixmap>

#include "src/head/bullet.h"
#include "src/head/templatefortower.h"

class Tower
{
public:
    int currX, currY;
    int elapsedReloadTime;
    TemplateForTower* templateForTower;

//    int player;
//    int capacity;
    std::vector<Bullet*> bullets;
    int radius;
    QPixmap pixmap;

    Tower(int x, int y, TemplateForTower* templateForTower);
    bool recharge(float delta);
    void createBullets(int difficultyLevel);
};

#endif // TOWER_H
