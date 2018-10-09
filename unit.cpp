#include "unit.h"

Unit::Unit() {

}

Unit::~Unit() {

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

