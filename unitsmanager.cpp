#include "unitsmanager.h"

void UnitsManager::createMass(int newSize)
{
    if(units == NULL)
    {
        units = new Unit[newSize];
        size = newSize;
        amount = 0;
    }
    else
    {
        deleteMass();
        createMass(newSize);
    }
}

void UnitsManager::deleteMass()
{
    if(units != NULL)
    {
        delete[] units;
        units = NULL;
    }
}

int UnitsManager::getSize()
{
    return size;
}

int UnitsManager::getAmount()
{
    return amount;
}

int UnitsManager::getHP(int x, int y)
{
    for(int k = 0; k < amount; k++)
    {
        int localX = units[k].coorByCellX;
        int localY = units[k].coorByCellY;

        if(localX == x && localY == y)
            return units[k].hp;
    }
    return 0;
}

bool UnitsManager::attackUnit(int x, int y, int damage, Unit *unit)
{
    for(int k = 0; k < amount; k++)
    {
        if(!units[k].alive)
            continue;

        int localX = units[k].coorByCellX;
        int localY = units[k].coorByCellY;

        if(localX == x && localY == y)
        {
            int localHP = units[k].hp;
            localHP = localHP - damage;

            if(localHP <= 0)
            {
                units[k].hp = 0;
//                units[k].coorByCellX = -1;
//                units[k].coorByCellY = -1;
                units[k].alive = false;
                units[k].preDeath = true;

                // ЛАЖА
                units[k].animationCurrIter = 0;
                Direction direction = units[k].direction;

                if(direction == DirectionUpLeft)
                {
                    units[k].animationMaxIter = units[k].defUnit->death_up_left.size();
                    units[k].activePixmaps = units[k].defUnit->death_up_left;
                }
                else if(direction == DirectionUp)
                {
                    units[k].animationMaxIter = units[k].defUnit->death_up.size();
                    units[k].activePixmaps = units[k].defUnit->death_up;
                }
                else if(direction == DirectionUpRight)
                {
                    units[k].animationMaxIter = units[k].defUnit->death_up_right.size();
                    units[k].activePixmaps = units[k].defUnit->death_up_right;
                }
                else if(direction == DirectionLeft)
                {
                    units[k].animationMaxIter = units[k].defUnit->death_left.size();
                    units[k].activePixmaps = units[k].defUnit->death_left;
                }
                else if(direction == DirectionRight)
                {
                    units[k].animationMaxIter = units[k].defUnit->death_right.size();
                    units[k].activePixmaps = units[k].defUnit->death_right;
                }
                else if(direction == DirectionDownLeft)
                {
                    units[k].animationMaxIter = units[k].defUnit->death_down_left.size();
                    units[k].activePixmaps = units[k].defUnit->death_down_left;
                }
                else if(direction == DirectionDown)
                {
                    units[k].animationMaxIter = units[k].defUnit->death_down.size();
                    units[k].activePixmaps = units[k].defUnit->death_down;
                }
                else if(direction == DirectionDownRight)
                {
                    units[k].animationMaxIter = units[k].defUnit->death_down_right.size();
                    units[k].activePixmaps = units[k].defUnit->death_down_right;
                }
//                qDebug() << "tmpUnit->animationMaxIter: " << tmpUnit << "->" << tmpUnit->animationMaxIter;
                units[k].pixmap = units[k].activePixmaps[0];
                // ЛАЖА !!!!!!

                unit = &units[k];
                return true;
            }
            else
                units[k].hp = localHP;

            break;
        }
    }
    return false;
}

Unit* UnitsManager::getUnit(int number)
{
    return &units[number];
}

Unit* UnitsManager::getUnit(int x, int y)
{
    for(int k = 0; k < amount; k++)
    {
        int localX = units[k].coorByCellX;
        int localY = units[k].coorByCellY;

        if(localX == x && localY == y)
        {
            return &units[k];
        }
    }
    return NULL;
}

Unit* UnitsManager::createUnit(int coorByCellX, int coorByCellY, int coorByMapX, int coorByMapY, DefaultUnit* unit)
{
    qDebug() << "UnitsManager::createUnit(); -- unit:" << unit;
//    if(amount == size)
//    {
//        Unit* tmpUnits;
//        tmpUnits = new Unit[size+1];

//        for(int k = 0; k < size; k++)
//            tmpUnits[k] = units[k];

//        size++;

//        delete[] units;
//        units = tmpUnits;
//    }

    if(amount < size)
    {
        units[amount].hp = 100;
        units[amount].alive = true;
        units[amount].preDeath = false;
        units[amount].number = amount+1;
        units[amount].coorByCellX = coorByCellX;
        units[amount].coorByCellY = coorByCellY;
        units[amount].coorByMapX = coorByMapX;
        units[amount].coorByMapY = coorByMapY;
        units[amount].lastX = coorByCellX;
        units[amount].lastY = coorByCellY;

//        units[amount].speed = speed;
//        units[amount].type = type;

        units[amount].animationCurrIter = 0;
        units[amount].animationMaxIter = 0;

        units[amount].pixmap = defaultPixmapForUnit;
        units[amount].defUnit = unit;

        return &units[amount++];
    }

    return NULL;
}

void UnitsManager::setDefaulPixmapForUnit(QPixmap pixmap)
{
    defaultPixmapForUnit = pixmap;
}

QPixmap UnitsManager::getUnitPixmap(int x, int y)
{
    for(int k = 0; k < amount; k++)
    {
        int localX = units[k].coorByCellX;
        int localY = units[k].coorByCellY;

        if(localX == x && localY == y)
        {
            return units[k].pixmap;
        }
    }
    return defaultPixmapForUnit;
}
