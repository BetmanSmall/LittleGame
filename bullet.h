#ifndef BULLET_H
#define BULLET_H

#include <QPixmap>

#include "unit.h"

class Bullet
{
public:
    int speed;
    bool flying;

//    int bulletFly_TimerMilliSec;
//    std::thread bulletFly_TimerId;

    int currX, currY;
    int destX, destY;
    Unit* unit;
//    Direction direction;

    QPixmap pixmap;

    Bullet();

    void setSpeed(int speed);

    void setCurrentCordinate(int currX, int currY);

//    void setDestinationCordinate(int destX, int destY);
    void setUnit(Unit* unit);

    void setPixmap(QPixmap pixmap);

    int getSpeed();

    int getCurrX();
    int getCurrY();

    int getDestinationX();
    int getDestinationY();

    QPixmap getPixmap();

public slots:
    void move();
};

#endif // BULLET_H
