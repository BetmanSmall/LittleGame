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

    global_pixmap = map->tileSets.getTileSet(0)->getLocalTile(85)->getPixmap();
    global_pixmap_PathPoint = map->tileSets.getTileSet(0)->getLocalTile(7)->getPixmap();
    global_pixmap_EnemyPathPoint = map->tileSets.getTileSet(0)->getLocalTile(9)->getPixmap();
    global_pixmap_DestinationPoint = map->tileSets.getTileSet(0)->getLocalTile(19)->getPixmap();
    global_pixmap_ExitPoint = map->tileSets.getTileSet(0)->getLocalTile(18)->getPixmap();

//    underConstruction = null;
//    green;
//    red?
    createField(sizeFieldX, sizeFieldY, map->getMapLayers());
    int terrainType = rand()%2;
    if (mapFile.contains("randomMap")) {
        for (int x = 0; x < sizeFieldX; x++) {
            for (int y = 0; y < sizeFieldY; y++) {
                if( (rand()%100) < 30 ) {
                    int randNumber = map->getTileSets()->getTileSet(1)->firshgid+( 42+(rand()%4) );
                    QPixmap pixmap = map->getTileSets()->getTileSet(1)->getGlobalTile(randNumber)->getPixmap();
                    getCell(x, y)->setTerrain(pixmap);
                }
            }
        }
    } else {
        for (int x = 0; x < sizeFieldX; x++) {
            for (int y = 0; y < sizeFieldY; y++) {
                if( (rand()%100) < 10 ) {
                    if (getCell(x, y)->isEmpty()) {
                        int randNumber = ( 124+(rand()%2) );
                        QPixmap pixmap = map->getTileSets()->getTileSet(0)->getLocalTile(randNumber)->getPixmap();
                        getCell(x, y)->setTerrain(pixmap);
                    }
                }
            }
        }
    }
    qDebug() << "Field::Field(); -- map:" << map;
    // camera 1
    mainCoorMapX = 0, mainCoorMapY = 0;
    spaceWidget = 0; // fix this. 16 and launch
    // camera 2
    gamePause = false;
