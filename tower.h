#ifndef TOWER_H
#define TOWER_H

#include <QDebug>
#include <QPixmap>

#include "defaulttower.h"

#include "bullet.h"

/**
 * @brief The Tower class - класс, описывающий общие характеристики башни.
 */
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
    DefaultTower* defTower;
    std::vector<Bullet*> bullets;

    Tower();

    bool recharge(float delta);

    void createBullets(int difficultyLevel);


};

#endif // TOWER_H
