#include "src/head/mapEditor/statictile.h"

//StaticTile::StaticTile() {
//}

//int StaticTile::getId() {
//    return id;
//}

//void StaticTile::setId(int id) {
//    this->id = id;
//}

//StaticTile::BlendMode StaticTile::getBlendMode() {
//    return blendMode;
//}

//void StaticTile::setBlendMode(BlendMode blendMode) {
//    this->blendMode = blendMode;
//}

//QMap<QString, QString> *StaticTile::getProperties() {
////    if (properties == NULL) {
////        properties = new QMap<QString, QString>();
////    }
//    return &properties;
//}

QPixmap StaticTile::getPixmap() {
    return pixmap;
}

void StaticTile::setPixmap(QPixmap pixmap) {
    this->pixmap = pixmap;
}

float StaticTile::getOffsetX() {
    return offsetX;
}

void StaticTile::setOffsetX(float offsetX) {
    this->offsetX = offsetX;
}

float StaticTile::getOffsetY() {
    return offsetY;
}

void StaticTile::setOffsetY(float offsetY) {
    this->offsetY = offsetY;
}

/** Creates a static tile with the given region
 *
 * @param QPixmap the {@link QPixmap} to use. */
StaticTile::StaticTile(QPixmap pixmap) {
    this->pixmap = pixmap;
}

/** Copy constructor
 *
 * @param copy the StaticTiledMapTile to copy. */
//StaticTile::StaticTile(StaticTile copy) {
//    if (copy.properties != NULL) {
//        getProperties() =+ copy.properties;
//    }
//    this->pixmap = copy.pixmap;
//    this->id = copy.id;
//}
