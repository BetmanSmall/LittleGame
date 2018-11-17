#ifndef FIELD_H
#define FIELD_H

#include <iostream>
#include <QPixmap>
#include <QPainter>

//#include "src/head/libs/astar.h"
#include "src/head/factionsmanager.h"
#include "wavemanager.h"
#include "src/head/unitsmanager.h"
#include "src/head/towersmanager.h"
#include "src/head/bullet.h"
#include "src/head/cell.h"
#include "src/head/underconstruction.h"
#include "cameracontroller.h"

class GameField
{

#ifdef QT_DEBUG
    QString ASSETS_PATH = "../../LittleGame/assets/";
#else
    QString ASSETS_PATH = "./assets/";
#endif

public: // we are friendly!
    FactionsManager* factionsManager;
    WaveManager* waveManager;
    TowersManager* towersManager;
    UnitsManager* unitsManager;
    Map* map;
    Cell* field;
    AStar::PathFinder* pathFinder;

    UnderConstruction* underConstruction;
    QPixmap* greenCheckmark;
    QPixmap* redCross;

    QPixmap global_pixmap;
    QPixmap global_pixmap_PathPoint;
    QPixmap global_pixmap_EnemyPathPoint;
    QPixmap global_pixmap_DestinationPoint;
//    QPixmap global_pixmap_ExitPoint;

    float timeOfGame;
    float gameSpeed;
    bool gamePaused;
    int gamerGold;

    bool isometric = false;
    // GameType need
    Cell* cellSpawnHero = NULL;
    Cell* cellExitHero = NULL;
    int gameOverLimitUnits;
    int currentFinishedUnits;

public:
    GameField(QString mapFile, FactionsManager* factionsManager, int enemyCount, int difficultyLevel, int towersCount);
    ~GameField();
    void createField();
    bool landscapeGenerator(QString mapFile);
    void turnRight();
    void turnLeft();
    void flipX();
    void flipY();
    Cell* getCell(int x, int y);
    void updateCellsGraphicCoordinates(float halfSizeCellX, float halfSizeCellY);
    void updateTowersGraphicCoordinates(CameraController *cameraController);
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

    void drawUnit(CameraController* cameraController, Unit* unit);
    void drawTower(CameraController* cameraController, Tower* tower);
//    void drawUnits(CameraController* cameraController);
//    void drawTowersByTowers(CameraController* cameraController);
    void drawGridNav(CameraController *cameraController);
    void drawGridNavs(CameraController *cameraController);
    void drawGridNavCell(CameraController *cameraController, int cellX, int cellY);
    void drawRoutes(CameraController *cameraController);

    void drawTowersUnderConstruction(CameraController* cameraController);
    void drawTowerUnderConstruction(CameraController* cameraController, int buildX, int buildY, TemplateForTower* templateForTower, bool enoughGold);
    void drawTowerUnderConstructionAndMarks(CameraController* cameraController, int map, TemplateForTower* templateForTower, Cell* mainCell, QPoint startDrawCell, QPoint finishDrawCell);
//    void drawTowerUnderConstruction(QPainter* painter, int buildX, int buildY, TemplateForTower* tower);

//    void setMainCoorMap(int mainCoorMapX, int mainCoorMapY);
//    void setSizeCell(int sizeCellX);
//    int getMainCoorMapX();
//    int getMainCoorMapY();
//    int getSizeCell();

//    void setMousePress(int x, int y);
//    bool getMousePress(int x = -1, int y = -1);
//    bool isSetSpawnPoint(int x = -1, int y = -1);
//    bool isSetExitPoint(int x = -1, int y = -1);
    void stepAllUnits(float deltaTime, CameraController *cameraController);
//    int stepOneUnit(Unit* unit);
    void shotAllTowers(float deltaTime, CameraController *cameraController);
    bool shotMeleeTower(Tower *tower);
    bool fireBallTowerAttack(int deltaTime, Tower *fireBallTower);
    void moveAllShells(float delta);

//    Unit* getUnit(int x, int y);
//    std::vector<Unit *> getUnits(int x, int y);
//    Unit* getUnitWithLowHP(int x, int y);
//    std::vector<Tower*> getAllTowers();
//    int containUnit(int x, int y, Unit* unit = NULL);
//    bool setTower(int x, int y, TemplateForTower* defTower);

    void spawnHeroInSpawnPoint();
    void spawnCompUnitToRandomExit(int x, int y);
    void createUnit(int x, int y, int x2, int y2, int player = 0);
    void createUnit(QPoint spawnPoint, QPoint exitPoint, TemplateForUnit* templateForUnit, int player = 0);

    UnderConstruction* createdRandomUnderConstruction();
    UnderConstruction* createdUnderConstruction(TemplateForTower *templateForTower);
    bool cancelUnderConstruction();
    UnderConstruction* getUnderConstruction();
    void buildTowersWithUnderConstruction(int x, int y);

//    void towerActions(int x, int y);
    bool createTower(int buildX, int buildY, TemplateForTower *templateForTower, int player);
    void removeLastTower();
    void removeTower(int cellX, int cellY);
//    bool deleteTower(int x = -1, int y = -1);
};

#endif // FIELD_H
