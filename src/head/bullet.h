#ifndef BULLET_H
#define BULLET_H

#include <QPixmap>

#include "src/head/defaulttower.h"
#include "src/head/unit.h"

class Bullet {
public:
    int speed;
    bool flying;
    int lastCellX, lastCellY;
    int currCellX, currCellY;

    Direction direction;
    int animationCurrIter;
    int animationMaxIter;
    QPixmap pixmap;
    std::vector<QPixmap> activePixmaps;

    DefaultTower* defTower;

    Bullet(int currCellX, int currCellY, Direction direction, DefaultTower* defTower);
    void setSpeed(int speed);
    void setCurrCellCoordinate(int currCellX, int currCellY);
    QPixmap getAnimationInformation(int *lastCellX, int *lastCellY, int *animationCurrIter, int *animationMaxIter);

    void setUnit(Unit* unit);
    void setPixmap(QPixmap pixmap);
    int getSpeed();
    int getCurrCellX();
    int getCurrCellY();

    QPixmap getPixmap();

public slots:

};

#endif // BULLET_H
