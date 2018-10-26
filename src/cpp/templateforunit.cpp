#include "src/head/templateforunit.h"

TemplateForUnit::TemplateForUnit(QString templateFile) {
    qDebug() << "TemplateForUnit::TemplateForUnit(); -- templateFile:" << templateFile;
    loadBasicTemplate(templateFile);
    specificLoad();
//    try {
        validate();
//    } catch (Exception exp) {
//        qDebug() << "TemplateForUnit::TemplateForUnit(); -- Could not load TemplateForUnit from " + templateFile.path() + " Exp:" + exp);
//        throw new Exception("TemplateForUnit::TemplateForUnit() -- Could not load TemplateForUnit from " + templateFile.path() + " Exp:" + exp);
//    }
    qDebug() << "TemplateForUnit::TemplateForUnit(); -end- ";
}

TemplateForUnit::~TemplateForUnit() {
    qDebug() << "TemplateForUnit::~TemplateForUnit(); -- ";
    clearVectors();
}

void TemplateForUnit::clearVectors() {
    idle.clear();

    walk_up.clear();
    walk_up_right.clear();
    walk_right.clear();
    walk_down_right.clear();
    walk_down.clear();
    walk_down_left.clear();
    walk_left.clear();
    walk_up_left.clear();

    attack_up.clear();
    attack_up_right.clear();
    attack_right.clear();
    attack_down_right.clear();
    attack_down.clear();
    attack_down_left.clear();
    attack_left.clear();
    attack_up_left.clear();

    death_up.clear();
    death_up_right.clear();
    death_right.clear();
    death_down_right.clear();
    death_down.clear();
    death_down_left.clear();
    death_left.clear();
    death_up_left.clear();
}

//void TemplateForUnit::setFaction(Faction* faction) {
//    this->faction = faction;
//}

void TemplateForUnit::specificLoad() {
    qDebug() << "TemplateForUnit::specificLoad(); -- animatedTiles.size():" << animatedTiles.size();
    foreach (AnimatedTile* tile, animatedTiles) {
//        qDebug() << "TemplateForUnit::specificLoad(); -- tile->getId():" << tile->getId();
//        qDebug() << "TemplateForUnit::specificLoad(); -- tile:" << tile;
        if (AnimatedTile* animatedTile = dynamic_cast<AnimatedTile*>(tile)) {
            QString actionAndDirection = animatedTile->getProperties()->value("actionAndDirection");
//            qDebug() << "TemplateForUnit::specificLoad(); -- actionAndDirection:" << actionAndDirection;
            if(actionAndDirection != NULL) {
                animations.insert(actionAndDirection, animatedTile);
//                qDebug() << "TemplateForUnit::specificLoad(); -- walk_Direction::UP_RIGHT:" << ("walk_" + Direction::toString(Direction::UP_RIGHT));
                if(actionAndDirection == ("walk_" + Direction::toString(Direction::UP_RIGHT)) ) {
                    animations.insert("walk_" + Direction::toString(Direction::UP_LEFT), flipAnimatedTiledMapTile(animatedTile));
                } else if(actionAndDirection == ("walk_" + Direction::toString(Direction::RIGHT)) ) {
                    animations.insert("walk_" + Direction::toString(Direction::LEFT), flipAnimatedTiledMapTile(animatedTile));
                } else if(actionAndDirection == ("walk_" + Direction::toString(Direction::DOWN_RIGHT)) ) {
                    animations.insert("walk_" + Direction::toString(Direction::DOWN_LEFT), flipAnimatedTiledMapTile(animatedTile));
                }
                if(actionAndDirection == ("death_" + Direction::toString(Direction::UP_RIGHT)) ) {
                    animations.insert("death_" + Direction::toString(Direction::UP_LEFT), flipAnimatedTiledMapTile(animatedTile));
                } else if(actionAndDirection == ("death_" + Direction::toString(Direction::RIGHT)) ) {
                    animations.insert("death_" + Direction::toString(Direction::LEFT), flipAnimatedTiledMapTile(animatedTile));
                } else if(actionAndDirection == ("death_" + Direction::toString(Direction::DOWN_RIGHT)) ) {
                    animations.insert("death_" + Direction::toString(Direction::DOWN_LEFT), flipAnimatedTiledMapTile(animatedTile));
                }
            }
//            qDebug() << "TemplateForUnit::specificLoad(); -- animatedTile:" << animatedTile;
        }
    }
    qDebug() << "TemplateForUnit::specificLoad(); -- animations.size:" << animations.size();
    qDebug() << "TemplateForUnit::specificLoad(); -end- ";
}

