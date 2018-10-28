#include "src/head/field.h"

Field::Field(QString mapFile, FactionsManager* factionsManager, int enemyCount, int difficultyLevel, int towersCount) {
    qDebug() << "Field::Field(); -- mapPath:" << mapFile;
    qDebug() << "Field::Field(); -- enemyCount:" << enemyCount;
    qDebug() << "Field::Field(); -- towersCount:" << towersCount;
    this->factionsManager = factionsManager;
    this->towersManager = new TowersManager(difficultyLevel);
    this->unitsManager = new UnitsManager(difficultyLevel);
    qDebug() << "Field::Field(); -- map:" << map;
    this->map = (new MapLoader())->load(mapFile);

    sizeFieldX = map->properties.value("width").toInt();// map->width;
    sizeFieldY = map->properties.value("height").toInt();// map->height;
    sizeCellX = map->properties.value("tilewidth").toInt();
    sizeCellY = map->properties.value("tileheight").toInt();
    halfSizeCellX = sizeCellX/2;
    halfSizeCellY = sizeCellY/2;

//    underConstruction = null;
//    green;
//    red?
    createField(sizeFieldX, sizeFieldY, map->getMapLayers());
    qDebug() << "Field::Field(); -- map:" << map;
    // camera 1
    mainCoorMapX = 0, mainCoorMapY = 0;
    spaceWidget = 0; // fix this. 16 and launch
    // camera 2
    mouseX = -1;
    mouseY = -1;
    spawnPointX = -1;
    spawnPointY = -1;
    exitPointX = -1;
    exitPointY = -1;

    gameOverLimitUnits = 10;
    currentFinishedUnits = 0;
    qDebug() << "Field::Field(); -end- ";
}

Field::~Field() {
    qDebug() << "Field::~Field(); -- ";
}

void Field::createField(int sizeFieldX, int sizeFieldY, MapLayers *mapLayers) {
    qDebug() << "Field::createField(); -- sizeFieldX:" << sizeFieldX << " sizeFieldY:" << sizeFieldY << " mapLayers:" << mapLayers;
    qDebug() << "Field::createField(); -1- field:" << field;
//    if(field == NULL) {
        isometric = false;
        field = new Cell[sizeFieldX*sizeFieldY];
        for (int y = 0; y < sizeFieldY; y++) {
            for (int x = 0; x < sizeFieldX; x++) {
//                field[sizeFieldX*y + x] = new Cell();
                field[sizeFieldX*y + x].setGraphicCoordinates(x, y, halfSizeCellX, halfSizeCellY);
                for (Layer* layer : mapLayers->layers) {
//                    if (layer instanceof TiledMapTileLayer) {
//                        TiledMapTileLayer layer = (TiledMapTileLayer) layer;
                        TileLayerCell* cell = layer->getCell(x, y);
                        if (cell != NULL) {
                            Tile* tile = cell->getTile();
                            if (tile != NULL) {
                                if(!layer->getProperties()->contains("background")) {
                                    field[sizeFieldX*y + x].foregroundTiles.push_back(tile->getPixmap());
                                } else {
                                    field[sizeFieldX*y + x].backgroundTiles.push_back(tile->getPixmap());
                                }
                                if (tile->getProperties()->contains("terrain")) {
                                    field[sizeFieldX*y + x].setTerrain();
                                } else if (tile->getProperties()->contains("spawnPoint")) {
//                                    spawnPoint = new GridPoint2(x, y);
//                                        waveManager.spawnPoints.add(new GridPoint2(x, y));
//                                    field[x][y].setTerrain();
                                    qDebug() << "GameField::GameField(); -- Set spawnPoint!";//: (" + x + ", " + y + ")";
                                } else if (tile->getProperties()->contains("exitPoint")) {
//                                    exitPoint = new GridPoint2(x, y);
//                                        waveManager.exitPoints.add(new GridPoint2(x, y));
//                                    field[x][y].setTerrain();
                                    qDebug() << "GameField::GameField(); -- Set exitPoint!";//: (" + x + ", " + y + ")");
                                }
//                                // task 6. отрисовка деревьев полностью
//                                if(tile.getProperties().get("treeName") != null) {
//                                    String treeName = tile.getProperties().get("treeName");
//                                    int treeWidth = Integer.parseInt(tile.getProperties().get("treeWidth", "1"));
//                                    int treeHeight = Integer.parseInt(tile.getProperties().get("treeHeight", "1"));
//                                    Gdx.app.log("GameField::createField()", "-- New Tree:" + treeName + "[" + treeWidth + "," + treeHeight + "]:{" + x + "," + y + "}");
//                                    float regionX = tile.getTextureRegion().getRegionX();
//                                    float regionY = tile.getTextureRegion().getRegionY();
//                                    float regionWidth = tile.getTextureRegion().getRegionWidth();
//                                    float regionHeight = tile.getTextureRegion().getRegionWidth();
//                                    Gdx.app.log("GameField::createField()", "-- regionX:" + regionX + " regionY:" + regionY + " regionWidth:" + regionWidth + " regionHeight:" + regionHeight);
//                                    TextureRegion textureRegion = new TextureRegion(tile.getTextureRegion());
//                                    textureRegion.setRegion(regionX - ((treeWidth>2) ? (treeWidth-2)*regionWidth : 0), regionY - ((treeHeight>1) ? (treeHeight-1)*regionHeight : 0), treeWidth*regionWidth, treeHeight*regionHeight);
////                                        Cell.Tree tree = new Cell.Tree(textureRegion, treeWidth, treeHeight);
//                                }
                            }
                        }
//                    } else {
//                        Gdx.app.log("GameField::createField()", "-- Не смог преобразовать MapLayer в TiledMapTileLayer");
//                    }
                }
            }
        }
//    }
    qDebug() << "Field::createField(); -2- field:" << field;
    qDebug() << "Field::createField(); -- pathFinder:" << &pathFinder;
//    updatePathFinderWalls();
}

