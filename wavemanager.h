#ifndef WAVEMANAGER_H
#define WAVEMANAGER_H

#include <QDebug>

//#include "cell.h"
#include "wave.h"

class Cell;

class TemplateNameAndPoints {
public:
    QString templateName;
    QPoint* spawnPoint;
    QPoint* exitPoint;

    TemplateNameAndPoints(QString templateName, QPoint *spawnPoint, QPoint *exitPoint) {
        this->templateName = templateName;
        this->spawnPoint = spawnPoint;
        this->exitPoint = exitPoint;
    }
};

class WaveManager
{
public:
    bool allTogether;
    Wave* currentWave;
    std::vector<Wave*> waves;
    std::vector<Wave*> wavesForUser;
    QPoint *lastExitPoint;
    float waitForNextSpawnUnit;

public:
    WaveManager();
    ~WaveManager();
    void addWave(Wave *wave);
    bool updateCurrentWave();
    TemplateNameAndPoints* getUnitForSpawn(float delta);
    std::vector<TemplateNameAndPoints*> getAllUnitsForSpawn(float delta);
    std::vector<QPoint*> getAllSpawnPoint();
    std::vector<QPoint*> getAllExitPoint();
//    bool setExitPoint(QPoint *exitPoint);
    int getNumberOfActions();
    void validationPoints(Cell* field, int sizeFieldX, int sizeFieldY);
    void checkRoutes(AStar::PathFinder* pathFinder);
};

#endif // WAVEMANAGER_H
