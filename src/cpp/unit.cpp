#include "src/head/unit.h"

Unit::Unit(AStar::CoordinateList route, TemplateForUnit *templateForUnit, int player, Cell *exitCell) {
    if (!route.empty()) {
        this->route = route;
        this->oldPosition = route.back();
        this->newPosition = oldPosition;
        route.pop_back();
        this->exitCell = exitCell;
        this->hp = templateForUnit->healthPoints;
        this->speed = templateForUnit->speed;
        this->stepsInTime = 0.0;
        this->deathElapsedTime = 0.0;

        this->player = player;
        this->currentPoint = new Vector2();
        this->backStepPoint = new Vector2();
        this->circle1 = new Circle(0.0, 0.0, 16.0);
        this->circle2 = new Circle(0.0, 0.0, 16.0);
        this->circle3 = new Circle(0.0, 0.0, 16.0);
        this->circle4 = new Circle(0.0, 0.0, 16.0);

        this->templateForUnit = templateForUnit;

        this->direction = Direction::type::UP;
        setAnimation("walk_");
//        this->shellEffectTypes = new Array<ShellEffectType>();
    } else {
        qDebug() << "Unit::Unit(); -- path.empty():" << route.empty();
    }
}

Unit::~Unit() {
    route.clear();
//    delete oldPosition;
//    delete newPosition;
    exitCell = NULL;

    delete currentPoint;
    delete backStepPoint;
    delete circle1;
    delete circle2;
    delete circle3;
    delete circle4;
    delete velocity;
    delete displacement;

    templateForUnit = NULL;

//    delete direction;
    delete animation;
    shellEffectTypes.clear();
}

