#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <QDebug>
#include <QtXml>

#include "maploader.h"
#include "animatedtile.h"
#include "tile.h"

class Template
{
public:
    QString templateName;
    QMap<QString, QString> properties;
    QMap<int, Tile*> tiles;
    QMap<int, AnimatedTile*> animatedTiles;

    Template();
    ~Template();
    void loadBasicTemplate(QString templateFile);
    void basicValidate();
    QMap<QString, QString>* getProperties();
    QString toStringProperties();
    QString toStringBasicParam();

    virtual QString toString() = 0;
    virtual QString toString(bool full) = 0;
};

#endif // TEMPLATE_H
