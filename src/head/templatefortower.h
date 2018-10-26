#ifndef TEMPLATEFORTOWER_H
#define TEMPLATEFORTOWER_H

#include <QDebug>
#include <QPixmap>
#include <QtXml>

#include "template.h"
//#include "src/head/direction.h"
//#include "src/head/faction.h"

/**
 * @brief The TemplateForTower class - класс, описывающий все реализации определенного типа Башни.
 */
class TemplateForTower : public Template
{
public:
    int     radiusDetection;
    int     attack;
    int     damage;
    int     radius;
    int     size;
    int     height;
    float   ammoSize;
    float   ammoSpeed;
    float   reloadTime;
    int     type;
    Tile* idleTile;
//    AnimatedTile idleTile;
    QMap<QString, Tile*> ammunitionPictures;

//    Float    radiusFlyShell;
//    Integer  cost;
//    TowerAttackType towerAttackType;
//    ShellAttackType shellAttackType;
//    ShellEffectType shellEffectType;
//    Integer capacity;
//    Integer ammoDistance;
    QPixmap pixmap;
    std::vector<QPixmap> bullet;

    std::vector<QPixmap> bullet_fly_up;
    std::vector<QPixmap> bullet_fly_up_right;
    std::vector<QPixmap> bullet_fly_right;
    std::vector<QPixmap> bullet_fly_down_right;
    std::vector<QPixmap> bullet_fly_down;
    std::vector<QPixmap> bullet_fly_down_left;
    std::vector<QPixmap> bullet_fly_left;
    std::vector<QPixmap> bullet_fly_up_left;

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
