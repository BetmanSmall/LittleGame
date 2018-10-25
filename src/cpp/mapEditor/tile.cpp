#include "src/head/mapEditor/tile.h"

Tile::Tile() {
//    qDebug() << "Tile::Tile();";
}

void Tile::setId(int id) {
//    qDebug() << "Tile::setId(" << id << ");";
    this->id = id;
}

int Tile::getId() {
//    qDebug() << "Tile::getId(); -- id:" << id;
    return id;
}

void Tile::setPixmap(QPixmap pixmap) {
    qDebug() << "Tile::setPixmap(" << pixmap << ");";
    this->pixmap = pixmap;
}

QPixmap Tile::getPixmap() {
//    qDebug() << "Tile::getPixmap(); --  pixmap:" << pixmap;
    return pixmap;
}

QPixmap *Tile::getPixmapRef() {
//    qDebug() << "Tile::getPixmapRef(); --  pixmap:" << pixmap;
    return &pixmap;
}

QMap<QString, QString> *Tile::addProperties(QMap<QString, QString> properties) {
    qDebug() << "Tile::addProperties(); -- properties:" << properties;
    foreach (QString key, properties.keys()) {
        this->properties.insert(key, properties.value(key));
    }
    return &(this->properties);
}

QMap<QString, QString> *Tile::getProperties() {
//    qDebug() << "Tile::getProperties(); -- properties:" << &properties; // .toString();
    return &properties;
}

void Tile::setBlendMode(BlendMode blendMode) {
    qDebug() << "Tile::setBlendMode(" << blendMode << ");";
    this->blendMode = blendMode;
}

Tile::BlendMode Tile::getBlendMode() {
    qDebug() << "Tile::getBlendMode(); -- blendMode:" << blendMode;
    return blendMode;
}

void Tile::setOffsetX(float offsetX) {
    qDebug() << "Tile::setOffsetX(" << offsetX << ");";
    this->offsetX = offsetX;
}

float Tile::getOffsetX() {
    qDebug() << "Tile::getOffsetX(); -- offsetX:" << offsetX;
    return offsetX;
}

void Tile::setOffsetY(float offsetY) {
    qDebug() << "Tile::setOffsetY(" << offsetY << ");";
    this->offsetY = offsetY;
}

float Tile::getOffsetY() {
    qDebug() << "Tile::getOffsetY(); -- offsetY:" << offsetY;
    return offsetY;
}
