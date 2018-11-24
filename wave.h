#ifndef WAVE_H
#define WAVE_H

#include <QDebug>
#include <QPoint>

#include "src/head/libs/astar.h"

class Wave
{
public:
    std::vector<QString> actions;
    QPoint *spawnPoint;
    QPoint *exitPoint;
    AStar::CoordinateList route;

private:
    float intervalForSpawn;
    float elapsedTime;

public:
    Wave(QPoint *spawnPoint, QPoint *exitPoint, float startToMove);
    QString getTemplateNameForSpawn(float delta);
    void addAction(QString action);
    QString toString();
};

#endif // WAVE_H
