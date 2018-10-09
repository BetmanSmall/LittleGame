#include "unitsmanager.h"

UnitsManager::UnitsManager() {
    qDebug() << "UnitsManager::UnitsManager(); -- ";
    hero = NULL;
    units = NULL;
}

UnitsManager::~UnitsManager() {
    qDebug() << "UnitsManager::~UnitsManager(); -- ";
    deleteMass();
}

void UnitsManager::createMass(int newSize) {
    if(units == NULL) {
        units = new Unit[newSize];
        size = newSize;
        amount = 0;
    } else {
        deleteMass();
        createMass(newSize);
    }
}

void UnitsManager::deleteMass() {
    if(units != NULL) {
        delete[] units;
        units = NULL;
    }
}

int UnitsManager::getSize() {
    return size;
}

int UnitsManager::getAmount() {
    return amount;
}

int UnitsManager::getHP(int x, int y) {
    for(int k = 0; k < amount; k++) {
        int localX = units[k].coorByCellX;
        int localY = units[k].coorByCellY;
        if(localX == x && localY == y) {
            return units[k].hp;
        }
    }
    return 0;
}

bool UnitsManager::attackUnit(int x, int y, int damage) {//, Unit *unit) {
    for(int k = 0; k < amount; k++) {
        Unit* unit = &units[k];
        if(!unit->alive) {
            continue;
        }
        int localX = unit->coorByCellX;
        int localY = unit->coorByCellY;
        if(localX == x && localY == y) {
            int localHP = unit->hp;
            localHP = localHP - damage;
            if(localHP <= 0) {
                unit->hp = 0;
                unit->alive = false;
                unit->preDeath = true;
                unit->animationLastAliveIter = unit->animationCurrIter;
                unit->animationCurrIter = 0;
                Direction direction = unit->direction;
                if(direction == DirectionUpLeft) {
                    unit->animationMaxIter = unit->defUnit->death_up_left.size();
                    unit->activePixmaps = unit->defUnit->death_up_left;
                } else if(direction == DirectionUp) {
                    unit->animationMaxIter = unit->defUnit->death_up.size();
                    unit->activePixmaps = unit->defUnit->death_up;
                } else if(direction == DirectionUpRight) {
                    unit->animationMaxIter = unit->defUnit->death_up_right.size();
                    unit->activePixmaps = unit->defUnit->death_up_right;
                } else if(direction == DirectionLeft) {
                    unit->animationMaxIter = unit->defUnit->death_left.size();
                    unit->activePixmaps = unit->defUnit->death_left;
                } else if(direction == DirectionRight) {
                    unit->animationMaxIter = unit->defUnit->death_right.size();
                    unit->activePixmaps = unit->defUnit->death_right;
                } else if(direction == DirectionDownLeft) {
                    unit->animationMaxIter = unit->defUnit->death_down_left.size();
                    unit->activePixmaps = unit->defUnit->death_down_left;
                } else if(direction == DirectionDown) {
                    unit->animationMaxIter = unit->defUnit->death_down.size();
                    unit->activePixmaps = unit->defUnit->death_down;
                } else if(direction == DirectionDownRight) {
                    unit->animationMaxIter = unit->defUnit->death_down_right.size();
                    unit->activePixmaps = unit->defUnit->death_down_right;
                }
                unit->animationMaxIter += unit->defUnit->explosion.size();
                unit->activePixmaps.insert(unit->activePixmaps.end(), unit->defUnit->explosion.begin(), unit->defUnit->explosion.end());
                unit->pixmap = unit->activePixmaps[0];
                return true;
            } else {
                unit->hp = localHP;
            }
            break;
        }
    }
    return false;
}

Unit* UnitsManager::getUnit(int number) {
    return &units[number];
}

Unit* UnitsManager::getUnit(int x, int y) {
    for (int k = 0; k < amount; k++) {
        int localX = units[k].coorByCellX;
        int localY = units[k].coorByCellY;
        if (localX == x && localY == y) {
            return &units[k];
        }
    }
    return NULL;
}

Unit* UnitsManager::createHero(int coorByCellX, int coorByCellY, int coorByMapX, int coorByMapY, DefaultUnit* unit) {
    if (hero == NULL) {
        hero = createUnit(coorByCellX, coorByCellY, coorByMapX, coorByMapY, unit, 0);
    }
    return hero;
}

Unit* UnitsManager::createUnit(int coorByCellX, int coorByCellY, int coorByMapX, int coorByMapY, DefaultUnit* unit, int type){
    if(amount < size) {
        units[amount].hp = 100;
        units[amount].alive = true;
        units[amount].preDeath = false;
        units[amount].number = amount+1;
        units[amount].coorByCellX = coorByCellX;
        units[amount].coorByCellY = coorByCellY;
        units[amount].coorByMapX = coorByMapX;
        units[amount].coorByMapY = coorByMapY;
        units[amount].lastX = coorByCellX;
        units[amount].lastY = coorByCellY;
        units[amount].type = type;
        units[amount].animationCurrIter = 0;
        units[amount].animationMaxIter = 0;
        units[amount].pixmap = defaultPixmapForUnit;
        units[amount].defUnit = unit;
        return &units[amount++];
    }
    return NULL;
}

void UnitsManager::setDefaulPixmapForUnit(QPixmap pixmap) {
    defaultPixmapForUnit = pixmap;
}

QPixmap UnitsManager::getUnitPixmap(int x, int y) {
    for(int k = 0; k < amount; k++) {
        int localX = units[k].coorByCellX;
        int localY = units[k].coorByCellY;
        if(localX == x && localY == y) {
            return units[k].pixmap;
        }
    }
    return defaultPixmapForUnit;
}
