#include "src/head/towersmanager.h"

TowersManager::TowersManager(int difficultyLevel) {
    this->difficultyLevel = difficultyLevel;
}

TowersManager::~TowersManager() {
    towers.clear();
}

Tower* TowersManager::createTower(int x, int y, TemplateForTower* templateForTower) {
    Tower* tower = new Tower(x, y, templateForTower);
    towers.push_back(tower);
    return tower;
}

bool TowersManager::removeTower(Tower* tower) {
    std::vector<Tower*>::iterator position = std::find(towers.begin(), towers.end(), tower);
    if (position != towers.end()) {
        towers.erase(position);
        return true;
    }
    return false;
}

bool TowersManager::deleteTower(int &x, int &y) {
    if (x == -1 && y == -1) {
        if (!towers.empty()) {
            Tower* tower = towers.back();
            x = tower->currX;
            y = tower->currY;
            towers.pop_back();
//            towers.erase(towers.end());
            return true;
        }
    } else {
        foreach (Tower* tower, towers) {
            if (tower->currX == x && tower->currY == y) {
                return removeTower(tower);
            }
        }
    }
    return false;
}

Tower* TowersManager::getTower(int number) {
    return towers.at(number);
}

Tower* TowersManager::getTower(int x, int y) {
    if (x == -1 && y == -1) {
        return towers.back();
    } else {
        foreach (Tower* tower, towers) {
            if (tower->currX == x && tower->currY == y) {
                return tower;
            }
        }
    }
    return NULL;
}