void Field::deleteField() {
    if(field != NULL) {
        delete[] field;
        field = NULL;
//        towersManager->deleteField();
//        unitsManager->deleteMass();
        delete towersManager;
        delete unitsManager;
//        delete faction;
    }
}

Cell* Field::getCell(int x, int y) {
//    qDebug() << "Field::getCell(); -- x:" << x << " y:" << y;
//    qDebug() << "Field::getCell(); -- sizeFieldX:" << sizeFieldX << " sizeFieldY:" << sizeFieldX;
    if (x >= 0 && x < sizeFieldX) {
        if (y >= 0 && y < sizeFieldY) {
            return &field[sizeFieldX*y + x];
        }
    }
    return NULL;
}

//void Field::setFaction(Faction* faction) {
//    this->faction = faction;
//}

bool Field::createSpawnPoint(int num, int x, int y) {
//    foreach (Unit* unit, unitsManager->units) {
//        int unitX = unit->coorByCellX;
//        int unitY = unit->coorByCellY;
//        getCell(unitX, unitY)->clearUnit();
//    }
//    delete unitsManager;
//    unitsManager = new UnitsManager(1);
    if(x == -1 && y == -1) {
        if(!isSetSpawnPoint()) {
            return false;
        }
    } else {
        spawnPointX = x;
        spawnPointY = y;
//        getCell(x, y)->spawn = true;
        getCell(x, y)->removeTerrain(true);
    }
    currentFinishedUnits = 0;
    return true;
}

void Field::createExitPoint(int x, int y) {
    exitPointX = x;
    exitPointY = y;
//    getCell(x, y)->exit = true;
    getCell(x, y)->removeTerrain(true);
//    waveAlgorithm(x, y);
}

void Field::updateHeroDestinationPoint() {
    qDebug() << "Field::updateHeroDestinationPoint(); -- ";
    Unit* hero = unitsManager->hero;
    qDebug() << "Field::updateHeroDestinationPoint(); -- hero:" << hero;
    if (hero != NULL && !hero->path.empty()) {
        updateHeroDestinationPoint(hero->path.front().x, hero->path.front().y);
    } else {
        updatePathFinderWalls();
    }
}

