#include "field.h"

using namespace std;

void Field::createField(int newSizeX, int newSizeY)
{
    if(field == NULL)
    {
        field = new Cell[newSizeX*newSizeY];
        towersManager.createField(newSizeX*newSizeY);
        unitsManager.createMass(30);

        isometric = false;
//        unitSet = true;

        gameOverLimitUnits = 10;
        currentFinishedUnits = 0;

        sizeX = newSizeX;
        sizeY = newSizeY;

        mainCoorMapX = 0, mainCoorMapY = 0;
        spaceWidget = 0; // fix this. 16 and launch
        sizeCell = 64;

        mouseX = -1;
        mouseY = -1;
        spawnPointX = -1;
        spawnPointY = -1;
        exitPointX = -1;
        exitPointY = -1;
    }
    else
    {
        deleteField();
        createField(newSizeX, newSizeY);
    }
}

void Field::deleteField()
{
    if(field != NULL)
    {
        delete[] field;
        field = NULL;
        towersManager.deleteField();
        unitsManager.deleteMass();
    }
}

Cell* Field::getCell(int x, int y) {
    if (x >= 0 && x < sizeX) {
        if (y >= 0 && y < sizeY) {
            return &field[sizeX*y + x];
        }
    }
    qDebug() << "Field::getCell(); -- Bad x:" << x << " y:" << y;
    return NULL;
}

void Field::setFaction(Faction* faction)
{
    faction1 = faction;
}

bool Field::createSpawnPoint(int num, int x, int y){
    for(int k = 0; k < unitsManager.getAmount(); k++) {
        Unit* unit = unitsManager.getUnit(k);
        int unitX = unit->coorByCellX;
        int unitY = unit->coorByCellY;
        getCell(unitX, unitY)->clearUnit();
    }
    if(x == -1 && y == -1) {
        if(!isSetSpawnPoint()) {
            return false;
        }
    } else {
        spawnPointX = x;
        spawnPointY = y;
        getCell(x, y)->spawn = true;
        getCell(x, y)->removeTerrain(); // clearBusy(x,y);
    }
    unitsManager.deleteMass();
    unitsManager.createMass(num);
    currentFinishedUnits = 0;
    return true;
}

void Field::createExitPoint(int x, int y) {
    exitPointX = x;
    exitPointY = y;
    getCell(x, y)->exit = true;
    getCell(x, y)->removeTerrain(); // clearBusy(x, y);
    waveAlgorithm(x, y);
}

void Field::updateHeroDestinationPoint() {
    updateHeroDestinationPoint(exitPointX, exitPointY);
}

void Field::updateHeroDestinationPoint(int x, int y) {
    updatePathFinderWalls();
    for (int k = 0; k < unitsManager.getAmount(); k++) {
        Unit* tmpUnit = unitsManager.getUnit(k);
        if (tmpUnit->type == 0) {
            qDebug() << "Field::updateHeroDestionPoint(" << x << ", " << y << "); -- ";
            tmpUnit->path = pathFinder.findPath({tmpUnit->coorByCellX, tmpUnit->coorByCellY}, {x, y});
//            for(auto& coordinate : tmpUnit->path) {
//                qDebug() << "Field::updateHeroDestionPoint(); -- x:" << coordinate.x << " y:" << coordinate.y;
//            }
        }
    }
}

void Field::updatePathFinderWalls() {
    pathFinder.clearCollisions();
    for (int x = 0; x < sizeX; x++) {
        for (int y = 0; y < sizeY; y++) {
            if(getCell(x, y)->isTerrain()) {// || field[x, y].tower) {
                pathFinder.addCollision({x, y});
            }
        }
    }
}

int Field::getSizeX()
{
    return sizeX;
}

int Field::getSizeY()
{
    return sizeY;
}

void Field::setMainCoorMap(int mainCoorMapX, int mainCoorMapY) {
    this->mainCoorMapX = mainCoorMapX;
    this->mainCoorMapY = mainCoorMapY;
}

