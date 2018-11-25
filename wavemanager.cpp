#include "wavemanager.h"

WaveManager::WaveManager() {
//    this->waves = new std::vector<Wave>();
//    this->wavesForUser = new std::vector<Wave>();
}

void WaveManager::addWave(Wave* wave) {
    this->waves.push_back(wave);
}

std::vector<WaveManager::TemplateNameAndPoints*> WaveManager::getAllUnitsForSpawn(float delta) {
    waitForNextSpawnUnit -= delta;
    std::vector<TemplateNameAndPoints*> allUnitsForSpawn;
    foreach (Wave* wave, waves) {
        if(!wave->actions.empty()) {
            QString templateName = wave->getTemplateNameForSpawn(delta);
            if (!templateName.isNull() && templateName != "") {
                if (templateName.contains("wait")) {
                    waitForNextSpawnUnit = templateName.right(templateName.indexOf("=") + 1).toFloat();
                    // bitch naxyui =( || but work mb =)
                } else {
                    allUnitsForSpawn.push_back(new TemplateNameAndPoints(templateName, wave->spawnPoint, wave->exitPoint));
                }
            }
        } else {
            waves.erase(std::find(waves.begin(), waves.end(), wave));
        }
    }
    return allUnitsForSpawn;
}

std::vector<QPoint*> WaveManager::getAllSpawnPoint() {
    std::vector<QPoint*> points;
    foreach (Wave* wave, waves) {
        points.push_back(wave->spawnPoint);
    }
    foreach (Wave* wave, wavesForUser) {
        points.push_back(wave->spawnPoint);
    }
    return points;
}

std::vector<QPoint*> WaveManager::getAllExitPoint() {
    std::vector<QPoint*> points;
    foreach (Wave* wave, waves) {
        points.push_back(wave->exitPoint);
    }
    foreach (Wave* wave, wavesForUser) {
        points.push_back(wave->exitPoint);
    }
    if (lastExitPoint != NULL) {
        points.push_back(lastExitPoint);
    }
    return points;
}

//bool WaveManager::setExitPoint(QPoint* exitPoint) {
//    this->lastExitPoint = exitPoint;
//    if (waves.size() != 0) {
//        waves.front()->exitPoint = exitPoint;
//        return true;
//    }
//    return false;
//}

int WaveManager::getNumberOfActions() {
    int actions = 0;
    foreach (Wave* wave, waves) {
        actions += wave->actions.size();
    }
    return actions;
}

void WaveManager::validationPoints(Cell* field, int sizeFieldX, int sizeFieldY) {
    qDebug() << "WaveManager::validationPoints(); -- field:" << field;
    if(field != NULL) {
        int wavesSize = waves.size();
        qDebug() << "WaveManager::validationPoints(); -- sizeField:(" << sizeFieldX << ", " << sizeFieldY << ") waves:(" << wavesSize << ":" << wavesSize << ")";
//        for (int w = 0; w < waves.size; w++) {
//            Wave* wave = waves.get(w);
////        foreach (Wave* wave, waves) {
//            QPoint* spawnPoint = wave->spawnPoint;
//            QPoint* exitPoint = wave->exitPoint;
//            int spawnPointX = spawnPoint->x();
//            int spawnPointY = spawnPoint->y();
//            int exitPointX = exitPoint->x();
//            int exitPointY = exitPoint->y();
//            qDebug() << "WaveManager::validationPoints(); -- spawnPoint:" << spawnPoint. << " exitPoint:" << exitPoint << " wave:" << wave;
//            if (spawnPoint == NULL || spawnPointX < 0 || spawnPointX >= sizeFieldX || spawnPointY < 0 || spawnPointY >= sizeFieldY || !field[spawnPointX][spawnPointY].isPassable()) {
//                qDebug() << "WaveManager::validationPoints(); -- SpawnPoint bad:" << spawnPoint << " wave:" << wave;
//                waves.removeValue(wave, true);
//                w--;
//            } else if (exitPoint.isNull() || exitPointX < 0 || exitPointX >= sizeFieldX || exitPointY < 0 || exitPointY >= sizeFieldY || !field[exitPointX][exitPointY].isPassable()) {
//                qDebug() << "WaveManager::validationPoints(); -- ExitPoint bad:" << exitPoint << " wave:" << wave;
//                waves.removeValue(wave, true);
//                w--;
//            }
//        }
        qDebug() << "WaveManager::validationPoints(); -- sizeField:(" << sizeFieldX << ", " << sizeFieldY << ") waves:(" << wavesSize << ":" << waves.size() << ")";
        int wavesForUserSize = waves.size();
        qDebug() << "WaveManager::validationPoints(); -- sizeField:(" << sizeFieldX << ", " << sizeFieldY << ") wavesForUser:(" << wavesForUserSize << ":" << wavesForUser.size() << ")";
//        for (int w = 0; w < wavesForUser.size; w++) {
//            Wave wave = wavesForUser.get(w);
//            QPoint spawnPoint = wave->spawnPoint;
//            QPoint exitPoint = wave->exitPoint;
//            qDebug() << "WaveManager::validationPoints(); -- spawnPoint:" << spawnPoint << " exitPoint:" << exitPoint << " wave:" << wave;
//            if (spawnPoint.isNull() || spawnPointX < 0 || spawnPointX >= sizeFieldX || spawnPointY < 0 || spawnPointY >= sizeFieldY || !field[spawnPointX][spawnPointY].isPassable()) {
//                qDebug() << "WaveManager::validationPoints(); -- SpawnPoint bad:" << spawnPoint << " wave:" << wave;
//                wavesForUser.removeValue(wave, true);
//                w--;
//            } else if (exitPoint.isNull() || exitPointX < 0 || exitPointX >= sizeFieldX || exitPointY < 0 || exitPointY >= sizeFieldY || !field[exitPointX][exitPointY].isPassable()) {
//                qDebug() << "WaveManager::validationPoints(); -- ExitPoint bad:" << exitPoint << " wave:" << wave;
//                wavesForUser.removeValue(wave, true);
//                w--;
//            }
//        }
        qDebug() << "WaveManager::validationPoints(); -- sizeField:(" << sizeFieldX << ", " << sizeFieldY << ") wavesForUser:(" << wavesForUserSize << ":" << wavesForUser.size() << ")";
    }
}

