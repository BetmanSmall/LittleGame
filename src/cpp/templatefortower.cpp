#include "src/head/templatefortower.h"

TemplateForTower::TemplateForTower(QString templateFile) {
    qDebug() << "TemplateForTower::TemplateForTower(); -- templateFile:" << templateFile;
    this->radiusDetection = 0.0;
    this->radiusFlyShell = 0.0;
    this->capacity = 0;
//    this->reloadTime = 3000;
    loadBasicTemplate(templateFile);
    specificLoad();
    validate();
    this->size = 1;
    qDebug() << "TemplateForTower::TemplateForTower(); -end- templateFile:" << templateFile;
}

TemplateForTower::~TemplateForTower() {
    qDebug() << "TemplateForTower::~TemplateForTower(); -- ";
}

void TemplateForTower::loadFireBall(SimpleTemplate* fireBall) {
    if (fireBall != NULL) {
        foreach (AnimatedTile* animatedTile, fireBall->animatedTiles.values()) {
            QString tileName = animatedTile->getProperties()->value("tileName", NULL);
            if (tileName != NULL) {
                if(tileName.contains("fireball_")) {
                    qDebug() << "TemplateForTower::loadFireBall(); -setAmmoTiles- tileName:" << tileName;
                    setAmmoTiles(tileName.replace("fireball_", "ammo_"), animatedTile);
                }
            }
        }
    }
}

void TemplateForTower::specificLoad() {
    foreach (Tile* tile, tiles) {
        QString tileName = tile->getProperties()->value("tileName", NULL);
        if (tileName != NULL) {
            if(tileName == "idleTile") {
                idleTile = tile;
//            } else if(tileName.contains("ammo_")) {
//                qDebug() << "TemplateForTower::specificLoad(); -setAmmoTiles- tileName:" << tileName;
//                setAmmoTiles(tileName, tile);
            }
        }
    }
}

void TemplateForTower::setAmmoTiles(QString tileName, AnimatedTile* tile) {
    if(tile != NULL) {
        if(tileName == ("ammo_" + Direction::UP)) {
            animations.insert("ammo_" + Direction::UP, tile);
        } else if(tileName == ("ammo_" + Direction::UP_RIGHT)) {
            animations.insert("ammo_" + Direction::UP_RIGHT, tile);
            animations.insert("ammo_" + Direction::UP_LEFT, flipAnimatedTiledMapTile(tile));
        } else if(tileName == ("ammo_" + Direction::RIGHT)) {
            animations.insert("ammo_" + Direction::RIGHT, tile);
            animations.insert("ammo_" + Direction::LEFT, flipAnimatedTiledMapTile(tile));
        } else if(tileName == ("ammo_" + Direction::DOWN_RIGHT)) {
            animations.insert("ammo_" + Direction::DOWN_RIGHT, tile);
            animations.insert("ammo_" + Direction::DOWN_LEFT, flipAnimatedTiledMapTile(tile));
        } else if(tileName == ("ammo_" + Direction::DOWN)) {
            animations.insert("ammo_" + Direction::DOWN, tile);
        }
    }
}