void Field::setSizeCell(int sizeCell) {
    this->sizeCell = sizeCell;
}

int Field::getMainCoorMapX() {
    return mainCoorMapX;
}

int Field::getMainCoorMapY() {
    return mainCoorMapY;
}

int Field::getSpaceWidget() {
    return spaceWidget;
}

int Field::getSizeCell() {
    return sizeCell;
}

void Field::setIsometric(bool isometric) {
    this->isometric = isometric;
}

void Field::setTileMapSize(int tileMapWidth, int tileMapHeight) {
    this->tileMapWidth = tileMapWidth;
    this->tileMapHeight = tileMapHeight;
}

bool Field::getIsometric() {
    return isometric;
}

int Field::getTileMapWidth() {
    return tileMapWidth;
}

int Field::getTileMapHeight() {
    return tileMapHeight;
}

bool Field::towersAttack()
{
    for(int k = 0; k < towersManager.getAmount(); k++)
    {
        Tower* tmpTower = towersManager.getTowerById(k);

        // DIBILOID CODE
//        for(int iBullet = 0; iBullet < tmpTower->bullets.size(); iBullet++) {
//            qDebug() << "k: " << k << " iBullet: " << iBullet;
//            tmpTower->bullets[iBullet].move();
//        }
        // !!DIBILOID CODE!!

        int x = tmpTower->currX;
        int y = tmpTower->currY;
//        int type = tmpTower->type;
        int radius = tmpTower->radius; // 1 // 5
        int size = tmpTower->size;

        tmpTower->attackX = -1;
        tmpTower->attackY = -1;

        Unit* unit = NULL;
        int defaultHp = 100;

        int attackX = x, attackY = y;

        for(int tmpY = (0-radius); tmpY < radius+size; tmpY++)
        {
            for(int tmpX = (0-radius); tmpX < radius+size; tmpX++)
            {
                if(!(tmpX == 0 && tmpY == 0))
                {
                    Unit* tmpUnit = getUnitWithLowHP(x + tmpX, y + tmpY);
                    if(tmpUnit != NULL)
                    {
                        int hp = tmpUnit->hp;//getUnitHpInCell(x + tmpX, y + tmpY);
                        if(hp <= defaultHp && hp != 0)
                        {
                            unit = tmpUnit;
                            defaultHp = hp;
                            attackX = x + tmpX;
                            attackY = y + tmpY;
                        }
                    }
                }
            }
        }

//        if(attackX != x || attackY != y)
        if(unit != NULL)
        {
//            if(type == 1)
//            Unit* unit = NULL;
//            qDebug() << "unit: " << unit;
//            if(units.attackUnit(attackX, attackY, tmpTower->attack, unit))

            if(tmpTower->bullets.size() == 0) {
                qDebug() << "createBulletAndShot(" << attackX << ", " << attackY << ");";

                int bullet_grafCoorX = tmpTower->currX*sizeCell + (sizeCell/3) + abs(mainCoorMapX);
                int bullet_grafCoorY = tmpTower->currY*sizeCell + (sizeCell/3) + abs(mainCoorMapY);
                tmpTower->createBulletAndShot(unit, bullet_grafCoorX, bullet_grafCoorY);
            } else {
                if(!tmpTower->bullets[0].flying) {
                    tmpTower->bullets.clear();
                }
            }

            if(unit->takeDamage(tmpTower->attack)) {
//                qDebug() << "unit: " << unit;
//                if(unit != NULL)
//                    qDebug() << "unit->hp: " << unit->hp;

                if(getCell(attackX, attackY)->clearUnit(unit)) { //clearUnit(attackX, attackY, unit))
                    qDebug() << "Dead!";
                }
            } else {
                tmpTower->attackX = attackX;
                tmpTower->attackY = attackY;
            }
        }
    }
    return true;
}

