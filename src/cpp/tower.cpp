#include "src/head/tower.h"

Tower::Tower(int x, int y, TemplateForTower *templateForTower) {
    qDebug() << "Tower::Tower(); -- x:" << x << ", y:" << y << ", templateForTower:" << templateForTower;
    this->currX = x;
    this->currY = y;
    this->elapsedReloadTime = templateForTower->reloadTime;
    this->templateForTower = templateForTower;

//    this.player = player;
//    this.capacity = (templateForTower.capacity != null) ? templateForTower.capacity : 0;
//    this->bullets = new Array<Shell>();
//    this->radiusDetectionСircle = templateForTower->radiusDetectionСircle;
//    this->radiusDetectionСircle = new Circle(getCenterGraphicCoord(1), (templateForTower->radiusDetection == null) ? 0f : templateForTower->radiusDetection); // AlexGor
//    if(templateForTower->shellAttackType == ShellAttackType->FirstTarget && templateForTower->radiusFlyShell != null && templateForTower->radiusFlyShell >= templateForTower->radiusDetection) {
//        this->radiusFlyShellСircle = new Circle(getCenterGraphicCoord(1), templateForTower->radiusFlyShell);
//    }
}

bool Tower::recharge(float delta) {
    elapsedReloadTime += delta;
    if(elapsedReloadTime >= templateForTower->reloadTime) {
        return true;
    }
    return false;
}

void Tower::createBullets(int difficultyLevel) {
    elapsedReloadTime = 0;
    if (difficultyLevel == 2) {
        for (int dir = 1; dir < 9; dir++) { // dir = 1 because in Direction::type first IDLE for Unit
            Bullet* bullet0 = new Bullet(currX, currY, (Direction::type)dir, templateForTower);
            bullets.push_back(bullet0);
        }
    } else  /*if (difficultyLevel == 1)*/ {
        Bullet* bullet1 = new Bullet(currX, currY, Direction::UP_LEFT, templateForTower);
        bullets.push_back(bullet1);
        Bullet* bullet2 = new Bullet(currX, currY, Direction::DOWN_LEFT, templateForTower);
        bullets.push_back(bullet2);
        Bullet* bullet3 = new Bullet(currX, currY, Direction::UP_RIGHT, templateForTower);
        bullets.push_back(bullet3);
        Bullet* bullet4 = new Bullet(currX, currY, Direction::DOWN_RIGHT, templateForTower);
        bullets.push_back(bullet4);

    }
}

