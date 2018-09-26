#ifndef BULLET_H
#define BULLET_H

#include <QPixmap>

#include "defaulttower.h"
//#include "tower.h"
#include "unit.h"

class Bullet {
public:
    int speed;
    bool flying;
    int lastCellX, lastCellY;
    int currCellX, currCellY;
//    int destX, destY;
    Direction direction;
    int animationCurrIter;
    int animationMaxIter;
    QPixmap pixmap;
    std::vector<QPixmap> activePixmaps;
//    Unit* unit;
    DefaultTower* defTower;

    Bullet(int currCellX, int currCellY, Direction direction, DefaultTower* defTower);
    void setSpeed(int speed);
    void setCurrCellCoordinate(int currCellX, int currCellY);
    QPixmap getAnimationInformation(int *lastCellX, int *lastCellY, int *animationCurrIter, int *animationMaxIter);

//    void setDestinationCordinate(int destX, int destY);
    void setUnit(Unit* unit);
    void setPixmap(QPixmap pixmap);
    int getSpeed();
    int getCurrCellX();
    int getCurrCellY();
//    int getDestinationX();
//    int getDestinationY();
    QPixmap getPixmap();

public slots:
//    void move();
};

#endif // BULLET_H
