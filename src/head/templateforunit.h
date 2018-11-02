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
//    float bounty;
//    float cost;
    float healthPoints;
    float speed;
    QMap<QString, AnimatedTile*> animations;

//    QPixmap pixmap;
//    QPixmap idle_up;
//    QPixmap idle_up_right;
//    QPixmap idle_right;
//    QPixmap idle_down_right;
//    QPixmap idle_down;
//    QPixmap idle_down_left;
//    QPixmap idle_left;
//    QPixmap idle_up_left;

//    std::vector<QPixmap> idle;
//    std::vector<QPixmap> walk_up;
//    std::vector<QPixmap> walk_up_right;
//    std::vector<QPixmap> walk_right;
//    std::vector<QPixmap> walk_down_right;
//    std::vector<QPixmap> walk_down;
//    std::vector<QPixmap> walk_down_left;
//    std::vector<QPixmap> walk_left;
//    std::vector<QPixmap> walk_up_left;

//    std::vector<QPixmap> attack_up;
//    std::vector<QPixmap> attack_up_right;
//    std::vector<QPixmap> attack_right;
//    std::vector<QPixmap> attack_down_right;
//    std::vector<QPixmap> attack_down;
//    std::vector<QPixmap> attack_down_left;
//    std::vector<QPixmap> attack_left;
//    std::vector<QPixmap> attack_up_left;

//    std::vector<QPixmap> death_up;
//    std::vector<QPixmap> death_up_right;
//    std::vector<QPixmap> death_right;
//    std::vector<QPixmap> death_down_right;
//    std::vector<QPixmap> death_down;
//    std::vector<QPixmap> death_down_left;
//    std::vector<QPixmap> death_left;
//    std::vector<QPixmap> death_up_left;

    std::vector<QPixmap> explosion;

    TemplateForUnit(QString templateFile);
    ~TemplateForUnit();
    void clearVectors();
//    void setFaction(Faction* faction);
    void specificLoad();

private:
    AnimatedTile* flipAnimatedTiledMapTile(AnimatedTile* animatedTiledMapTile);
    void validate();

public:
    QString toString();
    QString toString(bool full);
};

#endif // TEMPLATEFORUNIT_H
