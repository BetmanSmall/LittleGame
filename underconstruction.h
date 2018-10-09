#ifndef UNDERCONSTRUCTION_H
#define UNDERCONSTRUCTION_H

#include <QDebug>

#include "defaulttower.h"

class UnderConstruction
{
public:
    UnderConstruction(DefaultTower* tower);
    UnderConstruction(int startX, int startY, DefaultTower* tower);

    bool setStartCoors(int startX, int startY);
    bool setEndCoors(int endX, int endY);
    bool clearStartCoors();

    int state;
    int startX, startY;
    int endX, endY;

    DefaultTower* tower;

    std::vector<int> coorsX;
    std::vector<int> coorsY;
};

#endif // UNDERCONSTRUCTION_H
