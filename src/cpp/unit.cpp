#include "src/head/unit.h"

Unit::Unit(AStar::CoordinateList route, TemplateForUnit *templateForUnit, int player) {
    if (!route.empty()) {
        this->route = route;
        this->oldPosition = route.back();
        this->newPosition = oldPosition;
        route.pop_back();
        this->hp = templateForUnit->healthPoints;
        this->speed = templateForUnit->speed;
        this->stepsInTime = 0.0;
        this->deathElapsedTime = 0.0;

        this->player = player;
//        this->coorByMapX = coorByMapX;
//        this->coorByMapY = coorByMapY;
        this->circle1 = new Circle(0.0, 0.0, 16.0);
        this->circle2 = new Circle(0.0, 0.0, 16.0);
        this->circle3 = new Circle(0.0, 0.0, 16.0);
        this->circle4 = new Circle(0.0, 0.0, 16.0);
//        this->alive = true;
//        this->preDeath = false;
//        this->type = templateForUnit->type;

        this->templateForUnit = templateForUnit;

        this->direction = Direction::type::UP;
//        this->animationCurrIter = 0;
//        this->animationMaxIter = 0;
        setAnimation("walk_");
//        this->effects
    } else {
        qDebug() << "Unit::Unit(); -- path.empty():" << route.empty();
    }
}

Unit::~Unit() {
    route.clear();
//    activePixmaps.clear();
    delete circle1;
    delete circle2;
    delete circle3;
    delete circle4;
    delete animation;
}

void Unit::setAnimation(QString action) {
    qDebug() << "Unit::setAnimation(); -- action_+:" << action + Direction::toString(direction);
//    try {
        AnimatedTile* animatedTiledMapTile = templateForUnit->animations.value( action + Direction::toString(direction) );
        QVector<StaticTile*> staticTiledMapTiles = animatedTiledMapTile->getFrameTiles();
        std::vector<QPixmap> textureRegions;
        for (int k = 0; k < staticTiledMapTiles.size(); k++) {
            textureRegions.push_back(staticTiledMapTiles.at(k)->getPixmap());
        }
//        qDebug() << "Unit::setAnimation(); -- textureRegions:" << textureRegions[0];
        this->animation = new Animation(speed / staticTiledMapTiles.size(), textureRegions);
//        qDebug() << "Unit::setAnimation(); -- animation:" << animation;
//    } catch (Exception exp) {
//        Gdx.app.log("Unit::setAnimation(" + action + direction + ")", "-- UnitName: " + templateForUnit.name + " Exp: " + exp);
//    }
//    qDebug() << "Unit::setAnimation(); -end- ";
}

void Unit::correct_fVc(float &fVx, float &fVy, Direction::type direction, float sizeCellX) {
    this->direction = direction;
    if (direction == Direction::type::UP) {
        fVy -= (sizeCellX / 2 / speed) * (speed - stepsInTime);
    } else if (direction == Direction::type::UP_RIGHT) {
        fVx -= (sizeCellX / 2 / speed) * (speed - stepsInTime);
        fVy -= (sizeCellX / 4 / speed) * (speed - stepsInTime);
    } else if (direction = Direction::type::RIGHT) {
        fVx -= (sizeCellX / speed) * (speed - stepsInTime);
    } else if (direction == Direction::type::DOWN_RIGHT) {
        fVx -= (sizeCellX / 2 / speed) * (speed - stepsInTime);
        fVy += (sizeCellX / 4 / speed) * (speed - stepsInTime);
    } else if (direction = Direction::type::DOWN) {
        fVy += (sizeCellX / 2 / speed) * (speed - stepsInTime);
    } else if (direction = Direction::type::DOWN_LEFT) {
        fVx += (sizeCellX / 2 / speed) * (speed - stepsInTime);
        fVy += (sizeCellX / 4 / speed) * (speed - stepsInTime);
    } else if (direction = Direction::type::LEFT) {
        fVx += (sizeCellX / speed) * (speed - stepsInTime);
    } else if (direction = Direction::type::UP_LEFT) {
        fVx += (sizeCellX / 2 / speed) * (speed - stepsInTime);
        fVy -= (sizeCellX / 4 / speed) * (speed - stepsInTime);
    }
}

