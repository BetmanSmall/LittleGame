#ifndef UNIT_H
#define UNIT_H

#include <QDebug>
#include <QPixmap>

//#include "src/head/field.h"
#include "src/head/libs/astar.h"
#include "src/head/direction.h"
#include "src/head/templateforunit.h"
#include "cameracontroller.h"
#include "animation.h"
#include "circle.h"

class Unit
{
public:
    AStar::CoordinateList route;
    AStar::Vec2i oldPosition;
    AStar::Vec2i newPosition;
    int hp;
    float speed;
    float stepsInTime;
    float deathElapsedTime;

    int player;
//    int coorByMapX, coorByMapY;
    Circle* circle1;
    Circle* circle2;
    Circle* circle3;
    Circle* circle4;

//    bool alive;
//    bool preDeath;
//    QString type;

    TemplateForUnit* templateForUnit;

    Direction::type direction;
    Animation* animation;
//    QPixmap pixmap;
//    std::vector<QPixmap> activePixmaps;
//    int animationLastAliveIter;
//    int animationCurrIter;
//    int animationMaxIter;

public:
    Unit(AStar::CoordinateList route, TemplateForUnit* templateForUnit, int player);
    ~Unit();
    void setAnimation(QString action);
    void correct_fVc(float &fVx, float &fVy, Direction::type direction, float sizeCellX);
    AStar::Vec2i* move(float deltaTime, CameraController* cameraController);
    bool changeDeathFrame(float delta);
//    QPixmap getAnimationInformation(int *lastX, int *lastY, int *animationCurrIter, int *animationMaxIter);
    bool isAlive();

    QPixmap getCurrentFrame();
    QPixmap getCurrentDeathFrame();
    QString toString();
};

#endif // UNIT_H
