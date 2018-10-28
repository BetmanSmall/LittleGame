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
    bullet.clear();

    bullet_fly_up.clear();
    bullet_fly_up_right.clear();
    bullet_fly_right.clear();
    bullet_fly_down_right.clear();
    bullet_fly_down.clear();
    bullet_fly_down_left.clear();
    bullet_fly_left.clear();
    bullet_fly_up_left.clear();
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
    // Need check range values
//    if(this->templateName != NULL)
//        qDebug() << "TemplateForTower::validate(); -- Load TemplateForTower:" << this->templateName;
//    if(this->factionName == NULL)
//        qDebug() << "TemplateForTower::validate(); -- Can't get 'factionName'! Check the file";
//    else if(this->name == NULL)
//        qDebug() << "TemplateForTower::validate(); -- Can't get 'name'! Check the file";
//    else if(this.radiusDetection == NULL && this.towerAttackType != TowerAttackType.Pit)
//        qDebug() << "TemplateForTower::validate(); -- Can't get 'radiusDetection'! Check the file";
//    else if(this.radiusFlyShell == NULL && this.shellAttackType != ShellAttackType.FirstTarget)
//        qDebug() << "TemplateForTower::validate(); -- Can't get 'radiusFlyShell'! Check the file";
//            this.radiusFlyShell = 0f;
//    else if(this->damage == NULL)
//        qDebug() << "TemplateForTower::validate(); -- Can't get 'damage'! Check the file";
//    else if(this->ammoSize == NULL)
//        qDebug() << "TemplateForTower::validate(); -- Can't get 'ammoSize'! Check the file";
//    else if(this->ammoSpeed == NULL)
//        qDebug() << "TemplateForTower::validate(); -- Can't get 'ammoSpeed'! Check the file";
//    else if(this->reloadTime == NULL)
//        qDebug() << "TemplateForTower::validate(); -- Can't get 'reloadTime'! Check the file";

//    if(idleTile == NULL)
//        qDebug() << "TemplateForTower::validate(); -- Can't get 'idleTile'! Check the file";
//    else if(ammunitionPictures.size() == 0)
//        qDebug() << "TemplateForTower::validate(); -- Can't get 'ammo'! Check the file";
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
    if(full) {
        sb.append(toStringProperties());
        sb.append(toStringBasicParam());
        sb.append(QString(",radiusDetection:%1").arg(radiusDetection));
        sb.append(QString(",attack:%1").arg(attack));
        sb.append(QString(",damage:%1").arg(damage));
        sb.append(QString(",radius:%1").arg(radius));
        sb.append(QString(",size:%1").arg(size));
        sb.append(QString(",height:%1").arg(height));
        sb.append(QString(",ammoSize:%1").arg(ammoSize));
        sb.append(QString(",ammoSize:%1").arg(ammoSize));
        sb.append(QString(",reloadTime:%1").arg(reloadTime));
        sb.append(QString(",type:%1").arg(type));
        sb.append(QString(",idleTile!= NULL:%1").arg( idleTile!= NULL ));
        sb.append(QString(",ammunitionPictures.size():%1").arg(ammunitionPictures.size()));

//        sb += ",radiusFlyShell:" + radiusFlyShell;
//        sb += ",cost:" + cost;
//        sb += ",towerAttackType:" + towerAttackType;
//        sb += ",shellAttackType:" + shellAttackType;
//        sb += ",shellEffectEnum:" + shellEffectType;
//        sb += ",capacity:" + capacity;
//        sb += ",ammoDistance:" + ammoDistance;
        sb.append(QString(",!pixmap.isNull():%1").arg(!pixmap.isNull()));
        sb.append(QString(",bullet.size():%1").arg(bullet.size()));
    }
    sb.append("]");
    return sb;
}
