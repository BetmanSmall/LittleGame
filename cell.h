#ifndef CELL_H
#define CELL_H

#include <iostream>
#include <qstring.h>
#include <QPixmap>

#include "tower.h"
#include "unit.h"

//class Tree {
//    TextureRegion textureRegion;
//    int width, height;

//    Tree(TextureRegion textureRegion, int width, int height) {
//        this.textureRegion = textureRegion;
//        this.width = width;
//        this.height = height;
//    }
//}

/**
 * @brief Ячейка
 */
class Cell {
public:
    int unitStepWA;
    bool hero;
    bool empty;
    bool terrain;
    Tower* tower;
//    bool tower;
    std::vector<Unit*> units;

    bool spawn;
    bool exit;

    QPixmap backgroundPixmap;
    QPixmap busyPixmap;
    std::vector<QPixmap> backgroundTiles;
    std::vector<QPixmap> foregroundiles;
//        Array<Tree> trees;
//        private bool empty;
// //        private bool terrain;
//        private Tower tower;
//        private Array<Creep> creeps;
//        int cellX, cellY;
//        Vector2 graphicCoordinatesBottom, graphicCoordinatesRight, graphicCoordinatesTop, graphicCoordinatesLeft;
public:
    Cell();

//    void setGraphicCoordinates(int cellX, int cellY, float halfSizeCellX, float halfSizeCellY);
//    Vector2 getGraphicCoordinates(int map);
//    void addTiledMapTile(TiledMapTile tiledMapTile);

    bool isEmpty();
    bool isTerrain();
    bool setTerrain(QPixmap pixmap = QPixmap());
    bool removeTerrain();
    bool isPassable();
//    bool getTower();
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