void WaveManager::checkRoutes(AStar::PathFinder* pathFinder) {
    qDebug() << "WaveManager::checkRoutes(); -- pathFinder:" << pathFinder;
    if(pathFinder != NULL) {
        int wavesSize = waves.size();
        qDebug() << "WaveManager::checkRoutes(); -- waves:(" << wavesSize << ":" << waves.size() << ")";
//        for (int w = 0; w < waves.size; w++) {
//            Wave wave = waves.get(w);
//            QPoint spawnPoint = wave->spawnPoint;
//            QPoint exitPoint = wave->exitPoint;
//            qDebug() << "WaveManager::checkRoutes(); -- spawnPoint:" << spawnPoint << " exitPoint:" << exitPoint;
//            std::vectorDeque<Node> route = pathFinder.route(spawnPointX, spawnPointY, exitPointX, exitPointY);
//            if (route == null) {
//                qDebug() << "WaveManager::checkRoutes(); -- Not found route for this points | Remove wave:" << wave;
//                waves.removeValue(wave, true);
//                w--;
//            } else {
//                wave->route = route;
//            }
//        }
        qDebug() << "WaveManager::checkRoutes(); -- waves:(" << wavesSize << ":" << waves.size() << ")";
        int wavesForUserSize = wavesForUser.size();
        qDebug() << "WaveManager::checkRoutes(); -- wavesForUser:(" << wavesForUserSize << ":" << waves.size() << ")";
//        for (int w = 0; w < wavesForUser.size; w++) {
//            Wave wave = wavesForUser.get(w);
//            QPoint spawnPoint = wave->spawnPoint;
//            QPoint exitPoint = wave->exitPoint;
//            qDebug() << "WaveManager::checkRoutes(); -- spawnPoint:" << spawnPoint << " exitPoint:" << exitPoint;
//            std::vectorDeque<Node> route = pathFinder.route(spawnPointX, spawnPointY, exitPointX, exitPointY);
//            if (route == null) {
//                qDebug() << "WaveManager::checkRoutes(); -- Not found route for this points | Remove wave:" << wave;
//                wavesForUser.removeValue(wave, true);
//                w--;
//            } else {
//                wave->route = route;
//            }
//        }
        qDebug() << "WaveManager::checkRoutes(); -- wavesForUser:(" << wavesForUserSize << ":" << waves.size() << ")";
    } else {
        qDebug() << "WaveManager::checkRoutes(); -- pathFinder == null";
    }
}
