#include "faction.h"

Faction::Faction(QString name) {
    qDebug() << "Faction::Faction(); -- name:" << name;
    this->name = name;
}

Faction::~Faction() {
    qDebug() << "Faction::~Faction(); -1- name:" << name;
    if (towers.size()) {
        towers.clear();
    }
    if (units.size()) {
        units.clear();
    }
    qDebug() << "Faction::~Faction(); -2- name:" << name;
}

void Faction::addTower(TemplateForTower* tower) {
    towers.push_back(tower);
}

void Faction::addUnit(TemplateForUnit* unit) {
    units.push_back(unit);
}

std::vector<TemplateForTower*> Faction::getFirstTowers() {
    std::vector<TemplateForTower*> exitTowers;
    for (int k = 0; k < towers.size(); k++) {
        exitTowers.push_back(towers[k]);
    }
    return exitTowers;
}

TemplateForUnit* Faction::getDefaultUnitById(int id) {
    if(id < units.size()) {
        return units[id];
    } else {
        qDebug() << "Faction::getDefaultUnitById(" << id << "); -- bad id. units.size:" << units.size();
        return NULL;
    }
}
