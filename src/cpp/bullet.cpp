#include "src/head/bullet.h"

Bullet::Bullet(int currCellX, int currCellY, Direction direction, DefaultTower* defTower) {
    this->flying = true;
    this->currCellX = currCellX;
    this->currCellY = currCellY;
    this->direction = direction;
    this->defTower = defTower;

    this->animationCurrIter = 0;
    this->animationMaxIter = 0;

    this->pixmap = defTower->bullet[0];
}

void Bullet::setSpeed(int speed) {
    this->speed = speed;
}

void Bullet::setCurrCellCoordinate(int currX, int currY) {
    this->currCellX = currX;
    this->currCellY = currY;
}

QPixmap Bullet::getAnimationInformation(int *lastX, int *lastY, int *animationCurrIter, int *animationMaxIter) {
    *lastX = this->lastCellX;
    *lastY = this->lastCellY;
    *animationCurrIter = this->animationCurrIter;
    *animationMaxIter = this->animationMaxIter;
    return this->pixmap;
}

void Bullet::setPixmap(QPixmap pixmap) {
    this->pixmap = pixmap;
}

int Bullet::getSpeed() {
    return speed;
}

int Bullet::getCurrCellX() {
    return currCellX;
}

int Bullet::getCurrCellY() {
    return currCellY;
}

QPixmap Bullet::getPixmap() {
    return pixmap;
}
