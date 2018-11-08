#ifndef CELL_H
#define CELL_H

#include <iostream>
#include <qstring.h>
#include <QPixmap>

#include "src/head/tower.h"
#include "src/head/unit.h"

class Cell {
    class Tree {
        QPixmap* textureRegion;
        int width, height;

        Tree(QPixmap* textureRegion, int width, int height) {
            this->textureRegion = textureRegion;
            this->width = width;
            this->height = height;
        }
    };
public:
    std::vector<Tile*> backgroundTiles;
    std::vector<Tile*> groundTiles;
    std::vector<Tile*> foregroundTiles;
    std::vector<Tree> trees;
    bool empty;
    bool terrain;
    bool removableTerrain;

    Tower* tower;
    std::vector<Unit*> units;
    bool spawn, exit;

    int cellX, cellY;
    QPointF* graphicCoordinatesBottom, *graphicCoordinatesRight, *graphicCoordinatesTop, *graphicCoordinatesLeft;

public:
    Cell();
    ~Cell();

    void setGraphicCoordinates(int cellX, int cellY, float halfSizeCellX, float halfSizeCellY);
    QPointF* getGraphicCoordinates(int map);

    bool isEmpty();
    bool isTerrain();
    bool setTerrain(Tile* tile, bool removable = true);
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
    int removeUnit(Unit* unit = NULL);
    QString toString();
};

#endif // CELL_H
