#include "unit.h"

Unit::Unit() {
//    qDebug() << "Unit::Unit(); -- ";
}

Unit::~Unit() {
//    qDebug() << "Unit::~Unit(); -- ";
    path.clear();
    activePixmaps.clear();
}

QPixmap Unit::getAnimationInformation(int *lastX, int *lastY, int *animationCurrIter, int *animationMaxIter) {
    *lastX = this->lastX;
    *lastY = this->lastY;
    *animationCurrIter = this->animationCurrIter;
    *animationMaxIter = this->animationMaxIter;
    return this->pixmap;
}

//bool Unit::takeDamage(int damage) {
//    if(alive) {
//        int localHP = hp - damage;
//        if(localHP <= 0) {
//            hp = 0;
////                currX = -1;
////                currY = -1;
//            alive = false;
//            preDeath = true;
//            // ЛАЖА
//            animationCurrIter = 0;
//            if(direction == DirectionUpLeft) {
//                animationMaxIter = defUnit->death_up_left.size();
//                activePixmaps = defUnit->death_up_left;
//            } else if(direction == DirectionUp) {
//                animationMaxIter = defUnit->death_up.size();
//                activePixmaps = defUnit->death_up;
//            } else if(direction == DirectionUpRight) {
//                animationMaxIter = defUnit->death_up_right.size();
//                activePixmaps = defUnit->death_up_right;
//            } else if(direction == DirectionLeft) {
//                animationMaxIter = defUnit->death_left.size();
//                activePixmaps = defUnit->death_left;
//            } else if(direction == DirectionRight) {
//                animationMaxIter = defUnit->death_right.size();
//                activePixmaps = defUnit->death_right;
//            } else if(direction == DirectionDownLeft) {
//                animationMaxIter = defUnit->death_down_left.size();
//                activePixmaps = defUnit->death_down_left;
//            } else if(direction == DirectionDown) {
//                animationMaxIter = defUnit->death_down.size();
//                activePixmaps = defUnit->death_down;
//            } else if(direction == DirectionDownRight) {
//                animationMaxIter = defUnit->death_down_right.size();
//                activePixmaps = defUnit->death_down_right;
//            }
//            animationMaxIter += defUnit->explosion.size();
//            activePixmaps.insert(activePixmaps.end(), defUnit->explosion.begin(), defUnit->explosion.end());
////                qDebug() << "tmpUnit->animationMaxIter: " << tmpUnit << "->" << tmpUnit->animationMaxIter;
//            pixmap = activePixmaps[0];
//            // ЛАЖА !!!!!!
//            return true;
//        } else {
//            hp = localHP;
//        }
//    }
//    return false;
//}
