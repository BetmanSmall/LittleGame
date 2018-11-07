#include "src/head/templatefortower.h"

TemplateForTower::TemplateForTower(QString templateFile) {
    qDebug() << "TemplateForTower::TemplateForTower(); -- templateFile:" << templateFile;
    this->radiusDetection = 0.0;
//    this->reloadTime = 3000;
    loadBasicTemplate(templateFile);
    specificLoad();
    validate();
    qDebug() << "TemplateForTower::TemplateForTower(); -end- templateFile:" << templateFile;
}

TemplateForTower::~TemplateForTower() {
    qDebug() << "TemplateForTower::~TemplateForTower(); -- ";
}

void TemplateForTower::specificLoad() {
//    qDebug() << "TemplateForTower::specificLoad(); -- tiles.size():" << tiles.size();
    foreach (Tile* tile, tiles) {
//        qDebug() << "TemplateForTower::specificLoad(); -- tile->getId():" << tile->getId();
//        qDebug() << "TemplateForTower::specificLoad(); -- tile:" << tile;
//        if (AnimatedTile* animatedTile = dynamic_cast<AnimatedTile*>(tile)) {
//        }
        QString tileName = tile->getProperties()->value("tileName", NULL);
        if (tileName != NULL) {
            if(tileName == "idleTile") {
                idleTile = tile;
            } else if(tileName.contains("ammo_")) {
                qDebug() << "TemplateForTower::specificLoad(); -setAmmoTiles- tileName:" << tileName;
                setAmmoTiles(tileName, tile);
            }
        }
    }
//    qDebug() << "TemplateForTower::specificLoad(); -- idleTile:" << idleTile;
//    qDebug() << "TemplateForTower::specificLoad(); -- ammunitionPictures.size:" << ammunitionPictures.size();
//    qDebug() << "TemplateForTower::specificLoad(); -end- ";
}

void TemplateForTower::setAmmoTiles(QString tileName, Tile* tile) {
    if(tile != NULL) {
        if(tileName == ("ammo_" + Direction::UP)) {
            ammunitionPictures.insert("ammo_" + Direction::UP, tile);
        } else if(tileName == ("ammo_" + Direction::UP_RIGHT)) {
            ammunitionPictures.insert("ammo_" + Direction::UP_RIGHT, tile);
//            ammunitionPictures.insert("ammo_" + Direction::UP_LEFT, flipTiledMapTile(tile));
        } else if(tileName == ("ammo_" + Direction::RIGHT)) {
            ammunitionPictures.insert("ammo_" + Direction::RIGHT, tile);
//            ammunitionPictures.insert("ammo_" + Direction::LEFT, flipTiledMapTile(tile));
        } else if(tileName == ("ammo_" + Direction::DOWN_RIGHT)) {
            ammunitionPictures.insert("ammo_" + Direction::DOWN_RIGHT, tile);
//            ammunitionPictures.insert("ammo_" + Direction::DOWN_LEFT, flipTiledMapTile(tile));
        } else if(tileName == ("ammo_" + Direction::DOWN)) {
            ammunitionPictures.insert("ammo_" + Direction::DOWN, tile);
        }
    }
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
        qDebug() << "TemplateForTower::validate(); -- Not Found: factionName";
    } else {
        factionName = properties.value("factionName");
    }
    if (!properties.contains("name")) {
        qDebug() << "TemplateForTower::validate(); -- Not Found: name";
    } else {
        name = properties.value("name");
    }
    if (!properties.contains("type")) {
        qDebug() << "TemplateForTower::validate(); -- Not Found: type";
    } else {
        type = properties.value("type");
    }
    if (!properties.contains("radiusDetection")) {
        qDebug() << "TemplateForTower::validate(); -- Not Found: radiusDetection";
    } else {
        radiusDetection = properties.value("radiusDetection").toInt();
    }
    if (!properties.contains("radiusFlyShell")) {
        qDebug() << "TemplateForTower::validate(); -- Not Found: radiusFlyShell";
    } else {
        radiusFlyShell = properties.value("radiusFlyShell").toInt();
    }
    if (!properties.contains("damage")) {
        qDebug() << "TemplateForTower::validate(); -- Not Found: damage";
    } else {
        damage = properties.value("damage").toInt();
    }
    if (!properties.contains("size")) {
        qDebug() << "TemplateForTower::validate(); -- Not Found: size";
    } else {
        size = properties.value("size").toInt();
    }
    if (!properties.contains("cost")) {
        qDebug() << "TemplateForTower::validate(); -- Not Found: cost";
    } else {
        cost = properties.value("cost").toInt();
    }
    if (!properties.contains("ammoSize")) {
        qDebug() << "TemplateForTower::validate(); -- Not Found: ammoSize";
    } else {
        ammoSize = properties.value("ammoSize").toFloat();
    }
    if (!properties.contains("ammoSpeed")) {
        qDebug() << "TemplateForTower::validate(); -- Not Found: ammoSpeed";
    } else {
        ammoSpeed = properties.value("ammoSpeed").toFloat();
    }
    if (!properties.contains("reloadTime")) {
        qDebug() << "TemplateForTower::validate(); -- Not Found: reloadTime";
    } else {
        reloadTime = properties.value("reloadTime").toFloat();
    }
    if (!properties.contains("towerAttackType")) {
        qDebug() << "TemplateForTower::validate(); -- Not Found: towerAttackType";
    } else {
        towerAttackType = TowerAttackType::from_string(properties.value("towerAttackType").toStdString());
    }
