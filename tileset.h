#ifndef TILESET_H
#define TILESET_H

#include <QMap>

#include <QPixmap>

#include "tile.h"

class TileSet {
//    int firstTileID;
//    QString name;
//    int spacing;
//    int margin;
//    int tileWidth;
//    int tileHeight;
//    QPixmap img;
//    std::vector<QRect> subRects;
//    std::vector<QString> tilesNames; // fireball specific load
//    std::vector<QPixmap> tiles;

public:
    QString name;
    QMap<int, Tile*> tiles;
    QMap<QString, QString> properties;
//public:
    TileSet();
    TileSet(QString name);
    void setName(QString name);
    QString getName();
    QMap<QString, QString> *getProperties();
    void putTile(int id, Tile *tile);
    Tile *getGlobalTile(int id);
    Tile *getLocalTile(int id);
    QList<Tile*> getTiles();

    int firshgid;
};

#endif // TILESET_H
