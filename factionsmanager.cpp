#include "factionsmanager.h"

FactionsManager::FactionsManager() {
    qDebug() << "FactionsManager::FactionsManager(); -- ";
    fireball_0 = new SimpleTemplate(ASSETS_PATH + "maps/other/fireball_0.tsx");
    qDebug() << "FactionsManager::FactionsManager(); -- fireball_0:" << fireball_0->toString();
    explosion = new SimpleTemplate(ASSETS_PATH + "maps/other/explosion.tsx");
    qDebug() << "FactionsManager::FactionsManager(); -- explosion:" << explosion->toString();
//    factions = new Array<Faction>();
    loadFactions();
}

FactionsManager::~FactionsManager() {
    qDebug() << "FactionsManager::~FactionsManager(); -- ";
    if (factions.size()) {
        factions.clear();
    }
}

TemplateForUnit* FactionsManager::getRandomTemplateForUnitFromFirstFaction() {
    Faction* faction = factions.front();
    if (faction != NULL) {
        TemplateForUnit* templateForUnit = faction->units[rand()%faction->units.size()];
        if (templateForUnit != NULL) {
            return templateForUnit;
        }
    }
    return NULL;
}

TemplateForUnit* FactionsManager::getRandomTemplateForUnitFromSecondFaction() {
    Faction* faction = factions.at(1);
    if (faction != NULL) {
        TemplateForUnit* templateForUnit = faction->units[rand()%faction->units.size()];
        if (templateForUnit != NULL) {
            return templateForUnit;
        }
    }
    return NULL;
}

TemplateForTower* FactionsManager::getRandomTemplateForTowerFromFirstFaction() {
    Faction* faction = factions.front();
    if (faction != NULL) {
        TemplateForTower* templateForTower = faction->towers[rand()%faction->towers.size()];
        if (templateForTower != NULL) {
            return templateForTower;
        }
    }
    return NULL;
}

TemplateForTower* FactionsManager::getRandomTemplateForTowerFromAllFaction() {
    std::vector<TemplateForTower*> allTowers = getAllTemplateForTowers();
//        int r = 0 + (int) (Math.random() * allTowers.size);
//        Gdx.app.log("TemplateForTower", "getRandomTemplateForTowerFromAllFaction(); -- r:" + r);
//        return allTowers.get(r);
//    qDebug() << "FactionsManager::getRandomTemplateForTowerFromAllFaction(); -- allTowers.size():" << allTowers.size();
    int randI = rand()%allTowers.size();
//    qDebug() << "FactionsManager::getRandomTemplateForTowerFromAllFaction(); -- randI:" << randI;
    return allTowers.at(randI);
}

TemplateForUnit* FactionsManager::getTemplateForUnitFromFirstFactionByIndex(int index) {
    qDebug() << "FactionsManager::getTemplateForUnitFromFirstFactionByIndex(); -- index:" << index;
    Faction* faction = factions.front();
    if (faction != NULL) {
        qDebug() << "FactionsManager::getTemplateForUnitFromFirstFactionByIndex(); -- faction->units.size():" << faction->units.size();
        TemplateForUnit* templateForUnit = faction->units[index];
        if (templateForUnit != NULL) {
            return templateForUnit;
        }
    }
    return NULL;
}

TemplateForUnit* FactionsManager::getTemplateForUnitFromFirstFactionByName(QString templateName) {
    Faction* faction = factions.front();
    if (faction != NULL) {
        foreach (TemplateForUnit* templateForUnit, faction->units) {
            if (templateForUnit != NULL) {
                if (templateForUnit->templateName == templateName) {
                    return templateForUnit;
                }
            }
        }
    }
    return NULL;
}

TemplateForUnit* FactionsManager::getTemplateForUnitByName(QString templateName) {
    foreach (Faction* faction, factions) {
        if (faction != NULL) {
            foreach (TemplateForUnit* templateForUnit, faction->units) {
                if (templateForUnit != NULL) {
                    if (templateForUnit->templateName == templateName) {
                        return templateForUnit;
                    }
                }
            }
        }
    }
    return NULL;
}

//std::vector<TemplateForTower*> FactionsManager::getAllFirstTowersFromFirstFaction() {
//    return factions.front()->towers;
//}

std::vector<TemplateForTower*> FactionsManager::getAllTemplateForTowers() {
    std::vector<TemplateForTower*> allTowers;
    for (Faction* faction : factions) {
        for (TemplateForTower* templateTower : faction->towers) {
            allTowers.push_back(templateTower);
        }
    }
    return allTowers;
}

