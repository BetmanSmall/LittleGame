#include "src/head/bullet.h"

Bullet::Bullet(Vector2 *currentPoint, TemplateForTower* templateForTower, Vector2 *destPoint, CameraController *cameraController) {
//    qDebug() << "Bullet::Bullet(); -- currentPoint:" << currentPoint;
//    qDebug() << "Bullet::Bullet(); -- templateForTower:" << templateForTower->toString(true).toStdString().c_str();
//    qDebug() << "Bullet::Bullet(); -- destPoint:" << destPoint;
//    qDebug() << "Bullet::Bullet(); -- cameraController:" << cameraController;
    this->ammoExpSize = templateForTower->ammoSize;
    this->ammoSize = templateForTower->ammoSize;
    this->ammoSpeed = templateForTower->ammoSpeed;
    this->templateForTower = templateForTower;
//    qDebug() << "Bullet::Bullet(); -- ammoExpSize:" << ammoExpSize;
//    qDebug() << "Bullet::Bullet(); -- ammoSize:" << ammoSize;
//    qDebug() << "Bullet::Bullet(); -- ammoSpeed:" << ammoSpeed;

    this->currentPoint = new Vector2(currentPoint);
    this->currCircle = new Circle(currentPoint, ammoSize);
    this->endPoint = new Vector2(destPoint);
    this->endCircle = new Circle(destPoint, 3.0);
//    qDebug() << "Bullet::Bullet(); -- currentPoint:" << currentPoint << " currCircle:" << currCircle;
//    qDebug() << "Bullet::Bullet(); -- endPoint:" << endPoint << " endCircle:" << endCircle;

    velocity = new Vector2(endPoint->x - currentPoint->x, endPoint->y - currentPoint->y);
    velocity->nor()->scl(qMin(currentPoint->dst(endPoint->x, endPoint->y), ammoSpeed));
//    qDebug() << "Bullet::Bullet(); -- velocity:" << velocity;
    setAnimation("ammo_");
}

Bullet::Bullet(Vector2 *currentPoint, TemplateForTower* templateForTower, Unit *unit, CameraController *cameraController) {
//    qDebug() << "Bullet::Bullet(); -- currentPoint:" << currentPoint;
//    qDebug() << "Bullet::Bullet(); -- templateForTower:" << templateForTower->toString(true).toStdString().c_str();
//    qDebug() << "Bullet::Bullet(); -- unit:" << unit;
//    qDebug() << "Bullet::Bullet(); -- cameraController:" << cameraController;
    this->ammoExpSize = templateForTower->ammoSize;
    this->ammoSize = templateForTower->ammoSize;
    this->ammoSpeed = templateForTower->ammoSpeed;
    this->templateForTower = templateForTower;
    this->unit = unit;

    this->currentPoint = new Vector2(currentPoint);
    this->currCircle = new Circle(currentPoint, ammoSize);
    this->endPoint = new Vector2(unit->currentPoint->x + unit->displacement->x, unit->currentPoint->y + unit->displacement->y);
    this->endCircle = new Circle(0, 0, 3.0);

    if(templateForTower->towerShellType == TowerShellType::MultipleTarget || templateForTower->towerShellType == TowerShellType::FirstTarget) {
        this->endCircle->setPosition(endPoint);
    } else if(templateForTower->towerShellType == TowerShellType::AutoTarget) {
        this->endCircle->setPosition(unit->currentPoint);
    }

    Tile* tiledMapTile = templateForTower->animations.value("ammo_" + Direction::UP);
    this->pixmap = (tiledMapTile != NULL) ? tiledMapTile->getPixmap() : templateForTower->idleTile->getPixmap();

//    qDebug() << "Bullet::Bullet()", "-- currentPoint:" << currentPoint << ", endCircle:" << endCircle;
//    qDebug() << "Bullet::Bullet()", "-- ammoSpeed:" << ammoSpeed;
    velocity = new Vector2(endCircle->x - currentPoint->x, endCircle->y - currentPoint->y);
    velocity->nor()->scl(qMin(currentPoint->dst(endCircle->x, endCircle->y), ammoSpeed));
//    qDebug() << "Bullet::Bullet()", "-- velocity:" << velocity;
}

Bullet::~Bullet() {
    unit = NULL;
    templateForTower = NULL;
//    pixmap = NULL;

    delete currentPoint;
    delete currCircle;
    delete endPoint;
    delete endCircle;
    delete velocity;

    delete animation;
}

