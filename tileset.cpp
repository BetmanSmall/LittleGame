#include "tileset.h"

TileSet::TileSet() {
//    firshgid = properties.value("firstgid").toInt();
}

TileSet::TileSet(QString name) {
    this->name = name;
}

void TileSet::setName(QString name) {
    this->name = name;
}

QString TileSet::getName() {
    return name;
}

QMap<QString, QString> *TileSet::getProperties() {
    return &properties;
}

void TileSet::putTile(int id, Tile *tile) {
    tiles.insert(id, tile);
}

Tile *TileSet::getGlobalTile(int id) {
    if(id >= firshgid)
        return tiles[id-firshgid];
    else
        return NULL;
}

Tile *TileSet::getLocalTile(int id) {
    if(id >= 1 && id <= tiles.size())
        return tiles[id];
    else
        return NULL;
}

QList<Tile*> TileSet::getTiles() {
    return tiles.values();
}
