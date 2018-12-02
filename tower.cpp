#include "tower.h"

Tower::Tower(Cell *cell, TemplateForTower *templateForTower, int player) {
//    qDebug() << "Tower::Tower(); -- position:" << cell << " templateForTower:" << templateForTower << " player:" << player;
    this->cell = cell;
    this->elapsedReloadTime = templateForTower->reloadTime;
    this->templateForTower = templateForTower;

    this->player = player;
    this->capacity = templateForTower->capacity;
//    this->bullets = new Array<Bullet>();
    this->centerGraphicCoord = new Vector2();
    this->radiusDetectionCircle = new Circle(0, 0, templateForTower->radiusDetection);
    this->radiusFlyShellCircle = NULL;
}

Tower::~Tower() {
    qDebug() << "Tower::~Tower(); -- ";
    bullets.clear();
    delete centerGraphicCoord;
    delete radiusDetectionCircle;
    if (radiusFlyShellCircle != NULL) {
        delete radiusFlyShellCircle;
    }
}

void Tower::updateGraphicCoordinates(CameraController *cameraController) {
    if (cameraController->isDrawableTowers == 1 || cameraController->isDrawableTowers == 5) {
        centerGraphicCoord->set(cell->graphicCoordinates1);
    } else if (cameraController->isDrawableTowers == 2) {
        centerGraphicCoord->set(cell->graphicCoordinates2);
    } else if (cameraController->isDrawableTowers == 3) {
        centerGraphicCoord->set(cell->graphicCoordinates3);
    } else if (cameraController->isDrawableTowers == 4) {
        centerGraphicCoord->set(cell->graphicCoordinates4);
    } else {
        centerGraphicCoord->setZero();
    }
    this->radiusDetectionCircle->setPosition(centerGraphicCoord);
    if (templateForTower->towerShellType == TowerShellType::FirstTarget) {
        if (templateForTower->radiusFlyShell != 0.0 && templateForTower->radiusFlyShell >= templateForTower->radiusDetection) {
            if (radiusFlyShellCircle == NULL) {
                this->radiusFlyShellCircle = new Circle(centerGraphicCoord, templateForTower->radiusFlyShell);
            } else {
                this->radiusFlyShellCircle->setPosition(centerGraphicCoord);
            }
        }
    }
}

bool Tower::recharge(float delta) {
    elapsedReloadTime += delta;
    if(elapsedReloadTime >= templateForTower->reloadTime) {
        return true;
    }
    return false;
}

bool Tower::shotFireBall(CameraController *cameraController) {
    if (elapsedReloadTime >= templateForTower->reloadTime) {
        if (templateForTower->towerAttackType == TowerAttackType::FireBall) {
            elapsedReloadTime = 0.0;
            int radius = qRound(cameraController->gameField->gameSettings->difficultyLevel);
            if ( radius == 0 ) {
                radius = qRound(templateForTower->radiusDetection);
            }
            Cell* towerCell = cell;
            qDebug() << "Tower::shotFireBall(); -- radius:" << radius << " towerCell:" << towerCell << " player:" << player;
            for (int tmpX = -radius; tmpX <= radius; tmpX++) {
                for (int tmpY = -radius; tmpY <= radius; tmpY++) {
                    Cell* cell = cameraController->gameField->getCell(tmpX + towerCell->cellX, tmpY + towerCell->cellY);
                    if (cell != NULL && cell != towerCell) {
                        bullets.push_back(new Bullet(centerGraphicCoord, templateForTower, cell->graphicCoordinates3, cameraController));
                    }
                }
            }
//                bullets.add(new Bullet(centerGraphicCoord, templateForTower, cameraController, Direction.UP));
//                bullets.add(new Bullet(centerGraphicCoord, templateForTower, cameraController, Direction.UP_RIGHT));
//                bullets.add(new Bullet(centerGraphicCoord, templateForTower, cameraController, Direction.RIGHT));
//                bullets.add(new Bullet(centerGraphicCoord, templateForTower, cameraController, Direction.DOWN_RIGHT));
//                bullets.add(new Bullet(centerGraphicCoord, templateForTower, cameraController, Direction.DOWN));
//                bullets.add(new Bullet(centerGraphicCoord, templateForTower, cameraController, Direction.DOWN_LEFT));
//                bullets.add(new Bullet(centerGraphicCoord, templateForTower, cameraController, Direction.LEFT));
//                bullets.add(new Bullet(centerGraphicCoord, templateForTower, cameraController, Direction.UP_LEFT));
            qDebug() << "Tower::shotFireBall(); -- bullets:" << bullets.size() << " templateForTower:" << templateForTower->toString() << " player:" << player;
            return true;
        }
    }
    return false;
}

bool Tower::shoot(Unit* unit, CameraController* cameraController) {
    if(elapsedReloadTime >= templateForTower->reloadTime) {
        if (templateForTower->towerShellType == TowerShellType::MassAddEffect) {
            bool effect = false;
            foreach (TowerShellEffect* towerShellEffect, unit->shellEffectTypes) {
                if (towerShellEffect->shellEffectEnum == TowerShellEffect::FreezeEffect) {
                    effect = true;
                    break;
                }
            }
            if (!effect) {
                unit->shellEffectTypes.push_back(new TowerShellEffect(templateForTower->towerShellEffect));
            }
        } else {
            bullets.push_back(new Bullet(centerGraphicCoord, templateForTower, unit, cameraController));
        }
        elapsedReloadTime = 0.0;
        return true;
    }
    return false;
}

void Tower::moveAllShells(float delta, CameraController *cameraController) {
    foreach (Bullet* bullet, bullets) {
        if(radiusFlyShellCircle == NULL) {
            moveShell(delta, bullet, cameraController);
        } else if(radiusFlyShellCircle->overlaps(bullet->currCircle)) {
            moveShell(delta, bullet, cameraController);
        } else {
            bullets.erase(std::find(bullets.begin(), bullets.end(), bullet));
//            bullet.dispose();
//            delete bullet;
        }
    }
}

void Tower::moveShell(float delta, Bullet *bullet, CameraController *cameraController) {
    switch (bullet->flightOfShell(delta, cameraController)) {
        case 1:
            break;
        case 0:
//            break;
        case -1:
            bullets.erase(std::find(bullets.begin(), bullets.end(), bullet));
//            delete bullet;
            break;
    }
}

QString Tower::toString() {
    return toString(false);
}

QString Tower::toString(bool full) {
    QString sb("Tower[");
    sb.append("cell->cellX:" + QString::number(cell->cellX));
    sb.append(",cell->cellY:" + QString::number(cell->cellY));
    if (full) {
        sb.append(QString(",elapsedReloadTime:%1").arg(elapsedReloadTime));
        sb.append(QString(",templateForTower:%1").arg(templateForTower->toString()));
        sb.append(QString(",player:%1").arg(player));
        sb.append(QString(",capacity:%1").arg(capacity));
        sb.append(QString(",bullets.size:%1").arg(bullets.size()));
        sb.append(",radiusDetectionCircle:" + radiusDetectionCircle->toString());
//        sb.append(",radiusFlyShellCircle:" + radiusFlyShellCircle);
    }
    sb.append("]");
    return sb;
}
