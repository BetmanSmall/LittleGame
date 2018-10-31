#include "src/head/field.h"

Field::Field(QString mapFile, FactionsManager* factionsManager, int enemyCount, int difficultyLevel, int towersCount) {
    qDebug() << "Field::Field(); -- mapPath:" << mapFile;
    qDebug() << "Field::Field(); -- enemyCount:" << enemyCount;
    qDebug() << "Field::Field(); -- towersCount:" << towersCount;
    this->factionsManager = factionsManager;
    this->towersManager = new TowersManager(difficultyLevel);
    this->unitsManager = new UnitsManager(difficultyLevel);
    qDebug() << "Field::Field(); -1- map:" << map;
    this->map = (new MapLoader())->load(mapFile);
    qDebug() << "Field::Field(); -2- map:" << map;

    global_pixmap = map->tileSets.getTileSet(0)->getLocalTile(8)->getPixmap();
    global_pixmap_PathPoint = map->tileSets.getTileSet(0)->getLocalTile(9)->getPixmap();
    global_pixmap_EnemyPathPoint = map->tileSets.getTileSet(0)->getLocalTile(10)->getPixmap();
    global_pixmap_DestinationPoint = map->tileSets.getTileSet(0)->getLocalTile(20)->getPixmap();
//    global_pixmap_ExitPoint = map->tileSets.getTileSet(0)->getLocalTile(18)->getPixmap();

//    underConstruction = null;
//    green;
//    red?

    createField();
//    int terrainType = rand()%2;
//    if (mapFile.contains("randomMap")) {
//        for (int x = 0; x < map->width; x++) {
//            for (int y = 0; y < map->height; y++) {
//                if( (rand()%100) < 30 ) {
//                    int randNumber = ( 43+(rand()%4) );
//                    QPixmap pixmap = map->getTileSets()->getTileSet(1)->getLocalTile(randNumber)->getPixmap();
//                    getCell(x, y)->setTerrain(pixmap);
//                }
//            }
//        }
//    } else {
//        for (int x = 0; x < map->width; x++) {
//            for (int y = 0; y < map->height; y++) {
//                if( (rand()%100) < 10 ) {
//                    if (getCell(x, y)->isEmpty()) {
//                        int randNumber = ( 125+(rand()%2) );
//                        QPixmap pixmap = map->getTileSets()->getTileSet(0)->getLocalTile(randNumber)->getPixmap();
//                        getCell(x, y)->setTerrain(pixmap);
//                    }
//                }
//            }
//        }
//    }
    spawnHeroInSpawnPoint();
    qDebug() << "Field::Field(); -- map:" << map;
    // camera 1
//    mainCoorMapX = 0, mainCoorMapY = 0;
//    spaceWidget = 0; // fix this. 16 and launch
    // camera 2
    gamePause = false;
//    gameSpeed = 1.0;
//    mouseX = -1;
//    mouseY = -1;
//    spawnPointX = -1;
//    spawnPointY = -1;
//    exitPointX = -1;
//    exitPointY = -1;
//    cellExit?

    gameOverLimitUnits = 10;
    currentFinishedUnits = 0;
    qDebug() << "Field::Field(); -end- ";
}

Field::~Field() {
    qDebug() << "Field::~Field(); -- ";
    delete[] field;
    delete towersManager;
    delete unitsManager;
}