//    if (!properties.contains("shellAttackType") && towerAttackType != TowerAttackType.Pit) {
//        qDebug() << "TemplateForTower::validate(); -- Not Found: shellAttackType";
//    } else {
//        shellAttackType = ShellAttackType.from_string(properties.value("shellAttackType"));
//    }
//    if (!properties.contains("shellEffectType")) {
//        qDebug() << "TemplateForTower::validate(); -- Not Found: shellEffectType";
//    } else {
//        shellEffectType = new ShellEffectType(ShellEffectType.ShellEffectEnum.from_string(properties.value("shellEffectType")));
//    }
    if (towerAttackType == TowerAttackType::Pit && properties.contains("capacity")) {
        capacity = properties.value("capacity").toInt();
    } else if (towerAttackType == TowerAttackType::Pit) {
        qDebug() << "TemplateForTower::validate(); -- Not Found: capacity! When towerAttackType==Pit";
    }
//    if(!properties.contains("radiusDetection") && towerAttackType != TowerAttackType.Pit) {
//        qDebug() << "TemplateForTower::validate(); -- Not Found: radiusDetection";
//    }
//    if(this.radiusFlyShell == null && this.shellAttackType != ShellAttackType.FirstTarget) {
//        qDebug() << "TemplateForTower::validate(); -- Not Found: radiusFlyShell";
//        this->radiusFlyShell = 0.0;
//    }

    if(idleTile == NULL)
        qDebug() << "TemplateForTower::validate(); -- Not Found: idleTile";
    else if(ammunitionPictures.size() == 0)
        qDebug() << "TemplateForTower::validate(); -- Not Found: ammunitionPictures";
//    foreach (QString key, ammunitionPictures.keys()) {
//        qDebug() << "TemplateForTower::validate(); -- Dir:" << key << " properties:" << ammunitionPictures.value(key)->properties;
//    }
    qDebug() << "TemplateForTower::validate(); --" << toString(true).toStdString().c_str();
}

QString TemplateForTower::toString() {
    return toString(false);
}

QString TemplateForTower::toString(bool full) {
    QString sb("TemplateForTower[");
    sb.append(toStringBasicParam());
    if(full) {
        sb.append(toStringProperties());
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
        sb.append(",capacity:%1" + QString::number(capacity));
        sb.append(QString(",idleTile!= NULL:%1").arg( idleTile!= NULL ));
        sb.append(QString(",ammunitionPictures.size():%1").arg(ammunitionPictures.size()));
    }
    sb.append("]");
    return sb;
}