void Unit::setAnimation(QString action) {
//    qDebug() << "Unit::setAnimation(); -- action_+:" << action + Direction::toString(direction);
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

void Unit::correct_fVc(Vector2 *fVc, Direction::type direction, float sizeCellX) {
    this->direction = direction;
    float fVx = fVc->x;
    float fVy = fVc->y;
    if (direction == Direction::type::UP) {
        fVy += ( (sizeCellX / 2) / speed) * (speed - stepsInTime);
    } else if (direction == Direction::type::UP_RIGHT) {
        fVx -= ( (sizeCellX / 2) / speed) * (speed - stepsInTime);
        fVy += ( (sizeCellX / 4) / speed) * (speed - stepsInTime);
    } else if (direction == Direction::type::RIGHT) {
        fVx -= (sizeCellX / speed) * (speed - stepsInTime);
    } else if (direction == Direction::type::DOWN_RIGHT) {
        fVx -= ( (sizeCellX / 2) / speed) * (speed - stepsInTime);
        fVy -= ( (sizeCellX / 4) / speed) * (speed - stepsInTime);
    } else if (direction == Direction::type::DOWN) {
        fVy -= ( (sizeCellX / 2) / speed) * (speed - stepsInTime);
    } else if (direction == Direction::type::DOWN_LEFT) {
        fVx += ( (sizeCellX / 2) / speed) * (speed - stepsInTime);
        fVy -= ( (sizeCellX / 4) / speed) * (speed - stepsInTime);
    } else if (direction == Direction::type::LEFT) {
        fVx += (sizeCellX / speed) * (speed - stepsInTime);
    } else if (direction == Direction::type::UP_LEFT) {
        fVx += ( (sizeCellX / 2) / speed) * (speed - stepsInTime);
        fVy += ( (sizeCellX / 4) / speed) * (speed - stepsInTime);
    }
    fVc->set(fVx, fVy);
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
        Vector2 *fVc = new Vector2(); // fVc = floatVectorCoordinates
//        float fVx = 0, fVy = 0;
        Direction::type oldDirection = direction;
        int isDrawableUnits = cameraController->isDrawableUnits;
        if(isDrawableUnits == 4 || isDrawableUnits == 5) {
//            fVc = new Vector2(getCell(newX, newY).graphicsCoord4);
            float fVx = (-(halfSizeCellX * newY) - (newX * halfSizeCellX)) - halfSizeCellX;
            float fVy = ( (halfSizeCellY * newY) - (newX * halfSizeCellY)) + halfSizeCellY;
            fVc->set(fVx, fVy);
            if (newX < oldX && newY > oldY) {
                correct_fVc(fVc, Direction::type::DOWN, sizeCellX);
            } else if (newX < oldX && newY == oldY) {
                correct_fVc(fVc, Direction::type::DOWN_RIGHT, sizeCellX);
            } else if (newX < oldX && newY < oldY) {
                correct_fVc(fVc, Direction::type::RIGHT, sizeCellX);
            } else if (newX == oldX && newY < oldY) {
                correct_fVc(fVc, Direction::type::UP_RIGHT, sizeCellX);
            } else if (newX > oldX && newY < oldY) {
                correct_fVc(fVc, Direction::type::UP, sizeCellX);
            } else if (newX > oldX && newY == oldY) {
                correct_fVc(fVc, Direction::type::UP_LEFT, sizeCellX);
            } else if (newX > oldX && newY > oldY) {
                correct_fVc(fVc, Direction::type::LEFT, sizeCellX);
            } else if (newX == oldX && newY > oldY) {
                correct_fVc(fVc, Direction::type::DOWN_LEFT, sizeCellX);
            }
            circle4->set(fVx, fVy, 16.0);
        }
        if(isDrawableUnits == 3 || isDrawableUnits == 5) {
//            fVc = new Vector2(getCell(newX, newY).graphicsCoord3);
            float fVx = (-(halfSizeCellX * newY) + (newX * halfSizeCellX));
            float fVy = ( (halfSizeCellY * newY) + (newX * halfSizeCellY)) + halfSizeCellY*2;
            fVc->set(fVx, fVy);
            if (newX < oldX && newY < oldY) {
                correct_fVc(fVc, Direction::type::UP, sizeCellX);
            } else if (newX == oldX && newY < oldY) {
                correct_fVc(fVc, Direction::type::UP_RIGHT, sizeCellX);
            } else if (newX > oldX && newY < oldY) {
                correct_fVc(fVc, Direction::type::RIGHT, sizeCellX);
            } else if (newX > oldX && newY == oldY) {
                correct_fVc(fVc, Direction::type::DOWN_RIGHT, sizeCellX);
            } else if (newX > oldX && newY > oldY) {
                correct_fVc(fVc, Direction::type::DOWN, sizeCellX);
            } else if (newX == oldX && newY > oldY) {
                correct_fVc(fVc, Direction::type::DOWN_LEFT, sizeCellX);
            } else if (newX < oldX && newY > oldY) {
                correct_fVc(fVc, Direction::type::LEFT, sizeCellX);
            } else if (newX < oldX && newY == oldY) {
                correct_fVc(fVc, Direction::type::UP_LEFT, sizeCellX);
            }
            circle3->set(fVx, fVy, 16.0);
        }
        if(isDrawableUnits == 2 || isDrawableUnits == 5) {
//            fVc = new Vector2(getCell(newX, newY).graphicsCoord2);
            float fVx = (halfSizeCellX * newY) + (newX * halfSizeCellX) + halfSizeCellX;
            float fVy = (halfSizeCellY * newY) - (newX * halfSizeCellY) + halfSizeCellY;
            fVc->set(fVx, fVy);
            if (newX < oldX && newY > oldY) {
                correct_fVc(fVc, Direction::type::DOWN, sizeCellX);
            } else if (newX == oldX && newY > oldY) {
                correct_fVc(fVc, Direction::type::DOWN_RIGHT, sizeCellX);
            } else if (newX > oldX && newY > oldY) {
                correct_fVc(fVc, Direction::type::RIGHT, sizeCellX);
            } else if (newX > oldX && newY == oldY) {
                correct_fVc(fVc, Direction::type::UP_RIGHT, sizeCellX);
            } else if (newX > oldX && newY < oldY) {
                correct_fVc(fVc, Direction::type::UP, sizeCellX);
            } else if (newX == oldX && newY < oldY) {
                correct_fVc(fVc, Direction::type::UP_LEFT, sizeCellX);
            } else if (newX < oldX && newY < oldY) {
                correct_fVc(fVc, Direction::type::LEFT, sizeCellX);
            } else if (newX < oldX && newY == oldY) {
                correct_fVc(fVc, Direction::type::DOWN_LEFT, sizeCellX);
            }
            circle2->set(fVx, fVy, 16.0);
        }
        if(isDrawableUnits == 1 || isDrawableUnits == 5) {
            float fVx = (-(halfSizeCellX * newY) + (newX * halfSizeCellX));
            float fVy = (-(halfSizeCellY * newY) - (newX * halfSizeCellY));
            fVc->set(fVx, fVy);
            if (newX < oldX && newY < oldY) {
                correct_fVc(fVc, Direction::type::DOWN, sizeCellX);
            } else if (newX == oldX && newY < oldY) {
                correct_fVc(fVc, Direction::type::DOWN_RIGHT, sizeCellX);
            } else if (newX > oldX && newY < oldY) {
                correct_fVc(fVc, Direction::type::RIGHT, sizeCellX);
            } else if (newX > oldX && newY == oldY) {
                correct_fVc(fVc, Direction::type::UP_RIGHT, sizeCellX);
            } else if (newX > oldX && newY > oldY) {
                correct_fVc(fVc, Direction::type::UP, sizeCellX);
            } else if (newX == oldX && newY > oldY) {
                correct_fVc(fVc, Direction::type::UP_LEFT, sizeCellX);
            } else if (newX < oldX && newY > oldY) {
                correct_fVc(fVc, Direction::type::LEFT, sizeCellX);
            } else if (newX < oldX && newY == oldY) {
                correct_fVc(fVc, Direction::type::DOWN_LEFT, sizeCellX);
            }
            circle1->set(fVx, fVy, 16.0);
        }

        backStepPoint->set(currentPoint);
        currentPoint->set(fVc);
        delete fVc; // fVc = NULL;

        velocity = new Vector2(backStepPoint->x - currentPoint->x, backStepPoint->y - currentPoint->y);
        velocity = velocity->nor()->scl(qMin(currentPoint->dst(backStepPoint->x, backStepPoint->y), speed));
        displacement = new Vector2(velocity->x * deltaTime, velocity->y * deltaTime);

//        qDebug() << "Unit::move(); -- direction:" << direction << " oldDirection:" << oldDirection;
        if(direction != oldDirection) {
            setAnimation("walk_");
        }
        return &newPosition;
    } else {
//        dispose();
        return NULL;
    }
}

bool Unit::die(float damage, ShellEffectType shellEffectType) {
    if(hp > 0) {
        hp -= damage;
//        addEffect(shellEffectType);
        if(hp <= 0) {
            deathElapsedTime = 0;
            setAnimation("death_");
            return true;
        }
        return false;
    }
    return false;
}

//bool Unit::addEffect(ShellEffectType *shellEffectType) {
//    if(shellEffectType != NULL){
//        if(!shellEffectTypes.contains(shellEffectType, false)) {
//            shellEffectTypes.add(new ShellEffectType(shellEffectType));
//        }
//    }
//    return true;
//}

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
    return toString(false);
}

QString Unit::toString(bool full) {
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
//    sb.append(",animation:" + animation->);
//    sb.append("shellEffectTypes:" + shellEffectTypes);
    sb.append("]");
    return sb;
}
