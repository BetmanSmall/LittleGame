#include "cell.h"

Cell::Cell() {
        unitStepWA = 0;
        empty = true;
        this->removableTerrain = true;
        this->terrain = false;
        spawn = false;
        exit = false;

        hero = false;
        tower = NULL;
//        tower = false;
//        units = NULL;

//        backgroundPixmap = NULL;
//        busyPixmap = NULL;
//        Gdx.app.log("Cell::Cell()", "-- ");
//        this.backgroundTiles = new Array<TiledMapTile>();
//        this.foregroundTiles = new Array<TiledMapTile>();
//        this.trees = new Array<Tree>();
//        this.empty = true;
// //        this.terrain = false;
//        this.tower = NULL;
//        this.creeps = NULL;
//        setGraphicCoordinates(cellX, cellY, halfSizeCellX, halfSizeCellY);
}

bool Cell::isEmpty() {
    return empty;
}

bool Cell::isTerrain() {
    return terrain;
}

bool Cell::setTerrain(QPixmap pixmap, bool removable) {//int x, int y, QPixmap pixmap) {
    if (!pixmap.isNull()) { // Not good!
        terrainTiles.push_back(pixmap);
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
        if (unit->type == 0) { // type 0 = Hero
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
//        creeps = new Array<Creep>();
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

bool Cell::clearUnit(Unit* unit) {
    if(!empty && units.size()) {
        if(unit == NULL) {
            units.clear();
        } else if(int num = containUnit(unit)) {
            units.erase(units.begin()+(num-1));
        }
        if(units.empty()) {
            empty = true;
        }
        return true;
    }
    return false;
}

int Cell::removeUnit(Unit* unit) {
    if (!empty && units.size()) {
        if(unit == NULL) {
            units.clear();
        } else if(int num = containUnit(unit)) {
            units.erase(units.begin()+(num-1));
        }
        if (units.size() == 0) {
//            creeps = NULL;
            empty = true;
            return 0;
        }
        return units.size();
    }
    return -1;
}

//void Cell::dispose() {
//    backgroundTiles.clear();
//    foregroundTiles.clear();
//    backgroundTiles = NULL;
//    foregroundTiles = NULL;
//    tower = NULL;
//    creeps.clear();
//    creeps = NULL;
//}

//QString Cell::toString() {
//    StringBuilder sb = new StringBuilder();
//    sb.append("Cell[");
//    sb.append("cellX:" + cellX);
//    sb.append("," + "cellY:" + cellY);
//    sb.append("," + "empty:" + empty);
//    sb.append("," + "terrain:" + terrain);
//    sb.append("," + "tower:" + tower);
//    sb.append("," + "creeps:" + creeps);
//    sb.append("]");
//    return sb.toString();
//}
