#ifndef TEMPLATEFORTOWER_H
#define TEMPLATEFORTOWER_H

#include <QDebug>
#include <QPixmap>

//#include "src/head/libs/tinyxml2.h"

/**
 * @brief The TemplateForTower class - класс, описывающий все реализации определенного типа Башни.
 */
class TemplateForTower
{
public:
    QString name;
    int attack;
    int radius;
    int size;
    int reloadTime;
    int height;
    int type;

    QPixmap pixmap;
    std::vector<QPixmap> bullet;

    std::vector<QPixmap> bullet_fly_up;
    std::vector<QPixmap> bullet_fly_up_right;
    std::vector<QPixmap> bullet_fly_right;
    std::vector<QPixmap> bullet_fly_down_right;
    std::vector<QPixmap> bullet_fly_down;
    std::vector<QPixmap> bullet_fly_down_left;
    std::vector<QPixmap> bullet_fly_left;
    std::vector<QPixmap> bullet_fly_up_left;

    TemplateForTower();
    ~TemplateForTower();
};

#endif // TEMPLATEFORTOWER_H
