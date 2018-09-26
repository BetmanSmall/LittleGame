#include "tower.h"

Tower::Tower() {
    this->elapsedReloadTime = 0;
//    this->attackX = -1;
//    this->attackY = -1;
}

//Tower::Tower(int type, int radius, int attack, QPixmap pixmap) {
//    this->type = type;
//    this->radius = radius;
//    this->attack = attack;
//    this->pixmap = pixmap;
//}

bool Tower::recharge(float delta) {
//    qDebug() << "Tower::recharge(); -- delta:" << delta << " elapsedReloadTime:" << elapsedReloadTime << " reload:" << defTower->reloadTime;
    elapsedReloadTime += delta;
    if(elapsedReloadTime >= defTower->reloadTime) {
        return true;
    }
    return false;
}

void Tower::createBullets() {
    elapsedReloadTime = 0;
    Bullet* bullet1 = new Bullet(currX, currY, DirectionUpLeft, defTower);
    bullets.push_back(bullet1);
    Bullet* bullet2 = new Bullet(currX, currY, DirectionDownLeft, defTower);
    bullets.push_back(bullet2);
    Bullet* bullet3 = new Bullet(currX, currY, DirectionUpRight, defTower);
    bullets.push_back(bullet3);
    Bullet* bullet4 = new Bullet(currX, currY, DirectionDownRight, defTower);
    bullets.push_back(bullet4);
}

//void Tower::createBulletAndShot(int attackX, int attackY) {
//void Tower::createBulletAndShot(Unit* unit, int bullet_grafCoorX, int bullet_grafCoorY) {
//    Bullet bullet;

//    bullet.setSpeed(10);
//    bullet.setCurrCellCoordinate(bullet_grafCoorX, bullet_grafCoorY);
////    bullet.setDestinationCordinate(unit->currX, unit->currY);
//    bullet.setUnit(unit);
//    bullet.setPixmap(defTower->bullet_fly_up);

//    bullets.push_back(bullet);
//}
