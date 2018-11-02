#ifndef BULLET_H
#define BULLET_H

#include <QPixmap>

//#include "src/head/unit.h"
#include "src/head/templatefortower.h"
#include "src/head/direction.h"

class Bullet {
public:
    int speed;
    bool flying;
    int lastCellX, lastCellY;
    int currCellX, currCellY;

    Direction::type direction;
    int animationCurrIter;
    int animationMaxIter;
    QPixmap pixmap;
    std::vector<QPixmap> activePixmaps;

    TemplateForTower* templateForTower;

    Bullet(int currCellX, int currCellY, Direction::type direction, TemplateForTower* defTower);
    void setSpeed(int speed);
    void setCurrCellCoordinate(int currCellX, int currCellY);
    QPixmap getAnimationInformation(int *lastCellX, int *lastCellY, int *animationCurrIter, int *animationMaxIter);

//    void setUnit(Unit* unit);
    void setPixmap(QPixmap pixmap);
    int getSpeed();
    int getCurrCellX();
    int getCurrCellY();

    QPixmap getPixmap();

//public slots:

};

#endif // BULLET_H