void Field::waveAlgorithm(int x, int y)
{
    qDebug() << "Field::waveAlgorithm() :: X: " << x << " Y: " << y;
    if(x == -1 && y == -1)
        if(isSetExitPoint())
        {
            waveAlgorithm(exitPointX, exitPointY);
            return;
        }

    if(!getCell(x, y)->isTerrain() && !getCell(x, y)->getTower())
    {
        for(int tmpX = 0; tmpX < getSizeX(); tmpX++)
            for(int tmpY = 0; tmpY < getSizeY(); tmpY++)
                clearStepCell(tmpX, tmpY);

        setStepCell(x, y, 1);

        waveStep(x, y, 1);
    }
}

void Field::waveStep(int x, int y, int step)
{
#ifdef CIRCLET8
    //------------3*3----------------
    bool mass[3][3];
    int nextStep = step+1;

    for(int tmpY = -1; tmpY < 2; tmpY++)
        for(int tmpX = -1; tmpX < 2; tmpX++)
            mass[tmpX+1][tmpY+1] = setNumOfCell(x + tmpX, y + tmpY, nextStep);

    for(int tmpY = -1; tmpY < 2; tmpY++)
        for(int tmpX = -1; tmpX < 2; tmpX++)
            if(mass[tmpX+1][tmpY+1])
                waveStep(x + tmpX, y + tmpY, nextStep);
#else
    //------------2*2----------------
    bool mass[4];
    int nextStep = step+1;
    int x1 = x-1, x2 = x, x3 = x+1;
    int y1 = y-1, y2 = y, y3 = y+1;

    mass[0] = setNumOfCell(x1, y2, nextStep);
    mass[1] = setNumOfCell(x2, y1, nextStep);
    mass[2] = setNumOfCell(x2, y3, nextStep);
    mass[3] = setNumOfCell(x3, y2, nextStep);

    if(mass[0])
        waveStep(x1, y2, nextStep);
    if(mass[1])
        waveStep(x2, y1, nextStep);
    if(mass[2])
        waveStep(x2, y3, nextStep);
    if(mass[3])
        waveStep(x3, y2, nextStep);
#endif
}

void Field::setMousePress(int x, int y)
{
    mouseX = x;
    mouseY = y;
}

bool Field::getMousePress(int x, int y)
{
    if(mouseX != -1 && mouseY != -1)
        if((x == mouseX && y == mouseY) || (x == -1 && y == -1))
            return true;

    return false;
}

bool Field::isSetSpawnPoint(int x, int y) {
//    qDebug() << "Field::isSetSpawnPoint(" << x << ", " << y << "); -- spawnPointX: " << spawnPointX << " spawnPointY: " << spawnPointY;
    if(spawnPointX != -1 && spawnPointY != -1)
        if((x == spawnPointX && y == spawnPointY) || (x == -1 && y == -1))
            return true;

    return false;
}

bool Field::isSetExitPoint(int x, int y) {
    if(exitPointX != -1 && exitPointY != -1)
        if((x == exitPointX && y == exitPointY) || (x == -1 && y == -1))
            return true;

    return false;
}

int Field::stepAllUnits() {
//    qDebug() << "Field::stepAllUnits(); -- ";
    bool allDead = true;
    for(int k = 0; k < unitsManager.getAmount(); k++) {
        int result = stepOneUnit(k);
        if(result != -2) {
            allDead = false;
        }
        if(result == 1) {
            currentFinishedUnits++;
            if(currentFinishedUnits >= gameOverLimitUnits) {
                qDebug() << "Field::stepAllUnits(); -- return 1";
                return 1;
            }
        } else if(result == -1) {
            qDebug() << "Field::stepAllUnits(); -- return -1";
            return -1;
        } else if(result == 3) {
            return 3;
        } else if(result == 4) {
            return 4;
        }
    }
//    qDebug() << "Field::stepAllUnits(); -- allDead:" << allDead;
    if(allDead) {
        return 2;
    } else {
        return 0;
    }
}

