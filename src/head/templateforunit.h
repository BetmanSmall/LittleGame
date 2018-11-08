#ifndef TEMPLATEFORUNIT_H
#define TEMPLATEFORUNIT_H

#include <QDebug>
#include <QPixmap>
#include <QtXml>

#include "src/head/mapEditor/maploader.h"
#include "src/head/mapEditor/animatedtile.h"
#include "src/head/mapEditor/tile.h"
#include "src/head/direction.h"
//#include "src/head/faction.h"
#include "template.h"

class TemplateForUnit : public Template
{
public:
//    Faction* faction;
    QString factionName;
    QString name;

    float healthPoints;
    float bounty;
    float cost;
    float speed;
    QString type;

    QMap<QString, AnimatedTile*> animations;
    std::vector<QPixmap> explosion;

public:
    TemplateForUnit(QString templateFile);
    ~TemplateForUnit();

//    void loadExplosion(SimpleTemplate explosion);
    void specificLoad();

private:
    AnimatedTile* flipAnimatedTiledMapTile(AnimatedTile* animatedTiledMapTile);
    void validate();

public:
    QString toString();
    QString toString(bool full);
};

#endif // TEMPLATEFORUNIT_H
