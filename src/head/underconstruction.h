#ifndef UNDERCONSTRUCTION_H
#define UNDERCONSTRUCTION_H

#include <QDebug>

#include "src/head/templatefortower.h"

class UnderConstruction
{
public:
    int state;
    int startX, startY;
    int endX, endY;
    TemplateForTower* templateForTower;
    std::vector<int> coorsX;
    std::vector<int> coorsY;

public:
    UnderConstruction(TemplateForTower* templateForTower);
    UnderConstruction(int startX, int startY, TemplateForTower* templateForTower);
    bool setStartCoors(int startX, int startY);
    bool setEndCoors(int endX, int endY);
    bool clearStartCoors();
};

#endif // UNDERCONSTRUCTION_H
