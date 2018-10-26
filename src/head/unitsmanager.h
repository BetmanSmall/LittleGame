#ifndef CREEPS_H
#define CREEPS_H

#include <QDebug>
#include <stdlib.h>
#include <QPixmap>

#include "src/head/unit.h"

class UnitsManager {
public: // we are friendly to all!
    Unit* hero;
    Unit* units;
    QPixmap defaultPixmapForUnit;
    int size;
    int amount;

public:
    UnitsManager();
    ~UnitsManager();
    void createMass(int newSize);
    void deleteMass();
    int getSize();
    int getAmount();
    int getHP(int x, int y);
    bool attackUnit(int x, int y, int damage); //, Unit* unit);
    Unit* getUnit(int number);
    Unit* getUnit(int x, int y);
    Unit* createHero(int coorByCellX, int coorByCellY, int coorByMapX, int coorByMapY, TemplateForUnit* unit);
    Unit* createUnit(int coorByCellX, int coorByCellY, int coorByMapX, int coorByMapY, TemplateForUnit* unit, int type);
    void setDefaulPixmapForUnit(QPixmap pixmap);
    QPixmap getUnitPixmap(int x, int y);
};

#endif // CREEPS_H