int Field::stepOneUnit(int num) {
//    qDebug() << "Field::stepOneUnit(); -- ";
    Unit* tmpUnit = unitsManager.getUnit(num);
    if(tmpUnit->alive) {
        if(tmpUnit->animationCurrIter < tmpUnit->animationMaxIter) {
//            qDebug() << "tmpUnit->animationCurrIter: " << tmpUnit << "->" << tmpUnit->animationCurrIter;
            tmpUnit->pixmap = tmpUnit->activePixmaps[tmpUnit->animationCurrIter++];
//            tmpUnit->animationCurrIter = tmpUnit->animationCurrIter+1;
        } else {
            int currX = tmpUnit->coorByCellX;
            int currY = tmpUnit->coorByCellY;
            int exitX = currX, exitY = currY;
//            qDebug() << "Field::stepOneUnit(); -- exitX: " << exitX << " exitY: " << exitY;
//            qDebug() << "Field::stepOneUnit(); -- currX: " << currX << " currY: " << currY;
//            qDebug() << "Field::stepOneUnit(); -- exitPointX: " << exitPointX << " exitPointY: " << exitPointY;

            if (!tmpUnit->path.empty()) {
                AStar::Vec2i point = tmpUnit->path.back();
                tmpUnit->path.pop_back();
                exitX = point.x;
                exitY = point.y;
//                qDebug() << "Field::stepOneUnit(); -- exitX:" << exitX << " exitY:" << exitY;
            }
            if(tmpUnit->type == 0) {
                if (currX == exitPointX && currY == exitPointY) {
                    qDebug() << "Field::stepOneUnit(); -- HeroInExitPoint!";
                    return 3;
                }
            } else /*if (tmpUnit->type != 0)*/ { // Not hero!
                if (getCell(currX, currY)->getHero()) {
                    qDebug() << "Field::stepOneUnit(); -- Hero contact with Enemy!";
                    return 4;
                }
                if (tmpUnit->path.empty()) {
                    int randomX = rand()%sizeX;
                    int randomY = rand()%sizeY;
                    tmpUnit->path = pathFinder.findPath({tmpUnit->coorByCellX, tmpUnit->coorByCellY}, {randomX, randomY});
                }
//                int min = getNumStep(currX,currY);
//                if(min == 1)
//                    return 1;
//                if(min == 0)
//                    return -1;

//                int defaultStep = min;
//                //--------------Looking specific cell-----------------------
//                for(int tmpY = -1; tmpY < 2; tmpY++) {
//                    for(int tmpX = -1; tmpX < 2; tmpX++) {
//                        if(!(tmpX == 0 && tmpY == 0)) {
//                            int num = getNumStep(currX + tmpX, currY + tmpY);
//                            if(num <= min && num != 0) {
//                                if(num == min) {
//                                    if(rand()%2) {
//                                        exitX = currX + tmpX;
//                                        exitY = currY + tmpY;
//                                    }
//                                } else if(num == defaultStep-1) {
//                                    exitX = currX + tmpX;
//                                    exitY = currY + tmpY;
//                                    min = num;
//                                }
//                            }
//                        }
//                    }
//                }
//                //-----------------------------------------------------------
//                tmpUnit->number = min;
            }

            if(exitX != currX || exitY != currY) {
                getCell(currX, currY)->clearUnit(tmpUnit);//clearUnit(currX, currY, tmpUnit);
                tmpUnit->lastX = currX;
                tmpUnit->lastY = currY;
                tmpUnit->coorByCellX = exitX;
                tmpUnit->coorByCellY = exitY;

                if(!getIsometric()) {
                    if(exitX < currX && exitY < currY) {
                        tmpUnit->animationMaxIter = tmpUnit->defUnit->walk_up_left.size();
                        tmpUnit->activePixmaps = tmpUnit->defUnit->walk_up_left;
                        tmpUnit->direction = DirectionUpLeft;
                    } else if(exitX == currX && exitY < currY) {
                        tmpUnit->animationMaxIter = tmpUnit->defUnit->walk_up.size();
                        tmpUnit->activePixmaps = tmpUnit->defUnit->walk_up;
                        tmpUnit->direction = DirectionUp;
                    } else if(exitX > currX && exitY < currY) {
                        tmpUnit->animationMaxIter = tmpUnit->defUnit->walk_up_right.size();
                        tmpUnit->activePixmaps = tmpUnit->defUnit->walk_up_right;
                        tmpUnit->direction = DirectionUpRight;
                    } else if(exitX < currX && exitY == currY) {
                        tmpUnit->animationMaxIter = tmpUnit->defUnit->walk_left.size();
                        tmpUnit->activePixmaps = tmpUnit->defUnit->walk_left;
                        tmpUnit->direction = DirectionLeft;
                    } else if(exitX > currX && exitY == currY) {
                        tmpUnit->animationMaxIter = tmpUnit->defUnit->walk_right.size();
                        tmpUnit->activePixmaps = tmpUnit->defUnit->walk_right;
                        tmpUnit->direction = DirectionRight;
                    } else if(exitX < currX && exitY > currY) {
                        tmpUnit->animationMaxIter = tmpUnit->defUnit->walk_down_left.size();
                        tmpUnit->activePixmaps = tmpUnit->defUnit->walk_down_left;
                        tmpUnit->direction = DirectionDownLeft;
                    } else if(exitX == currX && exitY > currY) {
                        tmpUnit->animationMaxIter = tmpUnit->defUnit->walk_down.size();
                        tmpUnit->activePixmaps = tmpUnit->defUnit->walk_down;
                        tmpUnit->direction = DirectionDown;
                    } else if(exitX > currX && exitY > currY) {
                        tmpUnit->animationMaxIter = tmpUnit->defUnit->walk_down_right.size();
                        tmpUnit->activePixmaps = tmpUnit->defUnit->walk_down_right;
                        tmpUnit->direction = DirectionDownRight;
                    }
                } else {
                    if(exitX < currX && exitY < currY) {
                        tmpUnit->animationMaxIter = tmpUnit->defUnit->walk_up.size();
                        tmpUnit->activePixmaps = tmpUnit->defUnit->walk_up;
                        tmpUnit->direction = DirectionUp;
                    } else if(exitX == currX && exitY < currY) {
                        tmpUnit->animationMaxIter = tmpUnit->defUnit->walk_up_right.size();
                        tmpUnit->activePixmaps = tmpUnit->defUnit->walk_up_right;
                        tmpUnit->direction = DirectionUpRight;
                    } else if(exitX > currX && exitY < currY) {
                        tmpUnit->animationMaxIter = tmpUnit->defUnit->walk_right.size();
                        tmpUnit->activePixmaps = tmpUnit->defUnit->walk_right;
                        tmpUnit->direction = DirectionRight;
                    } else if(exitX < currX && exitY == currY) {
                        tmpUnit->animationMaxIter = tmpUnit->defUnit->walk_up_left.size();
                        tmpUnit->activePixmaps = tmpUnit->defUnit->walk_up_left;
                        tmpUnit->direction = DirectionUpLeft;
                    } else if(exitX > currX && exitY == currY) {
                        tmpUnit->animationMaxIter = tmpUnit->defUnit->walk_down_right.size();
                        tmpUnit->activePixmaps = tmpUnit->defUnit->walk_down_right;
                        tmpUnit->direction = DirectionDownRight;
                    } else if(exitX < currX && exitY > currY) {
                        tmpUnit->animationMaxIter = tmpUnit->defUnit->walk_left.size();
                        tmpUnit->activePixmaps = tmpUnit->defUnit->walk_left;
                        tmpUnit->direction = DirectionLeft;
                    } else if(exitX == currX && exitY > currY) {
                        tmpUnit->animationMaxIter = tmpUnit->defUnit->walk_down_left.size();
                        tmpUnit->activePixmaps = tmpUnit->defUnit->walk_down_left;
                        tmpUnit->direction = DirectionDownLeft;
                    } else if(exitX > currX && exitY > currY) {
                        tmpUnit->animationMaxIter = tmpUnit->defUnit->walk_down.size();
                        tmpUnit->activePixmaps = tmpUnit->defUnit->walk_down;
                        tmpUnit->direction = DirectionDown;
                    }
                }
//                qDebug() << "tmpUnit->animationMaxIter: " << tmpUnit << "->" << tmpUnit->animationMaxIter;
                tmpUnit->pixmap = tmpUnit->activePixmaps[0];
                tmpUnit->animationCurrIter = 0;
                getCell(exitX, exitY)->setUnit(tmpUnit);
            } else {
                tmpUnit->animationMaxIter = tmpUnit->defUnit->idle.size();
                tmpUnit->activePixmaps = tmpUnit->defUnit->idle;
                tmpUnit->direction = DirectionIdle;
                tmpUnit->pixmap = tmpUnit->activePixmaps[0];
            }
        }
    } else if(tmpUnit->preDeath) {
        if(tmpUnit->animationCurrIter < tmpUnit->animationMaxIter) {
//            qDebug() << "tmpUnit->animationCurrIter: " << tmpUnit << "->" << tmpUnit->animationCurrIter;
            tmpUnit->pixmap = tmpUnit->activePixmaps[tmpUnit->animationCurrIter++];
//            tmpUnit->animationCurrIter = tmpUnit->animationCurrIter+1;
        } else {
            tmpUnit->preDeath = false;
        }
    } else {
        return -2;
    }
    return 0;
}

