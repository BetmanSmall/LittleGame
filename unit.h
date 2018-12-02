#ifndef UNIT_H
#define UNIT_H

#include <QDebug>
#include <QPixmap>

#include "astar.h"
#include "cell.h"
#include "direction.h"
#include "templateforunit.h"
#include "cameracontroller.h"
#include "animation.h"
#include "circle.h"
#include "towershelleffect.h"

class CameraController;
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
    std::vector<TowerShellEffect*> shellEffectTypes;

public:
    Unit(AStar::CoordinateList route, TemplateForUnit* templateForUnit, int player, Cell *exitCell);
    ~Unit();
    void setAnimation(QString action);
    void shellEffectsMove(float delta);
    void correct_fVc(Vector2 *fVc, Direction::type direction, float sizeCellX, float sizeCellY, bool isometric);
    AStar::Vec2i* move(float deltaTime, CameraController* cameraController);
    bool die(float damage, TowerShellEffect *towerShellEffect);
    bool addEffect(TowerShellEffect *towerShellEffect);

    bool changeDeathFrame(float delta);
    bool isAlive();

    QPixmap getCurrentFrame();
    QPixmap getCurrentDeathFrame();
    QString toString();
    QString toString(bool full);
};

#endif // UNIT_H
