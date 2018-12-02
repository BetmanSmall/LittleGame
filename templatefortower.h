#ifndef TEMPLATEFORTOWER_H
#define TEMPLATEFORTOWER_H

#include <QDebug>
#include <QPixmap>
#include <QtXml>

#include "template.h"
#include "direction.h"
//#include "faction.h"
#include "towerattacktype.h"
#include "towershelltype.h"
#include "towershelleffect.h"
#include "simpletemplate.h"

class TemplateForTower : public Template
{
public:
//    Faction* faction;
    QString factionName;
    QString name;

    float   radiusDetection = 0.0;
    float   radiusFlyShell = 0.0;
    int     damage;
    int     size;
    int     cost;
    float   ammoSize;
    float   ammoSpeed;
    float   reloadTime;
//    QString type;
    TowerAttackType::type towerAttackType;
    TowerShellType::type towerShellType;
    TowerShellEffect* towerShellEffect = NULL;
    int capacity = 0;

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