int Field::getNumStep(int x, int y)
{
    if(x >= 0 && x < getSizeX())
        if(y >= 0 && y < getSizeY())
            if(!getCell(x, y)->isTerrain())
                if(!getCell(x, y)->getTower())
                    return getStepCell(x, y);

    return 0;
}

int Field::getStepCell(int x, int y)
{
    return field[sizeX*y + x].unitStepWA;
}

bool Field::setNumOfCell(int x, int y, int step)
{
    if(x >= 0 && x < getSizeX())
        if(y >= 0 && y < getSizeY())
            if(!getCell(x, y)->isTerrain() && !getCell(x, y)->getTower())
                if(getStepCell(x, y) > step || getStepCell(x, y) == 0)
                {
                    setStepCell(x, y, step);
                    return true;
                }

    return false;
}

void Field::setStepCell(int x, int y, int step)
{
    field[sizeX*y + x].unitStepWA = step;
}

void Field::clearStepCell(int x, int y)
{
    field[sizeX*y + x].unitStepWA = 0;
}

//Unit* Field::getUnit(int x, int y)
//{
//    if(!field[sizeX*y + x].units.empty())
//        return field[sizeX*y + x].units.front();
//    else
//        return NULL;
////    return field[sizeX*y + x].unit;
//}

//std::vector<Unit*> Field::getUnits(int x, int y)
//{
////    if(!field[sizeX*y + x].units.empty())
//        return field[sizeX*y + x].units;
////    else
////        return NULL;
//}

