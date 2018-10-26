#ifndef CELL_H
#define CELL_H

#include <iostream>
#include <qstring.h>
#include <QPixmap>

#include "src/head/tower.h"
#include "src/head/unit.h"

class Cell {
public:
    int unitStepWA;
    bool hero;
    bool empty;
    bool removableTerrain;
    bool terrain;
    Tower* tower;
    std::vector<Unit*> units;

    bool spawn;
    bool exit;

    std::vector<QPixmap> backgroundTiles;
    std::vector<QPixmap> terrainTiles;
    std::vector<QPixmap> foregroundTiles;

public:
    Cell();

    bool isEmpty();
    bool isTerrain();
    bool setTerrain(QPixmap pixmap = QPixmap(), bool removable = true);
    bool removeTerrain(bool force = false);
    bool isPassable();

    Tower* getTower();
    bool setTower(Tower* tower);
    bool removeTower();
    Unit* getHero();
    std::vector<Unit*> getUnits();
    Unit* getUnit();
    bool setUnit(Unit* unit);
    int containUnit(Unit* unit = NULL);
    bool clearUnit(Unit* unit = NULL);
    int removeUnit(Unit* unit = NULL);
    void dispose();
    QString toString();
};

#endif // CELL_H
