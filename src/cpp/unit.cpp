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
//    qDebug() << "Unit::setAnimation(); -- action+direction:" << (action + Direction::toString(direction) );
    AnimatedTile* animatedTiledMapTile = templateForUnit->animations.value( action + Direction::toString(direction) );
    if (animatedTiledMapTile != NULL) {
        QVector<StaticTile*> staticTiledMapTiles = animatedTiledMapTile->getFrameTiles();
        std::vector<QPixmap> textureRegions;
        for (int k = 0; k < staticTiledMapTiles.size(); k++) {
            textureRegions.push_back(staticTiledMapTiles.at(k)->getPixmap());
        }
        this->animation = new Animation(speed / staticTiledMapTiles.size(), textureRegions);
//        qDebug() << "Unit::setAnimation(); -- animation:" << animation << " textureRegions:" << textureRegions[0];
    } else {
        qDebug() << "Unit::setAnimation(" << (action + Direction::toString(direction)) << "); -- UnitName: " << templateForUnit->name << " animatedTiledMapTile: " << animatedTiledMapTile;
    }
}

//void shellEffectsMove(float delta) {
//    for (TowerShellEffect towerShellEffect : shellEffectTypes) {
////            Gdx.app.log("Unit::shellEffectsMove()", "-- towerShellEffect:" + towerShellEffect);
//        if (!towerShellEffect.used) {
//            towerShellEffect.used = true;
//            if (towerShellEffect.shellEffectEnum == TowerShellEffect.ShellEffectEnum.FreezeEffect) {
//                float smallSpeed = speed/100f;
//                float percentSteps = stepsInTime/smallSpeed;
//                speed += towerShellEffect.speed;
//                smallSpeed = speed/100f;
//                stepsInTime = smallSpeed*percentSteps;
//            } else if (towerShellEffect.shellEffectEnum == TowerShellEffect.ShellEffectEnum.FireEffect) {
//                hp -= towerShellEffect.damage;
////                    if(die(towerShellEffect.damage, null)) {
////                        GameField.gamerGold += templateForUnit.bounty;
////                    }
//            }
//        } else {
//            if (towerShellEffect.shellEffectEnum == TowerShellEffect.ShellEffectEnum.FireEffect) {
//                hp -= towerShellEffect.damage;
////                    if(die(towerShellEffect.damage, null)) {
////                        GameField.gamerGold += templateForUnit.bounty;
////                    }
//            }
//        }
//        towerShellEffect.elapsedTime += delta;
//        if (towerShellEffect.elapsedTime >= towerShellEffect.time) {
////                Gdx.app.log("Unit::shellEffectsMove()", "-- Remove towerShellEffect:" + towerShellEffect);
//            if (towerShellEffect.shellEffectEnum == TowerShellEffect.ShellEffectEnum.FreezeEffect) {
//                float smallSpeed = speed/100f;
//                float percentSteps = stepsInTime/smallSpeed;
//                speed = speed- towerShellEffect.speed;
//                smallSpeed = speed/100f;
//                stepsInTime = smallSpeed*percentSteps;
//            }
//            shellEffectTypes.removeValue(towerShellEffect, true);
//        }
//    }
//}

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
//    shellEffectsMove(deltaTime);
//    stepsInTime += (speed*deltaTime);
    stepsInTime += deltaTime; // wtf? check Bullet::flightOfShell()
    if (stepsInTime >= speed) {
        if (/*route != null &&*/ !route.empty()) {
            stepsInTime = 0.0;
            oldPosition = newPosition;
            newPosition = route.back();
            route.pop_back();
//            qDebug() << "Unit::move(); -- newPosition:" << newPosition.toString().c_str();
//            if (newPosition == route.end()) {
//                newPosition = oldPosition;
//            }
        } else {
            direction = Direction::type::UP;
            setAnimation("idle_");
            return NULL;
        }
    }
    int oldX = oldPosition.x, oldY = oldPosition.y;
    int newX = newPosition.x, newY = newPosition.y;
    float sizeCellX = cameraController->sizeCellX;
    float sizeCellY = cameraController->sizeCellY;
    float halfSizeCellX = sizeCellX/2;
    float halfSizeCellY = sizeCellY/2;
    Vector2 *fVc = new Vector2(); // fVc = floatVectorCoordinates
    Direction::type oldDirection = direction;
    int isDrawableUnits = cameraController->isDrawableUnits;
    if (isDrawableUnits == 4 || isDrawableUnits == 5) {
//            fVc = new Vector2(getCell(newX, newY).graphicsCoord4);
        float fVx = (-(halfSizeCellX * newY) - (newX * halfSizeCellX)) - halfSizeCellX;
        float fVy = ( (halfSizeCellY * newY) - (newX * halfSizeCellY));
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
        circle4->setPosition(fVc);
    }
    if(isDrawableUnits == 3 || isDrawableUnits == 5) {
//            fVc = new Vector2(getCell(newX, newY).graphicsCoord3);
        float fVx = (-(halfSizeCellX * newY) + (newX * halfSizeCellX));
        float fVy = ( (halfSizeCellY * newY) + (newX * halfSizeCellY)) + halfSizeCellY;
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
        circle3->setPosition(fVc);
    }
    if(isDrawableUnits == 2 || isDrawableUnits == 5) {
//            fVc = new Vector2(getCell(newX, newY).graphicsCoord2);
        float fVx = ( (halfSizeCellX * newY) + (newX * halfSizeCellX)) + halfSizeCellX;
        float fVy = ( (halfSizeCellY * newY) - (newX * halfSizeCellY));
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
        circle2->setPosition(fVc);
    }
    if(isDrawableUnits == 1 || isDrawableUnits == 5) {
//            fVc = new Vector2(getCell(newX, newY).graphicsCoord1);
        float fVx = (-(halfSizeCellX * newY) + (newX * halfSizeCellX));
        float fVy = (-(halfSizeCellY * newY) - (newX * halfSizeCellY)) - halfSizeCellY;
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
        circle1->setPosition(fVc);
    }

    backStepPoint->set(currentPoint);
    currentPoint->set(fVc);
    delete fVc; // fVc = NULL;

    velocity = new Vector2(backStepPoint->x - currentPoint->x, backStepPoint->y - currentPoint->y);
    velocity->nor()->scl(qMin(currentPoint->dst(backStepPoint->x, backStepPoint->y), speed));
    displacement = new Vector2(velocity->x * deltaTime, velocity->y * deltaTime);

//        qDebug() << "Unit::move(); -- direction:" << direction << " oldDirection:" << oldDirection;
    if(direction != oldDirection) {
        setAnimation("walk_");
    }
    return &newPosition;
}

bool Unit::die(float damage, TowerShellEffect *towerShellEffect) {
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
//    if (shellEffectType != NULL) {
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