void Field::createField() {
//    qDebug() << "Field::createField(); -- map->width:" << map->width << " map->height:" << map->height << " halfSizeCellX:" << halfSizeCellX << " halfSizeCellY:" << halfSizeCellY << " mapLayers:" << mapLayers;
//    qDebug() << "Field::createField(); -1- field:" << field;
//    if(field == NULL) {
        isometric = true;
        field = new Cell[map->width*map->height];
        for (int y = 0; y < map->height; y++) {
            for (int x = 0; x < map->width; x++) {
                Cell* cell = getCell(x, y);
                cell->setGraphicCoordinates(x, y, map->tileWidth/2, map->tileHeight/2);
                for (Layer* layer : map->getMapLayers()->layers) {
                        TileLayerCell* tileLayerCell = layer->getCell(x, y);
                        if (tileLayerCell != NULL) {
                            Tile* tile = tileLayerCell->getTile();
                            if (tile != NULL) {
                                QString layerName = layer->getName();
                                if (layerName != NULL) {
                                    if (layerName == "background") {
                                        cell->backgroundTiles.push_back(tile->getPixmap());
                                    } else if (layerName == "ground" || layerName == "entity") {
                                        cell->setTerrain(tile->getPixmap());
                                    } else if (layerName == "towers") {
//                                        this->setTower(x, y, factionsManager->getRandomTemplateForTowerFromAllFaction());
                                    } else {
                                        cell->foregroundTiles.push_back(tile->getPixmap());
                                    }
                                }
                                if (!tile->getProperties()->isEmpty()) {
                                    qDebug() << "Field::createField(); -- layerName:" << layerName;
                                    qDebug() << "Field::createField(); -- tile->getId():" << tile->getId();
                                    qDebug() << "Field::createField(); -- tile->getProperties()->size():" << tile->getProperties()->size();
                                    qDebug() << "Field::createField(); -- keys:" << tile->getProperties()->keys();
                                    qDebug() << "Field::createField(); -- values:" << tile->getProperties()->values();
                                }
                                if (tile->getProperties()->contains("spawnPoint")) {
                                    cellSpawnHero = cell;
                                    cellSpawnHero->spawn = true;
                                    qDebug() << "Field::createField(); -- Set cellSpawnHero:" << cellSpawnHero;
                                } else if (tile->getProperties()->contains("exitPoint")) {
                                    cellExitHero = cell;
                                    cellExitHero->exit = true;
                                    qDebug() << "Field::createField(); -- Set cellExitHero:" << cellExitHero;
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
////                                        Cell::Tree tree = new Cell::Tree(textureRegion, treeWidth, treeHeight);
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
    pathFinder = new AStar::PathFinder();
    pathFinder->setWorldSize({map->width, map->height});
    pathFinder->setHeuristic(AStar::Heuristic::euclidean);
    pathFinder->setDiagonalMovement(false);
    updatePathFinderWalls();
    qDebug() << "Field::createField(); -- pathFinder:" << pathFinder;
}

void Field::turnRight() {
    if(map->width == map->height) {
        Cell* newCells = new Cell[map->width*map->height];
        for(int y = 0; y < map->height; y++) {
            for(int x = 0; x < map->width; x++) {
                newCells[map->width*(x)+(map->width-y-1)] = field[map->width*y+x];
//                newCells[map->width*(x)+(map->width-y-1)].setGraphicCoordinates(map->width-y-1, x, halfSizeCellX, halfSizeCellY);
            }
        }
        delete field;
        field = newCells;
    } else {
        int oldWidth = map->width;
        int oldHeight = map->height;
        map->width = map->height;
        map->height = oldWidth;
        Cell* newCells = new Cell[map->width*map->height];
        for(int y = 0; y < oldHeight; y++) {
            for(int x = 0; x < oldWidth; x++) {
                newCells[map->width*(x)+(map->width-y-1)] = field[oldWidth*y+x];
//                newCells[map->width*(x)+(map->width-y-1)].setGraphicCoordinates(map->width-y-1, x, halfSizeCellX, halfSizeCellY);
            }
        }
        delete field;
        field = newCells;
    }
}

void Field::turnLeft() {
    if(map->width == map->height) {
        Cell* newCells = new Cell[map->width*map->height];
        for(int y = 0; y < map->height; y++) {
            for(int x = 0; x < map->width; x++) {
                newCells[map->width*(map->height-x-1)+(y)] = field[map->width*y+x];
//                newCells[map->width*(map->height-x-1)+(y)].setGraphicCoordinates(y, map->height-x-1, halfSizeCellX, halfSizeCellY);
            }
        }
        delete field;
        field = newCells;
    } else {
        int oldWidth = map->width;
        int oldHeight = map->height;
        map->width = map->height;
        map->height = oldWidth;
        Cell* newCells = new Cell[map->width*map->height];
        for(int y = 0; y < oldHeight; y++) {
            for(int x = 0; x < oldWidth; x++) {
                newCells[map->width*(map->height-x-1)+(y)] = field[oldWidth*y+x];
//                newCells[map->width*(map->height-x-1)+(y)].setGraphicCoordinates(y, map->height-x-1, halfSizeCellX, halfSizeCellY);
            }
        }
        delete field;
        field = newCells;
    }
}

void Field::flipX() {
    Cell* newCells = new Cell[map->width*map->height];
    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            newCells[map->width*(y)+(map->width-x-1)] = field[map->width*y+x];
//            newCells[map->width*(y)+(map->width-x-1)].setGraphicCoordinates(map->width-x-1, y, halfSizeCellX, halfSizeCellY);
        }
    }
    delete field;
    field = newCells;
}

void Field::flipY() {
    Cell* newCells = new Cell[map->width*map->height];
    for(int y = 0; y < map->height; y++) {
        for(int x = 0; x < map->width; x++) {
            newCells[map->width*(map->height-y-1)+(x)] = field[map->width*y+x];
//            newCells[map->width*(map->height-y-1)+(x)].setGraphicCoordinates(x, map->height-y-1, halfSizeCellX, halfSizeCellY);
        }
    }
    delete field;
    field = newCells;
}

Cell* Field::getCell(int x, int y) {
//    qDebug() << "Field::getCell(); -- x:" << x << " y:" << y;
//    qDebug() << "Field::getCell(); -- map->width:" << map->width << " map->height:" << map->width;
    if (x >= 0 && x < map->width) {
        if (y >= 0 && y < map->height) {
            return &field[map->width*y + x];
        }
    }
    return NULL;
}

void Field::updateCellsGraphicCoordinates(float halfSizeCellX, float halfSizeCellY) {
    for (int cellX = 0; cellX < map->width; cellX++) {
        for (int cellY = 0; cellY < map->height; cellY++) {
            field[map->width*cellY + cellX].setGraphicCoordinates(cellX, cellY, halfSizeCellX, halfSizeCellY);
        }
    }
}

void Field::updateHeroDestinationPoint() {
    qDebug() << "Field::updateHeroDestinationPoint(); -- ";
//    Unit* hero = unitsManager->hero;
    foreach (Unit* hero, unitsManager->hero) {
        qDebug() << "Field::updateHeroDestinationPoint(); -- hero:" << hero;
        if (hero != NULL && !hero->path.empty()) {
            updateHeroDestinationPoint(hero->path.front().x, hero->path.front().y);
        } else {
            updatePathFinderWalls();
        }
    }
}

void Field::updateHeroDestinationPoint(int x, int y) {
    updatePathFinderWalls();
//    Unit* hero = unitsManager->hero;
    foreach (Unit* hero, unitsManager->hero) {
        qDebug() << "Field::updateHeroDestionPoint(" << x << ", " << y << "); -- ";
        AStar::CoordinateList newPath = pathFinder->findPath({hero->coorByCellX, hero->coorByCellY}, {x, y});
        if (newPath.front().operator==({x, y})) {
            newPath.pop_back();
            hero->path = newPath;
        } else {
            hero->path.clear();
        }
    }
}

void Field::updatePathFinderWalls() {
    qDebug() << "Field::updatePathFinderWalls(); -- ";
    qDebug() << "Field::updatePathFinderWalls(); -1- pathFinder->walls.size():" << pathFinder->walls.size();
    pathFinder->clearCollisions();
    qDebug() << "Field::updatePathFinderWalls(); -2- pathFinder->walls.size():" << pathFinder->walls.size();
    for (int x = 0; x < map->width; x++) {
        for (int y = 0; y < map->height; y++) {
            Cell* cell = getCell(x, y);
            if (cell->isTerrain() || cell->getTower() != NULL) {
                cell->isPassable();
//                qDebug() << "Field::updatePathFinderWalls(); -- " << cell->toString().toStdString().c_str();
//                qDebug() << "Field::updatePathFinderWalls(); -- pathFinder->addCollision";
                pathFinder->addCollision({x, y});
            }
        }
    }
    qDebug() << "Field::updatePathFinderWalls(); -3- pathFinder->walls.size():" << pathFinder->walls.size();
    qDebug() << "Field::updatePathFinderWalls(); -end- ";
}

void Field::render(float deltaTime, CameraController* cameraController) {
//    qDebug() << "Field::render(); -- deltaTime:" << deltaTime;
//    if (timerId == unitsMove_TimerId) {
////        if (!gamePause) {
////            timeOfGame += unitsMove_TimerMilliSec;
//            if (int result = field->stepAllUnits()) {
//                if(result == 4) {
//                    global_text = "Hero contact With Enemy!";
//                    signal_closeWidgetGameFinished(false, timeOfGame);
//                    return;
//                } else if(result == 3) {
//                    global_text = "Hero in ExitPoint!";
//                    signal_closeWidgetGameFinished(true, timeOfGame);
//                    return;
//                } else if(result == 2) {
//                    global_text = "You WIN!";
//                } else if(result == 1) {
//                    global_text = "You LOSE!";
//                } else if(result == -1) {
////                    if(field->deleteTower()) {
//////                        field->waveAlgorithm();
////                    } else {
//////                        stopTimer_UnitsMoveAndTowerAttack();
////                    }
//                }
//            }
////        }
//    } else if (timerId == towersAttack_TimerId) {
//    //        if (!gamePause) {
//            field->towersAttack(towersAttack_TimerMilliSec);
//    //        }
//    } else
//    if (gameStart) {
//        drawFullField(cameraController);
        if (isDrawableBackground)
            drawBackGrounds(cameraController);
        if (isDrawableGround || isDrawableUnits || isDrawableTowers)
            drawGroundsWithUnitsAndTowers(cameraController);
        if (isDrawableForeground)
            drawForeGrounds(cameraController);

        if (isDrawableGrid)
            drawGrid(cameraController);
        if (isDrawableGridNav)
            drawGridNavs(cameraController);
//        if (isDrawableRoutes)
//            drawPaths(cameraController);
//        painter->drawText(10, 20, QString("timeOfGame:%1").arg(timeOfGame));
//    }
}

void Field::drawFullField(CameraController* cameraController) {
//    qDebug() << "Field::drawFullField(); -- map:" << map;
//    qDebug() << "Field::drawFullField(); -- map->tileSets:size" << map->tileSets.size();
//    qDebug() << "Field::drawFullField(); -- map->tileSets.getTile(85):" << map->tileSets.getTile(85);
//    qDebug() << "Field::drawFullField(); -- map->tileSets.getTile(85)->getPixmap():" << map->tileSets.getTile(85)->getPixmap();
    if(isometric) {
        QPixmap pixmap = map->tileSets.getTile(85)->getPixmap(); // draw water2
        int sizeX = 30;//width()/sizeCellX)+1;
        int sizeY = 30;//(height()/sizeCellY)*2+2;
        int isometricSpaceX = 0;
        int isometricSpaceY = -(cameraController->sizeCellY/2);
        for (int y = 0; y <= sizeY; y++) {
            for (int x = 0; x <= sizeX; x++) {
                cameraController->painter->drawPixmap(isometricSpaceX - cameraController->sizeCellX/2 + x*cameraController->sizeCellX, isometricSpaceY - cameraController->sizeCellY, cameraController->sizeCellX, cameraController->sizeCellY*2, pixmap);
            }
            isometricSpaceY += cameraController->sizeCellY/2;
            isometricSpaceX = isometricSpaceX != 0 ? 0 : cameraController->sizeCellX/2;
        }
    }
}

void Field::drawGrid(CameraController* cameraController) {
    cameraController->painter->setPen(QPen(QColor(100, 60, 21), 1));
    int cameraX = cameraController->cameraX;
    int cameraY = cameraController->cameraY;
    float halfSizeCellX = cameraController->halfSizeCellX;
    float halfSizeCellY = cameraController->halfSizeCellY;
//    qDebug() << "Field::drawGrid(camera); -- cameraX:" << cameraX << " cameraY:" << cameraY << " halfSizeCellX:" << halfSizeCellX << " halfSizeCellY:" << halfSizeCellY;
    int widthForTop = map->height * halfSizeCellX; // A - B
    int heightForTop = map->height * halfSizeCellY; // B - Top
    int widthForBottom = map->width * halfSizeCellX; // A - C
    int heightForBottom = map->height * halfSizeCellY; // C - Bottom
//    qDebug() << "Field::drawGrid(camera); -- widthForTop:" << widthForTop << " heightForTop:" << heightForTop << " widthForBottom:" << widthForBottom << " heightForBottom:" << heightForBottom;
    if(isDrawableGrid == 1 || isDrawableGrid == 5) {
        for (int x = 0; x <= map->width; x++)
            cameraController->painter->drawLine(cameraX+(halfSizeCellX*x),cameraY-(halfSizeCellY*x)/*+halfSizeCellY*/,cameraX-(widthForTop)+(halfSizeCellX*x),cameraY-(heightForTop)-(x*halfSizeCellY)/*+halfSizeCellY*/);
        for (int y = 0; y <= map->height; y++)
            cameraController->painter->drawLine(cameraX-(halfSizeCellX*y),cameraY-(halfSizeCellY*y)/*+halfSizeCellY*/,cameraX+(widthForBottom)-(halfSizeCellX*y),cameraY-(heightForBottom)-(halfSizeCellY*y)/*+halfSizeCellY*/);
    }
    if(isDrawableGrid == 2 || isDrawableGrid == 5) {
        for (int x = 0; x <= map->width; x++)
            cameraController->painter->drawLine(cameraX+(halfSizeCellX*x),cameraY-(halfSizeCellY*x)/*+halfSizeCellY*/,cameraX+(widthForTop)+(halfSizeCellX*x),cameraY+(heightForTop)-(x*halfSizeCellY)/*+halfSizeCellY*/);
        for (int y = 0; y <= map->height; y++)
            cameraController->painter->drawLine(cameraX+(halfSizeCellX*y),cameraY+(halfSizeCellY*y)/*+halfSizeCellY*/,cameraX+(widthForBottom)+(halfSizeCellX*y),cameraY-(heightForBottom)+(halfSizeCellY*y)/*+halfSizeCellY*/);
    }
    if(isDrawableGrid == 3 || isDrawableGrid == 5) {
        for (int x = 0; x <= map->height; x++) // WHT??? map->height check groundDraw
            cameraController->painter->drawLine(cameraX-(halfSizeCellX*x),cameraY+(halfSizeCellY*x)/*+halfSizeCellY*/,cameraX+(widthForBottom)-(halfSizeCellX*x),cameraY+(heightForBottom)+(x*halfSizeCellY)/*+halfSizeCellY*/);
        for (int y = 0; y <= map->width; y++) // WHT??? map->width check groundDraw
            cameraController->painter->drawLine(cameraX+(halfSizeCellX*y),cameraY+(halfSizeCellY*y)/*+halfSizeCellY*/,cameraX-(widthForTop)+(halfSizeCellX*y),cameraY+(heightForTop)+(halfSizeCellY*y)/*+halfSizeCellY*/);
    }
    if(isDrawableGrid == 4 || isDrawableGrid == 5) {
        for (int x = 0; x <= map->height; x++) // WHT??? map->height check groundDraw
            cameraController->painter->drawLine(cameraX-(halfSizeCellX*x),cameraY+(halfSizeCellY*x)/*+halfSizeCellY*/,cameraX-(widthForBottom)-(halfSizeCellX*x),cameraY-(heightForBottom)+(x*halfSizeCellY)/*+halfSizeCellY*/);
        for (int y = 0; y <= map->width; y++) // WHT??? map->width check groundDraw
            cameraController->painter->drawLine(cameraX-(halfSizeCellX*y),cameraY-(halfSizeCellY*y)/*+halfSizeCellY*/,cameraX-(widthForTop)-(halfSizeCellX*y),cameraY+(heightForTop)-(halfSizeCellY*y)/*+halfSizeCellY*/);
    }
//    if(isometric) {
//        int isometricSpaceX = cameraController->halfSizeCellX*map->height;
//        int isometricSpaceY = cameraController->halfSizeCellY*map->height;
//        for(int x = 0; x < map->width+1; x++) {
//            int x1 = cameraController->x + isometricSpaceX + x*cameraController->halfSizeCellX;
//            int y1 = cameraController->y + x*cameraController->halfSizeCellY;
//            int x2 = cameraController->x + x*cameraController->halfSizeCellX;
//            int y2 = cameraController->y + isometricSpaceY + x*cameraController->halfSizeCellY;
//            cameraController->painter->drawLine(x1, y1, x2, y2);
//        }
//        for(int y = 0; y < map->height+1; y++) {
//            int x1 = cameraController->x + isometricSpaceX - y*cameraController->halfSizeCellX;
//            int y1 = cameraController->y + y*cameraController->halfSizeCellY;
//            int x2 = cameraController->x + isometricSpaceX*2 - (cameraController->halfSizeCellX*(map->height-map->width)) - y*cameraController->halfSizeCellX;
//            int y2 = cameraController->y + isometricSpaceY - (cameraController->halfSizeCellY*(map->height-map->width)) + y*cameraController->halfSizeCellY;
//            cameraController->painter->drawLine(x1, y1, x2, y2);
//        }
//    } else {
//        for(int x = 0; x < map->width+1; x++) {
//            cameraController->painter->drawLine(cameraController->x + x*sizeCellX, cameraController->y, cameraController->x + x*sizeCellX, cameraController->y + sizeCellX*map->height);
//        }
//        for(int y = 0; y < map->height+1; y++) {
//            cameraController->painter->drawLine(cameraController->x, cameraController->y + y*sizeCellX, cameraController->x + sizeCellX*map->width, cameraController->y + y*sizeCellX);
//        }
//    }
}

void Field::drawBackGrounds(CameraController* cameraController) {
    if(drawOrder == 0) {
        for (int y = 0; y < map->height; y++) {
            for (int x = 0; x < map->width; x++) {
                drawBackGroundCell(cameraController, x, y);
            }
        }
    } else if(drawOrder == 1) {
        for (int x = 0; x < map->width; x++) {
            for (int y = 0; y < map->height; y++) {
                drawBackGroundCell(cameraController, x, y);
            }
        }
    } else if(drawOrder == 2) {
        for (int y = map->height-1; y >= 0; y--) {
            for (int x = map->width-1; x >= 0; x--) {
                drawBackGroundCell(cameraController, x, y);
            }
        }
    } else if(drawOrder == 3) {
        for (int x = map->width-1; x >= 0; x--) {
            for (int y = map->height-1; y >= 0; y--) {
                drawBackGroundCell(cameraController, x, y);
            }
        }
    } else if(drawOrder == 4) {
        for (int y = map->height-1; y >= 0; y--) {
            for (int x = 0; x < map->width; x++) {
                drawBackGroundCell(cameraController, x, y);
            }
        }
    } else if(drawOrder == 5) {
        for (int x = 0; x < map->width; x++) {
            for (int y = map->height-1; y >= 0; y--) {
                drawBackGroundCell(cameraController, x, y);
            }
        }
    } else if(drawOrder == 6) {
        for (int y = 0; y < map->height; y++) {
            for (int x = map->width-1; x >= 0; x--) {
                drawBackGroundCell(cameraController, x, y);
            }
        }
    } else if(drawOrder == 7) {
        for (int x = map->width-1; x >= 0; x--) {
            for (int y = 0; y < map->height; y++) {
                drawBackGroundCell(cameraController, x, y);
            }
        }
    } else if(drawOrder == 8) {
//        qDebug() << "GameField::drawBackGrounds(); -- camera.position:" << camera.position;
        int x = 0, y = 0;
        int length = (map->width > map->height) ? map->width : map->height;
        while (x < length) {
            if(x < map->width && y < map->height) {
                if (x == length - 1 && y == length - 1) {
                    drawBackGroundCell(cameraController, x, y);
                } else {
                    drawBackGroundCell(cameraController, x, y);
                }
            }
            if (x == length - 1) {
                x = y + 1;
                y = length - 1;
            } else if (y == 0) {
                y = x + 1;
                x = 0;
            } else {
                x++;
                y--;
            }
        }
    }
}

void Field::drawBackGroundCell(CameraController* cameraController, int cellX, int cellY) {
    Cell* cell = getCell(cellX, cellY);
    foreach (QPixmap textureRegion, cell->backgroundTiles) {
        if (isDrawableBackground == 1 || isDrawableBackground == 5) {
            cameraController->painter->drawPixmap(cameraController->cameraX+cell->graphicCoordinatesBottom->x()-(cameraController->halfSizeCellX), cameraController->cameraY+cell->graphicCoordinatesBottom->y()-(cameraController->sizeCellY*2), cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
        }
        if (isDrawableBackground == 2 || isDrawableBackground == 5) {
            cameraController->painter->drawPixmap(cameraController->cameraX+cell->graphicCoordinatesRight->x()-(cameraController->halfSizeCellX), cameraController->cameraY+cell->graphicCoordinatesRight->y()-(cameraController->sizeCellY*2), cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
        }
        if (isDrawableBackground == 3 || isDrawableBackground == 5) {
            cameraController->painter->drawPixmap(cameraController->cameraX+cell->graphicCoordinatesTop->x()-(cameraController->halfSizeCellX), cameraController->cameraY+cell->graphicCoordinatesTop->y()-(cameraController->sizeCellY*2), cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
        }
        if (isDrawableBackground == 4 || isDrawableBackground == 5) {
            cameraController->painter->drawPixmap(cameraController->cameraX+cell->graphicCoordinatesLeft->x()-(cameraController->halfSizeCellX), cameraController->cameraY+cell->graphicCoordinatesLeft->y()-(cameraController->sizeCellY*2), cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
        }
    }
}

//void Field::drawBackGround(QPainter* painter) {
//    int isometricCoorX = (sizeCellX/2) * map->height;
//    int isometricCoorY = 0;
//    for(int y = 0; y < map->height; y++) {
//        for(int x = 0; x < map->width; x++) {
//            if(map != NULL) {
//                Cell* cell = getCell(x, y);
////                qDebug() << "Field::drawFullField(); -- cell->backgroundTiles:" << cell->backgroundTiles.size();
////                    qDebug() << "Field::drawFullField(); -- pix:" << pix;
//                foreach (QPixmap pix, cell->backgroundTiles) {
//                    if(!isometric) {
//                        int pxlsX = mainCoorMapX + x*sizeCellX;
//                        int pxlsY = mainCoorMapY + y*sizeCellX;
//                        int localSizeCell = sizeCellX;
//                        painter->drawPixmap(pxlsX, pxlsY, localSizeCell, localSizeCell, pix);
//                    } else {
//                        int x1 = mainCoorMapX + isometricCoorX - (sizeCellX/2) + x*(sizeCellX/2);
//                        int y1 = mainCoorMapY + isometricCoorY - (sizeCellY) + x*(sizeCellY/2);
//                        painter->drawPixmap(x1, y1, sizeCellX, sizeCellY*2, pix);
//                    }
//                }
//            }
//        }
//        isometricCoorX = (getSizeCell()/2) * (map->height - (y+1));
//        isometricCoorY = (getSizeCell()/4) * (y+1);
//    }
//}

void Field::drawGroundsWithUnitsAndTowers(CameraController* cameraController) {
    if(drawOrder == 0) {
        for (int y = 0; y < map->height; y++) {
            for (int x = 0; x < map->width; x++) {
                drawGroundCellWithUnitsAndTower(cameraController, x, y);
            }
        }
    } else if(drawOrder == 1) {
        for (int x = 0; x < map->width; x++) {
            for (int y = 0; y < map->height; y++) {
                drawGroundCellWithUnitsAndTower(cameraController, x, y);
            }
        }
    } else if(drawOrder == 2) {
        for (int y = map->height-1; y >= 0; y--) {
            for (int x = map->width-1; x >= 0; x--) {
                drawGroundCellWithUnitsAndTower(cameraController, x, y);
            }
        }
    } else if(drawOrder == 3) {
        for (int x = map->width-1; x >= 0; x--) {
            for (int y = map->height-1; y >= 0; y--) {
                drawGroundCellWithUnitsAndTower(cameraController, x, y);
            }
        }
    } else if(drawOrder == 4) {
        for (int y = map->height-1; y >= 0; y--) {
            for (int x = 0; x < map->width; x++) {
                drawGroundCellWithUnitsAndTower(cameraController, x, y);
            }
        }
    } else if(drawOrder == 5) {
        for (int x = 0; x < map->width; x++) {
            for (int y = map->height-1; y >= 0; y--) {
                drawGroundCellWithUnitsAndTower(cameraController, x, y);
            }
        }
    } else if(drawOrder == 6) {
        for (int y = 0; y < map->height; y++) {
            for (int x = map->width-1; x >= 0; x--) {
                drawGroundCellWithUnitsAndTower(cameraController, x, y);
            }
        }
    } else if(drawOrder == 7) {
        for (int x = map->width-1; x >= 0; x--) {
            for (int y = 0; y < map->height; y++) {
                drawGroundCellWithUnitsAndTower(cameraController, x, y);
            }
        }
    } else if(drawOrder == 8) {
        int x = 0, y = 0;
        int length = (map->width > map->height) ? map->width : map->height;
        while (x < length) {
            if(x < map->width && y < map->height) {
                if (x == length - 1 && y == length - 1) {
                    drawGroundCellWithUnitsAndTower(cameraController, x, y);
                } else {
                    drawGroundCellWithUnitsAndTower(cameraController, x, y);
                }
            }
            if (x == length - 1) {
                x = y + 1;
                y = length - 1;
            } else if (y == 0) {
                y = x + 1;
                x = 0;
            } else {
                x++;
                y--;
            }
        }
    }
}

void Field::drawGroundCellWithUnitsAndTower(CameraController* cameraController, int cellX, int cellY) {
    Cell* cell = getCell(cellX, cellY);
    foreach (QPixmap textureRegion, cell->groundTiles) {
        if(isDrawableGround == 1 || isDrawableGround == 5) {
            cameraController->painter->drawPixmap(cameraController->cameraX+cell->graphicCoordinatesBottom->x()-(cameraController->halfSizeCellX), cameraController->cameraY+cell->graphicCoordinatesBottom->y()-(cameraController->sizeCellY*2), cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
        }
        if(isDrawableGround == 2 || isDrawableGround == 5) {
            cameraController->painter->drawPixmap(cameraController->cameraX+cell->graphicCoordinatesRight->x()-(cameraController->halfSizeCellX), cameraController->cameraY+cell->graphicCoordinatesRight->y()-(cameraController->sizeCellY*2), cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
        }
        if(isDrawableGround == 3 || isDrawableGround == 5) {
            cameraController->painter->drawPixmap(cameraController->cameraX+cell->graphicCoordinatesTop->x()-(cameraController->halfSizeCellX), cameraController->cameraY+cell->graphicCoordinatesTop->y()-(cameraController->sizeCellY*2), cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
        }
        if(isDrawableGround == 4 || isDrawableGround == 5) {
            cameraController->painter->drawPixmap(cameraController->cameraX+cell->graphicCoordinatesLeft->x()-(cameraController->halfSizeCellX), cameraController->cameraY+cell->graphicCoordinatesLeft->y()-(cameraController->sizeCellY*2), cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
        }
    }
//    Array<Unit> units = field[cellX][cellY].getUnits();
//    if(units != null) {
//        Color oldColorSB = painter.getColor();
//        for (Unit unit : units) {
//            drawUnit(unit, painter);
//        }
//        painter.setColor(oldColorSB);
//    }
//    Tower tower = field[cellX][cellY].getTower();
//    if(tower != null) {
//        drawTower(tower, painter);
//    }
//    for (QPixmap textureRegion : cell->foregroundTiles) {
//        if(isDrawableForeground == 1 || isDrawableForeground == 5) {
//            cameraController->painter->drawPixmap(cameraController->cameraX+cell->graphicCoordinatesBottom->x()-(cameraController->halfSizeCellX), cameraController->cameraY+cell->graphicCoordinatesBottom->y()-(cameraController->sizeCellY*2), cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
//        }
//        if(isDrawableForeground == 2 || isDrawableForeground == 5) {
//            cameraController->painter->drawPixmap(cameraController->cameraX+cell->graphicCoordinatesRight->x()-(cameraController->halfSizeCellX), cameraController->cameraY+cell->graphicCoordinatesRight->y()-(cameraController->sizeCellY*2), cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
//        }
//        if(isDrawableForeground == 3 || isDrawableForeground == 5) {
//            cameraController->painter->drawPixmap(cameraController->cameraX+cell->graphicCoordinatesTop->x()-(cameraController->halfSizeCellX), cameraController->cameraY+cell->graphicCoordinatesTop->y()-(cameraController->sizeCellY*2), cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
//        }
//        if(isDrawableForeground == 4 || isDrawableForeground == 5) {
//            cameraController->painter->drawPixmap(cameraController->cameraX+cell->graphicCoordinatesLeft->x()-(cameraController->halfSizeCellX), cameraController->cameraY+cell->graphicCoordinatesLeft->y()-(cameraController->sizeCellY*2), cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
//        }
//    }
}

//void Field::drawGround(QPainter* painter) {
//    int isometricCoorX = (sizeCellX/2) * map->height;
//    int isometricCoorY = 0;
//    for(int y = 0; y < map->height; y++) {
//        for(int x = 0; x < map->width; x++) {
//            Cell* cell = getCell(x, y);
//            if(cell != NULL && cell->isTerrain()) {
////                qDebug() << "Field::drawGround(); -- cell->terrainTiles:" << cell->terrainTiles.size();
//                foreach (QPixmap pix, cell->terrainTiles) {
//                    if(!isometric) {
//                        int pxlsX = mainCoorMapX + x*sizeCellX;//+1;
//                        int pxlsY = mainCoorMapY + y*sizeCellX;//+1;
//                        int localSizeCell = sizeCellX;//-1;
//                            painter->fillRect(pxlsX+1, pxlsY+1, localSizeCell-1, localSizeCell-1, QColor(0, 0, 0));
//                            painter->drawPixmap(pxlsX, pxlsY, localSizeCell, localSizeCell, pix);
//                    } else {
////                        qDebug() << "Field::drawGround(); -- pix:" << pix;
//                        int x1 = mainCoorMapX + isometricCoorX - (sizeCellX/2) + x*(sizeCellX/2);
//                        int y1 = mainCoorMapY + isometricCoorY - (sizeCellY) + x*(sizeCellY/2);
//                        painter->drawPixmap(x1, y1, sizeCellX, sizeCellY*2, pix);
//                    }
//                }
//            }
//        }
//        isometricCoorX = (getSizeCell()/2) * (map->height - (y+1));
//        isometricCoorY = (getSizeCell()/4) * (y+1);
//    }
//}

//void Field::drawForeGround(QPainter* painter) {
//    int isometricCoorX = (sizeCellX/2) * map->height;
//    int isometricCoorY = 0;
//    for (int y = 0; y < map->height; y++) {
//        for (int x = 0; x < map->width; x++) {
//            Cell* cell = getCell(x, y);
//            if (cell != NULL) {
//                foreach (QPixmap pix, cell->foregroundTiles) {
//                    if(!isometric) {
//                    } else {
//                        int x1 = mainCoorMapX + isometricCoorX - (sizeCellX/2) + x*(sizeCellX/2);
//                        int y1 = mainCoorMapY + isometricCoorY - (sizeCellY) + x*(sizeCellY/2);
//                        painter->drawPixmap(x1, y1, sizeCellX, sizeCellY*2, pix);
//                    }
//                }
//            }
//        }
//        isometricCoorX = (getSizeCell()/2) * (map->height - (y+1));
//        isometricCoorY = (getSizeCell()/4) * (y+1);
//    }
//}


void Field::drawForeGrounds(CameraController* cameraController) {
    if(drawOrder == 0) {
        for (int y = 0; y < map->height; y++) {
            for (int x = 0; x < map->width; x++) {
                drawForeGroundCell(cameraController, x, y);
            }
        }
    } else if(drawOrder == 1) {
        for (int x = 0; x < map->width; x++) {
            for (int y = 0; y < map->height; y++) {
                drawForeGroundCell(cameraController, x, y);
            }
        }
    } else if(drawOrder == 2) {
        for (int y = map->height-1; y >= 0; y--) {
            for (int x = map->width-1; x >= 0; x--) {
                drawForeGroundCell(cameraController, x, y);
            }
        }
    } else if(drawOrder == 3) {
        for (int x = map->width-1; x >= 0; x--) {
            for (int y = map->height-1; y >= 0; y--) {
                drawForeGroundCell(cameraController, x, y);
            }
        }
    } else if(drawOrder == 4) {
        for (int y = map->height-1; y >= 0; y--) {
            for (int x = 0; x < map->width; x++) {
                drawForeGroundCell(cameraController, x, y);
            }
        }
    } else if(drawOrder == 5) {
        for (int x = 0; x < map->width; x++) {
            for (int y = map->height-1; y >= 0; y--) {
                drawForeGroundCell(cameraController, x, y);
            }
        }
    } else if(drawOrder == 6) {
        for (int y = 0; y < map->height; y++) {
            for (int x = map->width-1; x >= 0; x--) {
                drawForeGroundCell(cameraController, x, y);
            }
        }
    } else if(drawOrder == 7) {
        for (int x = map->width-1; x >= 0; x--) {
            for (int y = 0; y < map->height; y++) {
                drawForeGroundCell(cameraController, x, y);
            }
        }
    } else if(drawOrder == 8) {
//        qDebug() << "GameField::drawBackGrounds(); -- camera.position:" << camera.position;
        int x = 0, y = 0;
        int length = (map->width > map->height) ? map->width : map->height;
        while (x < length) {
            if(x < map->width && y < map->height) {
                if (x == length - 1 && y == length - 1) {
                    drawForeGroundCell(cameraController, x, y);
                } else {
                    drawForeGroundCell(cameraController, x, y);
                }
            }
            if (x == length - 1) {
                x = y + 1;
                y = length - 1;
            } else if (y == 0) {
                y = x + 1;
                x = 0;
            } else {
                x++;
                y--;
            }
        }
    }
}

void Field::drawForeGroundCell(CameraController* cameraController, int cellX, int cellY) {
    Cell* cell = getCell(cellX, cellY);
    foreach (QPixmap textureRegion, cell->foregroundTiles) {
        if (isDrawableForeground == 1 || isDrawableForeground == 5) {
            cameraController->painter->drawPixmap(cameraController->cameraX+cell->graphicCoordinatesBottom->x()-(cameraController->halfSizeCellX), cameraController->cameraY+cell->graphicCoordinatesBottom->y()-(cameraController->sizeCellY*2), cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
        }
        if (isDrawableForeground == 2 || isDrawableForeground == 5) {
            cameraController->painter->drawPixmap(cameraController->cameraX+cell->graphicCoordinatesRight->x()-(cameraController->halfSizeCellX), cameraController->cameraY+cell->graphicCoordinatesRight->y()-(cameraController->sizeCellY*2), cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
        }
        if (isDrawableForeground == 3 || isDrawableForeground == 5) {
            cameraController->painter->drawPixmap(cameraController->cameraX+cell->graphicCoordinatesTop->x()-(cameraController->halfSizeCellX), cameraController->cameraY+cell->graphicCoordinatesTop->y()-(cameraController->sizeCellY*2), cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
        }
        if (isDrawableForeground == 4 || isDrawableForeground == 5) {
            cameraController->painter->drawPixmap(cameraController->cameraX+cell->graphicCoordinatesLeft->x()-(cameraController->halfSizeCellX), cameraController->cameraY+cell->graphicCoordinatesLeft->y()-(cameraController->sizeCellY*2), cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
        }
    }
}

//void Field::drawTowersByTowers(QPainter* painter) {
//    foreach (Tower* tmpTower, towersManager->towers) {
//        if (tmpTower != NULL) {
//            int towerX = tmpTower->currX;
//            int towerY = tmpTower->currY;
//            int size = tmpTower->templateForTower->size;
//            if(!isometric) {
//            } else {
//                int height = tmpTower->templateForTower->height;
//                int isometricSpaceX = (map->height-towerY)*(sizeCellX/2);
//                int isometricSpaceY = towerY*(sizeCellY/2);
//                int pxlsX = mainCoorMapX + isometricSpaceX + towerX*(sizeCellX/2);
//                int pxlsY = mainCoorMapY + isometricSpaceY + towerX*(sizeCellY/2);
//                if (!map) {
//                    painter->fillRect(pxlsX+1, pxlsY+1, sizeCellX-1, sizeCellX-1, QColor(127, 255, 0));
//                } else {
//                    painter->drawPixmap(pxlsX - sizeCellX/2, pxlsY + sizeCellY - (sizeCellY*2)*height, sizeCellX, (sizeCellY*2)*height, tmpTower->pixmap);
//                }
//                for (int iBullet = 0; iBullet < tmpTower->bullets.size(); iBullet++) {
//                    Bullet* tmpBullet = tmpTower->bullets[iBullet];
//                    int bulletCellX = tmpBullet->currCellX;
//                    int bulletCellY = tmpBullet->currCellY;
//                    int lastX, lastY;
//                    int animationCurrIter, animationMaxIter;
//                    QPixmap pixmap = tmpBullet->getAnimationInformation(&lastX, &lastY, &animationCurrIter, &animationMaxIter);
//                    int isometricCoorX = (sizeCellX/2) * map->height;
//                    int isometricCoorY = 0;
//                    isometricCoorX = (getSizeCell()/2) * (map->height - (bulletCellY));
//                    isometricCoorY = (getSizeCell()/4) * (bulletCellY);
//                    int mainX = mainCoorMapX + isometricCoorX + bulletCellX*(sizeCellX/2);
//                    int mainY = mainCoorMapY + isometricCoorY + bulletCellX*(sizeCellY/2);
//                    int pxlsX = mainX - sizeCellX/2;
//                    int pxlsY = mainY - sizeCellY;
//                    if(bulletCellX > lastX && bulletCellY > lastY) {
//                        pxlsY -= (sizeCellY/animationMaxIter)*(animationMaxIter-animationCurrIter);
//                    } else if(bulletCellX == lastX && bulletCellY > lastY) {
//                        pxlsX += (sizeCellX/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
//                        pxlsY -= (sizeCellY/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
//                    } else if (bulletCellX < lastX && bulletCellY > lastY) {
//                        pxlsX += (sizeCellX/animationMaxIter)*(animationMaxIter-animationCurrIter);
//                    } else if (bulletCellX < lastX && bulletCellY == lastY) {
//                        pxlsX += (sizeCellX/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
//                        pxlsY += (sizeCellY/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
//                    } else if (bulletCellX < lastX && bulletCellY < lastY) {
//                        pxlsY += (sizeCellY/animationMaxIter)*(animationMaxIter-animationCurrIter);
//                    } else if (bulletCellX == lastX && bulletCellY < lastY) {
//                        pxlsX -= (sizeCellX/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
//                        pxlsY += (sizeCellY/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
//                    } else if (bulletCellX > lastX && bulletCellY < lastY) {
//                        pxlsX -= (sizeCellX/animationMaxIter)*(animationMaxIter-animationCurrIter);
//                    } else if (bulletCellX > lastX && bulletCellY == lastY) {
//                        pxlsX -= (sizeCellX/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
//                        pxlsY -= (sizeCellY/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
//                    }
//                    painter->drawPixmap(pxlsX, pxlsY+sizeCellY/2, sizeCellX, sizeCellY*2, pixmap);
//                }
//            }
//        }
//    }
//}

//void Field::drawUnits(QPainter* painter) {
//    int isometricCoorX = (sizeCellX/2) * map->height;
//    int isometricCoorY = 0;
//    for(int y = 0; y < map->height; y++) {
//        for(int x = 0; x < map->width; x++) {
////            int num = containUnit(x, y);
////            if(num) {
//                std::vector<Unit*> units = getCell(x, y)->getUnits();
//                int size = units.size();
//                qDebug() << "Field::drawUnits(); -- size:" << size;
//                for(int k = 0; k < size; k++) {
//                    if(units[k]->alive || units[k]->preDeath) {// fixed!!!
//                        int lastX, lastY;
//                        int animationCurrIter, animationMaxIter;
//                        QPixmap pixmap = units[k]->getAnimationInformation(&lastX, &lastY, &animationCurrIter, &animationMaxIter);
//                        qDebug() << "Field::drawUnits(); -- isometric:" << isometric;
//                        qDebug() << "Field::drawUnits(); -- pixmap:" << pixmap;
//                        if(!isometric) {
//                            int pxlsX = mainCoorMapX + x*sizeCellX;//+1;
//                            int pxlsY = mainCoorMapY + y*sizeCellX;// - sizeCellX/2;//+1;
//                            int localSizeCell = sizeCellX;//-1;
//                            int localSpaceCell = sizeCellX/3;
//                            pxlsX = mainCoorMapX + x*sizeCellX - localSpaceCell;
//                            pxlsY = mainCoorMapY + y*sizeCellX - localSpaceCell;
//                            if(lastX < x)
//                                pxlsX -= (sizeCellX/animationMaxIter)*(animationMaxIter-animationCurrIter);
//                            if(lastX > x)
//                                pxlsX += (sizeCellX/animationMaxIter)*(animationMaxIter-animationCurrIter);
//                            if(lastY < y)
//                                pxlsY -= (sizeCellX/animationMaxIter)*(animationMaxIter-animationCurrIter);
//                            if(lastY > y)
//                                pxlsY += (sizeCellX/animationMaxIter)*(animationMaxIter-animationCurrIter);
//                            painter->drawPixmap(pxlsX, pxlsY, localSizeCell + localSpaceCell*2, localSizeCell + localSpaceCell*2, pixmap);
//                            int maxHP = 100;
//                            int hp = units[k]->hp;
//                            float hpWidth = localSizeCell-5;
//                            hpWidth = hpWidth/maxHP*hp;
//                            painter->drawRect(pxlsX + localSpaceCell+2, pxlsY, localSizeCell-4, 10);
//                            painter->fillRect(pxlsX + localSpaceCell+3, pxlsY+1, hpWidth, 9, QColor(Qt::green));
//                            units[k]->coorByMapX = pxlsX;
//                            units[k]->coorByMapY = pxlsY;
//                        } else {
//                            int mainX = mainCoorMapX + isometricCoorX + x*(sizeCellX/2);
//                            int mainY = mainCoorMapY + isometricCoorY + x*(sizeCellY/2);
//                            int pxlsX = mainX - sizeCellX/2;
//                            int pxlsY = mainY - sizeCellY;
//                            if(x > lastX && y > lastY) {
//                                pxlsY -= (sizeCellY/animationMaxIter)*(animationMaxIter-animationCurrIter);
//                            } else if(x == lastX && y > lastY) {
//                                pxlsX += (sizeCellX/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
//                                pxlsY -= (sizeCellY/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
//                            } else if (x < lastX && y > lastY) {
//                                pxlsX += (sizeCellX/animationMaxIter)*(animationMaxIter-animationCurrIter);
//                            } else if (x < lastX && y == lastY) {
//                                pxlsX += (sizeCellX/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
//                                pxlsY += (sizeCellY/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
//                            } else if (x < lastX && y < lastY) {
//                                pxlsY += (sizeCellY/animationMaxIter)*(animationMaxIter-animationCurrIter);
//                            } else if (x == lastX && y < lastY) {
//                                pxlsX -= (sizeCellX/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
//                                pxlsY += (sizeCellY/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
//                            } else if (x > lastX && y < lastY) {
//                                pxlsX -= (sizeCellX/animationMaxIter)*(animationMaxIter-animationCurrIter);
//                            } else if (x > lastX && y == lastY) {
//                                pxlsX -= (sizeCellX/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
//                                pxlsY -= (sizeCellY/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
//                            }
//                            painter->drawPixmap(pxlsX, pxlsY+sizeCellY/2, sizeCellX, sizeCellY*2, pixmap);
//                        }
//                    }
//                }
////            }
//        }
//        isometricCoorX = (getSizeCell()/2) * (map->height - (y+1));
//        isometricCoorY = (getSizeCell()/4) * (y+1);
//    }
//}

void Field::drawGridNavs(CameraController* cameraController) {
    if(drawOrder == 0) {
        for (int y = 0; y < map->height; y++) {
            for (int x = 0; x < map->width; x++) {
                drawGridNavCell(cameraController, x, y);
            }
        }
    } else if(drawOrder == 1) {
        for (int x = 0; x < map->width; x++) {
            for (int y = 0; y < map->height; y++) {
                drawGridNavCell(cameraController, x, y);
            }
        }
    } else if(drawOrder == 2) {
        for (int y = map->height-1; y >= 0; y--) {
            for (int x = map->width-1; x >= 0; x--) {
                drawGridNavCell(cameraController, x, y);
            }
        }
    } else if(drawOrder == 3) {
        for (int x = map->width-1; x >= 0; x--) {
            for (int y = map->height-1; y >= 0; y--) {
                drawGridNavCell(cameraController, x, y);
            }
        }
    } else if(drawOrder == 4) {
        for (int y = map->height-1; y >= 0; y--) {
            for (int x = 0; x < map->width; x++) {
                drawGridNavCell(cameraController, x, y);
            }
        }
    } else if(drawOrder == 5) {
        for (int x = 0; x < map->width; x++) {
            for (int y = map->height-1; y >= 0; y--) {
                drawGridNavCell(cameraController, x, y);
            }
        }
    } else if(drawOrder == 6) {
        for (int y = 0; y < map->height; y++) {
            for (int x = map->width-1; x >= 0; x--) {
                drawGridNavCell(cameraController, x, y);
            }
        }
    } else if(drawOrder == 7) {
        for (int x = map->width-1; x >= 0; x--) {
            for (int y = 0; y < map->height; y++) {
                drawGridNavCell(cameraController, x, y);
            }
        }
    } else if(drawOrder == 8) {
//        qDebug() << "GameField::drawGridNavs(); -- camera.position:" << camera.position;
        int x = 0, y = 0;
        int length = (map->width > map->height) ? map->width : map->height;
        while (x < length) {
            if(x < map->width && y < map->height) {
                if (x == length - 1 && y == length - 1) {
                    drawGridNavCell(cameraController, x, y);
                } else {
                    drawGridNavCell(cameraController, x, y);
                }
            }
            if (x == length - 1) {
                x = y + 1;
                y = length - 1;
            } else if (y == 0) {
                y = x + 1;
                x = 0;
            } else {
                x++;
                y--;
            }
        }
    }
}

void Field::drawGridNavCell(CameraController* cameraController, int cellX, int cellY) {
    Cell* cell = getCell(cellX, cellY);
    if (pathFinder->detectCollision({cellX, cellY})) {
//    foreach (QPixmap textureRegion, cell->backgroundTiles) {
        QPixmap textureRegion = global_pixmap;
        if (isDrawableGridNav == 1 || isDrawableGridNav == 5) {
            cameraController->painter->drawPixmap(cameraController->cameraX+cell->graphicCoordinatesBottom->x()-(cameraController->halfSizeCellX), cameraController->cameraY+cell->graphicCoordinatesBottom->y()-(cameraController->sizeCellY*2), cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
        }
        if (isDrawableGridNav == 2 || isDrawableGridNav == 5) {
            cameraController->painter->drawPixmap(cameraController->cameraX+cell->graphicCoordinatesRight->x()-(cameraController->halfSizeCellX), cameraController->cameraY+cell->graphicCoordinatesRight->y()-(cameraController->sizeCellY*2), cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
        }
        if (isDrawableGridNav == 3 || isDrawableGridNav == 5) {
            cameraController->painter->drawPixmap(cameraController->cameraX+cell->graphicCoordinatesTop->x()-(cameraController->halfSizeCellX), cameraController->cameraY+cell->graphicCoordinatesTop->y()-(cameraController->sizeCellY*2), cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
        }
        if (isDrawableGridNav == 4 || isDrawableGridNav == 5) {
            cameraController->painter->drawPixmap(cameraController->cameraX+cell->graphicCoordinatesLeft->x()-(cameraController->halfSizeCellX), cameraController->cameraY+cell->graphicCoordinatesLeft->y()-(cameraController->sizeCellY*2), cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
        }
//    }
    }
}

//void Field::drawGridNav(CameraController* cameraController) {
//    int isometricCoorX = (cameraController->sizeCellX/2) * map->height;
//    int isometricCoorY = 0;
////    painter->setPen(QColor(255,0,0));
//    for (int y = 0; y < map->height; y++) {
//        for (int x = 0; x < map->width; x++) {
//            if (isometric) {
//                int x2 = cameraController->cameraX + isometricCoorX - (cameraController->sizeCellX/2) + x*(cameraController->sizeCellX/2);
//                int y2 = cameraController->cameraY + isometricCoorY - (cameraController->sizeCellY) + x*(cameraController->sizeCellY/2);
//                if (pathFinder->detectCollision({x, y})) {
//                    cameraController->painter->drawPixmap(x2, y2, cameraController->sizeCellX, cameraController->sizeCellY*2, global_pixmap);
//                }
//            }
//        }
//        isometricCoorX = (cameraController->sizeCellX/2) * (map->height - (y+1));
//        isometricCoorY = (cameraController->sizeCellX/4) * (y+1);
//    }
//}

//void Field::drawPaths(CameraController *cameraController) {
//    int isometricCoorX = (sizeCellX/2) * map->height;
//    int isometricCoorY = 0;
//    cameraController->setPen(QColor(255,0,0));

//    QPixmap pixmapPathPoint;
//    foreach (Unit* tmpUnit, unitsManager->units) {
//        if (tmpUnit == unitsManager->hero.at(0)) {
//            pixmapPathPoint = global_pixmap_PathPoint;
//        } else {
//            pixmapPathPoint = global_pixmap_EnemyPathPoint;
//        }
//        AStar::CoordinateList unitPath = tmpUnit->path;
//        if (!unitPath.empty()) {
//            for (int k = 1; k < unitPath.size(); k++) {
//                AStar::Vec2i point = unitPath[k];
//                isometricCoorX = (getSizeCell()/2) * (map->height - (point.y+0));
//                isometricCoorY = (getSizeCell()/4) * (point.y+0);
//                int pPx = mainCoorMapX + isometricCoorX - (sizeCellX/2) + point.x*(sizeCellX/2);
//                int pPy = mainCoorMapY + isometricCoorY - (sizeCellY) + point.x*(sizeCellY/2);
//                cameraController->drawPixmap(pPx, pPy, sizeCellX, sizeCellY*2, pixmapPathPoint);
//            }
//            AStar::Vec2i destinationPoint = unitPath.front();
//            isometricCoorX = (getSizeCell()/2) * (map->height - (destinationPoint.y+0));
//            isometricCoorY = (getSizeCell()/4) * (destinationPoint.y+0);
//            int destinationPointX = mainCoorMapX + isometricCoorX - (sizeCellX/2) + destinationPoint.x*(sizeCellX/2);
//            int destinationPointY = mainCoorMapY + isometricCoorY - (sizeCellY) + destinationPoint.x*(sizeCellY/2);
//            cameraController->drawPixmap(destinationPointX, destinationPointY, sizeCellX, sizeCellY*2, global_pixmap_DestinationPoint);
//        }
//    }
//}

//void Field::drawTowerUnderConstruction(QPainter* painter, int buildX, int buildY, TemplateForTower *tower) {
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
//                int pxlsX = mainCoorMapX + (buildX+x)*sizeCellX;//+1;
//                int pxlsY = mainCoorMapY + (buildY+y)*sizeCellX;//+1;
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
//                int isometricSpaceX = (map->height-(buildY+y))*(sizeCellX/2);
//                int isometricSpaceY = (buildY+y)*(sizeCellY/2);
//                int pxlsX = mainCoorMapX + isometricSpaceX + (buildX+x)*(sizeCellX/2);
//                int pxlsY = mainCoorMapY + isometricSpaceY + (buildX+x)*(sizeCellY/2);
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
//}

//void Field::setMainCoorMap(int mainCoorMapX, int mainCoorMapY) {
//    this->mainCoorMapX = mainCoorMapX;
//    this->mainCoorMapY = mainCoorMapY;
//}

//void Field::setSizeCell(int sizeCellX) {
//    this->sizeCellX = sizeCellX;
//    this->sizeCellY = sizeCellX/2;
//    this->halfSizeCellX = sizeCellX/2;
//    this->halfSizeCellY = sizeCellY/2;
//}

//int Field::getMainCoorMapX() {
//    return mainCoorMapX;
//}

//int Field::getMainCoorMapY() {
//    return mainCoorMapY;
//}

//int Field::getSizeCell() {
//    return sizeCellX;
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
            if (currX < 0 || currX >= map->width || currY < 0 || currY >= map->height) {
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

//void Field::setMousePress(int x, int y) {
//    mouseX = x;
//    mouseY = y;
//}

//bool Field::getMousePress(int x, int y) {
//    if(mouseX != -1 && mouseY != -1)
//        if((x == mouseX && y == mouseY) || (x == -1 && y == -1))
//            return true;
//    return false;
//}

//bool Field::isSetSpawnPoint(int x, int y) {
//    if(spawnPointX != -1 && spawnPointY != -1)
//        if((x == spawnPointX && y == spawnPointY) || (x == -1 && y == -1))
//            return true;
//    return false;
//}

//bool Field::isSetExitPoint(int x, int y) {
//    if(exitPointX != -1 && exitPointY != -1)
//        if((x == exitPointX && y == exitPointY) || (x == -1 && y == -1))
//            return true;
//    return false;
//}

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

int Field::stepOneUnit(Unit* hero) {
//    Unit* tmpUnit = unitsManager->getUnit(num);
    if(hero->alive) {
        if(hero->animationCurrIter < hero->animationMaxIter) {
            hero->pixmap = hero->activePixmaps[hero->animationCurrIter++];
        } else {
            int currX = hero->coorByCellX;
            int currY = hero->coorByCellY;
            int exitX = currX, exitY = currY;
            if (!hero->path.empty()) {
                AStar::Vec2i point = hero->path.back();
                hero->path.pop_back();
                exitX = point.x;
                exitY = point.y;

            }
            if(hero == unitsManager->hero.at(0)) {
                if (cellExitHero->cellX == currX && cellExitHero->cellY == currY) {
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
                if (hero->path.empty() || getCell(exitX, exitY)->isTerrain()) {
                    int randomX = rand()%map->width;
                    int randomY = rand()%map->height;
                    hero->path = pathFinder->findPath({hero->coorByCellX, hero->coorByCellY}, {randomX, randomY});
                }
            }
            if(exitX != currX || exitY != currY) {
                getCell(currX, currY)->clearUnit(hero);
                hero->lastX = currX;
                hero->lastY = currY;
                hero->coorByCellX = exitX;
                hero->coorByCellY = exitY;
                if(!isometric) {
                    if(exitX < currX && exitY < currY) {
                        hero->animationMaxIter = hero->templateForUnit->walk_up_left.size();
                        hero->activePixmaps = hero->templateForUnit->walk_up_left;
                        hero->direction = Direction::type::UP_LEFT;
                    } else if(exitX == currX && exitY < currY) {
                        hero->animationMaxIter = hero->templateForUnit->walk_up.size();
                        hero->activePixmaps = hero->templateForUnit->walk_up;
                        hero->direction = Direction::UP;
                    } else if(exitX > currX && exitY < currY) {
                        hero->animationMaxIter = hero->templateForUnit->walk_up_right.size();
                        hero->activePixmaps = hero->templateForUnit->walk_up_right;
                        hero->direction = Direction::UP_RIGHT;
                    } else if(exitX < currX && exitY == currY) {
                        hero->animationMaxIter = hero->templateForUnit->walk_left.size();
                        hero->activePixmaps = hero->templateForUnit->walk_left;
                        hero->direction = Direction::LEFT;
                    } else if(exitX > currX && exitY == currY) {
                        hero->animationMaxIter = hero->templateForUnit->walk_right.size();
                        hero->activePixmaps = hero->templateForUnit->walk_right;
                        hero->direction = Direction::RIGHT;
                    } else if(exitX < currX && exitY > currY) {
                        hero->animationMaxIter = hero->templateForUnit->walk_down_left.size();
                        hero->activePixmaps = hero->templateForUnit->walk_down_left;
                        hero->direction = Direction::DOWN_LEFT;
                    } else if(exitX == currX && exitY > currY) {
                        hero->animationMaxIter = hero->templateForUnit->walk_down.size();
                        hero->activePixmaps = hero->templateForUnit->walk_down;
                        hero->direction = Direction::DOWN;
                    } else if(exitX > currX && exitY > currY) {
                        hero->animationMaxIter = hero->templateForUnit->walk_down_right.size();
                        hero->activePixmaps = hero->templateForUnit->walk_down_right;
                        hero->direction = Direction::DOWN_RIGHT;
                    }
                } else {
                    if(exitX < currX && exitY < currY) {
                        hero->animationMaxIter = hero->templateForUnit->walk_up.size();
                        hero->activePixmaps = hero->templateForUnit->walk_up;
                        hero->direction = Direction::UP;
                    } else if(exitX == currX && exitY < currY) {
                        hero->animationMaxIter = hero->templateForUnit->walk_up_right.size();
                        hero->activePixmaps = hero->templateForUnit->walk_up_right;
                        hero->direction = Direction::UP_RIGHT;
                    } else if(exitX > currX && exitY < currY) {
                        hero->animationMaxIter = hero->templateForUnit->walk_right.size();
                        hero->activePixmaps = hero->templateForUnit->walk_right;
                        hero->direction = Direction::RIGHT;
                    } else if(exitX < currX && exitY == currY) {
                        hero->animationMaxIter = hero->templateForUnit->walk_up_left.size();
                        hero->activePixmaps = hero->templateForUnit->walk_up_left;
                        hero->direction = Direction::UP_LEFT;
                    } else if(exitX > currX && exitY == currY) {
                        hero->animationMaxIter = hero->templateForUnit->walk_down_right.size();
                        hero->activePixmaps = hero->templateForUnit->walk_down_right;
                        hero->direction = Direction::DOWN_RIGHT;
                    } else if(exitX < currX && exitY > currY) {
                        hero->animationMaxIter = hero->templateForUnit->walk_left.size();
                        hero->activePixmaps = hero->templateForUnit->walk_left;
                        hero->direction = Direction::LEFT;
                    } else if(exitX == currX && exitY > currY) {
                        hero->animationMaxIter = hero->templateForUnit->walk_down_left.size();
                        hero->activePixmaps = hero->templateForUnit->walk_down_left;
                        hero->direction = Direction::DOWN_LEFT;
                    } else if(exitX > currX && exitY > currY) {
                        hero->animationMaxIter = hero->templateForUnit->walk_down.size();
                        hero->activePixmaps = hero->templateForUnit->walk_down;
                        hero->direction = Direction::DOWN;
                    }
                }
                hero->pixmap = hero->activePixmaps[0];
                hero->animationCurrIter = 0;
                getCell(exitX, exitY)->setUnit(hero);
            } else {
                hero->animationMaxIter = hero->templateForUnit->idle.size();
                hero->activePixmaps = hero->templateForUnit->idle;
                hero->direction = Direction::IDLE;
                hero->pixmap = hero->activePixmaps[0];
            }
        }
    } else if(hero->preDeath) {
        if(hero->animationCurrIter < hero->animationMaxIter) {
            hero->pixmap = hero->activePixmaps[hero->animationCurrIter++];
        } else {
            hero->preDeath = false;
            return 4;
        }
    } else {
        return -2;
    }
    return 0;
}

//int Field::getUnitHpInCell(int x, int y) {
//    if(x >= 0 && x < map->width)
//        if(y >= 0 && y < map->height)
//            if(containUnit(x,y))
//                return unitsManager->getHP(x, y);
//    return 0;
//}

//Unit* Field::getUnitWithLowHP(int x, int y) {
//    if(x >= 0 && x < map->width) {
//        if(y >= 0 && y < map->height) {
//            if(!field[map->width*y + x].units.empty()) {
//                Unit* unit = field[map->width*y + x].units.front();
//                int localHp = unit->hp;
//                int size = field[map->width*y + x].units.size();
//                for(int k = 1; k < size; k++) {
//                    int hp = field[map->width*y + x].units[k]->hp;
//                    if(hp < localHp) {
//                        unit = field[map->width*y + x].units[k];
//                        localHp = unit->hp;
//                    }
//                }
//                return unit;
//            }
//        }
//    }
//    return NULL;
//}

int Field::containUnit(int x, int y, Unit *hero) {
    if(!field[map->width*y + x].units.empty()) {
        int size = field[map->width*y + x].units.size();
        if(hero == NULL) {
            return size;
        } else {
            for(int k = 0; k < size; k++) {
                if(field[map->width*y + x].units[k] == hero) {
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

void Field::spawnHeroInSpawnPoint() {
    qDebug() << "Field::spawnHeroInSpawnPoint(); -- ";
    cellSpawnHero->removeTerrain(true);
    deleteTower(cellSpawnHero->cellX, cellSpawnHero->cellY);
    createUnit(cellSpawnHero->cellX, cellSpawnHero->cellY, cellSpawnHero->cellX, cellSpawnHero->cellY, 1); // player1 = hero
}

void Field::spawnCompUnitToRandomExit(int x, int y) {
    qDebug() << "Field::spawnCompUnitToRandomExit(); -- x:" << x << " y:" << y;
    int randomX = rand()%map->width;
    int randomY = rand()%map->height;
    createUnit(x, y, randomX, randomY, 0);
}

void Field::createUnit(int x, int y, int x2, int y2, int player) {
    qDebug() << "Field::createUnit(); -- x:" << x << " y:" << y << " x2:" << x2 << " y2:" << y2 << " player:" << player;
//    int coorByMapX, coorByMapY;
//    if(!isometric) {
//        coorByMapX = mainCoorMapX + x*sizeCellX;
//        coorByMapY = mainCoorMapY + y*sizeCellX;
//    } else {
//        int isometricCoorX = halfSizeCellX*map->height;
//        int isometricCoorY = halfSizeCellY*y;
//        coorByMapX = mainCoorMapX + isometricCoorX - halfSizeCellX + x*halfSizeCellX;
//        coorByMapY = mainCoorMapY + isometricCoorY - halfSizeCellY*2 + x*halfSizeCellY;
//    }
    if (player == 0) {
        createUnit(QPoint(x, y), QPoint(x2, y2), factionsManager->getRandomTemplateForUnitFromFirstFaction(), player);
    } else if (player == 1) {
        createUnit(QPoint(x, y), QPoint(x2, y2), factionsManager->getTemplateForUnitByName("unit3_footman"), player);
//        updateHeroDestinationPoint(exitPointX, exitPointY);
    }
}

void Field::createUnit(QPoint spawnPoint, QPoint exitPoint, TemplateForUnit* templateForUnit, int player) {
    qDebug() << "Field::createUnit(); -- spawnPoint:" << spawnPoint;
    qDebug() << "Field::createUnit(); -- exitPoint:" << exitPoint;
    qDebug() << "Field::createUnit(); -- templateForUnit:" << templateForUnit->toString(true).toStdString().c_str();
    qDebug() << "Field::createUnit(); -- player:" << player;
    if (!spawnPoint.isNull() && !exitPoint.isNull() && templateForUnit != NULL/* && pathFinder != NULL*/) {
        AStar::CoordinateList path = pathFinder->findPath({spawnPoint.x(), spawnPoint.y()}, {exitPoint.x(), exitPoint.y()});
        qDebug() << "Field::createUnit(); -- path:" << path.size();
        if (!path.empty()) {
            Unit* unit = unitsManager->createUnit(path, templateForUnit, player);
            getCell(spawnPoint.x(), spawnPoint.y())->setUnit(unit);
            qDebug() << "Field::createUnit(); -- unit:" << unit;
        } else {
//            Gdx.app.log("GameField::createUnit()", "-- Not found route for createUnit!");
//            if(towersManager.amountTowers() > 0) {
//                Gdx.app.log("GameField::createUnit()", "-- Remove one last tower! And retry call createUnit()");
//                removeLastTower();
//                createUnit(spawnPoint, exitPoint, templateForUnit, player);
//            }
        }
    } else {
        qDebug() << "GameField::createUnit(); -- Bad spawnPoint:" << spawnPoint << " || exitPoint:" << exitPoint << " || pathFinder:" << pathFinder;
    }
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
