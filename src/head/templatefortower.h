#ifndef TEMPLATEFORTOWER_H
#define TEMPLATEFORTOWER_H

#include <QDebug>
#include <QPixmap>
#include <QtXml>

#include "template.h"
#include "src/head/direction.h"
//#include "src/head/faction.h"
#include "towerattacktype.h"

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
    QString type;
    TowerAttackType::type towerAttackType;
//    ShellAttackType shellAttackType;
//    ShellEffectType shellEffectType;
    int capacity;

    Tile*   idleTile;
//    AnimatedTile idleTile;
    QMap<QString, Tile*> ammunitionPictures;

    TemplateForTower(QString templateFile);
    ~TemplateForTower();
    void specificLoad();
    void setAmmoTiles(QString tileName, Tile* tile);
    //Tile* flipTiledMapTile(Tile tiledMapTile);

private:
    void validate();

public:
    QString toString();
    QString toString(bool full);
};

#endif // TEMPLATEFORTOWER_H
