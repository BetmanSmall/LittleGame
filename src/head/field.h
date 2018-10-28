#ifndef FIELD_H
#define FIELD_H

#include <iostream>
#include <QPixmap>
#include <QPainter>

//#include "src/head/libs/astar.h"
#include "src/head/unitsmanager.h"
#include "src/head/towersmanager.h"
#include "src/head/factionsmanager.h"
#include "src/head/bullet.h"
#include "src/head/cell.h"

class Field
{
public: // we are friendly!
    AStar::PathFinder pathFinder;
    Map* map;
    Cell* field;
    FactionsManager* factionsManager;
    TowersManager* towersManager;
    UnitsManager* unitsManager;

    int sizeFieldX, sizeFieldY;
    int sizeCellX, sizeCellY;
    int halfSizeCellX, halfSizeCellY;

    int mouseX, mouseY;
    int mainCoorMapX, mainCoorMapY; // camera need create
    int spaceWidget; // fix this. 16 and launch
    bool isometric;
//    int tileMapWidth, tileMapHeight;
    QPixmap global_pixmap;
    QPixmap global_pixmap_PathPoint;
    QPixmap global_pixmap_EnemyPathPoint;
    QPixmap global_pixmap_DestinationPoint;
    QPixmap global_pixmap_ExitPoint;

    int spawnPointX, spawnPointY;
    int exitPointX, exitPointY;
    int gameOverLimitUnits;
    int currentFinishedUnits;

public:
    Field(QString mapFile, FactionsManager* factionsManager, int enemyCount, int difficultyLevel, int towersCount);
    ~Field();
    Cell* getCell(int x, int y);
    void createField(int sizeFieldX, int sizeFieldY, MapLayers* mapLayers);
    void deleteField();
    bool createSpawnPoint(int num, int x = -1, int y = -1);
    void createExitPoint(int x, int y);
    void updateHeroDestinationPoint();
    void updateHeroDestinationPoint(int x, int y);
    void updatePathFinderWalls();
    void render(float deltaTime, QPainter *painter);

    void drawFullField(QPainter* painter);
    void drawGrid(QPainter* painter);
    void drawBackGround(QPainter* painter);
    void drawGround(QPainter* painter);
    void drawForeGround(QPainter* painter);

    void drawTowersByTowers(QPainter* painter);
    void drawUnits(QPainter* painter);
    void drawBlackTiles(QPainter* painter);
    void drawPaths(QPainter* painter);
    void drawTowersUnderConstruction(QPainter* painter);
    void drawTowerUnderConstruction(QPainter* painter, int buildX, int buildY, TemplateForTower* tower);

    int getSizeX();
    int getSizeY();
    void setMainCoorMap(int mainCoorMapX, int mainCoorMapY);
    void setSizeCell(int sizeCellX);
    int getMainCoorMapX();
    int getMainCoorMapY();
//    int getSpaceWidget();
    int getSizeCell();
//    void setIsometric(bool isometric);
//    void setTileMapSize(int tileMapWidth, int tileMapHeight);
//    bool getIsometric();
//    int getTileMapWidth();
//    int getTileMapHeight();
    bool towersAttack(int deltaTime);
    void setMousePress(int x, int y);
    bool getMousePress(int x = -1, int y = -1);
    bool isSetSpawnPoint(int x = -1, int y = -1);
    bool isSetExitPoint(int x = -1, int y = -1);
    int stepAllUnits();
    int stepOneUnit(Unit* unit);
    Unit* getUnit(int x, int y);
    std::vector<Unit *> getUnits(int x, int y);
//    Unit* getUnitWithLowHP(int x, int y);
    std::vector<Tower*> getAllTowers();
//    bool containEmpty(int x, int y);
//    bool isTerrain(int x, int y);
//    bool containTower(int x, int y);
    int containUnit(int x, int y, Unit* unit = NULL);
//    bool setBusy(int x, int y, QPixmap pixmap);
    bool setTower(int x, int y, TemplateForTower* defTower);
    bool spawnHeroInSpawnPoint();//Unit* unit = NULL);//, int type = 0);
    Unit* createUnit(int x, int y, int type = -1);
    bool deleteTower(int x = -1, int y = -1);
};

#endif // FIELD_H
