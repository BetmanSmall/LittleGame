#ifndef BULLET_H
#define BULLET_H

#include <QPixmap>

//#include "unit.h"
//#include "templatefortower.h"
//#include "direction.h"
#include "vector2.h"
#include "circle.h"
#include "animation.h"
#include "cameracontroller.h"

class CameraController;
class Unit;

class Bullet {
public:
//    public UnitsManager* unitsManager;
    Unit* unit;
    float ammoExpSize;
    float ammoSize;
    float ammoSpeed;
    TemplateForTower* templateForTower;

    QPixmap pixmap;

    Vector2* currentPoint;
    Circle* currCircle;
    Vector2* endPoint;
    Circle* endCircle;
    Vector2* velocity;

    Direction::type direction;
    Animation* animation = NULL;
    float flyingTime;

public:
    Bullet(Vector2 *currentPoint, TemplateForTower *templateForTower, Vector2 *destPoint, CameraController *cameraController);
    Bullet(Vector2 *currentPoint, TemplateForTower *templateForTower, Unit *unit, CameraController *cameraController);
    ~Bullet();

    void setAnimation(QString action);
    int flightOfShell(float delta, CameraController *cameraController);
    bool tryToHitUnits(CameraController *cameraController);
};

#endif // BULLET_H
