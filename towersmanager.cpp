#include "towersmanager.h"

TowersManager::TowersManager() {
    qDebug() << "TowersManager::TowersManager(); -- ";
//    towers = new Array<Tower*>();
}

TowersManager::~TowersManager() {
    qDebug() << "TowersManager::~TowersManager(); -- ";
    towers.clear();
}

Tower* TowersManager::createTower(Cell *cell, TemplateForTower *templateForTower, int player) {
    Tower* tower = new Tower(cell, templateForTower, player);
    towers.push_back(tower);
    return tower;
}

Tower* TowersManager::getTower(int number) {
    return towers.at(number);
}

Tower* TowersManager::getTower(int x, int y) {
    if (x == -1 && y == -1) {
        return towers.back();
    } else {
        foreach (Tower* tower, towers) {
            if (tower->cell->cellX == x && tower->cell->cellY == y) {
                return tower;
            }
        }
    }
    return NULL;
}

bool TowersManager::removeTower(Tower* tower) {
    std::vector<Tower*>::iterator it = std::find(towers.begin(), towers.end(), tower);
    if (it != towers.end()) {
        towers.erase(it);
        return true;
    }
    return false;
}

bool TowersManager::removeTower(int &x, int &y) {
    if (x == -1 && y == -1) {
        if (!towers.empty()) {
            Tower* tower = towers.back();
            x = tower->cell->cellX;
            y = tower->cell->cellY;
            towers.pop_back();
//            towers.erase(towers.end());
            return true;
        }
    } else {
        foreach (Tower* tower, towers) {
            if (tower->cell->cellX == x && tower->cell->cellY == y) {
                return removeTower(tower);
            }
        }
    }
    return false;
}
