#ifndef FIELD_H
#define FIELD_H

#include <iostream>
#include <QPixmap>
#include <QPainter>

//#include "astar.h"
#include "factionsmanager.h"
#include "wavemanager.h"
#include "towersmanager.h"
#include "unitsmanager.h"
#include "gamesettings.h"
#include "bullet.h"
#include "cell.h"
#include "underconstruction.h"
#include "cameracontroller.h"

class CameraController;
class WaveManager;
//class WaveManager::TemplateNameAndPoints;
class UnitsManager;
class Unit;
class TowersManager;
class GameSettings;

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
    GameSettings* gameSettings;
    Map* map;
    Cell* field;
    AStar::PathFinder* pathFinder;

    UnderConstruction* underConstruction;
    QPixmap* greenCheckmark;
    QPixmap* redCross;

    float timeOfGame;
    float gameSpeed;
    bool gamePaused;
    int gamerGold;

public:
    GameField(QString mapPath, FactionsManager *factionsManager, GameSettings *gameSettings);
    ~GameField();
    void createField();
    bool landscapeGenerator(QString mapPath);
    Cell* getCell(int x, int y);
    void updateCellsGraphicCoordinates(float sizeCellX, float sizeCellY);
    void updateTowersGraphicCoordinates(CameraController *cameraController);

    void render(float deltaTime, CameraController* cameraController);
//    void drawFullField(CameraController *cameraController);
    void drawGrid(CameraController* cameraController);
    void drawBackGrounds(CameraController* cameraController);
    void drawBackGroundCell(CameraController* cameraController, int cellX, int cellY);
    void drawGroundsWithUnitsAndTowers(CameraController* cameraController);
    void drawGroundCellWithUnitsAndTower(CameraController* cameraController, int cellX, int cellY);
    void drawForeGrounds(CameraController* cameraController);
    void drawForeGroundCell(CameraController* cameraController, int cellX, int cellY);

    void drawUnit(CameraController *cameraController, Unit *unit);
    void drawUnitsBars(CameraController *cameraController);
    void drawUnitBar(CameraController *cameraController, Unit *unit, float fVx, float fVy);

    void drawTower(CameraController* cameraController, Tower* tower);
    void drawBullets(CameraController *cameraController);

    void drawGridNav(CameraController *cameraController);
    void drawGridNavs(CameraController *cameraController);
    void drawGridNavCell(CameraController *cameraController, int cellX, int cellY);
    void drawRoutes(CameraController *cameraController);

//    void drawWavesRoutes(CameraController *cameraController);
//    void drawWave(CameraController *cameraController, Wave *wave);

    void drawTowersUnderConstruction(CameraController* cameraController);
    void drawTowerUnderConstruction(CameraController* cameraController, int buildX, int buildY, TemplateForTower* templateForTower, bool enoughGold);
    void drawTowerUnderConstructionAndMarks(CameraController* cameraController, int map, TemplateForTower* templateForTower, Cell* mainCell, QPoint startDrawCell, QPoint finishDrawCell);

    Unit *spawnUnitFromUser(TemplateForUnit *templateForUnit);
    void spawnUnits(float delta);
    Unit *spawnUnit(TemplateNameAndPoints* templateNameAndPoints);
    Unit *spawnHeroInSpawnPoint();
    Unit *spawnHero(int cellX, int cellY);
    Unit *spawnCompUnitToRandomExit(int x, int y);
    Unit *createUnit(Cell *spawnCell, Cell *destCell, TemplateForUnit* templateForUnit, int player = 0, Cell *exitCell = NULL);

    UnderConstruction* createdRandomUnderConstruction();
    UnderConstruction* createdUnderConstruction(TemplateForTower *templateForTower);
    bool cancelUnderConstruction();
    UnderConstruction* getUnderConstruction();
    void buildTowersWithUnderConstruction(int x, int y);

    bool towerActions(int x, int y);
    Tower *createTowerWithGoldCheck(int buildX, int buildY, TemplateForTower *templateForTower, int player);
    Tower *createTower(int buildX, int buildY, TemplateForTower *templateForTower, int player);
    bool removeLastTower();
    bool removeTowerWithGold(int cellX, int cellY);
    int removeTower(int cellX, int cellY);

    void updatePathFinderWalls();
//    char[][] getCharMatrix();
    void rerouteHero();
    void rerouteHero(int x, int y);
    void rerouteAllUnits();
    void rerouteAllUnits(int x, int y);
    void rerouteUnitPath(Unit *unit, int x, int y);

    void stepAllUnits(float deltaTime, CameraController *cameraController);
//    int stepOneUnit(Unit* unit);
    void shotAllTowers(float deltaTime, CameraController *cameraController);
    bool chechPitTower(Tower *tower);
    bool shotMeleeTower(Tower *tower);
//    bool fireBallTowerAttack(int deltaTime, Tower *fireBallTower);
    void moveAllShells(float delta, CameraController *cameraController);

    void turnRight();
    void turnLeft();
    void flipX();
    void flipY();
};

#endif // FIELD_H