AnimatedTile* TemplateForTower::flipAnimatedTiledMapTile(AnimatedTile* animatedTiledMapTile) {
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

//Tile TemplateForTower::flipTiledMapTile(Tile tiledMapTile) {
//    TextureRegion textureRegion = new TextureRegion(tiledMapTile.getTextureRegion());
//    textureRegion.flip(true, false);
//    return new StaticTiledMapTile(textureRegion);
//}

void TemplateForTower::validate() {
    basicValidate();
    // Need check range values
    if (!properties.contains("factionName")) {
        qDebug() << "TemplateForTower::validate(); -- NotFound: factionName";
    } else {
        factionName = properties.value("factionName");
    }
    if (!properties.contains("name")) {
        qDebug() << "TemplateForTower::validate(); -- NotFound: name";
    } else {
        name = properties.value("name");
    }
//    if (!properties.contains("type")) {
//        qDebug() << "TemplateForTower::validate(); -- NotFound: type";
//    } else {
//        type = properties.value("type");
//    }
    if (!properties.contains("radiusDetection")) {
        qDebug() << "TemplateForTower::validate(); -- NotFound: radiusDetection";
    } else {
        radiusDetection = properties.value("radiusDetection").toInt();
    }
    if (!properties.contains("radiusFlyShell")) {
        qDebug() << "TemplateForTower::validate(); -- NotFound: radiusFlyShell";
    } else {
        radiusFlyShell = properties.value("radiusFlyShell").toInt();
    }
    if (!properties.contains("damage")) {
        qDebug() << "TemplateForTower::validate(); -- NotFound: damage";
    } else {
        damage = properties.value("damage").toInt();
    }
    if (!properties.contains("size")) {
        qDebug() << "TemplateForTower::validate(); -- NotFound: size";
    } else {
        size = properties.value("size").toInt();
    }
    if (!properties.contains("cost")) {
        qDebug() << "TemplateForTower::validate(); -- NotFound: cost";
    } else {
        cost = properties.value("cost").toInt();
    }
    if (!properties.contains("ammoSize")) {
        qDebug() << "TemplateForTower::validate(); -- NotFound: ammoSize";
    } else {
        ammoSize = properties.value("ammoSize").toFloat();
    }
    if (!properties.contains("ammoSpeed")) {
        qDebug() << "TemplateForTower::validate(); -- NotFound: ammoSpeed";
    } else {
        ammoSpeed = properties.value("ammoSpeed").toFloat();
    }
    if (!properties.contains("reloadTime")) {
        qDebug() << "TemplateForTower::validate(); -- NotFound: reloadTime";
    } else {
        reloadTime = properties.value("reloadTime").toFloat();
    }
    if (!properties.contains("towerAttackType")) {
        qDebug() << "TemplateForTower::validate(); -- NotFound: towerAttackType";
    } else {
        towerAttackType = TowerAttackType::from_string(properties.value("towerAttackType").toStdString());
    }
    if (!properties.contains("towerShellType") && towerAttackType != TowerAttackType::Pit) {
        qDebug() << "TemplateForTower::validate(); -- NotFound: towerShellType";
    } else {
        towerShellType = TowerShellType::from_string(properties.value("towerShellType").toStdString());
    }
    if (!properties.contains("towerShellEffect")) {
        qDebug() << "TemplateForTower::validate(); -- NotFound: towerShellEffect";
    } else {
        towerShellEffect = new TowerShellEffect(TowerShellEffect::from_string(properties.value("towerShellEffect").toStdString()));
        if (!properties.contains("shellEffectType_time")) {
            qDebug() << "TemplateForTower::validate(); -- NotFound: shellEffectType_time";
        } else {
            towerShellEffect->time = properties.value("shellEffectType_time").toFloat();
        }
        if (!properties.contains("shellEffectType_damage")) {
            qDebug() << "TemplateForTower::validate(); -- NotFound: shellEffectType_damage";
        } else {
            towerShellEffect->damage = properties.value("shellEffectType_damage").toFloat();
        }
        if (!properties.contains("shellEffectType_speed")) {
            qDebug() << "TemplateForTower::validate(); -- NotFound: shellEffectType_speed";
        } else {
            towerShellEffect->speed = properties.value("shellEffectType_speed").toFloat();
        }
    }
    if (towerAttackType == TowerAttackType::Pit && properties.contains("capacity")) {
        capacity = properties.value("capacity").toInt();
    } else if (towerAttackType == TowerAttackType::Pit) {
        qDebug() << "TemplateForTower::validate(); -- NotFound: capacity! When towerAttackType==Pit";
    }
    if(this->radiusDetection == 0.0 && towerAttackType != TowerAttackType::Pit) {
        qDebug() << "TemplateForTower::validate(); -- NotFound: radiusDetection";
    }
    if(this->radiusFlyShell == 0.0 && this->towerShellType != TowerShellType::FirstTarget) {
        qDebug() << "TemplateForTower::validate(); -- NotFound: radiusFlyShell";
        this->radiusFlyShell = 0.0;
    }

    if(idleTile == NULL)
        qDebug() << "TemplateForTower::validate(); -- NotFound: idleTile";
    else if(animations.size() == 0)
        qDebug() << "TemplateForTower::validate(); -- NotFound: animations";

//    foreach (QString key, ammunitionPictures.keys()) {
//        qDebug() << "TemplateForTower::validate(); -- Dir:" << key << " properties:" << ammunitionPictures.value(key)->properties;
//    }
//    qDebug() << "TemplateForTower::validate(); --" << toString(true).toStdString().c_str();
}

QString TemplateForTower::toString() {
    return toString(false);
}

QString TemplateForTower::toString(bool full) {
    QString sb("TemplateForTower[");
    sb.append(toStringBasicParam());
    if(full) {
//        sb.append(toStringProperties());
        sb.append(QString(",factionName:%1").arg(factionName));
        sb.append(QString(",name:%1").arg(name));
        sb.append(QString(",radiusDetection:%1").arg(radiusDetection));
        sb.append(QString(",radiusFlyShell:%1").arg(radiusFlyShell));
        sb.append(QString(",damage:%1").arg(damage));
        sb.append(QString(",size:%1").arg(size));
        sb.append(QString(",cost:%1").arg(cost));
        sb.append(QString(",ammoSize:%1").arg(ammoSize));
        sb.append(QString(",ammoSpeed:%1").arg(ammoSpeed));
        sb.append(QString(",reloadTime:%1").arg(reloadTime));
        sb.append(QString(",towerAttackType:%1").arg(towerAttackType));
//        sb += ",shellAttackType:" + shellAttackType;
//        sb += ",shellEffectEnum:" + shellEffectType;
        sb.append(",capacity:" + QString::number(capacity));
        sb.append(QString(",idleTile!= NULL:%1").arg( idleTile!= NULL ));
        sb.append(QString(",ammunitionPictures.size():%1").arg(animations.size()));
    }
    sb.append("]");
    return sb;
}
