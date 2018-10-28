#ifndef CELL_H
#define CELL_H

#include <iostream>
#include <qstring.h>
#include <QPixmap>

#include "src/head/tower.h"
#include "src/head/unit.h"

class Cell {
public:
    std::vector<QPixmap> backgroundTiles;
    std::vector<QPixmap> terrainTiles;
    std::vector<QPixmap> foregroundTiles;
//    std::vector<Tree> trees;
//    int unitStepWA;
    bool empty;
    bool removableTerrain;
    bool terrain;
    Tower* tower;
    std::vector<Unit*> units;

    int cellX, cellY;
    QPointF* graphicCoordinatesBottom, *graphicCoordinatesRight, *graphicCoordinatesTop, *graphicCoordinatesLeft;

    bool spawn, exit;

public:
    Cell();
    ~Cell();

    void setGraphicCoordinates(int cellX, int cellY, float halfSizeCellX, float halfSizeCellY);
    QPointF* getGraphicCoordinates(int map);

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