int Field::getUnitHpInCell(int x, int y)
{
    if(x >= 0 && x < getSizeX())
        if(y >= 0 && y < getSizeY())
            if(containUnit(x,y))
                return unitsManager.getHP(x, y);

    return 0;
}

Unit* Field::getUnitWithLowHP(int x, int y)
{
    if(x >= 0 && x < getSizeX())
        if(y >= 0 && y < getSizeY())
            if(!field[sizeX*y + x].units.empty())
            {
                Unit* unit = field[sizeX*y + x].units.front();
                int localHp = unit->hp;
                int size = field[sizeX*y + x].units.size();
                for(int k = 1; k < size; k++)
                {
                    int hp = field[sizeX*y + x].units[k]->hp;
                    if(hp < localHp)
                    {
                        unit = field[sizeX*y + x].units[k];
                        localHp = unit->hp;
                    }
                }
                return unit;
            }
    return NULL;
}

std::vector<Tower*> Field::getAllTowers()
{
    std::vector<Tower*> exitTowers;

    for(int k = 0; k < towersManager.getAmount(); k++)
        exitTowers.push_back(towersManager.getTowerById(k));

    return exitTowers;
}

//bool Field::containEmpty(int x, int y) {
//    return field[sizeX*y + x].empty;
//}

//bool Field::isTerrain(int x, int y) {
//    return field[sizeX*y + x].isTerrain();
//}

