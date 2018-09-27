#ifndef UNIT_H
#define UNIT_H

#include <QDebug>
#include <QPixmap>

#include "defaultunit.h"
#include "astar.h"

enum Direction
{
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
//    int speed;
    int type;
    AStar::CoordinateList path;

    int lastX, lastY;
    int coorByCellX, coorByCellY;
    int coorByMapX, coorByMapY;
    Direction direction;
    int animationCurrIter;
    int animationMaxIter;
    QPixmap pixmap;
    std::vector<QPixmap> activePixmaps;
    DefaultUnit* defUnit;

    Unit();
    ~Unit();
    QPixmap getAnimationInformation(int *lastX, int *lastY, int *animationCurrIter, int *animationMaxIter);
//    bool takeDamage(int damage);
};

#endif // UNIT_H