AnimatedTile* TemplateForUnit::flipAnimatedTiledMapTile(AnimatedTile* animatedTiledMapTile) {
//    qDebug() << "TemplateForUnit::flipAnimatedTiledMapTile(); -- animatedTiledMapTile:" << animatedTiledMapTile;
    QVector<StaticTile*> frames = QVector<StaticTile*>(animatedTiledMapTile->getFrameTiles());
//    qDebug() << "TemplateForUnit::flipAnimatedTiledMapTile(); -- frames.length():" << frames.length();
    for (int k = 0; k < frames.length(); k++) {
        if (frames.at(k) == NULL) {
            qDebug() << "TemplateForUnit::flipAnimatedTiledMapTile(); -1- k:" << k;
            qDebug() << "TemplateForUnit::flipAnimatedTiledMapTile(); -- frames.at(k):" << frames.at(k);
        }
//        qDebug() << "TemplateForUnit::flipAnimatedTiledMapTile(); -2- frames.at(k)->getPixmap():" << frames.at(k)->getPixmap();
        QPixmap textureRegion = QPixmap(frames.at(k)->getPixmap());
//        qDebug() << "TemplateForUnit::flipAnimatedTiledMapTile(); -3- textureRegion:" << textureRegion;
        textureRegion = QPixmap::fromImage(textureRegion.toImage().mirrored(true, false));
        StaticTile* frame = new StaticTile(textureRegion);
        frames.replace(k, frame);
    }
    QVector<int> intervals = QVector<int>(animatedTiledMapTile->getAnimationIntervals());
    AnimatedTile* an = new AnimatedTile(intervals, frames);
//    qDebug() << "TemplateForUnit::flipAnimatedTiledMapTile(); -- an:" << an;
    return an;
}

void TemplateForUnit::validate() {
    basicValidate();
    // Need check range values
    if (!properties.contains("healthPoints")) {
        qDebug() << "Template::basicValidate(); -- Can't get 'healthPoints'! Check the file";
    } else {
        healthPoints = properties.value("healthPoints").toFloat();
    }
    if (!properties.contains("speed")) {
        qDebug() << "Template::basicValidate(); -- Can't get 'speed'! Check the file";
    } else {
        speed = properties.value("speed").toFloat();
    }
//    speed = 0.1f;
//    foreach (QString key, animations.keys()) {
//        qDebug() << "TemplateForUnit::validate(); -- Dir:" << key << " Lenght:" << animations.value(key)->getFrameTiles().length();
//    }
    qDebug() << "TemplateForUnit::validate(); --" << toString(true).toStdString().c_str();
    qDebug() << "TemplateForUnit::validate(); -- animations.size():" << animations.size();
    qDebug() << "TemplateForUnit::validate(); -- ------end-------";
}

QString TemplateForUnit::toString() {
    return toString(false);
}

QString TemplateForUnit::toString(bool full) {
    QString sb("TemplateForUnit[");
    if(full) {
        sb.append(toStringProperties());
        sb.append(toStringBasicParam());
//        sb.append("," + "bounty:" + bounty);
//        sb.append("," + "cost:" + cost);
        sb.append(QString(",healthPoints:%1").arg(healthPoints));
        sb.append(QString(",speed:%1").arg(speed));
        sb.append(QString(",animations.size:%1").arg(animations.size()));
    }
    sb.append("]");
    return sb;
}
