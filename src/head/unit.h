#ifndef UNIT_H
#define UNIT_H

#include <QDebug>
#include <QPixmap>

#include "src/head/libs/astar.h"
#include "src/head/direction.h"
#include "src/head/templateforunit.h"

class Unit
{
public:
    AStar::CoordinateList path;
    int lastX, lastY;
    int coorByCellX, coorByCellY;
    int hp;
    float speed;
    float stepsInTime;
    float deathElapsedTime;

//    int player;
    int coorByMapX, coorByMapY;
    bool alive;
    bool preDeath;
    QString type;

    TemplateForUnit* templateForUnit;

    Direction::type direction;
    QPixmap pixmap;
    std::vector<QPixmap> activePixmaps;
    int animationLastAliveIter;
    int animationCurrIter;
    int animationMaxIter;

public:
    Unit(AStar::CoordinateList path, TemplateForUnit* templateForUnit);
    ~Unit();
    QPixmap getAnimationInformation(int *lastX, int *lastY, int *animationCurrIter, int *animationMaxIter);
};

#endif // UNIT_H
