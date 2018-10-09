#include "faction.h"

Faction::Faction() {
    qDebug() << "Faction::Faction(); -- ";
}

Faction::~Faction() {
    qDebug() << "Faction::~Faction(); -- ";
    towers.clear();
    units.clear();
}

void Faction::addTower(DefaultTower* tower) {
    towers.push_back(tower);
}

void Faction::addUnit(DefaultUnit* unit) {
    units.push_back(unit);
}

vector<DefaultTower*> Faction::getFirstTowers() {
    vector<DefaultTower*> exitTowers;
    for (int k = 0; k < towers.size(); k++) {
        exitTowers.push_back(towers[k]);
    }
    return exitTowers;
}

DefaultUnit* Faction::getDefaultUnitById(int id) {
    if(id < units.size()) {
        return units[id];
    } else {
        qDebug() << "Faction::getDefaultUnitById(" << id << "); -- bad id. units.size:" << units.size();
        return NULL;
    }
}
