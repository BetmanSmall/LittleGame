#ifndef TOWERS_H
#define TOWERS_H

#include <stdlib.h>

#include <QDebug>
#include <QPixmap>

#include "src/head/tower.h"

class TowersManager {
public: // we are friendly to all!
    Tower* towers;
    QPixmap defaultPixmapForTower;
    int difficultyLevel;
    int size;
    int amount;

public:
    TowersManager():towers(NULL) {}
    ~TowersManager() {deleteField();}
    void createField(int size);
    void deleteField();
    int getSize();
    int getAmount();
    Tower* getTowerById(int number);
    Tower* getTower(int x = -1, int y = -1);
    Tower* createTower(int x, int y, int type = 0);
    Tower* createTower(int x, int y, TemplateForTower* tower);
    bool deleteTower(int &x, int &y);
    void setDefaulPixmapForTower(QPixmap pixmap);
    QPixmap getTowerPixmap(int x, int y);
};

#endif // TOWERS_H