void Field::updateHeroDestinationPoint(int x, int y) {
    updatePathFinderWalls();
//    foreach (Unit* unit, unitsManager->units) {
//        if (unit->type == 0) {
    Unit* unit = unitsManager->hero;
            qDebug() << "Field::updateHeroDestionPoint(" << x << ", " << y << "); -- ";
            AStar::CoordinateList newPath = pathFinder.findPath({unit->coorByCellX, unit->coorByCellY}, {x, y});
            if (newPath.front().operator==({x, y})) {
                newPath.pop_back();
                unit->path = newPath;
            } else {
                unit->path.clear();
            }
//        }
//    }
}

void Field::updatePathFinderWalls() {
    qDebug() << "Field::updatePathFinderWalls(); -1- ";
    pathFinder.clearCollisions();
//    qDebug() << "Field::updatePathFinderWalls(); -2- ";
    for (int x = 0; x < sizeFieldX; x++) {
//        qDebug() << "Field::updatePathFinderWalls(); -3- ";
        for (int y = 0; y < sizeFieldY; y++) {
//            qDebug() << "Field::updatePathFinderWalls(); -4- ";
            Cell* cell = getCell(x, y);
            qDebug() << "Field::updatePathFinderWalls(); -4- cell:" << cell;
            if (cell->isTerrain() || cell->getTower() != NULL) {
//                qDebug() << "Field::updatePathFinderWalls(); -5- ";
                pathFinder.addCollision({x, y});
//                qDebug() << "Field::updatePathFinderWalls(); -6- ";
            }
//            qDebug() << "Field::updatePathFinderWalls(); -7- ";
        }
//        qDebug() << "Field::updatePathFinderWalls(); -8- ";
    }
    qDebug() << "Field::updatePathFinderWalls(); -end- ";
}

int Field::getSizeX() {
    return sizeFieldX;
}

int Field::getSizeY() {
    return sizeFieldY;
}

void Field::setMainCoorMap(int mainCoorMapX, int mainCoorMapY) {
    this->mainCoorMapX = mainCoorMapX;
    this->mainCoorMapY = mainCoorMapY;
}

void Field::setSizeCell(int sizeCell) {
    this->sizeCellX = sizeCell;
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
    return sizeCellX;
}

void Field::setIsometric(bool isometric) {
    this->isometric = isometric;
}

//void Field::setTileMapSize(int tileMapWidth, int tileMapHeight) {
//    this->tileMapWidth = tileMapWidth;
//    this->tileMapHeight = tileMapHeight;
//}

bool Field::getIsometric() {
    return isometric;
}

//int Field::getTileMapWidth() {
//    return tileMapWidth;
//}

//int Field::getTileMapHeight() {
//    return tileMapHeight;
//}

