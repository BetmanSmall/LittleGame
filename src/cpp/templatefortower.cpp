#include "src/head/templatefortower.h"

TemplateForTower::TemplateForTower() {
    qDebug() << "TemplateForTower::TemplateForTower(); -- ";
    this->reloadTime = 3000;
}

TemplateForTower::~TemplateForTower() {
    qDebug() << "TemplateForTower::~TemplateForTower(); -- ";
    bullet.clear();

    bullet_fly_up.clear();
    bullet_fly_up_right.clear();
    bullet_fly_right.clear();
    bullet_fly_down_right.clear();
    bullet_fly_down.clear();
    bullet_fly_down_left.clear();
    bullet_fly_left.clear();
    bullet_fly_up_left.clear();
}
