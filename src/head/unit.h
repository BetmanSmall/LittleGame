#ifndef UNIT_H
#define UNIT_H

#include <QDebug>
#include <QPixmap>

#include "src/head/libs/astar.h"
#include "src/head/direction.h"
#include "src/head/templateforunit.h"
#include "animation.h"

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

    int player;
    int coorByMapX, coorByMapY;
    bool alive;
    bool preDeath;
    QString type;

    TemplateForUnit* templateForUnit;

    Direction::type direction;
    Animation* animation;
    QPixmap pixmap;
    std::vector<QPixmap> activePixmaps;
    int animationLastAliveIter;
    int animationCurrIter;
    int animationMaxIter;

public:
    Unit(AStar::CoordinateList path, TemplateForUnit* templateForUnit, int player);
    ~Unit();
    void setAnimation(QString action);
    QPixmap getAnimationInformation(int *lastX, int *lastY, int *animationCurrIter, int *animationMaxIter);
    QString toString();
};

#endif // UNIT_H