bool Field::containTower(int x, int y)
{
//    return (field[sizeX*y + x].tower != NULL);
    return field[sizeX*y + x].tower;
}

int Field::containUnit(int x, int y, Unit *unit)
{
    if(!field[sizeX*y + x].units.empty())
    {
        int size = field[sizeX*y + x].units.size();
        if(unit == NULL)
            return size;
        else
            for(int k = 0; k < size; k++)
                if(field[sizeX*y + x].units[k] == unit)
                    return k+1;
    }
//    if(field[sizeX*y + x].unit != NULL)
//        if(field[sizeX*y + x].unit->alive)
//            return true;
    return 0;
}

bool Field::setTower(int x, int y)//, int type)
{
    if(field[sizeX*y + x].empty)
    {
        if(!isSetExitPoint(x, y) && !isSetSpawnPoint(x, y))
        {
            field[sizeX*y + x].tower = towersManager.createTower(x, y);
            field[sizeX*y + x].empty = false;
            return true;
        }
    }
    return false;
}

bool Field::setTower(int x, int y, DefaultTower* defTower)
{
    int size = defTower->size;
    for(int tmpX = 0; tmpX < size; tmpX++)
        for(int tmpY = 0; tmpY < size; tmpY++)
            if(!this->getCell(tmpX+x, tmpY+y)->isEmpty())
                return false;

    Tower* tower = towersManager.createTower(x, y, defTower);
    if(tower != NULL)
    {
        for(int tmpX = 0; tmpX < size; tmpX++)
            for(int tmpY = 0; tmpY < size; tmpY++)
//                if(containEmpty(tmpX+x, tmpY+y))
                {
//                    if(!isSetExitPoint(x, y) && !isSetSpawnPoint(x, y)) // BAGS!!!!!!!!!!!!!
//                    {
                        field[sizeX*(tmpY+y) + (tmpX+x)].tower = tower;
                        field[sizeX*(tmpY+y) + (tmpX+x)].empty = false;
//                        return true;
//                    }
                }
//                else
//                {
//                    towers.deleteTower(x, y);
//                    return false;
//                }
        return true;
    }
    return false;
}

bool Field::spawnHeroInSpawnPoint() { //Unit* unit, int type)
    getCell(spawnPointX, spawnPointY)->removeTerrain();
    return createUnit(spawnPointX, spawnPointY, 0); // type 0 = hero
}

