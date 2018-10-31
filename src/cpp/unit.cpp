#include "src/head/unit.h"

Unit::Unit(AStar::CoordinateList path, TemplateForUnit *templateForUnit, int player) {
    if (!path.empty()) {
        this->path = path;
        this->lastX = path.back().x;
        this->lastY = path.back().y;
        this->coorByCellX = lastX;
        this->coorByCellY = lastY;
        this->hp = templateForUnit->healthPoints;
        this->speed = templateForUnit->speed;
        this->stepsInTime = 0;
        this->deathElapsedTime = 0;

        this->player = player;
//        this->coorByMapX = coorByMapX;
//        this->coorByMapY = coorByMapY;
        this->alive = true;
        this->preDeath = false;
        this->type = templateForUnit->type;

        this->templateForUnit = templateForUnit;

        this->direction = Direction::UP;
        this->animationCurrIter = 0;
        this->animationMaxIter = 0;
//        this->effects
        setAnimation("walk_");
    }
}

Unit::~Unit() {
    path.clear();
    activePixmaps.clear();
}

void Unit::setAnimation(QString action) {
    qDebug() << "Unit::setAnimation(); -- action:" << action;
//    try {
        AnimatedTile* animatedTiledMapTile = templateForUnit->animations.value( action + Direction::toString(direction) );
        QVector<StaticTile*> staticTiledMapTiles = animatedTiledMapTile->getFrameTiles();
        std::vector<QPixmap> textureRegions;
        for (int k = 0; k < staticTiledMapTiles.size(); k++) {
            textureRegions.push_back(staticTiledMapTiles.at(k)->getPixmap());
        }
        qDebug() << "Unit::setAnimation(); -- action:" << action + Direction::toString(direction);
        qDebug() << "Unit::setAnimation(); -- action+direction:" << action+direction;
        qDebug() << "Unit::setAnimation(); -- textureRegions:" << textureRegions[0];
        this->animation = new Animation(speed / staticTiledMapTiles.size(), textureRegions);
        qDebug() << "Unit::setAnimation(); -- animation:" << animation;
//    } catch (Exception exp) {
//        Gdx.app.log("Unit::setAnimation(" + action + direction + ")", "-- UnitName: " + templateForUnit.name + " Exp: " + exp);
//    }
    qDebug() << "Unit::setAnimation(); -end- ";
}

QPixmap Unit::getAnimationInformation(int *lastX, int *lastY, int *animationCurrIter, int *animationMaxIter) {
    *lastX = this->lastX;
    *lastY = this->lastY;
    *animationCurrIter = this->animationCurrIter;
    *animationMaxIter = this->animationMaxIter;
    return this->pixmap;
}

QString Unit::toString() {
    QString sb("Unit[");
//    sb.append(QString("path:%1").arg(path));
    sb.append(QString("lastX:%1").arg(lastX));
    sb.append(QString(",lastY:%1").arg(lastY));
    sb.append(QString(",coorByCellX:%1").arg(coorByCellX));
    sb.append(QString(",coorByCellY:%1").arg(coorByCellY));
    sb.append(QString(",hp:%1").arg(hp));
    sb.append(QString(",speed:%1").arg(speed));
    sb.append(QString(",stepsInTime:%1").arg(stepsInTime));
    sb.append(QString(",deathElapsedTime:%1").arg(deathElapsedTime));
    sb.append(QString(",player:%1").arg(player));
    sb.append(QString(",coorByMapX:%1").arg(coorByMapX));
    sb.append(QString(",coorByMapY:%1").arg(coorByMapY));
    sb.append(QString(",alive:%1").arg(alive));
    sb.append(QString(",preDeath:%1").arg(preDeath));
    sb.append(QString(",type:%1").arg(type));
    sb.append(QString(",templateForUnit:%1").arg(templateForUnit->toString()));
//    sb.append("circle1:" + circle1 + ",");
//    sb.append("circle2:" + circle2 + ",");
    sb.append(QString(",direction:%1").arg(direction));
//    sb.append("animation:" + animation + ",");
//    sb.append("shellEffectTypes:" + shellEffectTypes + ",");
    sb.append("]");
    return sb;
}
