#ifndef FACTIONSMANAGER_H
#define FACTIONSMANAGER_H

#include <QDebug>
#include <QtXml>

#include <src/head/faction.h>

class FactionsManager
{
#ifdef QT_DEBUG
    QString ASSETS_PATH = "../../LittleGame/assets/";
#else
    QString ASSETS_PATH = "./assets/";
#endif

    std::vector<Faction*> factions;
public:
    FactionsManager();
    ~FactionsManager();

    void loadFactions();
    void loadFaction(QString factionFile);
};

#endif // FACTIONSMANAGER_H
