#ifndef CREEPS_H
#define CREEPS_H

#include <QDebug>
#include <stdlib.h>
#include <QPixmap>

#include "src/head/unit.h"

class UnitsManager {
public: // we are friendly to all!
    Unit* hero;
    std::vector<Unit*> units;
    int difficultyLevel;

public:
    UnitsManager(int difficultyLevel);
    ~UnitsManager();
    Unit* createHero(AStar::CoordinateList path, TemplateForUnit* templateForUnit);
    Unit* createUnit(AStar::CoordinateList path, TemplateForUnit* templateForUnit, int player);
    Unit* getUnit(int number);
    Unit* getUnit(int x, int y);
//    int getHP(int x, int y);
    bool attackUnit(int x, int y, int damage); //, Unit* unit);
//    QPixmap getUnitPixmap(int x, int y);
};

#endif // CREEPS_H