Unit* Field::createUnit(int x, int y, int type) {
//    if(x == -1 && y == -1)
//        return createUnit(spawnPointX, spawnPointY, unit);//, type);
    qDebug() << "Field::createUnit(" << x << ", " << y << ", " << type << "); -- ";
    int coorByMapX, coorByMapY;
    if(!getIsometric()) {
        coorByMapX = mainCoorMapX + spaceWidget + x*sizeCell;
        coorByMapY = mainCoorMapY + spaceWidget + y*sizeCell;
    } else {
        int halfSizeCellX = sizeCell/2;
        int halfSizeCellY = halfSizeCellX/2;
        int isometricCoorX = halfSizeCellX*getSizeY();
        int isometricCoorY = halfSizeCellY*y;
        coorByMapX = mainCoorMapX + isometricCoorX+spaceWidget - halfSizeCellX + x*halfSizeCellX;
        coorByMapY = mainCoorMapY + isometricCoorY+spaceWidget - halfSizeCellY*2 + x*halfSizeCellY;
    }
    Unit* unit;
    if (type == 0) {
        unit = unitsManager.createUnit(x, y, coorByMapX, coorByMapY, faction1->getDefaultUnitById(1), type);
        updateHeroDestinationPoint(exitPointX, exitPointY);
    } else /*if (type == 1)*/ {
        unit = unitsManager.createUnit(x, y, coorByMapX, coorByMapY, faction1->getDefaultUnitById((2+(rand()%(faction1->units.size()-2)))), type);
        if (unit != NULL) {
            int randomX = rand()%sizeX;
            int randomY = rand()%sizeY;
            unit->path = pathFinder.findPath({unit->coorByCellX, unit->coorByCellY}, {randomX, randomY});
        }
    }
    if(unit == NULL) {
        return false;
    }
    getCell(x, y)->setUnit(unit);
}

//bool Field::clearBusy(int x, int y)
//{
//    if(!field[sizeX*y + x].empty)
//    {
//        if(isTerrain(x,y))
//        {
//            field[sizeX*y + x].busy = false;
//            field[sizeX*y + x].empty = true;
//            return true;
//        }
//    }

//    return false;
//}

//bool Field::clearTower(int x, int y)
//{
//    if(!containEmpty(x, y))
//    {
//        if(containTower(x,y))
//        {
//            field[sizeX*y + x].tower = NULL;
//            field[sizeX*y + x].empty = true;
//            return true;
//        }
//    }
//    return false;
//}

//bool Field::clearUnit(int x, int y, Unit *unit)
//{
//    if(!field[sizeX*y + x].empty)
//    {
//        if(unit == NULL)
//            field[sizeX*y + x].units.clear();
//        else if(int num = containUnit(x, y, unit))
//        {
//            field[sizeX*y + x].units.erase(field[sizeX*y + x].units.begin()+(num-1));
////            field[sizeX*y + x].unit = NULL;
//        }

//        if(field[sizeX*y + x].units.empty())
//            field[sizeX*y + x].empty = true;

//        return true;
//    }
//    return false;
//}

bool Field::deleteTower(int x, int y) {
    Tower* tower = towersManager.getTower(x, y);
    if (tower != NULL) {
        int towerX = tower->currX;
        int towerY = tower->currY;
        int size = tower->defTower->size;
        towersManager.deleteTower(towerX, towerY);
        for (int tmpX = 0; tmpX < size; tmpX++) {
            for (int tmpY = 0; tmpY < size; tmpY++) {
                this->getCell(tmpX+towerX, tmpY+towerY)->removeTower();
            }
        }
        return true;
    }
    return false;
}

//void Field::setPixmapInCell(int x, int y, QPixmap pixmap)
//{
//    field[sizeX*y + x].backgroundPixmap = pixmap;
//}

//void Field::setPixmapForUnit(QPixmap pixmap)
//{
//    unitsManager.setDefaulPixmapForUnit(pixmap);
//}

//void Field::setPixmapForTower(QPixmap pixmap)
//{
//    towersManager.setDefaulPixmapForTower(pixmap);
//}

//QPixmap Field::getBusyPixmapOfCell(int x, int y)
//{
//    return field[sizeX*y + x].busyPixmap;
//}

//QPixmap Field::getPixmapOfCell(int x, int y)
//{
//    return field[sizeX*y + x].backgroundPixmap;
//}

//QPixmap Field::getUnitPixmap(int x, int y)
//{
//    return unitsManager.getUnitPixmap(x, y);
//}

//QPixmap Field::getTowerPixmap(int x, int y)
//{
//    return towersManager.getTowerPixmap(x, y);
//}
