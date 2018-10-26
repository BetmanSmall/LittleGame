#include "src/head/factionsmanager.h"

#include "src/head/mapEditor/maploader.h"

FactionsManager::FactionsManager() {
    qDebug() << "FactionsManager::FactionsManager(); -- ";
//    factions = new Array<Faction>();
    loadFactions();
}

FactionsManager::~FactionsManager() {
    qDebug() << "FactionsManager::~FactionsManager(); -- ";
    if (factions.size()) {
        factions.clear();
    }
}

//public void addUnitToFaction(TemplateForUnit unit) {
//    String newFactionName = unit.getFactionName();
//    for (Faction faction : factions) {
//        if (faction.getName().equals(newFactionName)) {
//            faction.getTemplateForUnits().add(unit);
//            unit.setFaction(faction);
//            return;
//        }
//    }
//    Faction faction = new Faction(newFactionName);
//    faction.getTemplateForUnits().add(unit);
//    unit.setFaction(faction);
//    factions.add(faction);
//}

//public void addTowerToFaction(TemplateForTower tower) {
//        Gdx.app.log("FactionsManager::addTowerToFaction()", "-- Tower name:" + tower.name);
//    String newFactionName = tower.getFactionName();
//    for (Faction faction : factions) {
//        if (faction.getName().equals(newFactionName)) {
//            faction.getTemplateForTowers().add(tower);
//            tower.setFaction(faction);
//            return;
//        }
//    }
//    Faction faction = new Faction(newFactionName);
//    faction.getTemplateForTowers().add(tower);
//    tower.setFaction(faction);
//    factions.add(faction);
//}

//public TemplateForUnit getRandomTemplateForUnitFromFirstFaction() {
//    Faction faction = factions.first();
//    if (faction != null) {
//        TemplateForUnit templateForUnit = faction.getTemplateForUnits().random();
//        if (templateForUnit != null) {
//            return templateForUnit;
//        }
//    }
//    return null;
//}

//public TemplateForTower getRandomTemplateForTowerFromFirstFaction() {
//    Faction faction = factions.first();
//    if (faction != null) {
//        TemplateForTower templateForTower = faction.getTemplateForTowers().random();
//        if (templateForTower != null) {
//            return templateForTower;
//        }
//    }
//    return null;
//}

//public TemplateForTower getRandomTemplateForTowerFromAllFaction() {
//    Array<TemplateForTower> allTowers = getAllTemplateForTowers();
////        int r = 0 + (int) (Math.random() * allTowers.size);
////        Gdx.app.log("TemplateForTower", "getRandomTemplateForTowerFromAllFaction(); -- r:" + r);
////        return allTowers.get(r);
//    return allTowers.random();
//}
//public TemplateForUnit getTemplateForUnitFromFirstFactionByIndex(int index) {
//    Faction faction = factions.first();
//    if (faction != null) {
//        TemplateForUnit templateForUnit = faction.getTemplateForUnits().get(index);
//        if (templateForUnit != null) {
//            return templateForUnit;
//        }
//    }
//    return null;
//}

//public TemplateForUnit getTemplateForUnitFromFirstFactionByName(String templateName) {
//    Faction faction = factions.first();
//    if (faction != null) {
//        for (TemplateForUnit templateForUnit : faction.getTemplateForUnits()) {
//            if (templateForUnit != null) {
//                if (templateForUnit.getTemplateName().equals(templateName)) {
//                    return templateForUnit;
//                }
//            }
//        }
//    }
//    return null;
//}

//public TemplateForUnit getTemplateForUnitByName(String templateName) {
//    for (Faction faction : factions) {
//        if (faction != null) {
//            for (TemplateForUnit templateForUnit : faction.getTemplateForUnits()) {
//                if (templateForUnit != null) {
//                    if (templateForUnit.getTemplateName().equals(templateName)) {
//                        return templateForUnit;
//                    }
//                }
//            }
//        }
//    }
//    return null;
//}

//public Array<TemplateForTower> getAllFirstTowersFromFirstFaction() {
//    return factions.first().getTemplateForTowers();
//}

//public Array<TemplateForTower> getAllTemplateForTowers() {
//    Array<TemplateForTower> allTowers = new Array<TemplateForTower>();
//    for (Faction faction : factions) {
//        for (TemplateForTower template : faction.getTemplateForTowers()) {
//            allTowers.add(template);
//        }
//    }
//    return allTowers;
//}

//public Array<TemplateForUnit> getAllTemplateForUnits() {
//    Array<TemplateForUnit> allTowers = new Array<TemplateForUnit>();
//    for (Faction faction : factions) {
//        for (TemplateForUnit template : faction.getTemplateForUnits()) {
//            allTowers.add(template);
//        }
//    }
//    return allTowers;
//}

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
    qDebug() << "FactionsManager::loadFaction(); -1- factionFile:" << factionFile;
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
//                    qDebug() << "FactionsManager::loadFaction(); -- source:" << source;
            if (source != NULL) {
                QString templateFile = MapLoader::findFile(factionFile, source);
//                        qDebug() << "FactionsManager::loadFaction(); -- templateFile:" << templateFile;
                TemplateForUnit* templateForUnit = new TemplateForUnit(templateFile);
//                        templateForUnit->setFaction(faction);
//                        templateForUnit->healthPoints = templateForUnit->healthPoints*levelOfDifficulty; // simple level of difficulty
                faction->units.push_back(templateForUnit);
            }
        }
        QDomNodeList templateForTowerElements = rootElement.elementsByTagName("templateForTower");
        qDebug() << "FactionsManager::loadFaction(); -- templateForTowerElements.length():" << templateForTowerElements.length();
        for (int k = 0; k < templateForTowerElements.length(); k++) {
            QDomNode tileSetNode = templateForTowerElements.item(k);
            QString source = tileSetNode.toElement().attribute("source", NULL);
            qDebug() << "FactionsManager::loadFaction(); -- source:" << source;
            if (source != NULL) {
                QString templateFile = MapLoader::findFile(factionFile, source);
                qDebug() << "FactionsManager::loadFaction(); -- templateFile:" << templateFile;
                TemplateForTower* templateForTower = new TemplateForTower(templateFile);
//                        templateForTower->setFaction(faction);
                faction->towers.push_back(templateForTower);
            }
        }
        factions.push_back(faction);
    }
    qDebug() << "FactionsManager::loadFaction(); -2- factionFile:" << factionFile;
}
