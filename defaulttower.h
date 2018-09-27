#ifndef DEFAULTTOWER_H
#define DEFAULTTOWER_H

#include <QDebug>
#include <QPixmap>

/**
 * @brief The DefaultTower class - класс, описывающий все реализации определенного типа Башни.
 */
class DefaultTower
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

//    QPixmap idle_up;
//    QPixmap idle_up_right;
//    QPixmap idle_right;
//    QPixmap idle_down_right;
//    QPixmap idle_down;
//    QPixmap idle_down_left;
//    QPixmap idle_left;
//    QPixmap idle_up_left;

//    std::vector<QPixmap> attack_up;
//    std::vector<QPixmap> attack_up_right;
//    std::vector<QPixmap> attack_right;
//    std::vector<QPixmap> attack_down_right;
//    std::vector<QPixmap> attack_down;
//    std::vector<QPixmap> attack_down_left;
//    std::vector<QPixmap> attack_left;
//    std::vector<QPixmap> attack_up_left;

    DefaultTower();
    ~DefaultTower();
};

#endif // DEFAULTTOWER_H
