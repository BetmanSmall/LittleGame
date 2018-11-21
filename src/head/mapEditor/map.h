#ifndef MAP_H
#define MAP_H

#include <QDebug>

#include "tilesets.h"
#include "maplayers.h"

class Map {
public:
    QString mapPath;
    TileSets tileSets;
    MapLayers mapLayers;
    QMap<QString, QString> properties;
    int width, height;
    int tileWidth, tileHeight;

public:
    Map(QString mapPath);
    Layer* addNewLayer();
    QMap<QString, QString> *getProperties();
    TileSets *getTileSets();
    MapLayers *getMapLayers();
    void turnRight();
    void turnLeft();
    void flipX();
    void flipY();
private:
};

#endif // MAP_H
