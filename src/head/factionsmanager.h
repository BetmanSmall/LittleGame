#ifndef FACTIONSMANAGER_H
#define FACTIONSMANAGER_H

#include <QDebug>
#include <QtXml>

#include <src/head/faction.h>

class FactionsManager
{
    std::vector<Faction> factions;
public:
    FactionsManager();
    void loadFactions();
    void loadFaction(QFile factionFile);
};

#endif // FACTIONSMANAGER_H
