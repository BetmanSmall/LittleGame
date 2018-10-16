#include "src/head/tower.h"

Tower::Tower() {
    this->elapsedReloadTime = 0;
}

bool Tower::recharge(float delta) {

    elapsedReloadTime += delta;
    if(elapsedReloadTime >= defTower->reloadTime) {
        return true;
    }
    return false;
}

void Tower::createBullets(int difficultyLevel) {
    elapsedReloadTime = 0;
    if (difficultyLevel == 2) {
        for (int dir = 1; dir < 9; dir++) { // dir = 1 because in Direction first IDLE for Unit
            Bullet* bullet0 = new Bullet(currX, currY, (Direction)dir, defTower);
            bullets.push_back(bullet0);
        }
    } else  /*if (difficultyLevel == 1)*/ {
        Bullet* bullet1 = new Bullet(currX, currY, DirectionUpLeft, defTower);
        bullets.push_back(bullet1);
        Bullet* bullet2 = new Bullet(currX, currY, DirectionDownLeft, defTower);
        bullets.push_back(bullet2);
        Bullet* bullet3 = new Bullet(currX, currY, DirectionUpRight, defTower);
        bullets.push_back(bullet3);
        Bullet* bullet4 = new Bullet(currX, currY, DirectionDownRight, defTower);
        bullets.push_back(bullet4);

    }
}

