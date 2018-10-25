#ifndef UNDERCONSTRUCTION_H
#define UNDERCONSTRUCTION_H

#include <QDebug>

#include "src/head/templatefortower.h"

class UnderConstruction
{
public:
    UnderConstruction(TemplateForTower* tower);
    UnderConstruction(int startX, int startY, TemplateForTower* tower);

    bool setStartCoors(int startX, int startY);
    bool setEndCoors(int endX, int endY);
    bool clearStartCoors();

    int state;
    int startX, startY;
    int endX, endY;

    TemplateForTower* tower;

    std::vector<int> coorsX;
    std::vector<int> coorsY;
};

#endif // UNDERCONSTRUCTION_H
