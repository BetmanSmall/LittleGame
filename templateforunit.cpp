#include "templateforunit.h"

TemplateForUnit::TemplateForUnit(QString templateFile) {
    qDebug() << "TemplateForUnit::TemplateForUnit(); -- templateFile:" << templateFile;
//    try {
        loadBasicTemplate(templateFile);
        specificLoad();
        validate();
//    } catch (Exception exp) {
//        qDebug() << "TemplateForUnit::TemplateForUnit(); -- Could not load TemplateForUnit from " + templateFile.path() + " Exp:" + exp);
//        throw new Exception("TemplateForUnit::TemplateForUnit() -- Could not load TemplateForUnit from " + templateFile.path() + " Exp:" + exp);
//    }
//    qDebug() << "TemplateForUnit::TemplateForUnit(); -end- ";
}

TemplateForUnit::~TemplateForUnit() {
    qDebug() << "TemplateForUnit::~TemplateForUnit(); -- ";
}

void TemplateForUnit::specificLoad() {
//    qDebug() << "TemplateForUnit::specificLoad(); -- animatedTiles.size():" << animatedTiles.size();
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
//    qDebug() << "TemplateForUnit::specificLoad(); -- animations.size:" << animations.size();
//    qDebug() << "TemplateForUnit::specificLoad(); -end- ";
}

AnimatedTile* TemplateForUnit::flipAnimatedTiledMapTile(AnimatedTile* animatedTiledMapTile) {
    QVector<StaticTile*> frames = QVector<StaticTile*>(animatedTiledMapTile->getFrameTiles());
    for (int k = 0; k < frames.length(); k++) {
        QPixmap textureRegion = QPixmap(frames.at(k)->getPixmap());
        textureRegion = QPixmap::fromImage(textureRegion.toImage().mirrored(true, false));
        StaticTile* frame = new StaticTile(textureRegion);
        frames.replace(k, frame);
    }
    QVector<int> intervals = QVector<int>(animatedTiledMapTile->getAnimationIntervals());
    AnimatedTile* an = new AnimatedTile(intervals, frames);
    return an;
}

void TemplateForUnit::validate() {
    basicValidate();
    // Need check range values
    if (!properties.contains("factionName")) {
        qDebug() << "TemplateForUnit::validate(); -- NotFound: factionName";
    } else {
        factionName = properties.value("factionName");
    }
    if (!properties.contains("name")) {
        qDebug() << "TemplateForUnit::validate(); -- NotFound: name";
    } else {
        name = properties.value("name");
    }
    if (!properties.contains("healthPoints")) {
        qDebug() << "TemplateForUnit::validate(); -- NotFound: healthPoints";
    } else {
        healthPoints = properties.value("healthPoints").toFloat();
    }
    if (!properties.contains("bounty")) {
        qDebug() << "TemplateForUnit::validate(); -- NotFound: bounty";
    } else {
        bounty = properties.value("bounty").toFloat();
    }
    if (!properties.contains("cost")) {
        qDebug() << "TemplateForUnit::validate(); -- NotFound: cost";
    } else {
        cost = properties.value("cost").toFloat();
    }
    if (!properties.contains("speed")) {
        qDebug() << "TemplateForUnit::validate(); -- NotFound: speed";
    } else {
        speed = properties.value("speed").toFloat();
    }
    if (!properties.contains("type")) {
        qDebug() << "TemplateForUnit::validate(); -- NotFound: type";
    } else {
        type = properties.value("type");
    }

    if(animations.size() == 0)
         qDebug() << "TemplateForUnit::validate(); -- NotFound: animations";

//    foreach (QString key, animations.keys()) {
//        qDebug() << "TemplateForUnit::validate(); -- Dir:" << key << " Lenght:" << animations.value(key)->getFrameTiles().length();
//    }
    qDebug() << "TemplateForUnit::validate(); --" << toString(true).toStdString().c_str();
}

QString TemplateForUnit::toString() {
    return toString(false);
}

QString TemplateForUnit::toString(bool full) {
    QString sb("TemplateForUnit[");
    sb.append(toStringBasicParam());
    if(full) {
        sb.append(toStringProperties());
        sb.append(",factionName:" + factionName);
        sb.append(",name:" + name);
        sb.append(QString(",healthPoints:%1").arg(healthPoints));
        sb.append(QString(",speed:%1").arg(bounty));
        sb.append(QString(",cost:%1").arg(cost));
        sb.append(QString(",speed:%1").arg(speed));
        sb.append(",type:" + type);
        sb.append(QString(",animations.size:%1").arg(animations.size()));
    }
    sb.append("]");
    return sb;
}
