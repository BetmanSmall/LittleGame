#include "src/head/unit.h"

Unit::Unit(AStar::CoordinateList path, TemplateForUnit *templateForUnit) {
    if (!path.empty()) {
        this->path = path;
        this->lastX = path.back().x;
        this->lastY = path.back().y;
        this->coorByCellX = lastX;
        this->coorByCellY = lastY;
        this->hp = templateForUnit->healthPoints;
        this->speed = templateForUnit->speed;
        this->stepsInTime = 0;
        this->deathElapsedTime = 0;

//        this->pla
//        this->coorByMapX = coorByMapX;
//        this->coorByMapY = coorByMapY;
        this->alive = true;
        this->preDeath = false;
        this->type = templateForUnit->type;

        this->templateForUnit = templateForUnit;

        this->direction = Direction::UP;
        this->animationCurrIter = 0;
        this->animationMaxIter = 0;
//        this->effects
    }
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
