#include "src/head/cell.h"

Cell::Cell() {
//    qDebug() << "Cell::Cell(); -- ";
//    this.backgroundTiles = new Array<TiledMapTile>();
//    this.foregroundTiles = new Array<TiledMapTile>();
//    this.trees = new Array<Tree>();
    this->empty = true;
    this->terrain = false;
    this->removableTerrain = true;

    this->tower = NULL;
//    this->units = NULL;
    this->spawn = false;
    this->exit = false;

//    setGraphicCoordinates(cellX, cellY, halfSizeCellX, halfSizeCellY);
    graphicCoordinatesBottom = new QPointF();
    graphicCoordinatesRight = new QPointF();
    graphicCoordinatesTop = new QPointF();
    graphicCoordinatesLeft = new QPointF();
}

Cell::~Cell() {
//    qDebug() << "Cell::~Cell(); -- units:" << units.size();
    backgroundTiles.clear();
    groundTiles.clear();
    foregroundTiles.clear();
    trees.clear();

    tower = NULL;
    units.clear();
    delete graphicCoordinatesBottom;
    delete graphicCoordinatesRight;
    delete graphicCoordinatesTop;
    delete graphicCoordinatesLeft;
//    qDebug() << "Cell::~Cell(); -end- ";
}

void Cell::setGraphicCoordinates(int cellX, int cellY, float halfSizeCellX, float halfSizeCellY) {
//    qDebug() << "Cell::setGraphicCoordinates(); -- cellX:" << cellX << " cellY:" << cellY << " halfSizeCellX:" << halfSizeCellX << " halfSizeCellY:" << halfSizeCellY;
    this->cellX = cellX;
    this->cellY = cellY;
//        if(map == 1) { // Нижняя карта-java // Верхняя карта-с++
//    graphicCoordinatesBottom = new QPointF( (-(halfSizeCellX * cellY) + (cellX * halfSizeCellX)),                   (-(halfSizeCellY * cellY) - (cellX * halfSizeCellY)));
    graphicCoordinatesBottom->setX( (-(halfSizeCellX * cellY) + (cellX * halfSizeCellX)) );
    graphicCoordinatesBottom->setY( (-(halfSizeCellY * cellY) - (cellX * halfSizeCellY)) );
//        } else if(map == 2) { // Правая карта
//    graphicCoordinatesRight = new QPointF(  ( (halfSizeCellX * cellY) + (cellX * halfSizeCellX)) + halfSizeCellX,   ( (halfSizeCellY * cellY) - (cellX * halfSizeCellY)) + halfSizeCellY);
    graphicCoordinatesRight->setX( ( (halfSizeCellX * cellY) + (cellX * halfSizeCellX)) + halfSizeCellX );
    graphicCoordinatesRight->setY( ( (halfSizeCellY * cellY) - (cellX * halfSizeCellY)) + halfSizeCellY );
//        } else if(map == 3) { // Верхняя карта-c++ // Нижняя карта-java
//    graphicCoordinatesTop = new QPointF(    (-(halfSizeCellX * cellY) + (cellX * halfSizeCellX)),                   ( (halfSizeCellY * cellY) + (cellX * halfSizeCellY)) + halfSizeCellY * 2);
    graphicCoordinatesTop->setX( (-(halfSizeCellX * cellY) + (cellX * halfSizeCellX)) );
    graphicCoordinatesTop->setY( ( (halfSizeCellY * cellY) + (cellX * halfSizeCellY)) + halfSizeCellY * 2);
//        } else if(map == 4) {// Левая карта
//    graphicCoordinatesLeft = new QPointF(   (-(halfSizeCellX * cellY) - (cellX * halfSizeCellX)) - halfSizeCellX,   ( (halfSizeCellY * cellY) - (cellX * halfSizeCellY)) + halfSizeCellY);
    graphicCoordinatesLeft->setX( (-(halfSizeCellX * cellY) - (cellX * halfSizeCellX)) - halfSizeCellX );
    graphicCoordinatesLeft->setY( ( (halfSizeCellY * cellY) - (cellX * halfSizeCellY)) + halfSizeCellY );
//        }
}

