#ifndef TEMPLATEFORTOWER_H
#define TEMPLATEFORTOWER_H

#include <QDebug>
#include <QPixmap>
#include <QtXml>

#include "template.h"
#include "src/head/direction.h"
//#include "src/head/faction.h"
#include "towerattacktype.h"
#include "shellattacktype.h"
#include "shelleffecttype.h"
#include "simpletemplate.h"

class TemplateForTower : public Template
{
public:
//    Faction* faction;
    QString factionName;
    QString name;

    float   radiusDetection;
    float   radiusFlyShell;
    int     damage;
    int     size;
    int     cost;
    float   ammoSize;
    float   ammoSpeed;
    float   reloadTime;
//    QString type;
    TowerAttackType::type towerAttackType;
    ShellAttackType::type shellAttackType;
    ShellEffectType* shellEffectType;
    int capacity;

//    AnimatedTile idleTile;
    Tile*   idleTile;
    QMap<QString, AnimatedTile*> animations;

public:
    TemplateForTower(QString templateFile);
    ~TemplateForTower();

    void loadFireBall(SimpleTemplate* fireBall);
    void specificLoad();
    void setAmmoTiles(QString tileName, AnimatedTile *tile);
    AnimatedTile* flipAnimatedTiledMapTile(AnimatedTile* animatedTiledMapTile);
    //Tile* flipTiledMapTile(Tile tiledMapTile);

private:
    void validate();

public:
    QString toString();
    QString toString(bool full);
};

#endif // TEMPLATEFORTOWER_H
