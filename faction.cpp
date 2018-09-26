#include "faction.h"

Faction::Faction() {
    qDebug() << "Faction::Faction(); -- ";
}

Faction::~Faction() {
    qDebug() << "Faction::~Faction(); -- ";
    towers.clear();
    units.clear();
}

//void Faction::creatyNewTower(int type, int radius, int attack, QPixmap pixmap)
//{
//    Tower tower(type, radius, attack, pixmap);

//    towers.push_back(tower);
//}

void Faction::addTower(DefaultTower* tower) {
//    DefaultTower tmp_tower = tower; // problem with memory too......
    towers.push_back(tower);
}

void Faction::addUnit(DefaultUnit* unit) {
//    DefaultUnit tmp_unit = unit; // problem with memory
    units.push_back(unit);
}

vector<DefaultTower*> Faction::getFirstTowers() {
    vector<DefaultTower*> exitTowers;
//    exitTowers = towers;
//    qDebug() << "towers.size(): " << towers.size();
    for(int k = 0; k < towers.size(); k++) {
        exitTowers.push_back(towers[k]);
    }
//    qDebug() << "exitTowers.size(): " << exitTowers.size();
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