QPointF* Cell::getGraphicCoordinates(int map) {
    if(map == 1) {
        return graphicCoordinatesBottom;
    } else if(map == 2) {
        return graphicCoordinatesRight;
    } else if(map == 3) {
        return graphicCoordinatesTop;
    } else if(map == 4) {
        return graphicCoordinatesLeft;
    }
    qDebug() << "Cell::getGraphicCoordinates(" << map << "); -- Bad map | return null!";
    return NULL;
}

bool Cell::isEmpty() {
    return empty;
}

bool Cell::isTerrain() {
    return terrain;
}

bool Cell::setTerrain(Tile *tile, bool removable) {
    if (tile != NULL) {
        groundTiles.push_back(tile);
    }
    if (empty && !spawn && !exit) {
        removableTerrain = removable;
        terrain = true;
        empty = false;
        return true;
    } else {
        return false;
    }
}

bool Cell::removeTerrain(bool force) {
    if (terrain && (removableTerrain || force) ) {
        terrain = false;
        empty = true;
        return true;
    }
    return false;
}

bool Cell::isPassable() {
    if (empty || (!terrain && tower != NULL) || units.size()) {
        return true;
    }
    return false;
}

Tower* Cell::getTower() {
    return tower;
}

bool Cell::setTower(Tower *tower) {
    if (empty) {
        this->tower = tower;
        empty = false;
        return true;
    }
    return false;
}

bool Cell::removeTower() {
    if (tower != NULL) {
        tower = NULL;
        empty = true;
        return true;
    }
    return false;
}

Unit *Cell::getHero() {
    foreach (Unit* unit, units) {
        if (unit->player == 1) {
            return unit;
        }
    }
    return NULL;
}

std::vector<Unit*> Cell::getUnits() {
    return units;
}

Unit* Cell::getUnit() {
    if (!units.empty()) {
        return units.front();
    }
    return NULL;
}

bool Cell::setUnit(Unit* unit) {
    if (empty) {
        units.push_back(unit);
        empty = false;
        return true;
    } else if (units.size()) {
        units.push_back(unit);
        return true;
    }
    return false;
}

int Cell::containUnit(Unit* unit) {
    if(!units.empty()) {
        int size = units.size();
        if(unit == NULL) {
            return size;
        } else {
            for(int k = 0; k < size; k++) {
                if(units[k] == unit) {
                    return k+1;
                }
            }
        }
    }
    return 0;
}

int Cell::removeUnit(Unit* unit) {
    if (!empty && units.size()) {
        if(unit == NULL) {
            units.clear();
        } else if(int num = containUnit(unit)) {
            units.erase(units.begin()+(num-1));
        }
        if (units.size() == 0) {
            empty = true;
            return 0;
        }
        return units.size();
    }
    return -1;
}

QString Cell::toString() {
    QString sb("Cell[");
    sb.append(QString("cellX:%1").arg(cellX));
    sb.append(QString(",cellY:%1").arg(cellY));
    sb.append(QString(",empty:%1").arg(empty));
    sb.append(QString(",terrain:%1").arg(terrain));
    sb.append(QString(",removableTerrain:%1").arg(removableTerrain));
    sb.append(QString(",tower:%1").arg(tower!=NULL));
    sb.append(QString(",units:%1").arg(units.size()));
    sb.append(QString(",spawn:%1").arg(spawn));
    sb.append(QString(",exit:%1").arg(exit));
    sb.append(QString(",backgroundTiles:%1").arg(backgroundTiles.size()));
    sb.append(QString(",groundTiles:%1").arg(groundTiles.size()));
    sb.append(QString(",foregroundTiles:%1").arg(foregroundTiles.size()));
//    sb.append(QString(",graphicCoordinatesBottom:%1").arg(graphicCoordinatesBottom));
    sb.append("]");
    return sb;
}
