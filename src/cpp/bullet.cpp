#include "src/head/bullet.h"


Bullet::Bullet(QPointF* currentPoint, TemplateForTower* templateForTower, Unit* unit) {
    qDebug() << "Bullet::Bullet(); -- currentPoint:" << currentPoint << " templateForTower:" << templateForTower->toString(true).toStdString().c_str() << " unit:" << unit;
    this->unit = unit;
    this->ammoExpSize = templateForTower->ammoSize;
    this->ammoSize = templateForTower->ammoSize;
    this->ammoSpeed = templateForTower->ammoSpeed;
    this->templateForTower = templateForTower;

    Tile* tiledMapTile = templateForTower->animations.value("ammo_" + Direction::UP);
    this->pixmap = (tiledMapTile != NULL) ? tiledMapTile->getPixmap() : templateForTower->idleTile->getPixmap();
//    this->ammunitionPictures = templateForTower->animations;

    this->currentPoint = new QPointF(currentPoint->x(), currentPoint->y());
    this->circle = new Circle(*currentPoint, ammoSize);
    if(templateForTower->shellAttackType == ShellAttackType::MultipleTarget || templateForTower->shellAttackType == ShellAttackType::FirstTarget) {
//        this->endPoint = new Circle(unit->currentPoint.x + unit.displacement.x, unit.currentPoint.y + unit.displacement.y, 3f);
    } else if(templateForTower->shellAttackType == ShellAttackType::AutoTarget) {
//        if(GameField.isDrawableUnits == 1 || GameField.isDrawableUnits == 5 || GameField.isDrawableUnits == 0)
//            this.endPoint = unit.circle1;
//        else if(GameField.isDrawableUnits == 2)
//            this.endPoint = unit.circle1;
//        else if(GameField.isDrawableUnits == 3)
//            this.endPoint = unit.circle1;
//        else if(GameField.isDrawableUnits == 4)
//            this.endPoint = unit.circle1;
//            this.endPoint.setRadius(3f);
//            this.endPoint = unit.currentPoint // LOL break
    } else if (templateForTower->shellAttackType == ShellAttackType::FireBall) {
//        Vector2 endPoint = new Vector2(unit.circle1.x, unit.circle1.y);
//        Direction direction = unit.direction;
//        float delta = GameField.sizeCellX;
//        float del = 1.8f;
//        if (direction == Direction.UP) {
//            endPoint.add(0, delta);
//        } else if (direction == Direction.UP_RIGHT) {
//            endPoint.add(delta / del, delta / del);
//        } else if (direction == Direction.RIGHT) {
//            endPoint.add(delta, 0);
//        } else if (direction == Direction.DOWN_RIGHT) {
//            endPoint.add(delta / del, -(delta / del));
//        } else if (direction == Direction.DOWN) {
//            endPoint.add(0, -delta);
//        } else if (direction == Direction.DOWN_LEFT) {
//            endPoint.add(-(delta / del), -(delta / del));
//        } else if (direction == Direction.LEFT) {
//            endPoint.add(-delta, 0);
//        } else if (direction == Direction.UP_LEFT) {
//            endPoint.add(-(delta / del), delta / del);
//        }
//        this->endPoint = new Circle(endPoint, 3.0);
    }
//    velocity = new Vector2(endPoint.x - currentPoint.x, endPoint.y - currentPoint.y).nor().scl(Math.min(currentPoint.dst(endPoint.x, endPoint.y), ammoSpeed));
}

Bullet::Bullet(int currCellX, int currCellY, Direction::type direction, TemplateForTower* templateForTower) {
    this->flying = true;
    this->currCellX = currCellX;
    this->currCellY = currCellY;
    this->direction = direction;
    this->templateForTower = templateForTower;

    this->animationCurrIter = 0;
    this->animationMaxIter = 0;

//    this->pixmap = templateForTower->bullet[0];
}

Bullet::~Bullet() {
//    unit = NULL;
    delete animation;
}

