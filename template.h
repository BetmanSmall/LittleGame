#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <QDebug>
#include <QtXml>

#include "src/head/mapEditor/maploader.h"
#include "src/head/mapEditor/animatedtile.h"
#include "src/head/mapEditor/tile.h"

class Template
{
public:
    QString templateName;
//    Faction* faction;
    QString factionName;
    QString name;
    QString type;
    QMap<QString, QString> properties;
    QMap<int, Tile*> tiles;
    QMap<int, AnimatedTile*> animatedTiles;

//    Template();
    void loadBasicTemplate(QString templateFile);
    void basicValidate();
    QMap<QString, QString>* getProperties();
    QString toStringProperties();
    QString toStringBasicParam();

    virtual QString toString() = 0;
    virtual QString toString(bool full) = 0;
};

#endif // TEMPLATE_H
