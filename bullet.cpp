#include "bullet.h"

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

//void Bullet::setDestinationCordinate(int destX, int destY) {
//    this->destX = destX;
//    this->destY = destY;
//}
//void Bullet::setUnit(Unit* unit) {
//    this->unit = unit;
//}

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

//int Bullet::getDestinationX() {
//    return destX;
//}

//int Bullet::getDestinationY() {
//    return destY;
//}

QPixmap Bullet::getPixmap() {
    return pixmap;
}

//void Bullet::move() {
//    int destX = unit->coorByMapX;
//    int destY = unit->coorByMapY;

//    if(currX != destX || currY != destY) {
//        if(currX == destX) {
//            if(currY < destY) {
//                currY += speed;
//            } else if (currY > destY) {
//                currY -= speed;
//            }
//        } else if(currY == destY) {
//            if(currX < destX) {
//                currX += speed;
//            } else if(currX > destX) {
//                currX -= speed;
//            }
//        } else if(currX < destX && currY > destY) {
//            currX += speed/2;
//            currY -= speed/2;
//        } else if(currX > destX && currY > destY) {
//            currX -= speed/2;
//            currY -= speed/2;
//        } else if(currX < destX && currY < destY) {
//            currX += speed/2;
//            currY += speed/2;
//        } else if(currX > destX && currY < destY) {
//            currX -= speed/2;
//            currY += speed/2;
//        }
////        return true;
//    } else {
//        flying = false;
//    }
////    return false;
//}