bool Field::towersAttack(int deltaTime) {
    foreach (Tower* tmpTower, towersManager->towers) {
        if (tmpTower->recharge(deltaTime)) {
            tmpTower->createBullets(towersManager->difficultyLevel);
        }
        for (int b = 0; b < tmpTower->bullets.size(); b++) {
            Bullet* tmpBullet = tmpTower->bullets[b];
            int currX = tmpBullet->currCellX;
            int currY = tmpBullet->currCellY;
            if (currX < 0 || currX >= sizeFieldX || currY < 0 || currY >= sizeFieldY) {
                tmpTower->bullets.erase(tmpTower->bullets.begin()+b);
                delete tmpBullet;
                b--;
            } else {
                if (getCell(currX, currY)->getHero() != NULL) {
                    unitsManager->attackUnit(currX, currY, 9999);//, getCell(currX, currY)->getHero()); // Magic number 9999
                }
            }
            if(tmpBullet->animationCurrIter < tmpBullet->animationMaxIter) {
                tmpBullet->pixmap = tmpBullet->activePixmaps[tmpBullet->animationCurrIter++];
            } else {
                int exitX = currX, exitY = currY;
                if (tmpBullet->direction == Direction::type::UP) {
                    exitX = currX-1;
                    exitY = currY-1;
                } else if (tmpBullet->direction == Direction::UP_RIGHT) {
                    exitX = currX;
                    exitY = currY-1;
                } else if (tmpBullet->direction == Direction::RIGHT) {
                    exitX = currX+1;
                    exitY = currY-1;
                } else if (tmpBullet->direction == Direction::UP_LEFT) {
                    exitX = currX-1;
                    exitY = currY;
                } else if (tmpBullet->direction == Direction::DOWN_RIGHT) {
                    exitX = currX+1;
                    exitY = currY;
                } else if (tmpBullet->direction == Direction::LEFT) {
                    exitX = currX-1;
                    exitY = currY+1;
                } else if (tmpBullet->direction == Direction::DOWN_LEFT) {
                    exitX = currX;
                    exitY = currY+1;
                } else if (tmpBullet->direction == Direction::DOWN) {
                    exitX = currX+1;
                    exitY = currY+1;
                }
                if(exitX != currX || exitY != currY) {
                    tmpBullet->lastCellX = currX;
                    tmpBullet->lastCellY = currY;
                    tmpBullet->currCellX = exitX;
                    tmpBullet->currCellY = exitY;
                    if(getIsometric()) {
                        if(exitX < currX && exitY < currY) {
                            tmpBullet->animationMaxIter = tmpBullet->defTower->bullet_fly_up.size();
                            tmpBullet->activePixmaps = tmpBullet->defTower->bullet_fly_up;
                            tmpBullet->direction = Direction::UP;
                        } else if(exitX == currX && exitY < currY) {
                            tmpBullet->animationMaxIter = tmpBullet->defTower->bullet_fly_up_right.size();
                            tmpBullet->activePixmaps = tmpBullet->defTower->bullet_fly_up_right;
                            tmpBullet->direction = Direction::UP_RIGHT;
                        } else if(exitX > currX && exitY < currY) {
                            tmpBullet->animationMaxIter = tmpBullet->defTower->bullet_fly_right.size();
                            tmpBullet->activePixmaps = tmpBullet->defTower->bullet_fly_right;
                            tmpBullet->direction = Direction::RIGHT;
                        } else if(exitX < currX && exitY == currY) {
                            tmpBullet->animationMaxIter = tmpBullet->defTower->bullet_fly_up_left.size();
                            tmpBullet->activePixmaps = tmpBullet->defTower->bullet_fly_up_left;
                            tmpBullet->direction = Direction::UP_LEFT;
                        } else if(exitX > currX && exitY == currY) {
                            tmpBullet->animationMaxIter = tmpBullet->defTower->bullet_fly_down_right.size();
                            tmpBullet->activePixmaps = tmpBullet->defTower->bullet_fly_down_right;
                            tmpBullet->direction = Direction::DOWN_RIGHT;
                        } else if(exitX < currX && exitY > currY) {
                            tmpBullet->animationMaxIter = tmpBullet->defTower->bullet_fly_left.size();
                            tmpBullet->activePixmaps = tmpBullet->defTower->bullet_fly_left;
                            tmpBullet->direction = Direction::LEFT;
                        } else if(exitX == currX && exitY > currY) {
                            tmpBullet->animationMaxIter = tmpBullet->defTower->bullet_fly_down_left.size();
                            tmpBullet->activePixmaps = tmpBullet->defTower->bullet_fly_down_left;
                            tmpBullet->direction = Direction::DOWN_LEFT;
                        } else if(exitX > currX && exitY > currY) {
                            tmpBullet->animationMaxIter = tmpBullet->defTower->bullet_fly_down.size();
                            tmpBullet->activePixmaps = tmpBullet->defTower->bullet_fly_down;
                            tmpBullet->direction = Direction::DOWN;
                        }
                    }
                    if (tmpBullet->activePixmaps.empty() && !tmpBullet->defTower->bullet.empty()) {
                        tmpBullet->animationMaxIter = tmpBullet->defTower->bullet.size();
                        tmpBullet->activePixmaps = tmpBullet->defTower->bullet;
                    }
                    tmpBullet->pixmap = tmpBullet->activePixmaps[0];
                    tmpBullet->animationCurrIter = 0;
                }
            }
        }
    }
    return true;
}

void Field::setMousePress(int x, int y) {
    mouseX = x;
    mouseY = y;
}

bool Field::getMousePress(int x, int y) {
    if(mouseX != -1 && mouseY != -1)
        if((x == mouseX && y == mouseY) || (x == -1 && y == -1))
            return true;
    return false;
}

