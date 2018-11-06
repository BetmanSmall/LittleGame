#ifndef TOWERS_H
#define TOWERS_H

#include <stdlib.h>

#include <QDebug>
#include <QPixmap>

#include "src/head/tower.h"

class TowersManager {
public: // we are friendly to all!
    std::vector<Tower*> towers;
    int difficultyLevel;

public:
    TowersManager(int difficultyLevel);
    ~TowersManager();
    Tower* createTower(QPoint* position, TemplateForTower* templateForTower, int player);
    bool removeTower(Tower* tower);
    bool deleteTower(int &x, int &y);
    Tower* getTower(int number);
    Tower* getTower(int x = -1, int y = -1);
};

#endif // TOWERS_H