void Bullet::setAnimation(QString action) {
    if (velocity->x > 0) {
        if (velocity->y > 0) {
            direction = Direction::type::UP_RIGHT;
        } else if (velocity->y == 0) {
            direction = Direction::type::RIGHT;
        } else if (velocity->y < 0) {
            direction = Direction::type::DOWN_RIGHT;
        }
    } else if (velocity->x == 0) {
        if (velocity->y > 0) {
            direction = Direction::type::UP;
        } else if (velocity->y == 0) {
//            direction = Direction::type::IDLE;
            qDebug() << "Bullet::Bullet(); -bad- velocity:" << velocity;
        } else if (velocity->y < 0) {
            direction = Direction::type::DOWN;
        }
    } else if (velocity->x < 0) {
        if (velocity->y > 0) {
            direction = Direction::type::UP_LEFT;
        } else if (velocity->y == 0) {
            direction = Direction::type::LEFT;
        } else if (velocity->y < 0) {
            direction = Direction::type::DOWN_LEFT;
        }
    }
//    qDebug() << "Bullet::setAnimation(); -- action+direction:" << (action + Direction::toString(direction) );
    AnimatedTile* animatedTiledMapTile = templateForTower->animations.value( action + Direction::toString(direction) );
    if (animatedTiledMapTile != NULL) {
        QVector<StaticTile*> staticTiledMapTiles = animatedTiledMapTile->getFrameTiles();
        std::vector<QPixmap> textureRegions;
        for (int k = 0; k < staticTiledMapTiles.size(); k++) {
            textureRegions.push_back(staticTiledMapTiles.at(k)->getPixmap());
        }
        this->animation = new Animation(ammoSpeed / staticTiledMapTiles.size(), textureRegions);
//        qDebug() << "Bullet::setAnimation(); -- animation:" << animation << " textureRegions:" << textureRegions[0];
    } else {
        qDebug() << "Bullet::setAnimation(" << (action + Direction::toString(direction)) << "); -- TowerName: " << templateForTower->name << " animatedTiledMapTile: " << animatedTiledMapTile;
    }
}

/*
 * Говорит пуле постараться достигнуть криппа.
 *
 * @param delta пока не используется. (нужна для ожидания пули времени для перемещения)
 * @return -1 - Пуля не передвинулась. Крип мертв. Нужно убрать пулю из массива пуль.<br>
 * 0 - Пуля передвинулась и достигла крипа.<br>
 * 1 - Пуля передвинулась, но не достигла крипа.<br>
 */
int Bullet::flightOfShell(float delta, CameraController *cameraController) {
//    Gdx.app.log("Bullet::flightOfShell()", "-- delta:" + delta);
//    Gdx.app.log("Bullet::flightOfShell()", "-- currentPoint:" + currentPoint);
//    Gdx.app.log("Bullet::flightOfShell()", "-- endCircle:" + endCircle);
//    Gdx.app.log("Bullet::flightOfShell()", "-- velocity:" + velocity);
//    Gdx.app.log("Bullet::flightOfShell()", "-- towerShellType:" + templateForTower.towerShellType);
//    Gdx.app.log("Bullet::flightOfShell()", "-- animation:" + animation);
//    Gdx.app.log("Bullet::flightOfShell()", "-- textureRegion:" + textureRegion);

        float displacementX = velocity->x * delta * ammoSpeed;
        float displacementY = velocity->y * delta * ammoSpeed;
        currentPoint->add(displacementX, displacementY);
        currCircle->setPosition(currentPoint);
        if (animation != NULL) {
            flyingTime += (ammoSpeed*delta);
            if (flyingTime >= ammoSpeed) {
                flyingTime = 0.0;
            }
            pixmap = animation->getKeyFrame(flyingTime, true);
        }
        if (templateForTower->towerShellType == TowerShellType::FirstTarget) {
            return (tryToHitUnits(cameraController) == false) ? 1 : 0;
        } else if(templateForTower->towerShellType == TowerShellType::MultipleTarget || templateForTower->towerShellType == TowerShellType::FirstTarget) {
            if (templateForTower->towerShellType == TowerShellType::MultipleTarget) {
                if(currCircle->overlaps(endCircle)) {
                    tryToHitUnits(cameraController);
                    return 0;
                }
            } else if(templateForTower->towerShellType == TowerShellType::FirstTarget) {
                if(tryToHitUnits(cameraController)) {
                    return 0;
                }
            }
            return 1;
        } else if(templateForTower->towerShellType == TowerShellType::AutoTarget) {
            if(unit->isAlive()) {
                this->endCircle->setPosition(unit->currentPoint);
                velocity = new Vector2(endCircle->x - currentPoint->x, endCircle->y - currentPoint->y);
                velocity->nor()->scl(qMin(currentPoint->dst(endCircle->x, endCircle->y), ammoSpeed));
                currentPoint->add(velocity->x * delta * ammoSpeed, velocity->y * delta * ammoSpeed);
                currCircle->setPosition(currentPoint);
                // endPoint2 == endCircle == unit.currentPoint ~= unit.circle1
                if (currCircle->overlaps(unit->circle3)) {
                    if (unit->die(templateForTower->damage, templateForTower->towerShellEffect)) {
                        cameraController->gameField->gamerGold += unit->templateForUnit->bounty;
                    }
                    return 0;
                }
                return 1;
            }
        }
        return -1;
}

bool Bullet::tryToHitUnits(CameraController *cameraController) {
    bool hit = false;
    foreach (Unit* unit, cameraController->gameField->unitsManager->units) {
        if (currCircle->overlaps(unit->circle3)) {
            hit = true;
            if (unit->die(templateForTower->damage, templateForTower->towerShellEffect)) {
                cameraController->gameField->gamerGold += unit->templateForUnit->bounty;
            }
        }
    }
    return hit;
}
