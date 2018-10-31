#include "wave.h"

Wave::Wave(QPoint spawnPoint, QPoint exitPoint, float startToMove) {
//    this->actions = new ArrayDeque<QString>();
    this->spawnPoint = spawnPoint;
    this->exitPoint = exitPoint;
    this->intervalForSpawn = startToMove;
    this->elapsedTime = 0.0;
}

//QString *Wave::getTemplateNameForSpawn(float delta) {
//    elapsedTime += delta;
//    QString* action = new QString();
//    if (elapsedTime >= intervalForSpawn) {
//        elapsedTime = 0.0;
//        action = actions.front();//pollFirst();
//        if (action == NULL) {
//            return NULL;
//        } else if (action->contains("delay")) {
//            intervalForSpawn = action->right(action->indexOf("=") + 1).toFloat();//, action->length())); // GOVNE GODE parseFloat1
////                Gdx.app.log("Wave::getNextNameTemplateForUnitForSpawnUnit()", "-- Delay after wave:" + intervalForSpawn + " sec.");
//            return "wait=" + intervalForSpawn;
//        } else if (action->contains("interval")) {
//            intervalForSpawn = action->right(action->indexOf("=") + 1).toFloat();//, action->length())); // GOVNE GODE parseFloat2
////                Gdx.app.log("Wave::getNextNameTemplateForUnitForSpawnUnit()", "-- Next unit spawn after:" + intervalForSpawn + " sec.");
//            return "wait=" + intervalForSpawn;
//        } else { // QString contain templateName.
//            intervalForSpawn = 0.0;
//            return action;
//        }
//    }
//    return new QString(("wait=%1").arg(intervalForSpawn-elapsedTime));
//}

void Wave::addAction(QString *action) {
    this->actions.push_back(action);
}

QString Wave::toString() {
    QString sb("Wave[");
//    sb.append("spawnPoint:" + spawnPoint);
//    sb.append(",exitPoint:" + exitPoint);
//    sb.append(",elapsedTime:" + elapsedTime);
//    sb.append(",intervalForSpawn:" + intervalForSpawn);
//    sb.append(",actions:" + actions);
//    sb.append(",route:" + route);
    sb.append("]");
    return sb;
}
