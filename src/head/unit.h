#ifndef UNIT_H
#define UNIT_H

#include <QDebug>
#include <QPixmap>

#include "src/head/libs/astar.h"
#include "src/head/cell.h"
#include "src/head/direction.h"
#include "src/head/templateforunit.h"
#include "cameracontroller.h"
#include "animation.h"
#include "circle.h"
#include "shelleffecttype.h"

class Cell;

class Unit
{
public:
    AStar::CoordinateList route;
    AStar::Vec2i oldPosition;
    AStar::Vec2i newPosition;
    Cell *exitCell;
    float hp;
    float speed;
    float stepsInTime;
    float deathElapsedTime;

    int player;
    Vector2 *currentPoint;
    Vector2 *backStepPoint;
    Circle* circle1;
    Circle* circle2;
    Circle* circle3;
    Circle* circle4;
    Vector2 *velocity;
    Vector2 *displacement;

    TemplateForUnit* templateForUnit;

    Direction::type direction;
    Animation* animation;
    std::vector<ShellEffectType*> shellEffectTypes;

public:
    Unit(AStar::CoordinateList route, TemplateForUnit* templateForUnit, int player, Cell *exitCell);
    ~Unit();
    void setAnimation(QString action);
    void correct_fVc(Vector2 *fVc, Direction::type direction, float sizeCellX);
    AStar::Vec2i* move(float deltaTime, CameraController* cameraController);
    bool die(float damage, ShellEffectType shellEffectType);
//    bool addEffect(ShellEffectType *shellEffectType);

    bool changeDeathFrame(float delta);
//    QPixmap getAnimationInformation(int *lastX, int *lastY, int *animationCurrIter, int *animationMaxIter);
    bool isAlive();

    QPixmap getCurrentFrame();
    QPixmap getCurrentDeathFrame();
    QString toString();
    QString toString(bool full);
};

#endif // UNIT_H
