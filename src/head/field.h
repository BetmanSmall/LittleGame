#ifndef FIELD_H
#define FIELD_H

#define CIRCLET8
#include <iostream>
#include <QPixmap>

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
    TowersManager* towersManager;
    UnitsManager* unitsManager;
    FactionsManager* factionsManager;
    int sizeX, sizeY;
    int sizeCell;
    int mouseX, mouseY;
    int mainCoorMapX, mainCoorMapY; // camera need create
    int spaceWidget; // fix this. 16 and launch
    bool isometric;
    int tileMapWidth, tileMapHeight;
    int spawnPointX, spawnPointY;
    int exitPointX, exitPointY;
    int gameOverLimitUnits;
    int currentFinishedUnits;

public:
    Field(QString mapPath, int enemyCount, int towersCount);
    ~Field();
    Cell* getCell(int x, int y);
    void createField(int newSizeX, int newSizeY);
    void deleteField();
    bool createSpawnPoint(int num, int x = -1, int y = -1);
    void createExitPoint(int x, int y);
    void updateHeroDestinationPoint();
    void updateHeroDestinationPoint(int x, int y);
    void updatePathFinderWalls();
    int getSizeX();
    int getSizeY();
    void setMainCoorMap(int mainCoorMapX, int mainCoorMapY);
    void setSizeCell(int sizeCell);
    int getMainCoorMapX();
    int getMainCoorMapY();
    int getSpaceWidget();
    int getSizeCell();
    void setIsometric(bool isometric);
    void setTileMapSize(int tileMapWidth, int tileMapHeight);
    bool getIsometric();
    int getTileMapWidth();
    int getTileMapHeight();
    bool towersAttack(int deltaTime);
    void waveAlgorithm(int x = -1, int y = -1);
    void waveStep(int x, int y, int step);
    void setMousePress(int x, int y);
    bool getMousePress(int x = -1, int y = -1);
    bool isSetSpawnPoint(int x = -1, int y = -1);
    bool isSetExitPoint(int x = -1, int y = -1);
    int stepAllUnits();
    int stepOneUnit(int num);
    int getNumStep(int x, int y);
    int getStepCell(int x, int y);
    bool setNumOfCell(int x, int y, int step);
    void setStepCell(int x, int y, int step);
    void clearStepCell(int x, int y);
    Unit* getUnit(int x, int y);
    std::vector<Unit *> getUnits(int x, int y);
    int getUnitHpInCell(int x, int y);
    Unit* getUnitWithLowHP(int x, int y);
    std::vector<Tower*> getAllTowers();
    bool containEmpty(int x, int y);
    bool isTerrain(int x, int y);
    bool containTower(int x, int y);
    int containUnit(int x, int y, Unit* unit = NULL);
    bool setBusy(int x, int y, QPixmap pixmap);
    bool setTower(int x, int y, TemplateForTower* defTower);
    bool spawnHeroInSpawnPoint();//Unit* unit = NULL);//, int type = 0);
    Unit* createUnit(int x, int y, int type = -1);
    bool clearBusy(int x, int y);
    bool clearTower(int x, int y);
    bool clearUnit(int x, int y, Unit* unit = NULL);
    bool deleteTower(int x = -1, int y = -1);
};

#endif // FIELD_H
