#include "towersmanager.h"

void TowersManager::createField(int size) {
    if (towers == NULL) {
        towers = new Tower[size];
        this->size = size;
        this->amount = 0;
    } else {
        deleteField();
        createField(size);
    }
}

void TowersManager::deleteField() {
    if (towers != NULL) {
        delete[] towers;
        towers = NULL;
    }
}

int TowersManager::getSize() {
    return size;
}

int TowersManager::getAmount() {
    return amount;
}

Tower* TowersManager::getTowerById(int number) {
    return &towers[number];
}

Tower* TowersManager::getTower(int x, int y) {
    if (x == -1 && y == -1) {
        return &towers[amount-1];
    } else {
        for (int k = 0; k < amount; k++)
            if (towers[k].currX == x && towers[k].currY == y)
                return &towers[k];
    }
    return NULL;
}

Tower* TowersManager::createTower(int x, int y, int type) {
    if (amount < size) {
        towers[amount].attack = 5;
        towers[amount].currX = x;
        towers[amount].currY = y;
        towers[amount].radius = 1;
        towers[amount].type = type;
        towers[amount].pixmap = defaultPixmapForTower;
        return &towers[amount++];
    }
    return NULL;
}

Tower* TowersManager::createTower(int x, int y, DefaultTower* tower) {
    if (amount < size) {
        towers[amount].name = tower->name;
        towers[amount].attack = tower->attack;
        towers[amount].radius = tower->radius;
        towers[amount].size = tower->size;
        towers[amount].type = tower->type;
        towers[amount].elapsedReloadTime = tower->reloadTime;
        towers[amount].currX = x;
        towers[amount].currY = y;
        towers[amount].pixmap = tower->pixmap;
        towers[amount].defTower = tower;
        return &towers[amount++];
    }
    return NULL;
}

bool TowersManager::deleteTower(int &x, int &y) {
    if (x == -1 && y == -1) {
        x = towers[amount-1].currX;
        y = towers[amount-1].currY;
        amount--;
        return true;
    } else {
        for (int k = 0; k < amount; k++) {
            if (towers[k].currX == x && towers[k].currY == y) {
                for (int l = k+1; l < amount; l++) {
                    towers[l-1] = towers[l];
                }
                amount--;
                return true;
            }
        }
    }
    return false;
}

void TowersManager::setDefaulPixmapForTower(QPixmap pixmap) {
    defaultPixmapForTower = pixmap;
}

QPixmap TowersManager::getTowerPixmap(int x, int y) {
    for (int k = 0; k < amount; k++) {
        int localX = towers[k].currX;
        int localY = towers[k].currY;
        if (localX == x && localY == y) {
            return towers[k].pixmap;
        }
    }
    return defaultPixmapForTower;
}