AStar::Vec2i* Unit::move(float deltaTime, CameraController* cameraController) {
//    qDebug() << "Unit::move(); -- Unit status:" << this->toString();
//    qDebug() << "Unit::move(); -- stepsInTime:" << stepsInTime;
//    qDebug() << "Unit::move(); -- deltaTime:" << deltaTime;
//    qDebug() << "Unit::move(); -- speed:" << speed;
    if(/*route != null &&*/ !route.empty()) {
        stepsInTime += deltaTime;
        if (stepsInTime >= speed) {
            stepsInTime = 0.0;
            oldPosition = newPosition;
            newPosition = route.back();
            route.pop_back();
        }

        int oldX = oldPosition.x, oldY = oldPosition.y;
        int newX = newPosition.x, newY = newPosition.y;
        int sizeCellX = cameraController->sizeCellX;
        int sizeCellY = cameraController->sizeCellY;
        float halfSizeCellX = sizeCellX/2;
        float halfSizeCellY = sizeCellY/2;
        float fVx = 0, fVy = 0;
        Direction::type oldDirection = direction;
        int isDrawableUnits = cameraController->isDrawableUnits;
        if(isDrawableUnits == 4 || isDrawableUnits == 5) {
            fVx = (-(halfSizeCellX * newY) - (newX * halfSizeCellX)) - halfSizeCellX;
            fVy = ( (halfSizeCellY * newY) - (newX * halfSizeCellY)) + halfSizeCellY;
            if (newX < oldX && newY > oldY) {
                direction = Direction::type::UP;
                fVy -= (sizeCellY / speed) * (speed - stepsInTime);
            } else if (newX < oldX && newY == oldY) {
                direction = Direction::type::UP_RIGHT;
                fVx -= (sizeCellX / 2 / speed) * (speed - stepsInTime);
                fVy -= (sizeCellY / 2 / speed) * (speed - stepsInTime);
            } else if (newX < oldX && newY < oldY) {
                direction = Direction::type::RIGHT;
                fVx -= (sizeCellX / speed) * (speed - stepsInTime);
            } else if (newX == oldX && newY < oldY) {
                direction = Direction::type::DOWN_RIGHT;
                fVx -= (sizeCellX / 2 / speed) * (speed - stepsInTime);
                fVy += (sizeCellY / 2 / speed) * (speed - stepsInTime);
            } else if (newX > oldX && newY < oldY) {
                direction = Direction::type::DOWN;
                fVy += (sizeCellY / speed) * (speed - stepsInTime);
            } else if (newX > oldX && newY == oldY) {
                direction = Direction::type::DOWN_LEFT;
                fVx += (sizeCellX / 2 / speed) * (speed - stepsInTime);
                fVy += (sizeCellY / 2 / speed) * (speed - stepsInTime);
            } else if (newX > oldX && newY > oldY) {
                direction = Direction::type::LEFT;
                fVx += (sizeCellX / speed) * (speed - stepsInTime);
            } else if (newX == oldX && newY > oldY) {
                direction = Direction::type::UP_LEFT;
                fVx += (sizeCellX / 2 / speed) * (speed - stepsInTime);
                fVy -= (sizeCellY / 2 / speed) * (speed - stepsInTime);
            }
//            currentPoint.set(fVx, fVy);
            circle4->set(fVx, fVy, 16.0);
        }
        if(isDrawableUnits == 3 || isDrawableUnits == 5) {
            fVx = (-(halfSizeCellX * newY) + (newX * halfSizeCellX));
            fVy = ( (halfSizeCellY * newY) + (newX * halfSizeCellY)) + halfSizeCellY*2;
            if (newX < oldX && newY > oldY) {
                direction = Direction::type::UP;
                fVy -= (sizeCellY / speed) * (speed - stepsInTime);
            } else if (newX > oldX && newY == oldY) {
                direction = Direction::type::UP_RIGHT;
                fVx -= (sizeCellX / 2 / speed) * (speed - stepsInTime);
                fVy -= (sizeCellY / 2 / speed) * (speed - stepsInTime);
            } else if (newX > oldX && newY < oldY) {
                direction = Direction::type::RIGHT;
                fVx -= (sizeCellX / speed) * (speed - stepsInTime);
            } else if (newX == oldX && newY < oldY) {
                direction = Direction::type::DOWN_RIGHT;
                fVx -= (sizeCellX / 2 / speed) * (speed - stepsInTime);
                fVy += (sizeCellY / 2 / speed) * (speed - stepsInTime);
            } else if (newX < oldX && newY < oldY) {
                direction = Direction::type::DOWN;
                fVy += (sizeCellY / speed) * (speed - stepsInTime);
            } else if (newX < oldX && newY == oldY) {
                direction = Direction::type::DOWN_LEFT;
                fVx += (sizeCellX / 2 / speed) * (speed - stepsInTime);
                fVy += (sizeCellY / 2 / speed) * (speed - stepsInTime);
            } else if (newX < oldX && newY > oldY) {
                direction = Direction::type::LEFT;
                fVx += (sizeCellX / speed) * (speed - stepsInTime);
            } else if (newX == oldX && newY > oldY) {
                direction = Direction::type::UP_LEFT;
                fVx += (sizeCellX / 2 / speed) * (speed - stepsInTime);
                fVy -= (sizeCellY / 2 / speed) * (speed - stepsInTime);
            }
//            currentPoint.set(fVx, fVy);
            circle3->set(fVx, fVy, 16.0);
        }
        if(isDrawableUnits == 2 || isDrawableUnits == 5) {
            fVx = (halfSizeCellX * newY) + (newX * halfSizeCellX) + halfSizeCellX;
            fVy = (halfSizeCellY * newY) - (newX * halfSizeCellY) + halfSizeCellY;
            if (newX < oldX && newY > oldY) {
                direction = Direction::type::UP;
                fVy -= (sizeCellY / speed) * (speed - stepsInTime);
            } else if (newX == oldX && newY > oldY) {
                direction = Direction::type::UP_RIGHT;
                fVx -= (sizeCellX / 2 / speed) * (speed - stepsInTime);
                fVy -= (sizeCellY / 2 / speed) * (speed - stepsInTime);
            } else if (newX > oldX && newY > oldY) {
                direction = Direction::type::RIGHT;
                fVx -= (sizeCellX / speed) * (speed - stepsInTime);
            } else if (newX > oldX && newY == oldY) {
                direction = Direction::type::DOWN_RIGHT;
                fVx -= (sizeCellX / 2 / speed) * (speed - stepsInTime);
                fVy += (sizeCellY / 2 / speed) * (speed - stepsInTime);
            } else if (newX > oldX && newY < oldY) {
                direction = Direction::type::DOWN;
                fVy += (sizeCellY / speed) * (speed - stepsInTime);
            } else if (newX == oldX && newY < oldY) {
                direction = Direction::type::DOWN_LEFT;
                fVx += (sizeCellX / 2 / speed) * (speed - stepsInTime);
                fVy += (sizeCellY / 2 / speed) * (speed - stepsInTime);
            } else if (newX < oldX && newY < oldY) {
                direction = Direction::type::LEFT;
                fVx += (sizeCellX / speed) * (speed - stepsInTime);
            } else if (newX < oldX && newY == oldY) {
                direction = Direction::type::UP_LEFT;
                fVx += (sizeCellX / 2 / speed) * (speed - stepsInTime);
                fVy -= (sizeCellY / 2 / speed) * (speed - stepsInTime);
            }
//            currentPoint.set(fVx, fVy);
            circle2->set(fVx, fVy, 16.0);
        }
        if(isDrawableUnits == 1 || isDrawableUnits == 5) {
            fVx = (-(halfSizeCellX * newY) + (newX * halfSizeCellX));
            fVy = (-(halfSizeCellY * newY) - (newX * halfSizeCellY));
            if (newX < oldX && newY < oldY) {
                direction = Direction::type::UP;
                fVy -= (sizeCellY / speed) * (speed - stepsInTime);
            } else if (newX == oldX && newY < oldY) {
                direction = Direction::type::UP_RIGHT;
                fVx -= (sizeCellX / 2 / speed) * (speed - stepsInTime);
                fVy -= (sizeCellY / 2 / speed) * (speed - stepsInTime);
            } else if (newX > oldX && newY < oldY) {
                direction = Direction::type::RIGHT;
                fVx -= (sizeCellX / speed) * (speed - stepsInTime);
            } else if (newX > oldX && newY == oldY) {
                direction = Direction::type::DOWN_RIGHT;
                fVx -= (sizeCellX / 2 / speed) * (speed - stepsInTime);
                fVy += (sizeCellY / 2 / speed) * (speed - stepsInTime);
            } else if (newX > oldX && newY > oldY) {
                direction = Direction::type::DOWN;
                fVy += (sizeCellY / speed) * (speed - stepsInTime);
            } else if (newX == oldX && newY > oldY) {
                direction = Direction::type::DOWN_LEFT;
                fVx += (sizeCellX / 2 / speed) * (speed - stepsInTime);
                fVy += (sizeCellY / 2 / speed) * (speed - stepsInTime);
            } else if (newX < oldX && newY > oldY) {
                direction = Direction::type::LEFT;
                fVx += (sizeCellX / speed) * (speed - stepsInTime);
            } else if (newX < oldX && newY == oldY) {
                direction = Direction::type::UP_LEFT;
                fVx += (sizeCellX / 2 / speed) * (speed - stepsInTime);
                fVy -= (sizeCellY / 2 / speed) * (speed - stepsInTime);
            }
//            currentPoint.set(fVx, fVy);
            circle1->set(fVx, fVy, 16.0);
        }

//        backStepPoint = currentPoint;
//        currentPoint.set(fVx, fVy);
//        velocity = new Vector2(backStepPoint.x - currentPoint.x,
//                backStepPoint.y - currentPoint.y).nor().scl(Math.min(currentPoint.dst(backStepPoint.x,
//                backStepPoint.y), speed));
//        displacement = new Vector2(velocity.x * deltaTime, velocity.y * deltaTime);

//        qDebug() << "Unit::move(); -- direction:" << direction << " oldDirection:" << oldDirection;
        if(direction != oldDirection) {
            setAnimation("walk_");
        }
        return &newPosition;
    } else {
//        dispose();`
        return NULL;
    }
}

