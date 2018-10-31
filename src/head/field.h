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
#include "cameracontroller.h"

class Field
{
public: // we are friendly!
    AStar::PathFinder* pathFinder;
    Map* map;
    Cell* field;
    FactionsManager* factionsManager;
    TowersManager* towersManager;
    UnitsManager* unitsManager;

//    int sizeFieldX, sizeFieldY;
//    int sizeCellX, sizeCellY;
//    int halfSizeCellX, halfSizeCellY;

    int isDrawableGrid = 3;
    int isDrawableUnits = 3;
    int isDrawableTowers = 3;
    int isDrawableBackground = 3;
    int isDrawableGround = 3;
    int isDrawableForeground = 3;
    int isDrawableGridNav = 3;
    int isDrawableRoutes = 3;
    int drawOrder = 8;

    bool isometric;
//    int tileMapWidth, tileMapHeight;
    QPixmap global_pixmap;
    QPixmap global_pixmap_PathPoint;
    QPixmap global_pixmap_EnemyPathPoint;
    QPixmap global_pixmap_DestinationPoint;
//    QPixmap global_pixmap_ExitPoint;

//    int mouseX, mouseY;
//    int mainCoorMapX, mainCoorMapY; // camera need create
//    int spaceWidget; // fix this. 16 and launch

//    bool gameStart;
    bool gamePause;
//    float gameSpeed;
//    int spawnPointX, spawnPointY;
//    int exitPointX, exitPointY;
    Cell* cellSpawnHero;
    Cell* cellExitHero;
    int gameOverLimitUnits;
    int currentFinishedUnits;

public:
    Field(QString mapFile, FactionsManager* factionsManager, int enemyCount, int difficultyLevel, int towersCount);
    ~Field();
    void createField();
    void turnRight();
    void turnLeft();
    void flipX();
    void flipY();
    Cell* getCell(int x, int y);
    void updateCellsGraphicCoordinates(float halfSizeCellX, float halfSizeCellY);
    void updateHeroDestinationPoint();
    void updateHeroDestinationPoint(int x, int y);
    void updatePathFinderWalls();

    void render(float deltaTime, CameraController* cameraController);
    void drawFullField(CameraController *cameraController);
    void drawGrid(CameraController* cameraController);
    void drawBackGrounds(CameraController* cameraController);
    void drawBackGroundCell(CameraController* cameraController, int cellX, int cellY);
//    void drawBackGround(QPainter* painter);
    void drawGroundsWithUnitsAndTowers(CameraController* cameraController);
    void drawGroundCellWithUnitsAndTower(CameraController* cameraController, int cellX, int cellY);
//    void drawGround(QPainter* painter);
    void drawForeGrounds(CameraController* cameraController);
    void drawForeGroundCell(CameraController* cameraController, int cellX, int cellY);

//    void drawTowersByTowers(QPainter* painter);
//    void drawUnits(QPainter* painter);
    void drawGridNavs(CameraController *cameraController);
    void drawGridNavCell(CameraController *cameraController, int cellX, int cellY);
    void drawGridNav(CameraController *cameraController);
    void drawPaths(CameraController *cameraController);
//    void drawTowersUnderConstruction(QPainter* painter);
//    void drawTowerUnderConstruction(QPainter* painter, int buildX, int buildY, TemplateForTower* tower);

    void setMainCoorMap(int mainCoorMapX, int mainCoorMapY);
    void setSizeCell(int sizeCellX);
    int getMainCoorMapX();
    int getMainCoorMapY();
    int getSizeCell();

    bool towersAttack(int deltaTime);
    void setMousePress(int x, int y);
    bool getMousePress(int x = -1, int y = -1);
//    bool isSetSpawnPoint(int x = -1, int y = -1);
//    bool isSetExitPoint(int x = -1, int y = -1);
    int stepAllUnits();
    int stepOneUnit(Unit* unit);
    Unit* getUnit(int x, int y);
    std::vector<Unit *> getUnits(int x, int y);
//    Unit* getUnitWithLowHP(int x, int y);
    std::vector<Tower*> getAllTowers();
    int containUnit(int x, int y, Unit* unit = NULL);
    bool setTower(int x, int y, TemplateForTower* defTower);

    void spawnHeroInSpawnPoint();
    void spawnCompUnitToRandomExit(int x, int y);
    void createUnit(int x, int y, int x2, int y2, int player = 0);
    void createUnit(QPoint spawnPoint, QPoint exitPoint, TemplateForUnit* templateForUnit, int player = 0);
    bool deleteTower(int x = -1, int y = -1);
};

#endif // FIELD_H