bool Field::isSetSpawnPoint(int x, int y) {
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
    bool allDead = true;
//    for(int k = 0; k < unitsManager->getAmount(); k++) {
    foreach (Unit* unit, unitsManager->units) {
        int result = stepOneUnit(unit);
//        int result = stepOneUnit(k);
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
    if(allDead) {
        return 2;
    } else {
        return 0;
    }
}

int Field::stepOneUnit(Unit* unit) {
//    Unit* tmpUnit = unitsManager->getUnit(num);
    if(unit->alive) {
        if(unit->animationCurrIter < unit->animationMaxIter) {
            unit->pixmap = unit->activePixmaps[unit->animationCurrIter++];
        } else {
            int currX = unit->coorByCellX;
            int currY = unit->coorByCellY;
            int exitX = currX, exitY = currY;
            if (!unit->path.empty()) {
                AStar::Vec2i point = unit->path.back();
                unit->path.pop_back();
                exitX = point.x;
                exitY = point.y;

            }
            if(unit == unitsManager->hero) {
                if (currX == exitPointX && currY == exitPointY) {
                    qDebug() << "Field::stepOneUnit(); -- HeroInExitPoint!";
                    return 3;
                }
            } else /*if (tmpUnit->type != 0)*/ { // Not hero!
                if (getCell(currX, currY)->getHero() != NULL) {
                    qDebug() << "Field::stepOneUnit(); -- Hero contact with Enemy!";
                    unitsManager->attackUnit(currX, currY, 9999);//, getCell(currX, currY)->getHero()); // Magic number 9999

                } else if (getCell(exitX, exitY)->getHero() != NULL) {
                    qDebug() << "Field::stepOneUnit(); -- Hero contact with Enemy!";
                    unitsManager->attackUnit(exitX, exitY, 9999);//, getCell(exitX, exitY)->getHero()); // Magic number 9999

                }

                if (getCell(currX, currY)->isTerrain()) {
                    getCell(currX, currY)->removeTerrain(true);
                    updatePathFinderWalls();
                }
                if (unit->path.empty() || getCell(exitX, exitY)->isTerrain()) {
                    int randomX = rand()%sizeFieldX;
                    int randomY = rand()%sizeFieldY;
                    unit->path = pathFinder.findPath({unit->coorByCellX, unit->coorByCellY}, {randomX, randomY});
                }
            }
            if(exitX != currX || exitY != currY) {
                getCell(currX, currY)->clearUnit(unit);
                unit->lastX = currX;
                unit->lastY = currY;
                unit->coorByCellX = exitX;
                unit->coorByCellY = exitY;
                if(!getIsometric()) {
                    if(exitX < currX && exitY < currY) {
                        unit->animationMaxIter = unit->templateForUnit->walk_up_left.size();
                        unit->activePixmaps = unit->templateForUnit->walk_up_left;
                        unit->direction = Direction::type::UP_LEFT;
                    } else if(exitX == currX && exitY < currY) {
                        unit->animationMaxIter = unit->templateForUnit->walk_up.size();
                        unit->activePixmaps = unit->templateForUnit->walk_up;
                        unit->direction = Direction::UP;
                    } else if(exitX > currX && exitY < currY) {
                        unit->animationMaxIter = unit->templateForUnit->walk_up_right.size();
                        unit->activePixmaps = unit->templateForUnit->walk_up_right;
                        unit->direction = Direction::UP_RIGHT;
                    } else if(exitX < currX && exitY == currY) {
                        unit->animationMaxIter = unit->templateForUnit->walk_left.size();
                        unit->activePixmaps = unit->templateForUnit->walk_left;
                        unit->direction = Direction::LEFT;
                    } else if(exitX > currX && exitY == currY) {
                        unit->animationMaxIter = unit->templateForUnit->walk_right.size();
                        unit->activePixmaps = unit->templateForUnit->walk_right;
                        unit->direction = Direction::RIGHT;
                    } else if(exitX < currX && exitY > currY) {
                        unit->animationMaxIter = unit->templateForUnit->walk_down_left.size();
                        unit->activePixmaps = unit->templateForUnit->walk_down_left;
                        unit->direction = Direction::DOWN_LEFT;
                    } else if(exitX == currX && exitY > currY) {
                        unit->animationMaxIter = unit->templateForUnit->walk_down.size();
                        unit->activePixmaps = unit->templateForUnit->walk_down;
                        unit->direction = Direction::DOWN;
                    } else if(exitX > currX && exitY > currY) {
                        unit->animationMaxIter = unit->templateForUnit->walk_down_right.size();
                        unit->activePixmaps = unit->templateForUnit->walk_down_right;
                        unit->direction = Direction::DOWN_RIGHT;
                    }
                } else {
                    if(exitX < currX && exitY < currY) {
                        unit->animationMaxIter = unit->templateForUnit->walk_up.size();
                        unit->activePixmaps = unit->templateForUnit->walk_up;
                        unit->direction = Direction::UP;
                    } else if(exitX == currX && exitY < currY) {
                        unit->animationMaxIter = unit->templateForUnit->walk_up_right.size();
                        unit->activePixmaps = unit->templateForUnit->walk_up_right;
                        unit->direction = Direction::UP_RIGHT;
                    } else if(exitX > currX && exitY < currY) {
                        unit->animationMaxIter = unit->templateForUnit->walk_right.size();
                        unit->activePixmaps = unit->templateForUnit->walk_right;
                        unit->direction = Direction::RIGHT;
                    } else if(exitX < currX && exitY == currY) {
                        unit->animationMaxIter = unit->templateForUnit->walk_up_left.size();
                        unit->activePixmaps = unit->templateForUnit->walk_up_left;
                        unit->direction = Direction::UP_LEFT;
                    } else if(exitX > currX && exitY == currY) {
                        unit->animationMaxIter = unit->templateForUnit->walk_down_right.size();
                        unit->activePixmaps = unit->templateForUnit->walk_down_right;
                        unit->direction = Direction::DOWN_RIGHT;
                    } else if(exitX < currX && exitY > currY) {
                        unit->animationMaxIter = unit->templateForUnit->walk_left.size();
                        unit->activePixmaps = unit->templateForUnit->walk_left;
                        unit->direction = Direction::LEFT;
                    } else if(exitX == currX && exitY > currY) {
                        unit->animationMaxIter = unit->templateForUnit->walk_down_left.size();
                        unit->activePixmaps = unit->templateForUnit->walk_down_left;
                        unit->direction = Direction::DOWN_LEFT;
                    } else if(exitX > currX && exitY > currY) {
                        unit->animationMaxIter = unit->templateForUnit->walk_down.size();
                        unit->activePixmaps = unit->templateForUnit->walk_down;
                        unit->direction = Direction::DOWN;
                    }
                }
                unit->pixmap = unit->activePixmaps[0];
                unit->animationCurrIter = 0;
                getCell(exitX, exitY)->setUnit(unit);
            } else {
                unit->animationMaxIter = unit->templateForUnit->idle.size();
                unit->activePixmaps = unit->templateForUnit->idle;
                unit->direction = Direction::IDLE;
                unit->pixmap = unit->activePixmaps[0];
            }
        }
    } else if(unit->preDeath) {
        if(unit->animationCurrIter < unit->animationMaxIter) {
            unit->pixmap = unit->activePixmaps[unit->animationCurrIter++];
        } else {
            unit->preDeath = false;
            return 4;
        }
    } else {
        return -2;
    }
    return 0;
}

//int Field::getUnitHpInCell(int x, int y) {
//    if(x >= 0 && x < getSizeX())
//        if(y >= 0 && y < getSizeY())
//            if(containUnit(x,y))
//                return unitsManager->getHP(x, y);
//    return 0;
//}

//Unit* Field::getUnitWithLowHP(int x, int y) {
//    if(x >= 0 && x < getSizeX()) {
//        if(y >= 0 && y < getSizeY()) {
//            if(!field[sizeFieldX*y + x].units.empty()) {
//                Unit* unit = field[sizeFieldX*y + x].units.front();
//                int localHp = unit->hp;
//                int size = field[sizeFieldX*y + x].units.size();
//                for(int k = 1; k < size; k++) {
//                    int hp = field[sizeFieldX*y + x].units[k]->hp;
//                    if(hp < localHp) {
//                        unit = field[sizeFieldX*y + x].units[k];
//                        localHp = unit->hp;
//                    }
//                }
//                return unit;
//            }
//        }
//    }
//    return NULL;
//}

int Field::containUnit(int x, int y, Unit *unit) {
    if(!field[sizeFieldX*y + x].units.empty()) {
        int size = field[sizeFieldX*y + x].units.size();
        if(unit == NULL) {
            return size;
        } else {
            for(int k = 0; k < size; k++) {
                if(field[sizeFieldX*y + x].units[k] == unit) {
                    return k+1;
                }
            }
        }
    }
    return 0;
}

bool Field::setTower(int x, int y, TemplateForTower* defTower) {
    int size = defTower->size;
    for(int tmpX = 0; tmpX < size; tmpX++)
        for(int tmpY = 0; tmpY < size; tmpY++)
            if(!this->getCell(tmpX+x, tmpY+y)->isEmpty())
                return false;

    Tower* tower = towersManager->createTower(x, y, defTower);
    if(tower != NULL) {
        for(int tmpX = 0; tmpX < size; tmpX++) {
            for(int tmpY = 0; tmpY < size; tmpY++) {
                Cell* cell = getCell(tmpX+x, tmpY+y);
                if (cell->isEmpty() && !cell->spawn && !cell->exit) {
                    cell->setTower(tower);
                }
            }
        }
        return true;
    }
    return false;
}

bool Field::spawnHeroInSpawnPoint() { //Unit* unit, int type)
    getCell(spawnPointX, spawnPointY)->removeTerrain(true);
    deleteTower(spawnPointX, spawnPointY);
    return createUnit(spawnPointX, spawnPointY, 0); // type 0 = hero
}

Unit* Field::createUnit(int x, int y, int type) {
    int coorByMapX, coorByMapY;
    if(!getIsometric()) {
        coorByMapX = mainCoorMapX + spaceWidget + x*sizeCellX;
        coorByMapY = mainCoorMapY + spaceWidget + y*sizeCellX;
    } else {
        int halfSizeCellX = sizeCellX/2;
        int halfSizeCellY = halfSizeCellX/2;
        int isometricCoorX = halfSizeCellX*getSizeY();
        int isometricCoorY = halfSizeCellY*y;
        coorByMapX = mainCoorMapX + isometricCoorX+spaceWidget - halfSizeCellX + x*halfSizeCellX;
        coorByMapY = mainCoorMapY + isometricCoorY+spaceWidget - halfSizeCellY*2 + x*halfSizeCellY;
    }
    Unit* unit;
    AStar::CoordinateList path = pathFinder.findPath({x, y}, {exitPointX, exitPointY});
    if (type == 0) {
        unit = unitsManager->createHero(path, factionsManager->getTemplateForUnitByName("unit3_footman"));//faction->getDefaultUnitById(0)); //, type);
        updateHeroDestinationPoint(exitPointX, exitPointY);
    } else /*if (type == 1)*/ {
        unit = unitsManager->createUnit(path, factionsManager->getRandomTemplateForUnitFromFirstFaction(), type);
        if (unit != NULL) { //
            int randomX = rand()%sizeFieldX;
            int randomY = rand()%sizeFieldY;
            unit->path = pathFinder.findPath({unit->coorByCellX, unit->coorByCellY}, {randomX, randomY});
        }
    }
    if(unit == NULL) {
        return false;
    }
    getCell(x, y)->setUnit(unit);
}

bool Field::deleteTower(int x, int y) {
    Tower* tower = towersManager->getTower(x, y);
    if (tower != NULL) {
        int towerX = tower->currX;
        int towerY = tower->currY;
        int size = tower->templateForTower->size;
        towersManager->deleteTower(towerX, towerY);
        for (int tmpX = 0; tmpX < size; tmpX++) {
            for (int tmpY = 0; tmpY < size; tmpY++) {
                this->getCell(tmpX+towerX, tmpY+towerY)->removeTower();
            }
        }
        return true;
    }
    return false;
}
