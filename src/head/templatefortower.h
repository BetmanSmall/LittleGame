#ifndef TEMPLATEFORTOWER_H
#define TEMPLATEFORTOWER_H

#include <QDebug>
#include <QPixmap>
#include <QtXml>

#include "template.h"
//#include "src/head/direction.h"
//#include "src/head/faction.h"

class TemplateForTower : public Template
{
public:
    int     radiusDetection;
    float   radiusFlyShell;
//    int     attack;
    int     damage;
//    int     radius;
    int     size;
    int     cost;
//    int     height;
    float   ammoSize;
    float   ammoSpeed;
    float   reloadTime;
//    int     type;
//    TowerAttackType towerAttackType;
//    ShellAttackType shellAttackType;
//    ShellEffectType shellEffectType;
    int capacity;
//    Integer ammoDistance;

    Tile*   idleTile;
//    AnimatedTile idleTile;
    QMap<QString, Tile*> ammunitionPictures;

//    QPixmap pixmap;
//    std::vector<QPixmap> bullet;
//    std::vector<QPixmap> bullet_fly_up;
//    std::vector<QPixmap> bullet_fly_up_right;
//    std::vector<QPixmap> bullet_fly_right;
//    std::vector<QPixmap> bullet_fly_down_right;
//    std::vector<QPixmap> bullet_fly_down;
//    std::vector<QPixmap> bullet_fly_down_left;
//    std::vector<QPixmap> bullet_fly_left;
//    std::vector<QPixmap> bullet_fly_up_left;

    TemplateForTower(QString templateFile);
    ~TemplateForTower();
//    void setFaction(Faction* faction);
    void specificLoad();

private:
    void validate();

public:
    QString toString();
    QString toString(bool full);
};

#endif // TEMPLATEFORTOWER_H
