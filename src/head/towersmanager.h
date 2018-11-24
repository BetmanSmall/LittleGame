#ifndef TOWERS_H
#define TOWERS_H

#include <stdlib.h>

#include <QDebug>
#include <QPixmap>

#include "src/head/tower.h"

class Tower;

class TowersManager {
public: // we are friendly to all!
    std::vector<Tower*> towers;

public:
    TowersManager();
    ~TowersManager();
    Tower* createTower(Cell *cell, TemplateForTower *templateForTower, int player);
    Tower* getTower(int number);
    Tower* getTower(int x = -1, int y = -1);
    bool removeTower(Tower* tower);
    bool removeTower(int &x, int &y);
};

#endif // TOWERS_H