bool Unit::changeDeathFrame(float delta) {
    if (hp <= 0) {
        if (deathElapsedTime >= speed) {
//                dispose();
            return false;
        } else {
            deathElapsedTime += delta;
        }
        return true;
    }
    return false;
}

//QPixmap Unit::getAnimationInformation(int *lastX, int *lastY, int *animationCurrIter, int *animationMaxIter) {
//    *lastX = this->lastX;
//    *lastY = this->lastY;
//    *animationCurrIter = this->animationCurrIter;
//    *animationMaxIter = this->animationMaxIter;
//    return this->pixmap;
//}

bool Unit::isAlive() {
    if(animation == NULL) {
        return false;
    }
    return hp > 0 ? true : false;
}

QPixmap Unit::getCurrentFrame() {
    return animation->getKeyFrame(stepsInTime, true);
}

QPixmap Unit::getCurrentDeathFrame() {
    return animation->getKeyFrame(deathElapsedTime, true);
}

QString Unit::toString() {
    QString sb("Unit[");
//    sb.append(QString("path:%1").arg(path));
    sb.append(QString("oldPosition:%1").arg(oldPosition.toString().c_str()));
    sb.append(QString(",newPosition:%1").arg(newPosition.toString().c_str()));
    sb.append(QString(",hp:%1").arg(hp));
    sb.append(QString(",speed:%1").arg(speed));
    sb.append(QString(",stepsInTime:%1").arg(stepsInTime));
    sb.append(QString(",deathElapsedTime:%1").arg(deathElapsedTime));
    sb.append(QString(",player:%1").arg(player));
    sb.append(",circle1:" + circle1->toString());
    sb.append(",circle2:" + circle2->toString());
    sb.append(",circle3:" + circle3->toString());
    sb.append(",circle4:" + circle4->toString());
//    sb.append(QString(",alive:%1").arg(alive));
//    sb.append(QString(",preDeath:%1").arg(preDeath));
//    sb.append(QString(",type:%1").arg(type));
    sb.append(QString(",templateForUnit:%1").arg(templateForUnit->toString()));
    sb.append(QString(",direction:%1").arg(direction));
//    sb.append(",animation:" + animation);
//    sb.append("shellEffectTypes:" + shellEffectTypes + ",");
    sb.append("]");
    return sb;
}