std::vector<TemplateForUnit *> FactionsManager::getAllTemplateForUnits() {
    std::vector<TemplateForUnit*> allTowers;
    foreach (Faction* faction, factions) {
        foreach (TemplateForUnit* templateUnit, faction->units) {
            allTowers.push_back(templateUnit);
        }
    }
    return allTowers;
}

void FactionsManager::loadFactions() {
    std::vector<QString> factions;
//    if(Gdx.app.getType() == Application.ApplicationType.Android) {
//        Gdx.app.log("FactionsManager::loadFactions()", "-- ApplicationType.Android");
//        FileHandle factionsDir = Gdx.files.internal("maps/factions");
//        factions.addAll(factionsDir.list());
//    } else if(Gdx.app.getType() == Application.ApplicationType.Desktop) {
//        boolean isExtAvailable = Gdx.files.isExternalStorageAvailable();
//        boolean isLocAvailable = Gdx.files.isLocalStorageAvailable();
//        String extRoot = Gdx.files.getExternalStoragePath();
//        String locRoot = Gdx.files.getLocalStoragePath();
//        Gdx.app.log("FactionsManager::loadFactions()", "-- ApplicationType.Desktop -- isExtAvailable:" + isExtAvailable + " isLocAvailable:" + isLocAvailable);
//        Gdx.app.log("FactionsManager::loadFactions()", "-- extRoot:" + extRoot + " locRoot:" + locRoot);
//        FileHandle factionsDir = Gdx.files.internal("maps/factions");
//        Gdx.app.log("FactionsManager::loadFactions()", "-- factionsDir.length:" + factionsDir.list().length);
//        if(factionsDir.list().length == 0) {
            factions.push_back(ASSETS_PATH + "maps/factions/humans_faction.fac");
            factions.push_back(ASSETS_PATH + "maps/factions/orcs_faction.fac");
//                factions.add(Gdx.files.internal("!!!add new faction in the future!!!"));
//        } else {
//            factions.addAll(factionsDir.list());
//        }
//    }.
    qDebug() << "FactionsManager::loadFactions(); -1- factions.size:" << factions.size();
    foreach (QString factionFile, factions) {
        if (factionFile.contains("fac")) {
            loadFaction(factionFile);
        }
    }
    qDebug() << "FactionsManager::loadFactions(); -2- factions.size:" << factions.size();
}

void FactionsManager::loadFaction(QString factionFile) {
    qDebug() << "FactionsManager::loadFaction(); -start- factionFile:" << factionFile;
    QDomDocument* domDocument = MapLoader::loadDomDocument(factionFile);
    QDomElement rootElement = domDocument->documentElement();
    QString factionName = rootElement.attribute("name", NULL);
    if (factionName != NULL) {
        Faction* faction = new Faction(factionName);
        QDomNodeList templateForUnitElements = rootElement.elementsByTagName("templateForUnit");
        qDebug() << "FactionsManager::loadFaction(); -- templateForUnitElements.length():" << templateForUnitElements.length();
        for (int k = 0; k < templateForUnitElements.length(); k++) {
            QDomNode tileSetNode = templateForUnitElements.item(k);
            QString source = tileSetNode.toElement().attribute("source", NULL);
            if (source != NULL) {
                QString templateFile = MapLoader::findFile(factionFile, source);
                TemplateForUnit* templateForUnit = new TemplateForUnit(templateFile);
//                templateForUnit->setFaction(faction);
                faction->units.push_back(templateForUnit);
                qDebug() << "FactionsManager::loadFaction(); -- " << templateForUnit->toString(true).toStdString().c_str();
            }
        }
        QDomNodeList templateForTowerElements = rootElement.elementsByTagName("templateForTower");
        qDebug() << "FactionsManager::loadFaction(); -- templateForTowerElements.length():" << templateForTowerElements.length();
        for (int k = 0; k < templateForTowerElements.length(); k++) {
            QDomNode tileSetNode = templateForTowerElements.item(k);
            QString source = tileSetNode.toElement().attribute("source", NULL);
            if (source != NULL) {
                QString templateFile = MapLoader::findFile(factionFile, source);
                TemplateForTower* templateForTower = new TemplateForTower(templateFile);
//                templateForTower->setFaction(faction);
                if (templateForTower->towerAttackType == TowerAttackType::FireBall) {
                    templateForTower->loadFireBall(fireball_0);
                }
                faction->towers.push_back(templateForTower);
                qDebug() << "FactionsManager::loadFaction(); -- " << templateForTower->toString(true).toStdString().c_str();
            }
        }
        factions.push_back(faction);
    }
    qDebug() << "FactionsManager::loadFaction(); -end- factionFile:" << factionFile;
}
