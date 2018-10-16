#ifndef FACTION_H
#define FACTION_H

#include <QDebug>

#include "src/head/tower.h"
#include "src/head/defaultunit.h"
#include "src/head/defaulttower.h"

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
