#include "src/head/towersmanager.h"

TowersManager::TowersManager(int difficultyLevel) {
    qDebug() << "TowersManager::TowersManager(); -- ";
    this->difficultyLevel = difficultyLevel;
}

TowersManager::~TowersManager() {
    qDebug() << "TowersManager::~TowersManager(); -- ";
    towers.clear();
}

Tower* TowersManager::createTower(QPoint *position, TemplateForTower* templateForTower, int player) {
    Tower* tower = new Tower(position, templateForTower, player);
    towers.push_back(tower);
    return tower;
}

bool TowersManager::removeTower(Tower* tower) {
    std::vector<Tower*>::iterator it = std::find(towers.begin(), towers.end(), tower);
    if (it != towers.end()) {
        towers.erase(it);
        return true;
    }
    return false;
}

bool TowersManager::deleteTower(int &x, int &y) {
    if (x == -1 && y == -1) {
        if (!towers.empty()) {
            Tower* tower = towers.back();
            x = tower->position->x();
            y = tower->position->y();
            towers.pop_back();
//            towers.erase(towers.end());
            return true;
        }
    } else {
        foreach (Tower* tower, towers) {
            if (tower->position->x() == x && tower->position->y() == y) {
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
            if (tower->position->x() == x && tower->position->y() == y) {
                return tower;
            }
        }
    }
    return NULL;
}