void Bullet::setAnimation(QString action) {
//    try {
        AnimatedTile* animatedTiledMapTile = templateForTower->animations.value(action + direction);
        QVector<StaticTile*> staticTiledMapTiles = animatedTiledMapTile->getFrameTiles();
        std::vector<QPixmap> textureRegions;
        for (int k = 0; k < staticTiledMapTiles.size(); k++) {
            textureRegions.push_back(staticTiledMapTiles.at(k)->getPixmap());
        }
        this->animation = new Animation(ammoSpeed / staticTiledMapTiles.size(), textureRegions);
//        Gdx.app.log("Unit::setAnimation()", "-- ActionAndDirection:" + action+direction + " textureRegions:" + textureRegions[0]);
//    } catch (Exception exp) {
//        Gdx.app.log("Bullet::setAnimation(" + action + direction + ")", "-- templateForTowerName: " + templateForTower.name + " Exp: " + exp);
//    }
}

/*
 * Говорит пуле постараться достигнуть криппа.
 *
 * @param delta пока не используется. (нужна для ожидания пули времени для перемещения)
 * @return -1 - Пуля не передвинулась. Крип мертв. Нужно убрать пулю из массива пуль.<br>
 * 0 - Пуля передвинулась и достигла крипа.<br>
 * 1 - Пуля передвинулась, но не достигла крипа.<br>
 */
int Bullet::flightOfShell(float delta) {
    if(unit->isAlive()) {
//            Gdx.app.log("Bullet", "flightOfShell(" + delta + "); -- " + currentPoint + ", " + endPoint + ", " + velocity);
        if(templateForTower->shellAttackType == ShellAttackType::MultipleTarget || templateForTower->shellAttackType == ShellAttackType::FirstTarget) {
            float displacementX = velocity->x() * delta * ammoSpeed;
            float displacementY = velocity->y() * delta * ammoSpeed;

//            currentPoint->add(displacementX, displacementY);
            circle->setPosition(*currentPoint);

            if (templateForTower->shellAttackType == ShellAttackType::MultipleTarget) {
                if(circle->overlaps(endPoint)) {
                    tryToHitUnits();
                    return 0;
                }
            } else if(templateForTower->shellAttackType == ShellAttackType::FirstTarget) {
                if(tryToHitUnits()) {
                    return 0;
                }
            }
            return 1;
        } else if(templateForTower->shellAttackType == ShellAttackType::AutoTarget) {
//            velocity = new Vector2(endPoint.x - currentPoint.x, endPoint.y - currentPoint.y).nor().scl(Math.min(currentPoint.dst(endPoint.x, endPoint.y), ammoSpeed));
//            currentPoint.add(velocity.x * delta * ammoSpeed, velocity.y * delta * ammoSpeed);
            circle->setPosition(*currentPoint);
            // endPoint2 == endPoint == unit.currentPoint ~= unit.circle1
            if (circle->overlaps(unit->circle1)) {
                if (unit->die(templateForTower->damage, templateForTower->shellEffectType)) {
//                    GameField.gamerGold += unit.templateForUnit.bounty;
                }
                return 0;
            }
            return 1;
        }
    }
    return -1;
}

bool Bullet::tryToHitUnits() {
    bool hit = false;
//    for (Unit unit : GameField.unitsManager.units) { // not good
//        if (Intersector.overlaps(circle, unit.circle1)) {
//            hit = true;
//            if (unit.die(templateForTower.damage, templateForTower.shellEffectType)) {
//                GameField.gamerGold += unit.templateForUnit.bounty;
//            }
//        }
//    }
    return hit;
}

//void Bullet::setCurrCellCoordinate(int currX, int currY) {
//    this->currCellX = currX;
//    this->currCellY = currY;
//}

//QPixmap Bullet::getAnimationInformation(int *lastX, int *lastY, int *animationCurrIter, int *animationMaxIter) {
//    *lastX = this->lastCellX;
//    *lastY = this->lastCellY;
//    *animationCurrIter = this->animationCurrIter;
//    *animationMaxIter = this->animationMaxIter;
//    return this->pixmap;
//}

//void Bullet::setPixmap(QPixmap pixmap) {
//    this->pixmap = pixmap;
//}

//int Bullet::getCurrCellX() {
//    return currCellX;
//}

//int Bullet::getCurrCellY() {
//    return currCellY;
//}

//QPixmap Bullet::getPixmap() {
//    return pixmap;
//}
