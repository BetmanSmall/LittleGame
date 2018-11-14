#include "src/head/tower.h"

Tower::Tower(QPoint *position, TemplateForTower *templateForTower, int player) {
    qDebug() << "Tower::Tower(); -- position:" << position << " templateForTower:" << templateForTower << " player:" << player;
    this->position = position;
    this->elapsedReloadTime = templateForTower->reloadTime;
    this->templateForTower = templateForTower;

    this->player = player;
    this->capacity = templateForTower->capacity;
//    this->bullets = new Array<Bullet>();
    radiusDetectionCircle = NULL;
    radiusFlyShellCircle = NULL;
}

Tower::~Tower() {
    qDebug() << "Tower::~Tower(); -- ";
}

void Tower::updateGraphicCoordinates(CameraController *cameraController) {
    if (radiusDetectionCircle != NULL) {
        delete radiusDetectionCircle;
    }
    this->radiusDetectionCircle = new Circle(cameraController->getCenterTowerGraphicCoord(position->x(), position->y()), templateForTower->radiusDetection);
    if (templateForTower->shellAttackType == ShellAttackType::FirstTarget && templateForTower->radiusFlyShell != 0.0 && templateForTower->radiusFlyShell >= templateForTower->radiusDetection) {
        if (radiusFlyShellCircle != NULL) {
            delete radiusFlyShellCircle;
        }
        this->radiusFlyShellCircle = new Circle(cameraController->getCenterTowerGraphicCoord(position->x(), position->y()), templateForTower->radiusFlyShell);
    }
}

bool Tower::recharge(float delta) {
    elapsedReloadTime += delta;
    if(elapsedReloadTime >= templateForTower->reloadTime) {
        return true;
    }
    return false;
}

bool Tower::shoot(Unit* unit, CameraController* cameraController) {
    if(elapsedReloadTime >= templateForTower->reloadTime) {
        if (templateForTower->shellAttackType == ShellAttackType::MassAddEffect) {
            bool effect = false;
            for (ShellEffectType* shellEffectType : unit->shellEffectTypes) {
                if (shellEffectType->shellEffectEnum == ShellEffectType::FreezeEffect) {
                    effect = true;
                    break;
                }
            }
            if (!effect) {
                unit->shellEffectTypes.push_back(new ShellEffectType(templateForTower->shellEffectType));
            }
        } else if (templateForTower->shellAttackType == ShellAttackType::FireBall) {

        } else {
            bullets.push_back(new Bullet(cameraController->getCenterTowerGraphicCoord(position->x(), position->y()), templateForTower, unit));
        }
        elapsedReloadTime = 0.0;
        return true;
    }
    return false;
}

void Tower::createBullets(int difficultyLevel) {
    elapsedReloadTime = 0;
    if (difficultyLevel == 2) {
        for (int dir = 1; dir < 9; dir++) { // dir = 1 because in Direction::type first IDLE for Unit
            Bullet* bullet0 = new Bullet(position->x(), position->y(), (Direction::type)dir, templateForTower);
            bullets.push_back(bullet0);
        }
    } else  /*if (difficultyLevel == 1)*/ {
        Bullet* bullet1 = new Bullet(position->x(), position->y(), Direction::type::UP_LEFT, templateForTower);
        bullets.push_back(bullet1);
        Bullet* bullet2 = new Bullet(position->x(), position->y(), Direction::type::DOWN_LEFT, templateForTower);
        bullets.push_back(bullet2);
        Bullet* bullet3 = new Bullet(position->x(), position->y(), Direction::type::UP_RIGHT, templateForTower);
        bullets.push_back(bullet3);
        Bullet* bullet4 = new Bullet(position->x(), position->y(), Direction::type::DOWN_RIGHT, templateForTower);
        bullets.push_back(bullet4);
    }
}

void Tower::moveAllShells(float delta) {
    foreach (Bullet* bullet, bullets) {
        if(radiusFlyShellCircle == NULL) {
            moveShell(delta, bullet);
//        } else if(Intersector.overlaps(bullet.circle, radiusFlyShellCircle)) {
        } else if(radiusFlyShellCircle->overlaps(bullet->circle)) {
            moveShell(delta, bullet);
        } else {
//            bullet.dispose();
            bullets.erase(std::find(bullets.begin(), bullets.end(), bullet));
            delete bullet;
        }
    }
}

void Tower::moveShell(float delta, Bullet* bullet) {
    switch (bullet->flightOfShell(delta)) {
        case 0:
//                if(bullet.unit.die(damage)) {
//                    GameField.gamerGold += bullet.unit.getTemplateForUnit().bounty;
//                }
//                break;
        case -1:
//            bullet.dispose();
//            bullets.removeValue(bullet, false);
        bullets.erase(std::find(bullets.begin(), bullets.end(), bullet));
        delete bullet;
    }
}

QString Tower::toString() {
    return toString(false);
}

QString Tower::toString(bool full) {
    QString sb("Tower[");
    sb.append("position->x():" + QString::number(position->x()));
    sb.append(",position->y():" + QString::number(position->y()));
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
