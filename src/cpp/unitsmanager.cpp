#include "src/head/unitsmanager.h"

UnitsManager::UnitsManager(int difficultyLevel) {
    qDebug() << "UnitsManager::UnitsManager(); -- ";
    this->difficultyLevel = difficultyLevel;
}

UnitsManager::~UnitsManager() {
    qDebug() << "UnitsManager::~UnitsManager(); -- ";
    units.clear();
}


Unit* UnitsManager::createUnit(AStar::CoordinateList path, TemplateForUnit* templateForUnit, int player){
    Unit* unit = new Unit(path, templateForUnit, player);
    units.push_back(unit);
    if (player != 0) {
        hero.push_back(unit);
    }
    return unit;
}

Unit* UnitsManager::getUnit(int number) {
    return units.at(number);
}

Unit* UnitsManager::getUnit(int x, int y) {
    foreach (Unit* unit, units) {
        int localX = unit->coorByCellX;
        int localY = unit->coorByCellY;
        if (localX == x && localY == y) {
            return unit;
        }
    }
    return NULL;
}

//int UnitsManager::getHP(int x, int y) {
//    foreach (Unit* unit, units) {
//        int localX = unit->coorByCellX;
//        int localY = unit->coorByCellY;
//        if(localX == x && localY == y) {
//            return unit.hp;
//        }
//    }
//    return 0;
//}

bool UnitsManager::attackUnit(int x, int y, int damage) {//, Unit *unit) {
    foreach (Unit* unit, units) {
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
                Direction::type direction = unit->direction;
                if(direction == Direction::type::UP_LEFT) {
                    unit->animationMaxIter = unit->templateForUnit->death_up_left.size();
                    unit->activePixmaps = unit->templateForUnit->death_up_left;
                } else if(direction == Direction::type::UP) {
                    unit->animationMaxIter = unit->templateForUnit->death_up.size();
                    unit->activePixmaps = unit->templateForUnit->death_up;
                } else if(direction == Direction::type::UP_RIGHT) {
                    unit->animationMaxIter = unit->templateForUnit->death_up_right.size();
                    unit->activePixmaps = unit->templateForUnit->death_up_right;
                } else if(direction == Direction::type::LEFT) {
                    unit->animationMaxIter = unit->templateForUnit->death_left.size();
                    unit->activePixmaps = unit->templateForUnit->death_left;
                } else if(direction == Direction::type::RIGHT) {
                    unit->animationMaxIter = unit->templateForUnit->death_right.size();
                    unit->activePixmaps = unit->templateForUnit->death_right;
                } else if(direction == Direction::type::DOWN_LEFT) {
                    unit->animationMaxIter = unit->templateForUnit->death_down_left.size();
                    unit->activePixmaps = unit->templateForUnit->death_down_left;
                } else if(direction == Direction::type::DOWN) {
                    unit->animationMaxIter = unit->templateForUnit->death_down.size();
                    unit->activePixmaps = unit->templateForUnit->death_down;
                } else if(direction == Direction::type::DOWN_RIGHT) {
                    unit->animationMaxIter = unit->templateForUnit->death_down_right.size();
                    unit->activePixmaps = unit->templateForUnit->death_down_right;
                }
                unit->animationMaxIter += unit->templateForUnit->explosion.size();
                unit->activePixmaps.insert(unit->activePixmaps.end(), unit->templateForUnit->explosion.begin(), unit->templateForUnit->explosion.end());
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
