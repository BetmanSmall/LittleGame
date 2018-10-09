#ifndef FACTION_H
#define FACTION_H

#include <QDebug>

#include "tower.h"
#include "defaultunit.h"
#include "defaulttower.h"

using namespace std;

class Faction {
public:
    vector<DefaultTower*> towers;
    vector<DefaultUnit*> units;

public:
    Faction();
    ~Faction();

    void addTower(DefaultTower* tower);
    void addUnit(DefaultUnit* unit);

    vector<DefaultTower*> getFirstTowers();
    DefaultUnit* getDefaultUnitById(int id);
};

#endif // FACTION_H
