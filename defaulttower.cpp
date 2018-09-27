#include "defaulttower.h"

DefaultTower::DefaultTower() {
    qDebug() << "DefaultTower::DefaultTower(); -- ";
    this->reloadTime = 3000;
}

DefaultTower::~DefaultTower() {
    qDebug() << "DefaultTower::~DefaultTower(); -- ";
    bullet.clear();

    bullet_fly_up.clear();
    bullet_fly_up_right.clear();
    bullet_fly_right.clear();
    bullet_fly_down_right.clear();
    bullet_fly_down.clear();
    bullet_fly_down_left.clear();
    bullet_fly_left.clear();
    bullet_fly_up_left.clear();
}
