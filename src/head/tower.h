#ifndef TOWER_H
#define TOWER_H

#include <QDebug>
#include <QPixmap>

#include "src/head/bullet.h"
#include "src/head/templatefortower.h"

class Tower
{
public:
    QString name;
    int attack;
    int radius;
    int size;
    int type;
    int elapsedReloadTime;

    int currX, currY;

    QPixmap pixmap;
    TemplateForTower* defTower;
    std::vector<Bullet*> bullets;

    Tower();
    bool recharge(float delta);
    void createBullets(int difficultyLevel);
};

#endif // TOWER_H
