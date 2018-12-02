#ifndef CREEPS_H
#define CREEPS_H

#include <QDebug>
#include <stdlib.h>
#include <QPixmap>

#include "src/head/cell.h"
#include "src/head/unit.h"

class Unit;

class UnitsManager {
public: // we are friendly to all!
    std::vector<Unit*> hero;
    std::vector<Unit*> units;

public:
    UnitsManager();
    ~UnitsManager();
    Unit* createUnit(AStar::CoordinateList path, TemplateForUnit *templateForUnit, int player, Cell *exitCell);
    Unit* getUnit(int number);
//    Unit* getUnit(int x, int y);
    bool removeUnit(Unit* unit);
};

#endif // CREEPS_H
