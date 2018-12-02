#include "cell.h"

Cell::Cell() {
//    qDebug() << "Cell::Cell(); -- ";
//    this.backgroundTiles = new Array<TiledMapTile>();
//    this.groundTiles = new Array<TiledMapTile>();
//    this.foregroundTiles = new Array<TiledMapTile>();
//    this.trees = new Array<Tree>();
    this->empty = true;
    this->terrain = false;
    this->removableTerrain = true;

    this->tower = NULL;
//    this->units = NULL;
    this->spawn = false;
    this->exit = false;

    graphicCoordinates1 = new Vector2();
    graphicCoordinates2 = new Vector2();
    graphicCoordinates3 = new Vector2();
    graphicCoordinates4 = new Vector2();
}

Cell::~Cell() {
//    qDebug() << "Cell::~Cell(); -- units:" << units.size();
    backgroundTiles.clear();
    groundTiles.clear();
    foregroundTiles.clear();
//    trees.clear();

    tower = NULL;
    units.clear();
    delete graphicCoordinates1;
    delete graphicCoordinates2;
    delete graphicCoordinates3;
    delete graphicCoordinates4;
//    qDebug() << "Cell::~Cell(); -end- ";
}

void Cell::setGraphicCoordinates(int cellX, int cellY, float sizeCellX, float sizeCellY, bool isometric) {
//    qDebug() << "Cell::setGraphicCoordinates(); -- cellX:" << cellX << " cellY:" << cellY << " sizeCellX:" << sizeCellX << " sizeCellY:" << sizeCellY << " isometric:" << isometric;
    this->cellX = cellX;
    this->cellY = cellY;
    float halfSizeCellX = sizeCellX/2;
    float halfSizeCellY = sizeCellY/2;
    if (isometric) {
//    if(map == 1) { // Нижняя карта-java // Верхняя карта-с++
        graphicCoordinates1->x = (-(halfSizeCellX * cellY) + (cellX * halfSizeCellX) );
        graphicCoordinates1->y = (-(halfSizeCellY * cellY) - (cellX * halfSizeCellY) ) - halfSizeCellY;
//    } else if(map == 2) { // Правая карта-(java && c++)
        graphicCoordinates2->x = ( (halfSizeCellX * cellY) + (cellX * halfSizeCellX) ) + halfSizeCellX;
        graphicCoordinates2->y = ( (halfSizeCellY * cellY) - (cellX * halfSizeCellY) );
//    } else if(map == 3) { // Верхняя карта-java // Нижняя карта-c++
        graphicCoordinates3->x = (-(halfSizeCellX * cellY) + (cellX * halfSizeCellX) );
        graphicCoordinates3->y = ( (halfSizeCellY * cellY) + (cellX * halfSizeCellY) ) + halfSizeCellY;
//    } else if(map == 4) {// Левая карта-(java && c++)
        graphicCoordinates4->x = (-(halfSizeCellX * cellY) - (cellX * halfSizeCellX) )- halfSizeCellX;
        graphicCoordinates4->y = ( (halfSizeCellY * cellY) - (cellX * halfSizeCellY) );
//    }
    } else {
//    if(map == 1) { // НижняяЛевая карта-java // ВерхняяЛевая карта-с++
        graphicCoordinates1->x = (-(cellX * sizeCellX) ) - halfSizeCellX;
        graphicCoordinates1->y = (-(cellY * sizeCellY) ) - halfSizeCellY;
//    } else if(map == 2) { // НижняяПравая карта-java // ВерхняяПравая карта-с++
        graphicCoordinates2->x = ( (cellX * sizeCellX) ) + halfSizeCellX;
        graphicCoordinates2->y = (-(cellY * sizeCellY) ) - halfSizeCellY;
//    } else if(map == 3) { // ВерхняяПравая карта-java // НижняяПравая карта-с++
        graphicCoordinates3->x = ( (cellX * sizeCellX) ) + halfSizeCellX;
        graphicCoordinates3->y = ( (cellY * sizeCellY) ) + halfSizeCellY;
//    } else if(map == 4) {// ВерхняяЛевая карта-java // НижняяЛевая карта-c++
        graphicCoordinates4->x = (-(cellX * sizeCellX) ) - halfSizeCellX;
        graphicCoordinates4->y = ( (cellY * sizeCellY) ) + halfSizeCellY;
//    }
    }
}

Vector2 *Cell::getGraphicCoordinates(int map) {
    if(map == 1) {
        return graphicCoordinates1;
    } else if(map == 2) {
        return graphicCoordinates2;
    } else if(map == 3) {
        return graphicCoordinates3;
    } else if(map == 4) {
        return graphicCoordinates4;
    }
    qDebug() << "Cell::getGraphicCoordinates(" << map << "); -- Bad map | return graphicCoordinates1!";
    return graphicCoordinates1;
}

bool Cell::isEmpty() {
    return empty;
}

bool Cell::isTerrain() {
    return terrain;
}

bool Cell::setTerrain(Tile *tile, bool removable, bool withTower) {
    if (tile != NULL) {
        groundTiles.push_back(tile);
    }
    if ( (empty && !spawn && !exit) || (withTower && tower != NULL) ) {
        removableTerrain = removable;
        terrain = true;
        empty = false;
        return true;
    }
    return false;
}

bool Cell::removeTerrain(bool force) {
    if (terrain && (removableTerrain || force) ) {
        groundTiles.clear();
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
//        units = new Array<Unit*>();
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
    if (!units.empty()) {
        int size = units.size();
        if (unit == NULL) {
            return size;
        } else {
            for (int k = 0; k < size; k++) {
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
        if (unit == NULL) {
            units.clear();
        } else /*if (int num = containUnit(unit))*/ {
            auto it = std::find(units.begin(), units.end(), unit);
            if (it != units.end()) {
//                units.erase(units.begin()+(num-1));
                units.erase(it);
            }
        }
        if (units.size() == 0) {
//            units = null;
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
