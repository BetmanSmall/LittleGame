#ifndef FACTIONSMANAGER_H
#define FACTIONSMANAGER_H

#include <QDebug>
#include <QtXml>

#include "src/head/faction.h"
#include "src/head/mapEditor/maploader.h"
#include "simpletemplate.h"

class FactionsManager
{
#ifdef QT_DEBUG
    QString ASSETS_PATH = "../../LittleGame/assets/";
#else
    QString ASSETS_PATH = "./assets/";
#endif

public:
    SimpleTemplate* fireball_0;
    SimpleTemplate* explosion;

private:
    std::vector<Faction*> factions;

public:
    FactionsManager();
    ~FactionsManager();

    TemplateForUnit* getRandomTemplateForUnitFromFirstFaction();
    TemplateForUnit* getRandomTemplateForUnitFromSecondFaction();
    TemplateForTower* getRandomTemplateForTowerFromFirstFaction();
    TemplateForTower* getRandomTemplateForTowerFromAllFaction();
    TemplateForUnit* getTemplateForUnitFromFirstFactionByIndex(int index);
    TemplateForUnit* getTemplateForUnitFromFirstFactionByName(QString templateName);
    TemplateForUnit* getTemplateForUnitByName(QString templateName);
//    std::vector<TemplateForTower*> getAllFirstTowersFromFirstFaction();
    std::vector<TemplateForTower*> getAllTemplateForTowers();
    std::vector<TemplateForUnit*> getAllTemplateForUnits();

    void loadFactions();
    void loadFaction(QString factionFile);
};

#endif // FACTIONSMANAGER_H
