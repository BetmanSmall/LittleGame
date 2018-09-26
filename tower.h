#ifndef TOWER_H
#define TOWER_H

#include <QDebug>
#include <QPixmap>

#include "defaulttower.h"
//#include "unit.h"
#include "bullet.h"

//using namespace std;
/**
 * @brief The Tower class - класс, описывающий общие характеристики башни.
 */
class Tower
{
//private:
public:
    QString name;
    int attack;
    int radius;
    int size;
    int type;
    int elapsedReloadTime;

    int currX, currY;
//    int attackX, attackY;

    QPixmap pixmap;
    DefaultTower* defTower;
    std::vector<Bullet*> bullets;

    Tower();
//    Tower(int type, int radius, int attack, QPixmap pixmap);
    bool recharge(float delta);

    void createBullets();
//    void createBulletAndShot(int attackX, int attackY);
//    void createBulletAndShot(Unit* unit, int bullet_grafCoorX, int bullet_grafCoorY);

};

#endif // TOWER_H
