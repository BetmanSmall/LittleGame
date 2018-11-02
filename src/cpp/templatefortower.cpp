#include "src/head/templatefortower.h"

TemplateForTower::TemplateForTower(QString templateFile) {
    qDebug() << "TemplateForTower::TemplateForTower(); -- templateFile:" << templateFile;
//    this->reloadTime = 3000;
    loadBasicTemplate(templateFile);
    specificLoad();
//    try {
        validate();
//    } catch (Exception exp) {
//        qDebug() << "TemplateForTower::TemplateForTower(); -- Could not load TemplateForTower from " + templateFile.path() + " Exp:" + exp);
//        throw new Exception("TemplateForTower::TemplateForTower() -- Could not load TemplateForTower from " + templateFile.path() + " Exp:" + exp);
//    }
    qDebug() << "TemplateForTower::TemplateForTower(); -end- templateFile:" << templateFile;
}

TemplateForTower::~TemplateForTower() {
    qDebug() << "TemplateForTower::~TemplateForTower(); -- ";
//    bullet.clear();
//    bullet_fly_up.clear();
//    bullet_fly_up_right.clear();
//    bullet_fly_right.clear();
//    bullet_fly_down_right.clear();
//    bullet_fly_down.clear();
//    bullet_fly_down_left.clear();
//    bullet_fly_left.clear();
//    bullet_fly_up_left.clear();
}

//void TemplateForTower::setFaction(Faction* faction) {
//    this->faction = faction;
//}

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
//                setAmmoTiles(tileName, tile);
            }
        }
    }
//    qDebug() << "TemplateForTower::specificLoad(); -- idleTile:" << idleTile;
//    qDebug() << "TemplateForTower::specificLoad(); -- ammunitionPictures.size:" << ammunitionPictures.size();
//    qDebug() << "TemplateForTower::specificLoad(); -end- ";
}

void TemplateForTower::validate() {
    basicValidate();
    // Need check range values
    if (!properties.contains("radiusDetection")) {
        qDebug() << "TemplateForTower::validate(); -- Can't get 'radiusDetection'! Check the file";
    } else {
        radiusDetection = properties.value("radiusDetection").toInt();
    }
    if (!properties.contains("radiusFlyShell")) {
        qDebug() << "TemplateForTower::validate(); -- Can't get 'radiusDetection'! Check the file";
    } else {
        radiusFlyShell = properties.value("radiusFlyShell").toInt();
    }
//    if(this.radiusDetection == NULL && this.towerAttackType != TowerAttackType.Pit)
//        qDebug() << "TemplateForTower::validate(); -- Can't get 'radiusDetection'! Check the file";
//    else if(this.radiusFlyShell == NULL && this.shellAttackType != ShellAttackType.FirstTarget)
//        qDebug() << "TemplateForTower::validate(); -- Can't get 'radiusFlyShell'! Check the file";
//            this.radiusFlyShell = 0f;
    if (!properties.contains("damage")) {
        qDebug() << "TemplateForTower::validate(); -- Can't get 'damage'! Check the file";
    } else {
        damage = properties.value("damage").toInt();
    }
    if (!properties.contains("size")) {
        qDebug() << "TemplateForTower::validate(); -- Can't get 'size'! Check the file";
    } else {
        size = properties.value("size").toInt();
    }
    if (!properties.contains("cost")) {
        qDebug() << "TemplateForTower::validate(); -- Can't get 'cost'! Check the file";
    } else {
        cost = properties.value("cost").toInt();
    }
    if (!properties.contains("ammoSize")) {
        qDebug() << "TemplateForTower::validate(); -- Can't get 'ammoSize'! Check the file";
    } else {
        ammoSize = properties.value("ammoSize").toFloat();
    }
    if (!properties.contains("ammoSpeed")) {
        qDebug() << "TemplateForTower::validate(); -- Can't get 'ammoSpeed'! Check the file";
    } else {
        ammoSpeed = properties.value("ammoSpeed").toFloat();
    }
    if (!properties.contains("reloadTime")) {
        qDebug() << "TemplateForTower::validate(); -- Can't get 'reloadTime'! Check the file";
    } else {
        reloadTime = properties.value("reloadTime").toFloat();
    }
    if (!properties.contains("capacity")) {
        qDebug() << "TemplateForTower::validate(); -- Can't get 'capacity'! Check the file";
    } else {
        capacity = properties.value("capacity").toInt();
    }

    if(idleTile == NULL)
        qDebug() << "TemplateForTower::validate(); -- Can't get 'idleTile'! Check the file";
    else if(ammunitionPictures.size() == 0)
        qDebug() << "TemplateForTower::validate(); -- Can't get 'ammo'! Check the file";
//    foreach (QString key, ammunitionPictures.keys()) {
//        qDebug() << "TemplateForTower::validate(); -- Dir:" << key << " properties:" << ammunitionPictures.value(key)->properties;
//    }
    qDebug() << "TemplateForTower::validate(); --" << toString(true).toStdString().c_str();
//    qDebug() << "TemplateForTower::validate(); -end- idleTile:" << idleTile;
//    qDebug() << "TemplateForTower::validate(); -- ------end-------";
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
//        sb += ",towerAttackType:" + towerAttackType;
//        sb += ",shellAttackType:" + shellAttackType;
//        sb += ",shellEffectEnum:" + shellEffectType;
        sb.append(QString(",idleTile!= NULL:%1").arg( idleTile!= NULL ));
        sb.append(QString(",ammunitionPictures.size():%1").arg(ammunitionPictures.size()));
//        sb += ",capacity:" + capacity;
//        sb += ",ammoDistance:" + ammoDistance;
//        sb.append(QString(",!pixmap.isNull():%1").arg(!pixmap.isNull()));
//        sb.append(QString(",bullet.size():%1").arg(bullet.size()));
    }
    sb.append("]");
    return sb;
}
