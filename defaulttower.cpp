#include "defaulttower.h"

DefaultTower::DefaultTower() {
    qDebug() << "DefaultTower::DefaultTower(); -- ";
    this->reloadTime = 3000;
}

DefaultTower::~DefaultTower() {
    qDebug() << "DefaultTower::~DefaultTower(); -- ";
    bullet.clear();
}
