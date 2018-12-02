#ifndef FACTION_H
#define FACTION_H

#include <QDebug>

//#include "src/head/tower.h"
#include "src/head/templatefortower.h"
#include "src/head/templateforunit.h"

class Faction {
public:
    QString name;
    std::vector<TemplateForTower*> towers;
    std::vector<TemplateForUnit*> units;

//public:
    Faction(QString name);
    ~Faction();

    void addTower(TemplateForTower* tower);
    void addUnit(TemplateForUnit* unit);

    std::vector<TemplateForTower*> getFirstTowers();
    TemplateForUnit* getDefaultUnitById(int id);
};

#endif // FACTION_H
