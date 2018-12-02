#ifndef CELL_H
#define CELL_H

#include <iostream>
#include <qstring.h>
#include <QPixmap>

#include "src/head/unit.h"
#include "src/head/tower.h"
#include "src/head/mapEditor/tile.h"
#include "vector2.h"

class Unit;
class Tower;

class Cell {
//    class Tree {
////    public:
//        QPixmap* textureRegion;
//        int width;
//        int height;

//        Tree(QPixmap* textureRegion, int width, int height) {
//            this->textureRegion = textureRegion;
//            this->width = width;
//            this->height = height;
//        }
//    };
public:
    std::vector<Tile*> backgroundTiles;
    std::vector<Tile*> groundTiles;
    std::vector<Tile*> foregroundTiles;
//    std::vector<Tree> trees;
    bool empty;
    bool terrain;
    bool removableTerrain;

    Tower *tower;
    std::vector<Unit*> units;
    bool spawn;
    bool exit;

    int cellX, cellY;
    Vector2 *graphicCoordinates1;
    Vector2 *graphicCoordinates2;
    Vector2 *graphicCoordinates3;
    Vector2 *graphicCoordinates4;

public:
    Cell();
    ~Cell();

    void setGraphicCoordinates(int cellX, int cellY, float sizeCellX, float sizeCellY, bool isometric);
    Vector2 *getGraphicCoordinates(int map);

    bool isEmpty();
    bool isTerrain();
    bool setTerrain(Tile* tile, bool removable = true, bool withTower = true);
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
