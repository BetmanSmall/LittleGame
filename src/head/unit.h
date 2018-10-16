#ifndef UNIT_H
#define UNIT_H

#include <QDebug>
#include <QPixmap>

#include "src/head/libs/astar.h"
#include "src/head/defaultunit.h"

enum Direction {
    DirectionIdle,
    DirectionUp,
    DirectionUpRight,
    DirectionRight,
    DirectionDownRight,
    DirectionDown,
    DirectionDownLeft,
    DirectionLeft,
    DirectionUpLeft
};

/**
 * @brief The Unit class - класс, описывающий общие характеристики юнита.
 */
class Unit
{
public:
    int hp;
    bool alive;
    bool preDeath;
    int number;

    int type;
    AStar::CoordinateList path;

    int lastX, lastY;
    int coorByCellX, coorByCellY;
    int coorByMapX, coorByMapY;
    Direction direction;
    int animationLastAliveIter;
    int animationCurrIter;
    int animationMaxIter;
    QPixmap pixmap;
    std::vector<QPixmap> activePixmaps;
    DefaultUnit* defUnit;

    Unit();
    ~Unit();
    QPixmap getAnimationInformation(int *lastX, int *lastY, int *animationCurrIter, int *animationMaxIter);
};

#endif // UNIT_H
