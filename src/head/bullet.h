#ifndef BULLET_H
#define BULLET_H

#include <QPixmap>

#include "src/head/unit.h"
#include "src/head/templatefortower.h"
#include "src/head/direction.h"

class Bullet {
public:
    Unit* unit;
    float ammoExpSize;
    float ammoSize;
    float ammoSpeed;
    TemplateForTower* templateForTower;

    QPixmap pixmap;
    QMap<QString, Tile*> ammunitionPictures;
//    std::vector<QPixmap> activePixmaps;

    bool flying;
    int lastCellX, lastCellY;
    int currCellX, currCellY;
    Vector2* currentPoint;
    Circle* circle;
    Circle* endPoint;
    Vector2* velocity;

    Direction::type direction;
    int animationCurrIter;
    int animationMaxIter;
    Animation* animation;

public:
    Bullet(Vector2 *currentPoint, TemplateForTower *templateForTower, Unit *unit);
    Bullet(int currCellX, int currCellY, Direction::type direction, TemplateForTower *templateForTower);
    ~Bullet();

    void setAnimation(QString action);
    int flightOfShell(float delta);
    bool tryToHitUnits();
//    void setCurrCellCoordinate(int currCellX, int currCellY);
//    QPixmap getAnimationInformation(int *lastCellX, int *lastCellY, int *animationCurrIter, int *animationMaxIter);

//    void setUnit(Unit* unit);
//    void setPixmap(QPixmap pixmap);
//    int getCurrCellX();
//    int getCurrCellY();
//    QPixmap getPixmap();

//public slots:

};

#endif // BULLET_H