//    gameSpeed = 1.0;
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
        isometric = true;
        field = new Cell[sizeFieldX*sizeFieldY];
        for (int y = 0; y < sizeFieldY; y++) {
            for (int x = 0; x < sizeFieldX; x++) {
//                field[sizeFieldX*y + x] = new Cell();
                getCell(x, y)->setGraphicCoordinates(x, y, halfSizeCellX, halfSizeCellY);
                for (Layer* layer : mapLayers->layers) {
                        TileLayerCell* cell = layer->getCell(x, y);
                        if (cell != NULL) {
                            Tile* tile = cell->getTile();
                            if (tile != NULL) {
                                QString layerName = layer->getName();
                                if (layerName != NULL) {
                                    if (layerName == "background") {
                                        getCell(x, y)->backgroundTiles.push_back(tile->getPixmap());
                                    } else if (layerName == "ground") {
                                        getCell(x, y)->setTerrain(tile->getPixmap());
                                    } else if (layerName == "towers") {
                                        qDebug() << "Field::createField(); -- layerName:" << layerName;
                                        qDebug() << "Field::createField(); -- tile->getId():" << tile->getId();
                                        qDebug() << "Field::createField(); -6- tile->getProperties():" << tile->getProperties();
//                                        this->setTower(x, y, factionsManager->getRandomTemplateForTowerFromAllFaction());
                                    } else {
                                        getCell(x, y)->foregroundTiles.push_back(tile->getPixmap());
                                    }
                                }
                                if (tile->getProperties()->value("spawnPoint") != NULL) {
                                    qDebug() << "Field::createField(); -7- x:" << x << " y:" << y;
                                    qDebug() << "GameField::GameField(); -- Set spawnPoint!";//: (" + x + ", " + y + ")";
                                } else if (tile->getProperties()->value("exitPoint") != NULL) {
                                    qDebug() << "Field::createField(); -7- x:" << x << " y:" << y;
                                    qDebug() << "GameField::GameField(); -- Set exitPoint!";//: (" + x + ", " + y + ")");
                                }
//                                qDebug() << "Field::createField(); -7- ";
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
    qDebug() << "Field::updatePathFinderWalls(); -- ";
    pathFinder.clearCollisions();
    for (int x = 0; x < sizeFieldX; x++) {
        for (int y = 0; y < sizeFieldY; y++) {
            Cell* cell = getCell(x, y);
//            qDebug() << "Field::updatePathFinderWalls(); -4- cell:" << cell;
            if (cell->isTerrain() || cell->getTower() != NULL) {
//                qDebug() << "Field::updatePathFinderWalls(); -- pathFinder.addCollision";
                pathFinder.addCollision({x, y});
            }
        }
    }
    qDebug() << "Field::updatePathFinderWalls(); -end- ";
}

void Field::render(float deltaTime, QPainter* painter) {
//    qDebug() << "Field::render(); -- deltaTime:" << deltaTime;
//    if (gameStart) {
//        drawFullField(painter);
//        if(ui->drawBackGround_checkBox->isChecked())
            drawBackGround(painter);
//        if(ui->drawUnits_checkBox->isChecked())
//            drawUnits(painter);
//        if(ui->drawGround_checkBox->isChecked())
            drawGround(painter);
//        if(ui->drawTowersByTowers_checkBox->isChecked())
//            drawTowersByTowers(painter);
//        if(ui->drawForeGround_checkBox->isChecked())
            drawForeGround(painter);

//        if(ui->drawBlackTiles_checkBox->isChecked())
            drawBlackTiles(painter);
//        if(ui->drawPaths_checkBox->isChecked())
            drawPaths(painter);
//        if(ui->drawGrid_checkBox->isChecked())
            drawGrid(painter);
//        painter->drawText(10, 20, QString("timeOfGame:%1").arg(timeOfGame));
//    }
}

void Field::drawFullField(QPainter* painter) {
//    qDebug() << "Field::drawFullField(); -- map:" << map;
//    qDebug() << "Field::drawFullField(); -- map->tileSets:size" << map->tileSets.size();
//    qDebug() << "Field::drawFullField(); -- map->tileSets.getTile(85):" << map->tileSets.getTile(85);
//    qDebug() << "Field::drawFullField(); -- map->tileSets.getTile(85)->getPixmap():" << map->tileSets.getTile(85)->getPixmap();
    if(isometric) {
        QPixmap pixmap = map->tileSets.getTile(85)->getPixmap(); // draw water2
        int sizeX = 30;//width()/sizeCellX)+1;
        int sizeY = 30;//(height()/sizeCellY)*2+2;
        int isometricSpaceX = 0;
        int isometricSpaceY = -(sizeCellY/2);
        for (int y = 0; y <= sizeY; y++) {
            for (int x = 0; x <= sizeX; x++) {
                painter->drawPixmap(isometricSpaceX - sizeCellX/2 + x*sizeCellX, isometricSpaceY - sizeCellY, sizeCellX, sizeCellY*2, pixmap);
            }
            isometricSpaceY += sizeCellY/2;
            isometricSpaceX = isometricSpaceX != 0 ? 0 : sizeCellX/2;
        }
    }
}

void Field::drawGrid(QPainter* painter) {
    painter->setPen(QColor(100, 60, 21));
    if(isometric) {
        int isometricSpaceX = halfSizeCellX*sizeFieldY;
        int isometricSpaceY = halfSizeCellY*sizeFieldY;
        for(int x = 0; x < sizeFieldX+1; x++) {
            int x1 = mainCoorMapX + isometricSpaceX+spaceWidget + x*halfSizeCellX;
            int y1 = mainCoorMapY + spaceWidget + x*halfSizeCellY;
            int x2 = mainCoorMapX + spaceWidget + x*halfSizeCellX;
            int y2 = mainCoorMapY + isometricSpaceY+spaceWidget + x*halfSizeCellY;
            painter->drawLine(x1, y1, x2, y2);
        }
        for(int y = 0; y < sizeFieldY+1; y++) {
            int x1 = mainCoorMapX + isometricSpaceX+spaceWidget - y*halfSizeCellX;
            int y1 = mainCoorMapY + spaceWidget + y*halfSizeCellY;
            int x2 = mainCoorMapX + isometricSpaceX*2+spaceWidget - (halfSizeCellX*(sizeFieldY-sizeFieldX)) - y*halfSizeCellX;
            int y2 = mainCoorMapY + isometricSpaceY+spaceWidget - (halfSizeCellY*(sizeFieldY-sizeFieldX)) + y*halfSizeCellY;
            painter->drawLine(x1, y1, x2, y2);
        }
    } else {
        for(int x = 0; x < sizeFieldX+1; x++) {
            painter->drawLine(mainCoorMapX + spaceWidget + x*sizeCellX, mainCoorMapY + spaceWidget, mainCoorMapX + spaceWidget + x*sizeCellX, mainCoorMapY + spaceWidget + sizeCellX*sizeFieldY);
        }
        for(int y = 0; y < sizeFieldY+1; y++) {
            painter->drawLine(mainCoorMapX + spaceWidget, mainCoorMapY + spaceWidget + y*sizeCellX, mainCoorMapX + spaceWidget + sizeCellX*sizeFieldX, mainCoorMapY + spaceWidget + y*sizeCellX);
        }
    }
}

void Field::drawBackGround(QPainter* painter) {
    int isometricCoorX = (sizeCellX/2) * sizeFieldY;
    int isometricCoorY = 0;
    for(int y = 0; y < sizeFieldY; y++) {
        for(int x = 0; x < sizeFieldX; x++) {
            if(map != NULL) {
                Cell* cell = getCell(x, y);
//                qDebug() << "Field::drawFullField(); -- cell->backgroundTiles:" << cell->backgroundTiles.size();
//                    qDebug() << "Field::drawFullField(); -- pix:" << pix;
                foreach (QPixmap pix, cell->backgroundTiles) {
                    if(!isometric) {
                        int pxlsX = mainCoorMapX + spaceWidget + x*sizeCellX;
                        int pxlsY = mainCoorMapY + spaceWidget + y*sizeCellX;
                        int localSizeCell = sizeCellX;
                        painter->drawPixmap(pxlsX, pxlsY, localSizeCell, localSizeCell, pix);
                    } else {
                        int x1 = mainCoorMapX + isometricCoorX - (sizeCellX/2) + x*(sizeCellX/2);
                        int y1 = mainCoorMapY + isometricCoorY - (sizeCellY) + x*(sizeCellY/2);
                        painter->drawPixmap(x1, y1, sizeCellX, sizeCellY*2, pix);
                    }
                }
            }
        }
        isometricCoorX = (getSizeCell()/2) * (sizeFieldY - (y+1));
        isometricCoorY = (getSizeCell()/4) * (y+1);
    }
}

void Field::drawGround(QPainter* painter) {
    int isometricCoorX = (sizeCellX/2) * sizeFieldY;
    int isometricCoorY = 0;
    for(int y = 0; y < sizeFieldY; y++) {
        for(int x = 0; x < sizeFieldX; x++) {
            Cell* cell = getCell(x, y);
            if(cell != NULL && cell->isTerrain()) {
//                qDebug() << "Field::drawGround(); -- cell->terrainTiles:" << cell->terrainTiles.size();
                foreach (QPixmap pix, cell->terrainTiles) {
                    if(!isometric) {
                        int pxlsX = mainCoorMapX + spaceWidget + x*sizeCellX;//+1;
                        int pxlsY = mainCoorMapY + spaceWidget + y*sizeCellX;//+1;
                        int localSizeCell = sizeCellX;//-1;
                            painter->fillRect(pxlsX+1, pxlsY+1, localSizeCell-1, localSizeCell-1, QColor(0, 0, 0));
                            painter->drawPixmap(pxlsX, pxlsY, localSizeCell, localSizeCell, pix);
                    } else {
//                        qDebug() << "Field::drawGround(); -- pix:" << pix;
                        int x1 = mainCoorMapX + isometricCoorX - (sizeCellX/2) + x*(sizeCellX/2);
                        int y1 = mainCoorMapY + isometricCoorY - (sizeCellY) + x*(sizeCellY/2);
                        painter->drawPixmap(x1, y1, sizeCellX, sizeCellY*2, pix);
                    }
                }
            }
        }
        isometricCoorX = (getSizeCell()/2) * (sizeFieldY - (y+1));
        isometricCoorY = (getSizeCell()/4) * (y+1);
    }
}

void Field::drawForeGround(QPainter* painter) {
    int isometricCoorX = (sizeCellX/2) * sizeFieldY;
    int isometricCoorY = 0;
    for (int y = 0; y < sizeFieldY; y++) {
        for (int x = 0; x < sizeFieldX; x++) {
            Cell* cell = getCell(x, y);
            if (cell != NULL) {
                foreach (QPixmap pix, cell->foregroundTiles) {
                    if(!isometric) {
                    } else {
                        int x1 = mainCoorMapX + isometricCoorX - (sizeCellX/2) + x*(sizeCellX/2);
                        int y1 = mainCoorMapY + isometricCoorY - (sizeCellY) + x*(sizeCellY/2);
                        painter->drawPixmap(x1, y1, sizeCellX, sizeCellY*2, pix);
                    }
                }
            }
        }
        isometricCoorX = (getSizeCell()/2) * (sizeFieldY - (y+1));
        isometricCoorY = (getSizeCell()/4) * (y+1);
    }
}

void Field::drawTowersByTowers(QPainter* painter) {
    foreach (Tower* tmpTower, towersManager->towers) {
        if (tmpTower != NULL) {
            int towerX = tmpTower->currX;
            int towerY = tmpTower->currY;
            int size = tmpTower->templateForTower->size;
            if(!isometric) {
            } else {
                int height = tmpTower->templateForTower->height;
                int isometricSpaceX = (getSizeY()-towerY)*(sizeCellX/2);
                int isometricSpaceY = towerY*(sizeCellY/2);
                int pxlsX = mainCoorMapX + isometricSpaceX+spaceWidget + towerX*(sizeCellX/2);
                int pxlsY = mainCoorMapY + isometricSpaceY+spaceWidget + towerX*(sizeCellY/2);
                if (!map) {
                    painter->fillRect(pxlsX+1, pxlsY+1, sizeCellX-1, sizeCellX-1, QColor(127, 255, 0));
                } else {
                    painter->drawPixmap(pxlsX - sizeCellX/2, pxlsY + sizeCellY - (sizeCellY*2)*height, sizeCellX, (sizeCellY*2)*height, tmpTower->pixmap);
                }
                for (int iBullet = 0; iBullet < tmpTower->bullets.size(); iBullet++) {
                    Bullet* tmpBullet = tmpTower->bullets[iBullet];
                    int bulletCellX = tmpBullet->currCellX;
                    int bulletCellY = tmpBullet->currCellY;
                    int lastX, lastY;
                    int animationCurrIter, animationMaxIter;
                    QPixmap pixmap = tmpBullet->getAnimationInformation(&lastX, &lastY, &animationCurrIter, &animationMaxIter);
                    int isometricCoorX = (sizeCellX/2) * sizeFieldY;
                    int isometricCoorY = 0;
                    isometricCoorX = (getSizeCell()/2) * (sizeFieldY - (bulletCellY));
                    isometricCoorY = (getSizeCell()/4) * (bulletCellY);
                    int mainX = mainCoorMapX + isometricCoorX + bulletCellX*(sizeCellX/2);
                    int mainY = mainCoorMapY + isometricCoorY + bulletCellX*(sizeCellY/2);
                    int pxlsX = mainX - sizeCellX/2;
                    int pxlsY = mainY - sizeCellY;
                    if(bulletCellX > lastX && bulletCellY > lastY) {
                        pxlsY -= (sizeCellY/animationMaxIter)*(animationMaxIter-animationCurrIter);
                    } else if(bulletCellX == lastX && bulletCellY > lastY) {
                        pxlsX += (sizeCellX/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
                        pxlsY -= (sizeCellY/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
                    } else if (bulletCellX < lastX && bulletCellY > lastY) {
                        pxlsX += (sizeCellX/animationMaxIter)*(animationMaxIter-animationCurrIter);
                    } else if (bulletCellX < lastX && bulletCellY == lastY) {
                        pxlsX += (sizeCellX/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
                        pxlsY += (sizeCellY/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
                    } else if (bulletCellX < lastX && bulletCellY < lastY) {
                        pxlsY += (sizeCellY/animationMaxIter)*(animationMaxIter-animationCurrIter);
                    } else if (bulletCellX == lastX && bulletCellY < lastY) {
                        pxlsX -= (sizeCellX/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
                        pxlsY += (sizeCellY/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
                    } else if (bulletCellX > lastX && bulletCellY < lastY) {
                        pxlsX -= (sizeCellX/animationMaxIter)*(animationMaxIter-animationCurrIter);
                    } else if (bulletCellX > lastX && bulletCellY == lastY) {
                        pxlsX -= (sizeCellX/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
                        pxlsY -= (sizeCellY/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
                    }
                    painter->drawPixmap(pxlsX, pxlsY+sizeCellY/2, sizeCellX, sizeCellY*2, pixmap);
                }
            }
        }
    }
}

void Field::drawUnits(QPainter* painter) {
    int isometricCoorX = (sizeCellX/2) * sizeFieldY;
    int isometricCoorY = 0;
    for(int y = 0; y < sizeFieldY; y++) {
        for(int x = 0; x < sizeFieldX; x++) {
            int num = containUnit(x, y);
            if(num) {
                std::vector<Unit*> units = getCell(x, y)->getUnits();
                int size = units.size();
                for(int k = 0; k < size; k++) {
                    if(units[k]->alive || units[k]->preDeath) {// fixed!!!
                        int lastX, lastY;
                        int animationCurrIter, animationMaxIter;
                        QPixmap pixmap = units[k]->getAnimationInformation(&lastX, &lastY, &animationCurrIter, &animationMaxIter);
                        if(!isometric) {
                            int pxlsX = mainCoorMapX + spaceWidget + x*sizeCellX;//+1;
                            int pxlsY = mainCoorMapY + spaceWidget + y*sizeCellX;// - sizeCellX/2;//+1;
                            int localSizeCell = sizeCellX;//-1;
                            int localSpaceCell = sizeCellX/3;
                            pxlsX = mainCoorMapX + spaceWidget + x*sizeCellX - localSpaceCell;
                            pxlsY = mainCoorMapY + spaceWidget + y*sizeCellX - localSpaceCell;
                            if(lastX < x)
                                pxlsX -= (sizeCellX/animationMaxIter)*(animationMaxIter-animationCurrIter);
                            if(lastX > x)
                                pxlsX += (sizeCellX/animationMaxIter)*(animationMaxIter-animationCurrIter);
                            if(lastY < y)
                                pxlsY -= (sizeCellX/animationMaxIter)*(animationMaxIter-animationCurrIter);
                            if(lastY > y)
                                pxlsY += (sizeCellX/animationMaxIter)*(animationMaxIter-animationCurrIter);
                            painter->drawPixmap(pxlsX, pxlsY, localSizeCell + localSpaceCell*2, localSizeCell + localSpaceCell*2, pixmap);
                            int maxHP = 100;
                            int hp = units[k]->hp;
                            float hpWidth = localSizeCell-5;
                            hpWidth = hpWidth/maxHP*hp;
                            painter->drawRect(pxlsX + localSpaceCell+2, pxlsY, localSizeCell-4, 10);
                            painter->fillRect(pxlsX + localSpaceCell+3, pxlsY+1, hpWidth, 9, QColor(Qt::green));
                            units[k]->coorByMapX = pxlsX;
                            units[k]->coorByMapY = pxlsY;
                        } else {
                            int mainX = mainCoorMapX + isometricCoorX + x*(sizeCellX/2);
                            int mainY = mainCoorMapY + isometricCoorY + x*(sizeCellY/2);
                            int pxlsX = mainX - sizeCellX/2;
                            int pxlsY = mainY - sizeCellY;
                            if(x > lastX && y > lastY) {
                                pxlsY -= (sizeCellY/animationMaxIter)*(animationMaxIter-animationCurrIter);
                            } else if(x == lastX && y > lastY) {
                                pxlsX += (sizeCellX/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
                                pxlsY -= (sizeCellY/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
                            } else if (x < lastX && y > lastY) {
                                pxlsX += (sizeCellX/animationMaxIter)*(animationMaxIter-animationCurrIter);
                            } else if (x < lastX && y == lastY) {
                                pxlsX += (sizeCellX/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
                                pxlsY += (sizeCellY/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
                            } else if (x < lastX && y < lastY) {
                                pxlsY += (sizeCellY/animationMaxIter)*(animationMaxIter-animationCurrIter);
                            } else if (x == lastX && y < lastY) {
                                pxlsX -= (sizeCellX/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
                                pxlsY += (sizeCellY/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
                            } else if (x > lastX && y < lastY) {
                                pxlsX -= (sizeCellX/animationMaxIter)*(animationMaxIter-animationCurrIter);
                            } else if (x > lastX && y == lastY) {
                                pxlsX -= (sizeCellX/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
                                pxlsY -= (sizeCellY/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
                            }
                            painter->drawPixmap(pxlsX, pxlsY+sizeCellY/2, sizeCellX, sizeCellY*2, pixmap);
                        }
                    }
                }
            }
        }
        isometricCoorX = (getSizeCell()/2) * (sizeFieldY - (y+1));
        isometricCoorY = (getSizeCell()/4) * (y+1);
    }
}

void Field::drawBlackTiles(QPainter* painter) {
    int sizeCellY = sizeCellX/2;
    int isometricCoorX = (sizeCellX/2) * sizeFieldY;
    int isometricCoorY = 0;
    painter->setPen(QColor(255,0,0));
    for (int y = 0; y < sizeFieldY; y++) {
        for (int x = 0; x < sizeFieldX; x++) {
            if (isometric) {
                int pxlsX = mainCoorMapX + spaceWidget + x*sizeCellX+1;
                int pxlsY = mainCoorMapY + spaceWidget + y*sizeCellX+1;
                int localSizeCell = sizeCellX-1;
                int localSpaceCell = sizeCellX/4;
//                painter->drawPixmap(sizeCellX, 0, global_pixmap.width(), global_pixmap.height(), global_pixmap);
//                if(getStepCell(x, y))
//                    painter->drawText(pxlsX + sizeCellX/2-5, pxlsY + sizeCellX/2+5, QString("%1").arg(getStepCell(x, y)));
                if(isSetSpawnPoint(x,y))
                    painter->fillRect(pxlsX + localSpaceCell, pxlsY + localSpaceCell, localSizeCell - 2*(localSpaceCell), localSizeCell - 2*(localSpaceCell), QColor(255, 162, 0));
                if(isSetExitPoint(x,y))
                    painter->fillRect(pxlsX + localSpaceCell, pxlsY + localSpaceCell, localSizeCell - 2*(localSpaceCell), localSizeCell - 2*(localSpaceCell), QColor(0, 255, 0));
            } else {
                int x2 = mainCoorMapX + isometricCoorX - (sizeCellX/2) + x*(sizeCellX/2);
                int y2 = mainCoorMapY + isometricCoorY - (sizeCellY) + x*(sizeCellY/2);
                if (pathFinder.detectCollision({x, y})) {
                    painter->drawPixmap(x2, y2, sizeCellX, sizeCellY*2, global_pixmap);
                }
            }
        }
        isometricCoorX = (getSizeCell()/2) * (sizeFieldY - (y+1));
        isometricCoorY = (getSizeCell()/4) * (y+1);
    }
}

void Field::drawPaths(QPainter* painter) {
    int sizeCellY = sizeCellX/2;
    int isometricCoorX = (sizeCellX/2) * sizeFieldY;
    int isometricCoorY = 0;
    painter->setPen(QColor(255,0,0));

    QPixmap pixmapPathPoint;
    foreach (Unit* tmpUnit, unitsManager->units) {
        if (tmpUnit == unitsManager->hero) {
            pixmapPathPoint = global_pixmap_PathPoint;
        } else {
            pixmapPathPoint = global_pixmap_EnemyPathPoint;
        }
        AStar::CoordinateList unitPath = tmpUnit->path;
        if (!unitPath.empty()) {
            for (int k = 1; k < unitPath.size(); k++) {
                AStar::Vec2i point = unitPath[k];
                isometricCoorX = (getSizeCell()/2) * (sizeFieldY - (point.y+0));
                isometricCoorY = (getSizeCell()/4) * (point.y+0);
                int pPx = mainCoorMapX + isometricCoorX - (sizeCellX/2) + point.x*(sizeCellX/2);
                int pPy = mainCoorMapY + isometricCoorY - (sizeCellY) + point.x*(sizeCellY/2);
                painter->drawPixmap(pPx, pPy, sizeCellX, sizeCellY*2, pixmapPathPoint);
            }
            AStar::Vec2i destinationPoint = unitPath.front();
            isometricCoorX = (getSizeCell()/2) * (sizeFieldY - (destinationPoint.y+0));
            isometricCoorY = (getSizeCell()/4) * (destinationPoint.y+0);
            int destinationPointX = mainCoorMapX + isometricCoorX - (sizeCellX/2) + destinationPoint.x*(sizeCellX/2);
            int destinationPointY = mainCoorMapY + isometricCoorY - (sizeCellY) + destinationPoint.x*(sizeCellY/2);
            painter->drawPixmap(destinationPointX, destinationPointY, sizeCellX, sizeCellY*2, global_pixmap_DestinationPoint);
        }
    }
}

void Field::drawTowerUnderConstruction(QPainter* painter, int buildX, int buildY, TemplateForTower *tower) {
//    int mainCoorMapX = getMainCoorMapX();
//    int mainCoorMapY = getMainCoorMapY();
//    int spaceWidget = getSpaceWidget();
//    int sizeCellX = getSizeCell();

//    QPixmap towerPix = tower->pixmap;
//    int towerSize = tower->size;
//    int pixSizeCell = towerPix.width() / towerSize;
//    QColor cGreen(0, 255, 0, 80);
//    QColor cRed(255, 0, 0, 80);
//    for(int x = 0; x < towerSize; x++) {
//        for(int y = 0; y < towerSize; y++) {
//            QPixmap pix = towerPix.copy(x*pixSizeCell, y*pixSizeCell, pixSizeCell, pixSizeCell);
//            if(!isometric) {
//                int pxlsX = mainCoorMapX + spaceWidget + (buildX+x)*sizeCellX;//+1;
//                int pxlsY = mainCoorMapY + spaceWidget + (buildY+y)*sizeCellX;//+1;
//                painter->drawPixmap(pxlsX, pxlsY, sizeCellX, sizeCellX, pix);
//                if(getCell(buildX+x, buildY+y)->isEmpty()) {
//                    painter->fillRect(pxlsX, pxlsY, sizeCellX, sizeCellX, cGreen);
//                } else {
//                    painter->fillRect(pxlsX, pxlsY, sizeCellX, sizeCellX, cRed);
//                }
//            } else {
//                int sizeCellX = sizeCellX;
//                int sizeCellY = sizeCellX/2;
//                int height = tower->height;
//                int isometricSpaceX = (getSizeY()-(buildY+y))*(sizeCellX/2);
//                int isometricSpaceY = (buildY+y)*(sizeCellY/2);
//                int pxlsX = mainCoorMapX + isometricSpaceX+spaceWidget + (buildX+x)*(sizeCellX/2);
//                int pxlsY = mainCoorMapY + isometricSpaceY+spaceWidget + (buildX+x)*(sizeCellY/2);
//                QPixmap pix = tower->pixmap;
//                if(getCell(buildX+x, buildY+y)->isEmpty()) {
//                    painter->setOpacity(0.5);
//                    painter->drawPixmap(pxlsX - sizeCellX/2, pxlsY + sizeCellY - (sizeCellY*2)*height, sizeCellX, (sizeCellY*2)*height, pix);
//                } else {
//                    painter->setOpacity(0.5);
//                    painter->drawPixmap(pxlsX - sizeCellX/2, pxlsY + sizeCellY - (sizeCellY*2)*height, sizeCellX, (sizeCellY*2)*height, pix);
//                    //QPainter* painter(&pix);
//                    painter->setCompositionMode(QPainter::CompositionMode_SourceIn);
//                    painter->fillRect(pix.rect(), cRed);
//                    //painter->end();
//                    painter->drawPixmap(pxlsX - sizeCellX/2, pxlsY + sizeCellY - (sizeCellY*2)*height, sizeCellX, (sizeCellY*2)*height, pix);
//                }
//            }
//        }
//    }
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

void Field::setSizeCell(int sizeCellX) {
    this->sizeCellX = sizeCellX;
    this->sizeCellY = sizeCellX/2;
    this->halfSizeCellX = sizeCellX/2;
    this->halfSizeCellY = sizeCellY/2;
}

int Field::getMainCoorMapX() {
    return mainCoorMapX;
}

int Field::getMainCoorMapY() {
    return mainCoorMapY;
}

//int Field::getSpaceWidget() {
//    return spaceWidget;
//}

int Field::getSizeCell() {
    return sizeCellX;
}

//void Field::setIsometric(bool isometric) {
//    this->isometric = isometric;
//}

//void Field::setTileMapSize(int tileMapWidth, int tileMapHeight) {
//    this->tileMapWidth = tileMapWidth;
//    this->tileMapHeight = tileMapHeight;
//}

//bool Field::getIsometric() {
//    return isometric;
//}

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
                    if(isometric) {
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
                if(!isometric) {
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
    if(!isometric) {
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
