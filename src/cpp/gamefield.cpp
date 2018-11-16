#include "src/head/gamefield.h"

GameField::GameField(QString mapFile, FactionsManager* factionsManager, int enemyCount, int difficultyLevel, int towersCount) {
    qDebug() << "GameField::GameField(); -- mapPath:" << mapFile;
    qDebug() << "GameField::GameField(); -- enemyCount:" << enemyCount;
    qDebug() << "GameField::GameField(); -- towersCount:" << towersCount;
    this->factionsManager = factionsManager;
    this->waveManager = new WaveManager();
    this->towersManager = new TowersManager(difficultyLevel);
    this->unitsManager = new UnitsManager(difficultyLevel);
    this->map = (new MapLoader())->load(mapFile);
    qDebug() << "GameField::GameField(); -- map:" << map;

    global_pixmap = map->tileSets.getTileSet(0)->getLocalTile(8)->getPixmap();
    global_pixmap_PathPoint = map->tileSets.getTileSet(0)->getLocalTile(9)->getPixmap();
    global_pixmap_EnemyPathPoint = map->tileSets.getTileSet(0)->getLocalTile(10)->getPixmap();
    global_pixmap_DestinationPoint = map->tileSets.getTileSet(0)->getLocalTile(20)->getPixmap();
//    global_pixmap_ExitPoint = map->tileSets.getTileSet(0)->getLocalTile(18)->getPixmap();

    underConstruction = NULL;
    greenCheckmark = new QPixmap(ASSETS_PATH + "maps/textures/green_checkmark.png");
    redCross = new QPixmap(ASSETS_PATH + "maps/textures/red_cross.png");
    if (greenCheckmark == NULL || redCross == NULL) {
        qDebug() << "GameField::GameField(); -- Achtung! NOT FOUND 'maps/textures/green_checkmark.png' || 'maps/textures/red_cross.png'";
    }

    createField();
    pathFinder = new AStar::PathFinder();
    pathFinder->setWorldSize({map->width, map->height});
    pathFinder->setHeuristic(AStar::Heuristic::euclidean);
    pathFinder->setDiagonalMovement(false);
    updatePathFinderWalls();
    qDebug() << "GameField::createField(); -- pathFinder:" << pathFinder;
//    int terrainType = rand()%2;
//    if (mapFile.contains("randomMap")) {
//        for (int x = 0; x < map->width; x++) {
//            for (int y = 0; y < map->height; y++) {
//                if( (rand()%100) < 30 ) {
//                    int randNumber = ( 43+(rand()%4) );
//                    Tile* tile = map->getTileSets()->getTileSet(1)->getLocalTile(randNumber);
//                    getCell(x, y)->setTerrain(tile);
//                }
//            }
//        }
//    } else {
//        for (int x = 0; x < map->width; x++) {
//            for (int y = 0; y < map->height; y++) {
//                if ( (rand()%100) < 10 ) {
//                    if (getCell(x, y)->isEmpty()) {
//                        int randNumber = ( 125+(rand()%2) );
//                        Tile* tile = map->getTileSets()->getTileSet(0)->getLocalTile(randNumber);
//                        getCell(x, y)->setTerrain(tile);
//                    }
//                }
//            }
//        }
//    }
    qDebug() << "GameField::GameField(); -- towersCount:" << towersCount;
    for (int k = 0; k < towersCount; k++) {
        int randomX = rand()%map->width;
        int randomY = rand()%map->height;
        createTower(randomX, randomY, factionsManager->getRandomTemplateForTowerFromAllFaction(), 0); // player0 = comp
        qDebug() << "GameField::GameField(); -- randomX:" << randomX << " randomY:" << randomY;
    }
    spawnHeroInSpawnPoint();

    qDebug() << "GameField::GameField(); -- enemyCount:" << enemyCount;
    int randomEnemyCount = enemyCount;
    for (int k = 0; k < randomEnemyCount; k++) {
        int randomX = rand()%map->width;
        int randomY = rand()%map->height;
        if (getCell(randomX, randomY)->isEmpty()) {
            spawnCompUnitToRandomExit(randomX, randomY);
        } else {
            k--;
        }
    }
    qDebug() << "GameField::GameField(); -- map:" << map;

    timeOfGame = 0.0;
    gameSpeed = 1.0;
    gamePaused = false;
//    gamerGold = Integer.valueOf(mapProperties.get("gamerGold", "10000", String.class)); // HARD GAME | one gold = one unit for computer!!!
    gamerGold = 100000;
//    mouseX = -1;
//    mouseY = -1;
//    spawnPointX = -1;
//    spawnPointY = -1;
//    exitPointX = -1;
//    exitPointY = -1;
//    cellExit?

    gameOverLimitUnits = 10;
    currentFinishedUnits = 0;
    qDebug() << "GameField::GameField(); -end- ";
}

GameField::~GameField() {
    qDebug() << "GameField::~GameField(); -- ";
//    delete factionsManager;
    delete waveManager;
    delete towersManager;
    delete unitsManager;
    delete map;
    delete[] field;
    delete pathFinder;

    delete underConstruction;
    delete greenCheckmark;
    delete redCross;

    delete cellSpawnHero;
    delete cellExitHero;

//    delete global_pixmap;
//    delete global_pixmap_PathPoint;
//    delete global_pixmap_EnemyPathPoint;
//    delete global_pixmap_DestinationPoint;
//    delete global_pixmap_ExitPoint;
}

void GameField::createField() {
//    qDebug() << "GameField::createField(); -- map->width:" << map->width << " map->height:" << map->height << " halfSizeCellX:" << halfSizeCellX << " halfSizeCellY:" << halfSizeCellY << " mapLayers:" << mapLayers;
//    qDebug() << "GameField::createField(); -1- field:" << field;
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
                                        cell->backgroundTiles.push_back(tile);
                                    } else if (layerName == "ground" || layerName == "entity") {
                                        cell->setTerrain(tile);
                                    } else if (layerName == "towers") {
                                        cell->removeTerrain(true);
                                        this->createTower(x, y, factionsManager->getRandomTemplateForTowerFromAllFaction(), 0);
                                    } else {
                                        cell->foregroundTiles.push_back(tile);
                                    }
                                }
//                                if (!tile->getProperties()->isEmpty()) {
//                                    qDebug() << "GameField::createField(); -- layerName:" << layerName;
//                                    qDebug() << "GameField::createField(); -- tile->getId():" << tile->getId();
//                                    qDebug() << "GameField::createField(); -- tile->getProperties()->size():" << tile->getProperties()->size();
//                                    qDebug() << "GameField::createField(); -- keys:" << tile->getProperties()->keys();
//                                    qDebug() << "GameField::createField(); -- values:" << tile->getProperties()->values();
//                                }
                                if (tile->getProperties()->contains("spawnPoint")) {
                                    cellSpawnHero = cell;
                                    cellSpawnHero->spawn = true;
                                    qDebug() << "GameField::createField(); -- Set cellSpawnHero:" << cellSpawnHero;
                                } else if (tile->getProperties()->contains("exitPoint")) {
                                    cellExitHero = cell;
                                    cellExitHero->exit = true;
                                    qDebug() << "GameField::createField(); -- Set cellExitHero:" << cellExitHero;
                                }
//                                qDebug() << "GameField::createField(); -7- ";
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
    qDebug() << "GameField::createField(); -2- field:" << field;
}

bool GameField::landscapeGenerator(QString mapFile) {
    qDebug() << "GameField::landscapeGenerator(); -- mapFile:" << mapFile;
    return true;
}

void GameField::turnRight() {
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

void GameField::turnLeft() {
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

void GameField::flipX() {
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

void GameField::flipY() {
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

Cell* GameField::getCell(int x, int y) {
    if (x >= 0 && x < map->width) {
        if (y >= 0 && y < map->height) {
            return &field[map->width*y + x];
        }
    }
    return NULL;
}

void GameField::updateCellsGraphicCoordinates(float halfSizeCellX, float halfSizeCellY) {
    for (int cellX = 0; cellX < map->width; cellX++) {
        for (int cellY = 0; cellY < map->height; cellY++) {
            field[map->width*cellY + cellX].setGraphicCoordinates(cellX, cellY, halfSizeCellX, halfSizeCellY);
        }
    }
}

void GameField::updateTowersGraphicCoordinates(CameraController *cameraController) {
    for (Tower* tower : towersManager->towers) {
        tower->updateGraphicCoordinates(cameraController);
    }
}

void GameField::updateHeroDestinationPoint() {
    qDebug() << "GameField::updateHeroDestinationPoint(); -- ";
//    Unit* hero = unitsManager->hero;
    foreach (Unit* hero, unitsManager->hero) {
        qDebug() << "GameField::updateHeroDestinationPoint(); -- hero:" << hero;
        if (hero != NULL && !hero->route.empty()) {
            updateHeroDestinationPoint(hero->route.front().x, hero->route.front().y);
        } else {
            updatePathFinderWalls();
        }
    }
}

void GameField::updateHeroDestinationPoint(int x, int y) {
    updatePathFinderWalls();
//    Unit* hero = unitsManager->hero;
    foreach (Unit* hero, unitsManager->hero) {
        qDebug() << "GameField::updateHeroDestionPoint(" << x << ", " << y << "); -- ";
        AStar::CoordinateList newPath = pathFinder->findPath(hero->newPosition, {x, y});
        qDebug() << "GameField::updateHeroDestionPoint(); -- newPath.front()" << newPath.front().toString().c_str() << " newPath.back()" << newPath.back().toString().c_str();
        if (newPath.front().equals({x, y})) {
            newPath.pop_back();
            hero->route = newPath;
        } else {
            hero->route.clear();
        }
    }
}

void GameField::updatePathFinderWalls() {
    qDebug() << "GameField::updatePathFinderWalls(); -start- pathFinder->walls.size():" << pathFinder->walls.size();
    pathFinder->clearCollisions();
    for (int x = 0; x < map->width; x++) {
        for (int y = 0; y < map->height; y++) {
            Cell* cell = getCell(x, y);
            if (cell->isTerrain() || cell->getTower() != NULL) {
                cell->isPassable();
                pathFinder->addCollision({x, y});
            }
        }
    }
    qDebug() << "GameField::updatePathFinderWalls(); -end- pathFinder->walls.size():" << pathFinder->walls.size();
}

void GameField::render(float deltaTime, CameraController* cameraController) {
//    qDebug() << "GameField::render(); -- deltaTime:" << deltaTime;
    deltaTime = deltaTime * gameSpeed;
    if (!gamePaused) {
        timeOfGame += deltaTime;
//        spawnUnits(delta);
        stepAllUnits(deltaTime, cameraController);
//        shotAllTowers(deltaTime, cameraController);
//        moveAllShells(delta);

//        if (int result = field->stepAllUnits()) {
//            if(result == 4) {
//                global_text = "Hero contact With Enemy!";
//                signal_closeWidgetGameFinished(false, timeOfGame);
//                return;
//            } else if(result == 3) {
//                global_text = "Hero in ExitPoint!";
//                signal_closeWidgetGameFinished(true, timeOfGame);
//                return;
//            } else if(result == 2) {
//                global_text = "You WIN!";
//            } else if(result == 1) {
//                global_text = "You LOSE!";
//            } else if(result == -1) {
//                if(field->deleteTower()) {
//                    field->waveAlgorithm();
//                } else {
//                    stopTimer_UnitsMoveAndTowerAttack();
//                }
//            }
//        }
    }
//    if (gameStart) {
//        drawFullField(cameraController);
        if (cameraController->isDrawableBackground)
            drawBackGrounds(cameraController);
        if (cameraController->isDrawableGround || cameraController->isDrawableUnits || cameraController->isDrawableTowers) {
            drawGroundsWithUnitsAndTowers(cameraController);
            drawTowersUnderConstruction(cameraController);
        }
        if (cameraController->isDrawableForeground)
            drawForeGrounds(cameraController);

        if (cameraController->isDrawableGrid)
            drawGrid(cameraController);
        if (cameraController->isDrawableGridNav) {
            drawGridNavs(cameraController);
            drawGridNav(cameraController);
        }
        if (cameraController->isDrawableRoutes)
            drawRoutes(cameraController);
//        painter->drawText(10, 20, QString("timeOfGame:%1").arg(timeOfGame));
//    }
}

void GameField::drawFullField(CameraController* cameraController) {
//    qDebug() << "GameField::drawFullField(); -- map:" << map;
//    qDebug() << "GameField::drawFullField(); -- map->tileSets:size" << map->tileSets.size();
//    qDebug() << "GameField::drawFullField(); -- map->tileSets.getTile(85):" << map->tileSets.getTile(85);
//    qDebug() << "GameField::drawFullField(); -- map->tileSets.getTile(85)->getPixmap():" << map->tileSets.getTile(85)->getPixmap();
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

void GameField::drawGrid(CameraController* cameraController) {
    cameraController->painter->setPen(QPen(QColor(100, 60, 21), 1));
    int cameraX = 0;//cameraController->cameraX;
    int cameraY = 0;//cameraController->cameraY;
    float halfSizeCellX = cameraController->halfSizeCellX;
    float halfSizeCellY = cameraController->halfSizeCellY;
//    qDebug() << "GameField::drawGrid(camera); -- cameraX:" << cameraX << " cameraY:" << cameraY << " halfSizeCellX:" << halfSizeCellX << " halfSizeCellY:" << halfSizeCellY;
    int widthForTop = map->height * halfSizeCellX; // A - B
    int heightForTop = map->height * halfSizeCellY; // B - Top
    int widthForBottom = map->width * halfSizeCellX; // A - C
    int heightForBottom = map->height * halfSizeCellY; // C - Bottom
//    qDebug() << "GameField::drawGrid(camera); -- widthForTop:" << widthForTop << " heightForTop:" << heightForTop << " widthForBottom:" << widthForBottom << " heightForBottom:" << heightForBottom;
    if(cameraController->isDrawableGrid == 1 || cameraController->isDrawableGrid == 5) {
        for (int x = 0; x <= map->width; x++)
            cameraController->painter->drawLine(cameraX+(halfSizeCellX*x),cameraY-(halfSizeCellY*x)/*+halfSizeCellY*/,cameraX-(widthForTop)+(halfSizeCellX*x),cameraY-(heightForTop)-(x*halfSizeCellY)/*+halfSizeCellY*/);
        for (int y = 0; y <= map->height; y++)
            cameraController->painter->drawLine(cameraX-(halfSizeCellX*y),cameraY-(halfSizeCellY*y)/*+halfSizeCellY*/,cameraX+(widthForBottom)-(halfSizeCellX*y),cameraY-(heightForBottom)-(halfSizeCellY*y)/*+halfSizeCellY*/);
    }
    if(cameraController->isDrawableGrid == 2 || cameraController->isDrawableGrid == 5) {
        for (int x = 0; x <= map->width; x++)
            cameraController->painter->drawLine(cameraX+(halfSizeCellX*x),cameraY-(halfSizeCellY*x)/*+halfSizeCellY*/,cameraX+(widthForTop)+(halfSizeCellX*x),cameraY+(heightForTop)-(x*halfSizeCellY)/*+halfSizeCellY*/);
        for (int y = 0; y <= map->height; y++)
            cameraController->painter->drawLine(cameraX+(halfSizeCellX*y),cameraY+(halfSizeCellY*y)/*+halfSizeCellY*/,cameraX+(widthForBottom)+(halfSizeCellX*y),cameraY-(heightForBottom)+(halfSizeCellY*y)/*+halfSizeCellY*/);
    }
    if(cameraController->isDrawableGrid == 3 || cameraController->isDrawableGrid == 5) {
        for (int x = 0; x <= map->height; x++) // WHT??? map->height check groundDraw
            cameraController->painter->drawLine(cameraX-(halfSizeCellX*x),cameraY+(halfSizeCellY*x)/*+halfSizeCellY*/,cameraX+(widthForBottom)-(halfSizeCellX*x),cameraY+(heightForBottom)+(x*halfSizeCellY)/*+halfSizeCellY*/);
        for (int y = 0; y <= map->width; y++) // WHT??? map->width check groundDraw
            cameraController->painter->drawLine(cameraX+(halfSizeCellX*y),cameraY+(halfSizeCellY*y)/*+halfSizeCellY*/,cameraX-(widthForTop)+(halfSizeCellX*y),cameraY+(heightForTop)+(halfSizeCellY*y)/*+halfSizeCellY*/);
    }
    if(cameraController->isDrawableGrid == 4 || cameraController->isDrawableGrid == 5) {
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

void GameField::drawBackGrounds(CameraController* cameraController) {
    if(cameraController->drawOrder == 0) {
        for (int y = 0; y < map->height; y++) {
            for (int x = 0; x < map->width; x++) {
                drawBackGroundCell(cameraController, x, y);
            }
        }
    } else if(cameraController->drawOrder == 1) {
        for (int x = 0; x < map->width; x++) {
            for (int y = 0; y < map->height; y++) {
                drawBackGroundCell(cameraController, x, y);
            }
        }
    } else if(cameraController->drawOrder == 2) {
        for (int y = map->height-1; y >= 0; y--) {
            for (int x = map->width-1; x >= 0; x--) {
                drawBackGroundCell(cameraController, x, y);
            }
        }
    } else if(cameraController->drawOrder == 3) {
        for (int x = map->width-1; x >= 0; x--) {
            for (int y = map->height-1; y >= 0; y--) {
                drawBackGroundCell(cameraController, x, y);
            }
        }
    } else if(cameraController->drawOrder == 4) {
        for (int y = map->height-1; y >= 0; y--) {
            for (int x = 0; x < map->width; x++) {
                drawBackGroundCell(cameraController, x, y);
            }
        }
    } else if(cameraController->drawOrder == 5) {
        for (int x = 0; x < map->width; x++) {
            for (int y = map->height-1; y >= 0; y--) {
                drawBackGroundCell(cameraController, x, y);
            }
        }
    } else if(cameraController->drawOrder == 6) {
        for (int y = 0; y < map->height; y++) {
            for (int x = map->width-1; x >= 0; x--) {
                drawBackGroundCell(cameraController, x, y);
            }
        }
    } else if(cameraController->drawOrder == 7) {
        for (int x = map->width-1; x >= 0; x--) {
            for (int y = 0; y < map->height; y++) {
                drawBackGroundCell(cameraController, x, y);
            }
        }
    } else if(cameraController->drawOrder == 8) {
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

void GameField::drawBackGroundCell(CameraController* cameraController, int cellX, int cellY) {
    float deltaX = cameraController->halfSizeCellX;
    float deltaY = cameraController->sizeCellY + cameraController->halfSizeCellY;
    Cell* cell = getCell(cellX, cellY);
    foreach (Tile* tile, cell->backgroundTiles) {
        QPixmap textureRegion = tile->getPixmap();
        if (cameraController->isDrawableBackground == 1 || cameraController->isDrawableBackground == 5) {
            cameraController->painter->drawPixmap(cell->graphicCoordinates1->x-deltaX, cell->graphicCoordinates1->y-deltaY, cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
        }
        if (cameraController->isDrawableBackground == 2 || cameraController->isDrawableBackground == 5) {
            cameraController->painter->drawPixmap(cell->graphicCoordinates2->x-deltaX, cell->graphicCoordinates2->y-deltaY, cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
        }
        if (cameraController->isDrawableBackground == 3 || cameraController->isDrawableBackground == 5) {
            cameraController->painter->drawPixmap(cell->graphicCoordinates3->x-deltaX, cell->graphicCoordinates3->y-deltaY, cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
        }
        if (cameraController->isDrawableBackground == 4 || cameraController->isDrawableBackground == 5) {
            cameraController->painter->drawPixmap(cell->graphicCoordinates4->x-deltaX, cell->graphicCoordinates4->y-deltaY, cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
        }
    }
}

//void GameField::drawBackGround(QPainter* painter) {
//    int isometricCoorX = (sizeCellX/2) * map->height;
//    int isometricCoorY = 0;
//    for(int y = 0; y < map->height; y++) {
//        for(int x = 0; x < map->width; x++) {
//            if(map != NULL) {
//                Cell* cell = getCell(x, y);
////                qDebug() << "GameField::drawFullField(); -- cell->backgroundTiles:" << cell->backgroundTiles.size();
////                    qDebug() << "GameField::drawFullField(); -- pix:" << pix;
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

void GameField::drawGroundsWithUnitsAndTowers(CameraController* cameraController) {
    if(cameraController->drawOrder == 0) {
        for (int y = 0; y < map->height; y++) {
            for (int x = 0; x < map->width; x++) {
                drawGroundCellWithUnitsAndTower(cameraController, x, y);
            }
        }
    } else if(cameraController->drawOrder == 1) {
        for (int x = 0; x < map->width; x++) {
            for (int y = 0; y < map->height; y++) {
                drawGroundCellWithUnitsAndTower(cameraController, x, y);
            }
        }
    } else if(cameraController->drawOrder == 2) {
        for (int y = map->height-1; y >= 0; y--) {
            for (int x = map->width-1; x >= 0; x--) {
                drawGroundCellWithUnitsAndTower(cameraController, x, y);
            }
        }
    } else if(cameraController->drawOrder == 3) {
        for (int x = map->width-1; x >= 0; x--) {
            for (int y = map->height-1; y >= 0; y--) {
                drawGroundCellWithUnitsAndTower(cameraController, x, y);
            }
        }
    } else if(cameraController->drawOrder == 4) {
        for (int y = map->height-1; y >= 0; y--) {
            for (int x = 0; x < map->width; x++) {
                drawGroundCellWithUnitsAndTower(cameraController, x, y);
            }
        }
    } else if(cameraController->drawOrder == 5) {
        for (int x = 0; x < map->width; x++) {
            for (int y = map->height-1; y >= 0; y--) {
                drawGroundCellWithUnitsAndTower(cameraController, x, y);
            }
        }
    } else if(cameraController->drawOrder == 6) {
        for (int y = 0; y < map->height; y++) {
            for (int x = map->width-1; x >= 0; x--) {
                drawGroundCellWithUnitsAndTower(cameraController, x, y);
            }
        }
    } else if(cameraController->drawOrder == 7) {
        for (int x = map->width-1; x >= 0; x--) {
            for (int y = 0; y < map->height; y++) {
                drawGroundCellWithUnitsAndTower(cameraController, x, y);
            }
        }
    } else if(cameraController->drawOrder == 8) {
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

void GameField::drawGroundCellWithUnitsAndTower(CameraController* cameraController, int cellX, int cellY) {
    float deltaX = cameraController->halfSizeCellX;
    float deltaY = cameraController->sizeCellY + cameraController->halfSizeCellY;
    Cell* cell = getCell(cellX, cellY);
    foreach (Tile* tile, cell->groundTiles) {
        QPixmap textureRegion = tile->getPixmap();
        if(cameraController->isDrawableGround == 1 || cameraController->isDrawableGround == 5) {
            cameraController->painter->drawPixmap(cell->graphicCoordinates1->x-deltaX, cell->graphicCoordinates1->y-deltaY, cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
        }
        if(cameraController->isDrawableGround == 2 || cameraController->isDrawableGround == 5) {
            cameraController->painter->drawPixmap(cell->graphicCoordinates2->x-deltaX, cell->graphicCoordinates2->y-deltaY, cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
        }
        if(cameraController->isDrawableGround == 3 || cameraController->isDrawableGround == 5) {
            cameraController->painter->drawPixmap(cell->graphicCoordinates3->x-deltaX, cell->graphicCoordinates3->y-deltaY, cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
        }
        if(cameraController->isDrawableGround == 4 || cameraController->isDrawableGround == 5) {
            cameraController->painter->drawPixmap(cell->graphicCoordinates4->x-deltaX, cell->graphicCoordinates4->y-deltaY, cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
        }
    }
    foreach (Unit* unit, cell->getUnits()) {
        drawUnit(cameraController, unit);
    }
    Tower* tower = cell->getTower();
    if(tower != NULL) {
        drawTower(cameraController, tower);
    }
}

//void GameField::drawGround(QPainter* painter) {
//    int isometricCoorX = (sizeCellX/2) * map->height;
//    int isometricCoorY = 0;
//    for(int y = 0; y < map->height; y++) {
//        for(int x = 0; x < map->width; x++) {
//            Cell* cell = getCell(x, y);
//            if(cell != NULL && cell->isTerrain()) {
////                qDebug() << "GameField::drawGround(); -- cell->terrainTiles:" << cell->terrainTiles.size();
//                foreach (QPixmap pix, cell->terrainTiles) {
//                    if(!isometric) {
//                        int pxlsX = mainCoorMapX + x*sizeCellX;//+1;
//                        int pxlsY = mainCoorMapY + y*sizeCellX;//+1;
//                        int localSizeCell = sizeCellX;//-1;
//                            painter->fillRect(pxlsX+1, pxlsY+1, localSizeCell-1, localSizeCell-1, QColor(0, 0, 0));
//                            painter->drawPixmap(pxlsX, pxlsY, localSizeCell, localSizeCell, pix);
//                    } else {
////                        qDebug() << "GameField::drawGround(); -- pix:" << pix;
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

//void GameField::drawForeGround(QPainter* painter) {
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


void GameField::drawForeGrounds(CameraController* cameraController) {
    if(cameraController->drawOrder == 0) {
        for (int y = 0; y < map->height; y++) {
            for (int x = 0; x < map->width; x++) {
                drawForeGroundCell(cameraController, x, y);
            }
        }
    } else if(cameraController->drawOrder == 1) {
        for (int x = 0; x < map->width; x++) {
            for (int y = 0; y < map->height; y++) {
                drawForeGroundCell(cameraController, x, y);
            }
        }
    } else if(cameraController->drawOrder == 2) {
        for (int y = map->height-1; y >= 0; y--) {
            for (int x = map->width-1; x >= 0; x--) {
                drawForeGroundCell(cameraController, x, y);
            }
        }
    } else if(cameraController->drawOrder == 3) {
        for (int x = map->width-1; x >= 0; x--) {
            for (int y = map->height-1; y >= 0; y--) {
                drawForeGroundCell(cameraController, x, y);
            }
        }
    } else if(cameraController->drawOrder == 4) {
        for (int y = map->height-1; y >= 0; y--) {
            for (int x = 0; x < map->width; x++) {
                drawForeGroundCell(cameraController, x, y);
            }
        }
    } else if(cameraController->drawOrder == 5) {
        for (int x = 0; x < map->width; x++) {
            for (int y = map->height-1; y >= 0; y--) {
                drawForeGroundCell(cameraController, x, y);
            }
        }
    } else if(cameraController->drawOrder == 6) {
        for (int y = 0; y < map->height; y++) {
            for (int x = map->width-1; x >= 0; x--) {
                drawForeGroundCell(cameraController, x, y);
            }
        }
    } else if(cameraController->drawOrder == 7) {
        for (int x = map->width-1; x >= 0; x--) {
            for (int y = 0; y < map->height; y++) {
                drawForeGroundCell(cameraController, x, y);
            }
        }
    } else if(cameraController->drawOrder == 8) {
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

void GameField::drawForeGroundCell(CameraController* cameraController, int cellX, int cellY) {
    float deltaX = cameraController->halfSizeCellX;
    float deltaY = cameraController->sizeCellY + cameraController->halfSizeCellY;
    Cell* cell = getCell(cellX, cellY);
    foreach (Tile* tile, cell->foregroundTiles) {
        QPixmap textureRegion = tile->getPixmap();
        if (cameraController->isDrawableForeground == 1 || cameraController->isDrawableForeground == 5) {
            cameraController->painter->drawPixmap(cell->graphicCoordinates1->x-deltaX, cell->graphicCoordinates1->y-deltaY, cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
        }
        if (cameraController->isDrawableForeground == 2 || cameraController->isDrawableForeground == 5) {
            cameraController->painter->drawPixmap(cell->graphicCoordinates2->x-deltaX, cell->graphicCoordinates2->y-deltaY, cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
        }
        if (cameraController->isDrawableForeground == 3 || cameraController->isDrawableForeground == 5) {
            cameraController->painter->drawPixmap(cell->graphicCoordinates3->x-deltaX, cell->graphicCoordinates3->y-deltaY, cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
        }
        if (cameraController->isDrawableForeground == 4 || cameraController->isDrawableForeground == 5) {
            cameraController->painter->drawPixmap(cell->graphicCoordinates4->x-deltaX, cell->graphicCoordinates4->y-deltaY, cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
        }
    }
}

void GameField::drawUnit(CameraController* cameraController, Unit* unit) {
//    qDebug() << "GameField::drawUnit(); -- cameraController:" << cameraController << " unit:" << unit;
//    for (ShellEffectType shellAttackType : unit.shellEffectTypes) {
//        if(shellAttackType.shellEffectEnum == ShellEffectType.ShellEffectEnum.FreezeEffect) {
//            spriteBatch.setColor(0.0f, 0.0f, 1.0f, 0.9f);
//            // Gdx.app.log("GameField::drawUnit(" + unit + "," + spriteBatch + ")", "-- FreezeEffect!");
//        }
//        if(shellAttackType.shellEffectEnum == ShellEffectType.ShellEffectEnum.FireEffect) {
//            spriteBatch.setColor(1.0f, 0.0f, 0.0f, 0.9f);
//            // Gdx.app.log("GameField::drawUnit(" + unit + "," + spriteBatch + ")", "-- FireEffect!");
//        }
//    }
    QPixmap currentFrame;
    if (unit->isAlive()) {
        currentFrame = unit->getCurrentFrame();
    } else {
        currentFrame = unit->getCurrentDeathFrame();
    }
    int deltaX = cameraController->halfSizeCellX;
    int deltaY = cameraController->sizeCellY + cameraController->halfSizeCellY;

    float fVx = 0;//cameraController->cameraX;
    float fVy = 0;//cameraController->cameraY;
//    qDebug() << "GameField::drawUnit(); -- unit->circle1->x :" << unit->circle1->x  << " unit->circle1->y:" << unit->circle1->y;
    if(cameraController->isDrawableUnits == 1 || cameraController->isDrawableUnits == 5) {
        fVx += unit->circle1->x - deltaX;
        fVy += unit->circle1->y - deltaY;
        cameraController->painter->drawEllipse(unit->circle1->getPosition()->getPointF(), unit->circle1->radius, unit->circle1->radius);
        cameraController->painter->drawPixmap(fVx, fVy, cameraController->sizeCellX, cameraController->sizeCellY*2, currentFrame);
    }
    if(cameraController->isDrawableUnits == 2 || cameraController->isDrawableUnits == 5) {
        fVx += unit->circle2->x - deltaX;
        fVy += unit->circle2->y - deltaY;
        cameraController->painter->drawEllipse(unit->circle2->getPosition()->getPointF(), unit->circle2->radius, unit->circle2->radius);
        cameraController->painter->drawPixmap(fVx, fVy, cameraController->sizeCellX, cameraController->sizeCellY*2, currentFrame);
    }
    if(cameraController->isDrawableUnits == 3 || cameraController->isDrawableUnits == 5) {
        fVx += unit->circle3->x - deltaX;
        fVy += unit->circle3->y - deltaY;
        cameraController->painter->drawEllipse(unit->circle3->getPosition()->getPointF(), unit->circle3->radius, unit->circle3->radius);
        cameraController->painter->drawPixmap(fVx, fVy, cameraController->sizeCellX, cameraController->sizeCellY*2, currentFrame);
    }
    if(cameraController->isDrawableUnits == 4 || cameraController->isDrawableUnits == 5) {
        fVx += unit->circle4->x - deltaX;
        fVy += unit->circle4->y - deltaY;
        cameraController->painter->drawEllipse(unit->circle4->getPosition()->getPointF(), unit->circle4->radius, unit->circle4->radius);
        cameraController->painter->drawPixmap(fVx, fVy, cameraController->sizeCellX, cameraController->sizeCellY*2, currentFrame);
    }
//    qDebug() << "GameField::drawUnit(); -- fVx:" << fVx << " fVy:" << fVy;
//        drawUnitBar(shapeRenderer, unit, currentFrame, fVx, fVy);
}

void GameField::drawTower(CameraController* cameraController, Tower* tower) {
    QPoint* cellPosition = tower->position;
    int towerSize = tower->templateForTower->size;
    Cell* cell = getCell(cellPosition->x(), cellPosition->y());
    Vector2* towerPos = new Vector2(cell->getGraphicCoordinates(cameraController->isDrawableTowers));
    cameraController->painter->drawEllipse(towerPos->getPointF(), 3, 3);
//    Vector2 *towerPos = new Vector2();
    QPixmap currentFrame = tower->templateForTower->idleTile->getPixmap();
    if (cameraController->isDrawableTowers == 5) {
        for (int m = 1; m < cameraController->isDrawableTowers; m++) {
            towerPos->set(cell->getGraphicCoordinates(m));
            cameraController->getCorrectGraphicTowerCoord(towerPos, towerSize, m);
            cameraController->painter->drawPixmap(towerPos->x, towerPos->y, cameraController->sizeCellX * towerSize, (cameraController->sizeCellY * 2) * towerSize, currentFrame);
//            towerPos->operator +=(QPointF(cameraController->cameraX, cameraController->cameraY));
//            qDebug() << "GameField::drawTower(); -- towerPos:" << towerPos << " radius:" << tower->radiusDetectionCircle->radius;
//            cameraController->painter->drawEllipse(*towerPos, tower->radiusDetectionCircle->radius, tower->radiusDetectionCircle->radius);
        }
    } else if (cameraController->isDrawableTowers != 0) {
        towerPos->set(cell->getGraphicCoordinates(cameraController->isDrawableTowers));
        cameraController->getCorrectGraphicTowerCoord(towerPos, towerSize, cameraController->isDrawableTowers);
        cameraController->painter->drawPixmap(towerPos->x, towerPos->y, cameraController->sizeCellX * towerSize, (cameraController->sizeCellY * 2) * towerSize, currentFrame);
//        towerPos->operator +=(QPointF(cameraController->cameraX, cameraController->cameraY));
//        qDebug() << "GameField::drawTower(); -- towerPos:" << towerPos << " radius:" << tower->radiusDetectionCircle->radius;
//        cameraController->painter->drawEllipse(*towerPos, tower->radiusDetectionCircle->radius, tower->radiusDetectionCircle->radius);
//        cameraController->painter->drawEllipse(tower->radiusDetectionCircle->getPosition()->getPointF(), tower->radiusDetectionCircle->radius, tower->radiusDetectionCircle->radius);
    }
    delete towerPos; // towerPos = NULL;
}

//void GameField::drawUnits(QPainter* painter) {
//    int isometricCoorX = (sizeCellX/2) * map->height;
//    int isometricCoorY = 0;
//    for(int y = 0; y < map->height; y++) {
//        for(int x = 0; x < map->width; x++) {
////            int num = containUnit(x, y);
////            if(num) {
//                std::vector<Unit*> units = getCell(x, y)->getUnits();
//                int size = units.size();
//                qDebug() << "GameField::drawUnits(); -- size:" << size;
//                for(int k = 0; k < size; k++) {
//                    if(units[k]->alive || units[k]->preDeath) {// fixed!!!
//                        int lastX, lastY;
//                        int animationCurrIter, animationMaxIter;
//                        QPixmap pixmap = units[k]->getAnimationInformation(&lastX, &lastY, &animationCurrIter, &animationMaxIter);
//                        qDebug() << "GameField::drawUnits(); -- isometric:" << isometric;
//                        qDebug() << "GameField::drawUnits(); -- pixmap:" << pixmap;
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

//void GameField::drawTowersByTowers(QPainter* painter) {
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

void GameField::drawGridNav(CameraController *cameraController) {
//    QPointF pos;
//    Vector2* pos = new Vector2();
    float gridNavRadius = cameraController->sizeCellX/20;
    for (int y = 0; y < cameraController->mapHeight; y++) {
        for (int x = 0; x < cameraController->mapWidth; x++) {
            Cell *cell = getCell(x, y);
            if (cell != NULL && !cell->isEmpty()) {
                if (cell->isTerrain()) {
                    cameraController->painter->setBrush(QColor(255, 0, 0));
                } else if (cell->getUnit() != NULL) {
                    cameraController->painter->setBrush(QColor(0, 255, 0));
                } else if (cell->getTower() != NULL) {
                    cameraController->painter->setBrush(QColor(255, 255, 0));
                }
                if (cameraController->isDrawableGridNav == 1 || cameraController->isDrawableGridNav == 5) {
//                    pos->set(cell->getGraphicCoordinates(1));
//                    shapeRenderer.circle(pos.x, pos.y, gridNavRadius);
                    cameraController->painter->drawEllipse(cell->getGraphicCoordinates(1)->getPointF(), gridNavRadius, gridNavRadius);
                }
                if(cameraController->isDrawableGridNav == 2 || cameraController->isDrawableGridNav == 5) {
//                    pos.set(cell.getGraphicCoordinates(2));
//                    shapeRenderer.circle(pos.x, pos.y, gridNavRadius);
                    cameraController->painter->drawEllipse(cell->getGraphicCoordinates(2)->getPointF(), gridNavRadius, gridNavRadius);
                }
                if(cameraController->isDrawableGridNav == 3 || cameraController->isDrawableGridNav == 5) {
//                    pos.set(cell.getGraphicCoordinates(3));
//                    shapeRenderer.circle(pos.x, pos.y, gridNavRadius);
                    cameraController->painter->drawEllipse(cell->getGraphicCoordinates(3)->getPointF(), gridNavRadius, gridNavRadius);
                }
                if(cameraController->isDrawableGridNav == 4 || cameraController->isDrawableGridNav == 5) {
//                    pos.set(cell.getGraphicCoordinates(4));
//                    shapeRenderer.circle(pos.x, pos.y, gridNavRadius);
                    cameraController->painter->drawEllipse(cell->getGraphicCoordinates(4)->getPointF(), gridNavRadius, gridNavRadius);
                }
            }
        }
    }

//    Array<GridPoint2> spawnPoints = waveManager.getAllSpawnPoint();
//    shapeRenderer.setColor(new Color(0f, 255f, 204f, 255f));
//    for (GridPoint2 spawnPoint : spawnPoints) {
//        Cell cell = field[spawnPoint.x][spawnPoint.y];
//        if(cameraController->isDrawableGridNav == 1 || cameraController->isDrawableGridNav == 5) {
//            pos.set(cell.getGraphicCoordinates(1));
//            shapeRenderer.circle(pos.x, pos.y, gridNavRadius);
//        }
//        if(cameraController->isDrawableGridNav == 2 || cameraController->isDrawableGridNav == 5) {
//            pos.set(cell.getGraphicCoordinates(2));
//            shapeRenderer.circle(pos.x, pos.y, gridNavRadius);
//        }
//        if(cameraController->isDrawableGridNav == 3 || cameraController->isDrawableGridNav == 5) {
//            pos.set(cell.getGraphicCoordinates(3));
//            shapeRenderer.circle(pos.x, pos.y, gridNavRadius);
//        }
//        if(cameraController->isDrawableGridNav == 4 || cameraController->isDrawableGridNav == 5) {
//            pos.set(cell.getGraphicCoordinates(4));
//            shapeRenderer.circle(pos.x, pos.y, gridNavRadius);
//        }
//    }

//    Array<GridPoint2> exitPoints = waveManager.getAllExitPoint();
//    shapeRenderer.setColor(new Color(255f, 0f, 102f, 255f));
//    for (GridPoint2 exitPoint : exitPoints) {
////            Gdx.app.log("GameField::drawGridNav()", "-- exitPoint.x:" + exitPoint.x + " exitPoint.y:" + exitPoint.y + " cameraController->isDrawableGridNav:" + cameraController->isDrawableGridNav);
//        Cell cell = field[exitPoint.x][exitPoint.y];
//        if(cameraController->isDrawableGridNav == 1 || cameraController->isDrawableGridNav == 5) {
//            pos.set(cell.getGraphicCoordinates(1));
//            shapeRenderer.circle(pos.x, pos.y, gridNavRadius);
//        }
//        if(cameraController->isDrawableGridNav == 2 || cameraController->isDrawableGridNav == 5) {
//            pos.set(cell.getGraphicCoordinates(2));
//            shapeRenderer.circle(pos.x, pos.y, gridNavRadius);
//        }
//        if(cameraController->isDrawableGridNav == 3 || cameraController->isDrawableGridNav == 5) {
//            pos.set(cell.getGraphicCoordinates(3));
//            shapeRenderer.circle(pos.x, pos.y, gridNavRadius);
//        }
//        if(cameraController->isDrawableGridNav == 4 || cameraController->isDrawableGridNav == 5) {
//            pos.set(cell.getGraphicCoordinates(4));
//            shapeRenderer.circle(pos.x, pos.y, gridNavRadius);
//        }
//    }

//    shapeRenderer.setColor(Color.ORANGE);
//    for (Tower tower : towersManager.getAllTowers()) {
//        for (Bullet bullet : tower.bullets) {
//            shapeRenderer.rectLine(bullet.currentPoint.x, bullet.currentPoint.y, bullet.endPoint.x, bullet.endPoint.y, sizeCellX/40f);
//            if (null != bullet.circle) {
//                shapeRenderer.circle(bullet.circle.x, bullet.circle.y, bullet.circle.radius);
//            }
//        }
//    }
//    shapeRenderer.end();

//    shapeRenderer.begin(ShapeRenderer.ShapeType.Line);
//    shapeRenderer.setColor(Color.RED);
//    for (Unit unit : unitsManager.units) {
//        if(isDrawableUnits == 1 || isDrawableUnits == 5)
//            shapeRenderer.circle(unit.circle1.x, unit.circle1.y, unit.circle1.radius);
//        if(isDrawableUnits == 2 || isDrawableUnits == 5)
//            shapeRenderer.circle(unit.circle2.x, unit.circle2.y, unit.circle2.radius);
//        if(isDrawableUnits == 3 || isDrawableUnits == 5)
//            shapeRenderer.circle(unit.circle3.x, unit.circle3.y, unit.circle3.radius);
//        if(isDrawableUnits == 4 || isDrawableUnits == 5)
//            shapeRenderer.circle(unit.circle4.x, unit.circle4.y, unit.circle4.radius);
//    }

//    shapeRenderer.setColor(new Color(153f, 255f, 51f, 255f));
//    Vector2 towerPos = new Vector2();
//    for (Tower tower : towersManager.getAllTowers()) { // Draw white towers radius! -- radiusDetectionCircle
//        if(cameraController->isDrawableGridNav == 5) {
//            if(isDrawableTowers == 5) {
//                for (int m = 1; m < isDrawableTowers; m++) {
//                    towerPos.set(tower.getCenterGraphicCoord(m)); // Need recoding this func!
//                    shapeRenderer.circle(towerPos.x, towerPos.y, tower.radiusDetectionCircle.radius);
//                }
//            } else if(isDrawableTowers != 0) {
//                towerPos.set(tower.getCenterGraphicCoord(isDrawableTowers));
//                shapeRenderer.circle(towerPos.x, towerPos.y, tower.radiusDetectionCircle.radius);
//            }
//        } else /*if(cameraController->isDrawableGridNav != 0)*/ {
//            if(cameraController->isDrawableGridNav == isDrawableTowers) {
//                towerPos.set(tower.getCenterGraphicCoord(isDrawableTowers));
//                shapeRenderer.circle(towerPos.x, towerPos.y, tower.radiusDetectionCircle.radius);
//            }
//        }
//    }

//    shapeRenderer.setColor(Color.FIREBRICK);
//    for (Tower tower : towersManager.getAllTowers()) { // Draw FIREBRICK towers radius! -- radiusFlyShellCircle
//        if (tower.radiusFlyShellCircle != null) {
//            if(cameraController->isDrawableGridNav == 5) {
//                if(isDrawableTowers == 5) {
//                    for (int m = 1; m <= isDrawableTowers; m++) {
//                        towerPos.set(tower.getCenterGraphicCoord(m)); // Need recoding this func!
//                        shapeRenderer.circle(towerPos.x, towerPos.y, tower.radiusFlyShellCircle.radius);
//                    }
//                } else {
//                    towerPos.set(tower.getCenterGraphicCoord(isDrawableTowers));
//                    shapeRenderer.circle(towerPos.x, towerPos.y, tower.radiusFlyShellCircle.radius);
//                }
//            } else {
//                if(cameraController->isDrawableGridNav == isDrawableTowers) {
//                    towerPos.set(tower.getCenterGraphicCoord(isDrawableTowers));
//                    shapeRenderer.circle(towerPos.x, towerPos.y, tower.radiusFlyShellCircle.radius);
//                }
//            }
//        }
//    }
//    shapeRenderer.end();

//    spriteBatch.begin();
//    bitmapFont.setColor(Color.YELLOW);
//    bitmapFont.getData().setScale(0.7f);
//    for (Tower tower : towersManager.getAllTowers()) { // Draw pit capacity value
//        if (tower.templateForTower.towerAttackType == TowerAttackType.Pit) {
//            if(cameraController->isDrawableGridNav == 5) {
//                if(isDrawableTowers == 5) {
//                    for (int m = 1; m <= isDrawableTowers; m++) {
//                        towerPos.set(tower.getCenterGraphicCoord(m)); // Need recoding this func!
//                        bitmapFont.draw(spriteBatch, String.valueOf(tower.capacity), towerPos.x, towerPos.y);
//                    }
//                } else {
//                    towerPos.set(tower.getCenterGraphicCoord(isDrawableTowers));
//                    bitmapFont.draw(spriteBatch, String.valueOf(tower.capacity), towerPos.x, towerPos.y);
//                }
//            } else {
//                if(cameraController->isDrawableGridNav == isDrawableTowers) {
//                    towerPos.set(tower.getCenterGraphicCoord(isDrawableTowers));
//                    bitmapFont.draw(spriteBatch, String.valueOf(tower.capacity), towerPos.x, towerPos.y);
//                }
//            }
//        }
//    }
}

void GameField::drawGridNavs(CameraController* cameraController) {
    if(cameraController->drawOrder == 0) {
        for (int y = 0; y < map->height; y++) {
            for (int x = 0; x < map->width; x++) {
                drawGridNavCell(cameraController, x, y);
            }
        }
    } else if(cameraController->drawOrder == 1) {
        for (int x = 0; x < map->width; x++) {
            for (int y = 0; y < map->height; y++) {
                drawGridNavCell(cameraController, x, y);
            }
        }
    } else if(cameraController->drawOrder == 2) {
        for (int y = map->height-1; y >= 0; y--) {
            for (int x = map->width-1; x >= 0; x--) {
                drawGridNavCell(cameraController, x, y);
            }
        }
    } else if(cameraController->drawOrder == 3) {
        for (int x = map->width-1; x >= 0; x--) {
            for (int y = map->height-1; y >= 0; y--) {
                drawGridNavCell(cameraController, x, y);
            }
        }
    } else if(cameraController->drawOrder == 4) {
        for (int y = map->height-1; y >= 0; y--) {
            for (int x = 0; x < map->width; x++) {
                drawGridNavCell(cameraController, x, y);
            }
        }
    } else if(cameraController->drawOrder == 5) {
        for (int x = 0; x < map->width; x++) {
            for (int y = map->height-1; y >= 0; y--) {
                drawGridNavCell(cameraController, x, y);
            }
        }
    } else if(cameraController->drawOrder == 6) {
        for (int y = 0; y < map->height; y++) {
            for (int x = map->width-1; x >= 0; x--) {
                drawGridNavCell(cameraController, x, y);
            }
        }
    } else if(cameraController->drawOrder == 7) {
        for (int x = map->width-1; x >= 0; x--) {
            for (int y = 0; y < map->height; y++) {
                drawGridNavCell(cameraController, x, y);
            }
        }
    } else if(cameraController->drawOrder == 8) {
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

void GameField::drawGridNavCell(CameraController* cameraController, int cellX, int cellY) {
    float deltaX = cameraController->halfSizeCellX;
    float deltaY = cameraController->sizeCellY + cameraController->halfSizeCellY;
    Cell* cell = getCell(cellX, cellY);
    if (pathFinder->detectCollision({cellX, cellY})) {
//    foreach (QPixmap textureRegion, cell->backgroundTiles) {
        QPixmap textureRegion = global_pixmap;
        if (cameraController->isDrawableGridNav == 1 || cameraController->isDrawableGridNav == 5) {
            cameraController->painter->drawPixmap(cell->graphicCoordinates1->x-deltaX, cell->graphicCoordinates1->y-deltaY, cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
        }
        if (cameraController->isDrawableGridNav == 2 || cameraController->isDrawableGridNav == 5) {
            cameraController->painter->drawPixmap(cell->graphicCoordinates2->x-deltaX, cell->graphicCoordinates2->y-deltaY, cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
        }
        if (cameraController->isDrawableGridNav == 3 || cameraController->isDrawableGridNav == 5) {
            cameraController->painter->drawPixmap(cell->graphicCoordinates3->x-deltaX, cell->graphicCoordinates3->y-deltaY, cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
        }
        if (cameraController->isDrawableGridNav == 4 || cameraController->isDrawableGridNav == 5) {
            cameraController->painter->drawPixmap(cell->graphicCoordinates4->x-deltaX, cell->graphicCoordinates4->y-deltaY, cameraController->sizeCellX, cameraController->sizeCellY*2, textureRegion);
        }
//    }
    }
}

//void GameField::drawGridNav(CameraController* cameraController) {
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

void GameField::drawRoutes(CameraController *cameraController) {
    float deltaX = cameraController->halfSizeCellX;
    float deltaY = cameraController->sizeCellY + cameraController->halfSizeCellY;
    QPixmap pixmapPathPoint;
    foreach (Unit* tmpUnit, unitsManager->units) {
        if (tmpUnit == unitsManager->hero.at(0)) {
            pixmapPathPoint = global_pixmap_PathPoint;
        } else {
            pixmapPathPoint = global_pixmap_EnemyPathPoint;
        }
        AStar::CoordinateList unitRoute = tmpUnit->route;
        if (!unitRoute.empty()) {
            for (int k = 1; k < unitRoute.size(); k++) {
                AStar::Vec2i point = unitRoute[k];
                Cell* cell = getCell(point.x, point.y);
                if (cameraController->isDrawableRoutes == 1 || cameraController->isDrawableRoutes == 5) {
                    cameraController->painter->drawPixmap(cell->graphicCoordinates1->x-deltaX, cell->graphicCoordinates1->y-deltaY, cameraController->sizeCellX, cameraController->sizeCellY*2, pixmapPathPoint);
                }
                if (cameraController->isDrawableRoutes == 2 || cameraController->isDrawableRoutes == 5) {
                    cameraController->painter->drawPixmap(cell->graphicCoordinates2->x-deltaX, cell->graphicCoordinates2->y-deltaY, cameraController->sizeCellX, cameraController->sizeCellY*2, pixmapPathPoint);
                }
                if (cameraController->isDrawableRoutes == 3 || cameraController->isDrawableRoutes == 5) {
                    cameraController->painter->drawPixmap(cell->graphicCoordinates3->x-deltaX, cell->graphicCoordinates3->y-deltaY, cameraController->sizeCellX, cameraController->sizeCellY*2, pixmapPathPoint);
                }
                if (cameraController->isDrawableRoutes == 4 || cameraController->isDrawableRoutes == 5) {
                    cameraController->painter->drawPixmap(cell->graphicCoordinates4->x-deltaX, cell->graphicCoordinates4->y-deltaY, cameraController->sizeCellX, cameraController->sizeCellY*2, pixmapPathPoint);
                }
            }
            AStar::Vec2i destinationPoint = unitRoute.front();
            Cell* cell = getCell(destinationPoint.x, destinationPoint.y);
            if (cameraController->isDrawableRoutes == 1 || cameraController->isDrawableRoutes == 5) {
                cameraController->painter->drawPixmap(cell->graphicCoordinates1->x-deltaX, cell->graphicCoordinates1->y-deltaY, cameraController->sizeCellX, cameraController->sizeCellY*2, global_pixmap_DestinationPoint);
            }
            if (cameraController->isDrawableRoutes == 2 || cameraController->isDrawableRoutes == 5) {
                cameraController->painter->drawPixmap(cell->graphicCoordinates2->x-deltaX, cell->graphicCoordinates2->y-deltaY, cameraController->sizeCellX, cameraController->sizeCellY*2, global_pixmap_DestinationPoint);
            }
            if (cameraController->isDrawableRoutes == 3 || cameraController->isDrawableRoutes == 5) {
                cameraController->painter->drawPixmap(cell->graphicCoordinates3->x-deltaX, cell->graphicCoordinates3->y-deltaY, cameraController->sizeCellX, cameraController->sizeCellY*2, global_pixmap_DestinationPoint);
            }
            if (cameraController->isDrawableRoutes == 4 || cameraController->isDrawableRoutes == 5) {
                cameraController->painter->drawPixmap(cell->graphicCoordinates4->x-deltaX, cell->graphicCoordinates4->y-deltaY, cameraController->sizeCellX, cameraController->sizeCellY*2, global_pixmap_DestinationPoint);
            }
        }
    }
}

//void GameField::drawRoutes(CameraController *cameraController) {
//    int isometricCoorX = (cameraController->sizeCellX/2) * map->height;
//    int isometricCoorY = 0;
//    cameraController->painter->setPen(QColor(255,0,0));
//    QPixmap pixmapPathPoint;
//    foreach (Unit* tmpUnit, unitsManager->units) {
//        if (tmpUnit == unitsManager->hero.at(0)) {
//            pixmapPathPoint = global_pixmap_PathPoint;
//        } else {
//            pixmapPathPoint = global_pixmap_EnemyPathPoint;
//        }
//        AStar::CoordinateList unitRoute = tmpUnit->route;
//        if (!unitRoute.empty()) {
//            for (int k = 1; k < unitRoute.size(); k++) {
//                AStar::Vec2i point = unitRoute[k];
//                isometricCoorX = (cameraController->sizeCellX/2) * (map->height - (point.y+0));
//                isometricCoorY = (cameraController->sizeCellY/2) * (point.y+0);
//                int pPx = cameraController->cameraX + isometricCoorX - (cameraController->sizeCellX/2) + point.x*(cameraController->sizeCellX/2);
//                int pPy = cameraController->cameraY + isometricCoorY - (cameraController->sizeCellY) + point.x*(cameraController->sizeCellY/2);
//                cameraController->painter->drawPixmap(pPx, pPy, cameraController->sizeCellX, cameraController->sizeCellY*2, pixmapPathPoint);
//            }
//            AStar::Vec2i destinationPoint = unitRoute.front();
//            isometricCoorX = (cameraController->sizeCellX/2) * (map->height - (destinationPoint.y+0));
//            isometricCoorY = (cameraController->sizeCellY/2) * (destinationPoint.y+0);
//            int destinationPointX = cameraController->cameraX + isometricCoorX - (cameraController->sizeCellX/2) + destinationPoint.x*(cameraController->sizeCellX/2);
//            int destinationPointY = cameraController->cameraY + isometricCoorY - (cameraController->sizeCellY) + destinationPoint.x*(cameraController->sizeCellY/2);
//            cameraController->painter->drawPixmap(destinationPointX, destinationPointY, cameraController->sizeCellX, cameraController->sizeCellY*2, global_pixmap_DestinationPoint);
//        }
//    }
//}

void GameField::drawTowersUnderConstruction(CameraController* cameraController) {
//    qDebug() << "GameField::drawTowersUnderConstruction(); -- underConstruction:" << underConstruction;
    if (underConstruction != NULL) {
//        qDebug() << "GameField::drawTowersUnderConstruction(); -- underConstruction.templateForTower:" << underConstruction->templateForTower->toString(true).toStdString().c_str();
        int goldNeed = underConstruction->templateForTower->cost;
        bool enoughGold = (gamerGold >= goldNeed) ? true : false;
//        qDebug() << "GameField::drawTowersUnderConstruction(); -- underConstruction->state:" << underConstruction->state;
        if (underConstruction->state == 0) {
            drawTowerUnderConstruction(cameraController, underConstruction->endX, underConstruction->endY, underConstruction->templateForTower, enoughGold);
        } else if (underConstruction->state == 1) {
            drawTowerUnderConstruction(cameraController, underConstruction->startX, underConstruction->startY, underConstruction->templateForTower, enoughGold);
            for (int k = 0; k < underConstruction->coorsX.size(); k++) {
                goldNeed += underConstruction->templateForTower->cost;
                enoughGold = (gamerGold >= goldNeed) ? true : false;
                drawTowerUnderConstruction(cameraController, underConstruction->coorsX.at(k), underConstruction->coorsY.at(k), underConstruction->templateForTower, enoughGold);
            }
        }
    }
}

void GameField::drawTowerUnderConstruction(CameraController* cameraController, int buildX, int buildY, TemplateForTower* templateForTower, bool enoughGold) {
//    qDebug() << "GameField::drawTowerUnderConstruction(); -- buildX:" << buildX << " buildY:" << buildY << " templateForTower:" << templateForTower->toString(true).toStdString().c_str() << " enoughGold:" << enoughGold;
    bool drawFull = true;
    bool canBuild = true;
    int towerSize = templateForTower->size;
    int startX = 0, startY = 0, finishX = 0, finishY = 0;
//    qDebug() << "GameField::drawTowerUnderConstruction(); -0- towerSize:" << towerSize;
    if (towerSize != 1) {
        // Нижняя карта
        if (towerSize % 2 == 0) {
            startX = -(towerSize / 2);
            startY = -(towerSize / 2);
            finishX = (towerSize / 2)-1;
            finishY = (towerSize / 2)-1;
        } else {
            startX = -(towerSize / 2);
            startY = -(towerSize / 2);
            finishX = (towerSize / 2);
            finishY = (towerSize / 2);
        }
        // Правая карта
//            if (towerSize % 2 == 0) {
//                startX = -(towerSize / 2);
//                startY = -((towerSize / 2) - 1);
//                finishX = ((towerSize / 2) - 1);
//                finishY = (towerSize / 2);
//            } else {
//                startX = -(towerSize / 2);
//                startY = -(towerSize / 2);
//                finishX = (towerSize / 2);
//                finishY = (towerSize / 2);
//            }
    }
//    qDebug() << "GameField::drawTowerUnderConstruction(); -1- startX:" << startX << " startY:" << startY;
    QPoint startDrawCell(startX, startY);
    QPoint finishDrawCell(finishX, finishY);
//    qDebug() << "GameField::drawTowerUnderConstruction(); -2- finishX:" << finishX << " finishY:" << finishY;
    for (int x = startX; x <= finishX; x++) {
        for (int y = startY; y <= finishY; y++) {
            Cell* cell = getCell(buildX + x, buildY + y);
            if (cell != NULL) {
                if (!cell->isEmpty()) {
                    if (drawFull) {
                        canBuild = false;
                    }
                }
            }
        }
    }
//    qDebug() << "GameField::drawTowerUnderConstruction(); -3- canBuild:" << canBuild;
//    qDebug() << "GameField::drawTowerUnderConstruction(); -3- drawFull:" << drawFull;
    if (drawFull) {
        Cell* mainCell = getCell(buildX, buildY);
//        qDebug() << "GameField::drawTowerUnderConstruction(); -4- mainCell:" << mainCell;
        if(mainCell != NULL) {
//            cameraController->painter->save();
//            cameraController->painter->setCompositionMode(QPainter::CompositionMode_SourceIn);
            if (enoughGold && canBuild) {
//                cameraController->painter->setBrush(QColor(0, 255, 0));
//                cameraController->painter->setPen(QColor(0, 255, 0, 80));
            } else {
//                cameraController->painter->setBrush(QColor(255, 0, 0));
//                cameraController->painter->setPen(QColor(255, 0, 0, 80));
            }
//            cameraController->painter->setOpacity(0.5);
//            qDebug() << "GameField::drawTowerUnderConstruction(); -5- isDrawableTowers:" << cameraController->isDrawableTowers;
            if (cameraController->isDrawableTowers == 5) {
                for (int map = 1; map < cameraController->isDrawableTowers; map++) {
                    drawTowerUnderConstructionAndMarks(cameraController, map, templateForTower, mainCell, startDrawCell, finishDrawCell);
                }
            } else if (cameraController->isDrawableTowers != 0) {
                drawTowerUnderConstructionAndMarks(cameraController, cameraController->isDrawableTowers, templateForTower, mainCell, startDrawCell, finishDrawCell);
            }
//            cameraController->painter->restore();
        }
    }
}

void GameField::drawTowerUnderConstructionAndMarks(CameraController* cameraController, int map, TemplateForTower* templateForTower, Cell* mainCell, QPoint startDrawCell, QPoint finishDrawCell) {
//    qDebug() << "GameField::drawTowerUnderConstructionAndMarks(); -- map:" << map << " templateForTower:" << templateForTower->toString().toStdString().c_str() << " mainCell:" << mainCell << " startDrawCell:" << startDrawCell << " finishDrawCell:" << finishDrawCell;
    QPixmap textureRegion = templateForTower->idleTile->getPixmap();
    int towerSize = templateForTower->size;
    Vector2 *towerPos = new Vector2(mainCell->getGraphicCoordinates(map));
    if (templateForTower->radiusDetection != 0.0) {
        cameraController->painter->drawEllipse(towerPos->getPointF(), templateForTower->radiusDetection, templateForTower->radiusDetection);
    }
    cameraController->getCorrectGraphicTowerCoord(towerPos, towerSize, map);
//    cameraController->painter->drawEllipse(*cameraPoint + *towerPos, 3, 3);//templateForTower->radiusDetection, templateForTower->radiusDetection);
    cameraController->painter->drawPixmap(towerPos->x, towerPos->y, cameraController->sizeCellX * towerSize, (cameraController->sizeCellY * 2) * towerSize, textureRegion);
//    cameraController->painter->fillRect(towerPos->x(), towerPos->y(), cameraController->sizeCellX * towerSize, (cameraController->sizeCellY * 2) * towerSize, cameraController->painter->pen().color());
    if (greenCheckmark != NULL && redCross != NULL) {
        Vector2 *markPos = new Vector2();
        for (int x = startDrawCell.x(); x <= finishDrawCell.x(); x++) {
            for (int y = startDrawCell.y(); y <= finishDrawCell.y(); y++) {
                Cell* markCell = getCell(mainCell->cellX + x, mainCell->cellY + y);
                if (markCell != NULL) {
                    markPos->set(markCell->getGraphicCoordinates(map));
                    markPos->add(-cameraController->halfSizeCellX, -cameraController->sizeCellY-cameraController->halfSizeCellY);
                    if(markCell->isEmpty()) {
                        cameraController->painter->drawPixmap(markPos->x, markPos->y, cameraController->sizeCellX, cameraController->sizeCellY*2, *greenCheckmark);
                    } else {
                        cameraController->painter->drawPixmap(markPos->x, markPos->y, cameraController->sizeCellX, cameraController->sizeCellY*2, *redCross);
                    }
                }
            }
        }
        delete markPos; // markPos = NULL;
    }
    delete towerPos; // towerPos = NULL;
}

//void GameField::drawTowerUnderConstruction(QPainter* painter, int buildX, int buildY, TemplateForTower *tower) {
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

//void GameField::setMainCoorMap(int mainCoorMapX, int mainCoorMapY) {
//    this->mainCoorMapX = mainCoorMapX;
//    this->mainCoorMapY = mainCoorMapY;
//}

//void GameField::setSizeCell(int sizeCellX) {
//    this->sizeCellX = sizeCellX;
//    this->sizeCellY = sizeCellX/2;
//    this->halfSizeCellX = sizeCellX/2;
//    this->halfSizeCellY = sizeCellY/2;
//}

//int GameField::getMainCoorMapX() {
//    return mainCoorMapX;
//}

//int GameField::getMainCoorMapY() {
//    return mainCoorMapY;
//}

//int GameField::getSizeCell() {
//    return sizeCellX;
//}

//void GameField::setMousePress(int x, int y) {
//    mouseX = x;
//    mouseY = y;
//}

//bool GameField::getMousePress(int x, int y) {
//    if(mouseX != -1 && mouseY != -1)
//        if((x == mouseX && y == mouseY) || (x == -1 && y == -1))
//            return true;
//    return false;
//}

//bool GameField::isSetSpawnPoint(int x, int y) {
//    if(spawnPointX != -1 && spawnPointY != -1)
//        if((x == spawnPointX && y == spawnPointY) || (x == -1 && y == -1))
//            return true;
//    return false;
//}

//bool GameField::isSetExitPoint(int x, int y) {
//    if(exitPointX != -1 && exitPointY != -1)
//        if((x == exitPointX && y == exitPointY) || (x == -1 && y == -1))
//            return true;
//    return false;
//}

void GameField::stepAllUnits(float deltaTime, CameraController* cameraController) {
    for (Unit* unit : unitsManager->units) {
        AStar::Vec2i oldPosition = unit->oldPosition;
        if (unit->isAlive()) {
            AStar::Vec2i* newPosition = unit->move(deltaTime, cameraController);
            if (newPosition != NULL) {
                if (!newPosition->operator ==(oldPosition)) {
                    getCell(oldPosition.x, oldPosition.y)->removeUnit(unit);
                    getCell(newPosition->x, newPosition->y)->setUnit(unit);
//                    qDebug() << "GameField::stepAllUnits(); -- Unit move to X:" << newPosition->x << " Y:" << newPosition->y;
                }
            } else {
                Cell *cell = getCell(oldPosition.x, oldPosition.y);
                if (unit->player == 1) {
//                    missedUnitsForComputer0++;
                    if (oldPosition.equals({cellExitHero->cellX, cellExitHero->cellY})) {
                        qDebug() << "GameField::stepAllUnits(); -- Hero Unit destination lastPoint! unit:" << unit->toString().toStdString().c_str();
                    }
                } else if (unit->player == 0) {
//                    if (unit->exitCell == cell) {
//                    cell->removeUnit(unit);
//                    unitsManager->removeUnit(unit);
//                        missedUnitsForPlayer1++;
//                    } else {
                        if (unit->route.empty()) {
                            int randomX = rand()%map->width;
                            int randomY = rand()%map->height;
                            unit->route = pathFinder->findPath({oldPosition.x, oldPosition.y}, {randomX, randomY});
//                            qDebug() << "GameField::stepAllUnits(); -reroute- randomX:" << randomX << " randomY:" << randomY << " unit:" << unit->toString().toStdString().c_str();
                        }
//                    }
//                    Cell* cell = getCell(oldPosition.x, oldPosition.y);
//                    if (cell->isTerrain()) {
//                        cell->removeTerrain(true);
//                        updatePathFinderWalls();
//                    }
                }
            }
        } else {
//        if (!unit->isAlive()) {
            if (!unit->changeDeathFrame(deltaTime)) {
                getCell(oldPosition.x, oldPosition.y)->removeUnit(unit);
//                GameField.gamerGold += unit.templateForUnit.bounty;
//                unit.dispose();
                unitsManager->removeUnit(unit);
                qDebug() << "GameField::stepAllUnits(); -- Unit death! and delete!";
            }
        }
    }
}

//int GameField::stepAllUnits() {
//    bool allDead = true;
////    for(int k = 0; k < unitsManager->getAmount(); k++) {
//    foreach (Unit* unit, unitsManager->units) {
//        int result = stepOneUnit(unit);
////        int result = stepOneUnit(k);
//        if(result != -2) {
//            allDead = false;
//        }
//        if(result == 1) {
//            currentFinishedUnits++;
//            if(currentFinishedUnits >= gameOverLimitUnits) {
//                qDebug() << "GameField::stepAllUnits(); -- return 1";
//                return 1;
//            }
//        } else if(result == -1) {
//            qDebug() << "GameField::stepAllUnits(); -- return -1";
//            return -1;
//        } else if(result == 3) {
//            return 3;
//        } else if(result == 4) {
//            return 4;
//        }
//    }
//    if(allDead) {
//        return 2;
//    } else {
//        return 0;
//    }
//}

//int GameField::stepOneUnit(Unit* hero) {
//    Unit* tmpUnit = unitsManager->getUnit(num);
//    if(hero->alive) {
//        if(hero->animationCurrIter < hero->animationMaxIter) {
//            hero->pixmap = hero->activePixmaps[hero->animationCurrIter++];
//        } else {
//            int currX = hero->coorByCellX;
//            int currY = hero->coorByCellY;
//            int exitX = currX, exitY = currY;
//            if (!hero->path.empty()) {
//                AStar::Vec2i point = hero->path.back();
//                hero->path.pop_back();
//                exitX = point.x;
//                exitY = point.y;

//            }
//            if(hero == unitsManager->hero.at(0)) {
//                if (cellExitHero->cellX == currX && cellExitHero->cellY == currY) {
//                    qDebug() << "GameField::stepOneUnit(); -- HeroInExitPoint!";
//                    return 3;
//                }
//            } else /*if (tmpUnit->type != 0)*/ { // Not hero!
//                if (getCell(currX, currY)->getHero() != NULL) {
//                    qDebug() << "GameField::stepOneUnit(); -- Hero contact with Enemy!";
//                    unitsManager->attackUnit(currX, currY, 9999);//, getCell(currX, currY)->getHero()); // Magic number 9999

//                } else if (getCell(exitX, exitY)->getHero() != NULL) {
//                    qDebug() << "GameField::stepOneUnit(); -- Hero contact with Enemy!";
//                    unitsManager->attackUnit(exitX, exitY, 9999);//, getCell(exitX, exitY)->getHero()); // Magic number 9999
//                }

//                if (getCell(currX, currY)->isTerrain()) {
//                    getCell(currX, currY)->removeTerrain(true);
//                    updatePathFinderWalls();
//                }
//                if (hero->path.empty() || getCell(exitX, exitY)->isTerrain()) {
//                    int randomX = rand()%map->width;
//                    int randomY = rand()%map->height;
//                    hero->path = pathFinder->findPath({hero->coorByCellX, hero->coorByCellY}, {randomX, randomY});
//                }
//            }
//            if(exitX != currX || exitY != currY) {
//                getCell(currX, currY)->clearUnit(hero);
//                hero->lastX = currX;
//                hero->lastY = currY;
//                hero->coorByCellX = exitX;
//                hero->coorByCellY = exitY;
//                if(!isometric) {
//                    if(exitX < currX && exitY < currY) {
//                        hero->animationMaxIter = hero->templateForUnit->walk_up_left.size();
//                        hero->activePixmaps = hero->templateForUnit->walk_up_left;
//                        hero->direction = Direction::type::UP_LEFT;
//                    } else if(exitX == currX && exitY < currY) {
//                        hero->animationMaxIter = hero->templateForUnit->walk_up.size();
//                        hero->activePixmaps = hero->templateForUnit->walk_up;
//                        hero->direction = Direction::UP;
//                    } else if(exitX > currX && exitY < currY) {
//                        hero->animationMaxIter = hero->templateForUnit->walk_up_right.size();
//                        hero->activePixmaps = hero->templateForUnit->walk_up_right;
//                        hero->direction = Direction::UP_RIGHT;
//                    } else if(exitX < currX && exitY == currY) {
//                        hero->animationMaxIter = hero->templateForUnit->walk_left.size();
//                        hero->activePixmaps = hero->templateForUnit->walk_left;
//                        hero->direction = Direction::LEFT;
//                    } else if(exitX > currX && exitY == currY) {
//                        hero->animationMaxIter = hero->templateForUnit->walk_right.size();
//                        hero->activePixmaps = hero->templateForUnit->walk_right;
//                        hero->direction = Direction::RIGHT;
//                    } else if(exitX < currX && exitY > currY) {
//                        hero->animationMaxIter = hero->templateForUnit->walk_down_left.size();
//                        hero->activePixmaps = hero->templateForUnit->walk_down_left;
//                        hero->direction = Direction::DOWN_LEFT;
//                    } else if(exitX == currX && exitY > currY) {
//                        hero->animationMaxIter = hero->templateForUnit->walk_down.size();
//                        hero->activePixmaps = hero->templateForUnit->walk_down;
//                        hero->direction = Direction::DOWN;
//                    } else if(exitX > currX && exitY > currY) {
//                        hero->animationMaxIter = hero->templateForUnit->walk_down_right.size();
//                        hero->activePixmaps = hero->templateForUnit->walk_down_right;
//                        hero->direction = Direction::DOWN_RIGHT;
//                    }
//                } else {
//                    if(exitX < currX && exitY < currY) {
//                        hero->animationMaxIter = hero->templateForUnit->walk_up.size();
//                        hero->activePixmaps = hero->templateForUnit->walk_up;
//                        hero->direction = Direction::UP;
//                    } else if(exitX == currX && exitY < currY) {
//                        hero->animationMaxIter = hero->templateForUnit->walk_up_right.size();
//                        hero->activePixmaps = hero->templateForUnit->walk_up_right;
//                        hero->direction = Direction::UP_RIGHT;
//                    } else if(exitX > currX && exitY < currY) {
//                        hero->animationMaxIter = hero->templateForUnit->walk_right.size();
//                        hero->activePixmaps = hero->templateForUnit->walk_right;
//                        hero->direction = Direction::RIGHT;
//                    } else if(exitX < currX && exitY == currY) {
//                        hero->animationMaxIter = hero->templateForUnit->walk_up_left.size();
//                        hero->activePixmaps = hero->templateForUnit->walk_up_left;
//                        hero->direction = Direction::UP_LEFT;
//                    } else if(exitX > currX && exitY == currY) {
//                        hero->animationMaxIter = hero->templateForUnit->walk_down_right.size();
//                        hero->activePixmaps = hero->templateForUnit->walk_down_right;
//                        hero->direction = Direction::DOWN_RIGHT;
//                    } else if(exitX < currX && exitY > currY) {
//                        hero->animationMaxIter = hero->templateForUnit->walk_left.size();
//                        hero->activePixmaps = hero->templateForUnit->walk_left;
//                        hero->direction = Direction::LEFT;
//                    } else if(exitX == currX && exitY > currY) {
//                        hero->animationMaxIter = hero->templateForUnit->walk_down_left.size();
//                        hero->activePixmaps = hero->templateForUnit->walk_down_left;
//                        hero->direction = Direction::DOWN_LEFT;
//                    } else if(exitX > currX && exitY > currY) {
//                        hero->animationMaxIter = hero->templateForUnit->walk_down.size();
//                        hero->activePixmaps = hero->templateForUnit->walk_down;
//                        hero->direction = Direction::DOWN;
//                    }
//                }
//                hero->pixmap = hero->activePixmaps[0];
//                hero->animationCurrIter = 0;
//                getCell(exitX, exitY)->setUnit(hero);
//            } else {
//                hero->animationMaxIter = hero->templateForUnit->idle.size();
//                hero->activePixmaps = hero->templateForUnit->idle;
//                hero->direction = Direction::IDLE;
//                hero->pixmap = hero->activePixmaps[0];
//            }
//        }
//    } else if(hero->preDeath) {
//        if(hero->animationCurrIter < hero->animationMaxIter) {
//            hero->pixmap = hero->activePixmaps[hero->animationCurrIter++];
//        } else {
//            hero->preDeath = false;
//            return 4;
//        }
//    } else {
//        return -2;
//    }
//    return 0;
//}

void GameField::shotAllTowers(float deltaTime, CameraController *cameraController) {
//    qDebug() << "GameField::shotAllTowers(); -- deltaTime:" << deltaTime;
    updateTowersGraphicCoordinates(cameraController);
    for (Tower* tower : towersManager->towers) {
        TowerAttackType::type towerAttackType = tower->templateForTower->towerAttackType;
        if (towerAttackType == TowerAttackType::Pit) {
            Unit* unit = getCell(tower->position->x(), tower->position->y())->getUnit();
            if (unit != NULL && (unit->templateForUnit->type != "fly") && unit->player != tower->player) {
                qDebug() << "GameField::shotAllTowers(); -- tower.capacity:" << tower->capacity << " unit.getHp:" << unit->hp;
//                    unit.die(unit.getHp());
                unitsManager->removeUnit(unit);
                getCell(tower->position->x(), tower->position->y())->removeUnit(unit);
                tower->capacity--;
                if (tower->capacity <= 0) {
                    towersManager->removeTower(tower);
                }
            }
        } else if (towerAttackType == TowerAttackType::Melee) {
            shotMeleeTower(tower);
        } else if (towerAttackType == TowerAttackType::Range || towerAttackType == TowerAttackType::RangeFly) {
            if (tower->recharge(deltaTime)) {
                for (Unit* unit : unitsManager->units) {
                    if (unit != NULL && unit->isAlive() && unit->player != tower->player) {
                        if ( ( (unit->templateForUnit->type == "fly") && towerAttackType == TowerAttackType::RangeFly) ||
                                ((unit->templateForUnit->type != "fly") && towerAttackType == TowerAttackType::Range)) { // Тупо но работает, потом переделать need =)
//                            if (Intersector.overlaps(tower.getRadiusDetectionСircle(), unit.circle1)) {
//                            qDebug() << "GameField::shotAllTowers(); -1- tower->radiusDetectionCircle:" << tower->radiusDetectionCircle;
//                            qDebug() << "GameField::shotAllTowers(); -- unit->circle1:" << unit->circle1;
//                            qDebug() << "GameField::shotAllTowers(); -- unit->circle2:" << unit->circle2;
//                            qDebug() << "GameField::shotAllTowers(); -- unit->circle3:" << unit->circle3;
//                            qDebug() << "GameField::shotAllTowers(); -- unit->circle4:" << unit->circle4;
                            if (tower->radiusDetectionCircle != NULL && tower->radiusDetectionCircle->overlaps(unit->circle3)) { // circle1 2 3 4?
                                    qDebug() << "GameField::shotAllTowers(); -- Intersector.overlaps()";
                                    qDebug() << "GameField::shotAllTowers(); -- tower:" << tower->toString(true).toStdString().c_str();
                                    qDebug() << "GameField::shotAllTowers(); -- unit:" << unit->toString(true).toStdString().c_str();
//                                if (tower->shoot(unit)) {
//                                    if(tower->templateForTower->shellAttackType != ShellAttackType.MassAddEffect) {
//                                        break;
//                                    }
//                                }
                            }
                        }
                    }
                }
            }
        } else if (towerAttackType == TowerAttackType::FireBall) {
            if (tower->recharge(deltaTime)) {
//                    fireBallTowerAttack(deltaTime, tower);
//                    tower.shoot();
            }
        }
    }
//    qDebug() << "GameField::shotAllTowers(); -end- ";
}

bool GameField::shotMeleeTower(Tower *tower) {
    bool attack = false;
    int radius = (int)tower->templateForTower->radiusDetection;
    for (int tmpX = -radius; tmpX <= radius; tmpX++) {
        for (int tmpY = -radius; tmpY <= radius; tmpY++) {
            QPoint* position = tower->position;
            Cell* cell = getCell(tmpX + position->x(), tmpY + position->y());
            if (cell != NULL && cell->getUnit() != NULL) {
                attack = true;
                Unit* unit = cell->getUnit();
                if (unit != NULL && (unit->templateForUnit->type != "fly") && unit->player != tower->player) {
//                    if (unit->die(tower.getDamage(), tower.getTemplateForTower().shellEffectType)) {
//                        gamerGold += unit.templateForUnit.bounty;
//                    }
//                    if (tower->templateForTower->shellAttackType == ShellAttackType.SingleTarget) {
//                        return true;
//                    }
                }
            }
        }
    }
    return attack;
}

bool GameField::fireBallTowerAttack(int deltaTime, Tower *fireBallTower) {
    foreach (Tower* tmpTower, towersManager->towers) {
        if (tmpTower->recharge(deltaTime)) {
            tmpTower->createBullets(towersManager->difficultyLevel);
        }
//        for (int b = 0; b < tmpTower->bullets.size(); b++) {
//            Bullet* tmpBullet = tmpTower->bullets[b];
//            int currX = tmpBullet->currCellX;
//            int currY = tmpBullet->currCellY;
//            if (currX < 0 || currX >= map->width || currY < 0 || currY >= map->height) {
//                tmpTower->bullets.erase(tmpTower->bullets.begin()+b);
//                delete tmpBullet;
//                b--;
//            } else {
//                if (getCell(currX, currY)->getHero() != NULL) {
////                    unitsManager->attackUnit(currX, currY, 9999);//, getCell(currX, currY)->getHero()); // Magic number 9999
//                }
//            }
//            if(tmpBullet->animationCurrIter < tmpBullet->animationMaxIter) {
//                tmpBullet->pixmap = tmpBullet->activePixmaps[tmpBullet->animationCurrIter++];
//            } else {
//                int exitX = currX, exitY = currY;
//                if (tmpBullet->direction == Direction::type::UP) {
//                    exitX = currX-1;
//                    exitY = currY-1;
//                } else if (tmpBullet->direction == Direction::UP_RIGHT) {
//                    exitX = currX;
//                    exitY = currY-1;
//                } else if (tmpBullet->direction == Direction::RIGHT) {
//                    exitX = currX+1;
//                    exitY = currY-1;
//                } else if (tmpBullet->direction == Direction::UP_LEFT) {
//                    exitX = currX-1;
//                    exitY = currY;
//                } else if (tmpBullet->direction == Direction::DOWN_RIGHT) {
//                    exitX = currX+1;
//                    exitY = currY;
//                } else if (tmpBullet->direction == Direction::LEFT) {
//                    exitX = currX-1;
//                    exitY = currY+1;
//                } else if (tmpBullet->direction == Direction::DOWN_LEFT) {
//                    exitX = currX;
//                    exitY = currY+1;
//                } else if (tmpBullet->direction == Direction::DOWN) {
//                    exitX = currX+1;
//                    exitY = currY+1;
//                }
//                if(exitX != currX || exitY != currY) {
//                    tmpBullet->lastCellX = currX;
//                    tmpBullet->lastCellY = currY;
//                    tmpBullet->currCellX = exitX;
//                    tmpBullet->currCellY = exitY;
//                    if(isometric) {
//                        if(exitX < currX && exitY < currY) {
//                            tmpBullet->animationMaxIter = tmpBullet->defTower->bullet_fly_up.size();
//                            tmpBullet->activePixmaps = tmpBullet->defTower->bullet_fly_up;
//                            tmpBullet->direction = Direction::UP;
//                        } else if(exitX == currX && exitY < currY) {
//                            tmpBullet->animationMaxIter = tmpBullet->defTower->bullet_fly_up_right.size();
//                            tmpBullet->activePixmaps = tmpBullet->defTower->bullet_fly_up_right;
//                            tmpBullet->direction = Direction::UP_RIGHT;
//                        } else if(exitX > currX && exitY < currY) {
//                            tmpBullet->animationMaxIter = tmpBullet->defTower->bullet_fly_right.size();
//                            tmpBullet->activePixmaps = tmpBullet->defTower->bullet_fly_right;
//                            tmpBullet->direction = Direction::RIGHT;
//                        } else if(exitX < currX && exitY == currY) {
//                            tmpBullet->animationMaxIter = tmpBullet->defTower->bullet_fly_up_left.size();
//                            tmpBullet->activePixmaps = tmpBullet->defTower->bullet_fly_up_left;
//                            tmpBullet->direction = Direction::UP_LEFT;
//                        } else if(exitX > currX && exitY == currY) {
//                            tmpBullet->animationMaxIter = tmpBullet->defTower->bullet_fly_down_right.size();
//                            tmpBullet->activePixmaps = tmpBullet->defTower->bullet_fly_down_right;
//                            tmpBullet->direction = Direction::DOWN_RIGHT;
//                        } else if(exitX < currX && exitY > currY) {
//                            tmpBullet->animationMaxIter = tmpBullet->defTower->bullet_fly_left.size();
//                            tmpBullet->activePixmaps = tmpBullet->defTower->bullet_fly_left;
//                            tmpBullet->direction = Direction::LEFT;
//                        } else if(exitX == currX && exitY > currY) {
//                            tmpBullet->animationMaxIter = tmpBullet->defTower->bullet_fly_down_left.size();
//                            tmpBullet->activePixmaps = tmpBullet->defTower->bullet_fly_down_left;
//                            tmpBullet->direction = Direction::DOWN_LEFT;
//                        } else if(exitX > currX && exitY > currY) {
//                            tmpBullet->animationMaxIter = tmpBullet->defTower->bullet_fly_down.size();
//                            tmpBullet->activePixmaps = tmpBullet->defTower->bullet_fly_down;
//                            tmpBullet->direction = Direction::DOWN;
//                        }
//                    }
//                    if (tmpBullet->activePixmaps.empty() && !tmpBullet->defTower->bullet.empty()) {
//                        tmpBullet->animationMaxIter = tmpBullet->defTower->bullet.size();
//                        tmpBullet->activePixmaps = tmpBullet->defTower->bullet;
//                    }
//                    tmpBullet->pixmap = tmpBullet->activePixmaps[0];
//                    tmpBullet->animationCurrIter = 0;
//                }
//            }
//        }
    }
    return true;
}

void GameField::moveAllShells(float delta) {
    foreach (Tower* tower, towersManager->towers) {
//        tower->moveAllShells(delta);
    }
}

//int GameField::getUnitHpInCell(int x, int y) {
//    if(x >= 0 && x < map->width)
//        if(y >= 0 && y < map->height)
//            if(containUnit(x,y))
//                return unitsManager->getHP(x, y);
//    return 0;
//}

//Unit* GameField::getUnitWithLowHP(int x, int y) {
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

//int GameField::containUnit(int x, int y, Unit *hero) {
//    if(!field[map->width*y + x].units.empty()) {
//        int size = field[map->width*y + x].units.size();
//        if(hero == NULL) {
//            return size;
//        } else {
//            for(int k = 0; k < size; k++) {
//                if(field[map->width*y + x].units[k] == hero) {
//                    return k+1;
//                }
//            }
//        }
//    }
//    return 0;
//}

//bool GameField::setTower(int x, int y, TemplateForTower* defTower) {
//    int size = defTower->size;
//    for(int tmpX = 0; tmpX < size; tmpX++)
//        for(int tmpY = 0; tmpY < size; tmpY++)
//            if(!this->getCell(tmpX+x, tmpY+y)->isEmpty())
//                return false;

//    Tower* tower = towersManager->createTower(x, y, defTower);
//    if(tower != NULL) {
//        for(int tmpX = 0; tmpX < size; tmpX++) {
//            for(int tmpY = 0; tmpY < size; tmpY++) {
//                Cell* cell = getCell(tmpX+x, tmpY+y);
//                if (cell->isEmpty() && !cell->spawn && !cell->exit) {
//                    cell->setTower(tower);
//                }
//            }
//        }
//        return true;
//    }
//    return false;
//}

void GameField::spawnHeroInSpawnPoint() {
    qDebug() << "GameField::spawnHeroInSpawnPoint(); -- ";
    if (cellSpawnHero != NULL && cellExitHero != NULL) {
        cellSpawnHero->removeTerrain(true);
        removeTower(cellSpawnHero->cellX, cellSpawnHero->cellY);
        createUnit(cellSpawnHero->cellX, cellSpawnHero->cellY, cellExitHero->cellX, cellExitHero->cellY, 1); // player1 = hero
    }
}

void GameField::spawnCompUnitToRandomExit(int x, int y) {
    qDebug() << "GameField::spawnCompUnitToRandomExit(); -- x:" << x << " y:" << y;
    int randomX = rand()%map->width;
    int randomY = rand()%map->height;
    createUnit(x, y, randomX, randomY, 0);
}

void GameField::createUnit(int x, int y, int x2, int y2, int player) {
    qDebug() << "GameField::createUnit(); -- x:" << x << " y:" << y << " x2:" << x2 << " y2:" << y2 << " player:" << player;
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

void GameField::createUnit(QPoint spawnPoint, QPoint exitPoint, TemplateForUnit* templateForUnit, int player) {
//    qDebug() << "GameField::createUnit(); -- spawnPoint:" << spawnPoint;
//    qDebug() << "GameField::createUnit(); -- exitPoint:" << exitPoint;
    qDebug() << "GameField::createUnit(); -- templateForUnit:" << templateForUnit->toString().toStdString().c_str();
//    qDebug() << "GameField::createUnit(); -- player:" << player;
    if (!spawnPoint.isNull() && !exitPoint.isNull() && templateForUnit != NULL/* && pathFinder != NULL*/) {
        AStar::CoordinateList path = pathFinder->findPath({spawnPoint.x(), spawnPoint.y()}, {exitPoint.x(), exitPoint.y()});
        qDebug() << "GameField::createUnit(); -- path:" << path.size();
        if (!path.empty()) {
            Unit* unit = unitsManager->createUnit(path, templateForUnit, player);
            getCell(spawnPoint.x(), spawnPoint.y())->setUnit(unit);
            qDebug() << "GameField::createUnit(); -- unit:" << unit;
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

UnderConstruction* GameField::createdRandomUnderConstruction() {
    return createdUnderConstruction(factionsManager->getRandomTemplateForTowerFromAllFaction());
}

UnderConstruction* GameField::createdUnderConstruction(TemplateForTower* templateForTower) {
    qDebug() << "GameField::createdUnderConstruction(); -- templateForTower:" << templateForTower->toString(true).toStdString().c_str();
    if (underConstruction != NULL) {
        delete underConstruction;
    }
    return (underConstruction = new UnderConstruction(templateForTower));
}

bool GameField::cancelUnderConstruction() {
    if (underConstruction != NULL) {
        delete underConstruction;
        underConstruction = NULL;
        return true;
    }
    return false;
}

UnderConstruction* GameField::getUnderConstruction() {
    return underConstruction;
}

void GameField::buildTowersWithUnderConstruction(int x, int y) {
    if (underConstruction != NULL) {
        underConstruction->setEndCoors(x, y);
        createTower(underConstruction->startX, underConstruction->startY, underConstruction->templateForTower, 1);
        for (int k = 0; k < underConstruction->coorsX.size(); k++) {
//            for (int k = underConstruction->coorsX.size()-1; k >= 0; k--) {
                createTower(underConstruction->coorsX.at(k), underConstruction->coorsY.at(k), underConstruction->templateForTower, 1);
//            }
        }
        underConstruction->clearStartCoors();
        updatePathFinderWalls(); // rerouteForAllUnits();
    }
}

//void GameField::towerActions(int x, int y) {
//    if (field[x][y].isEmpty()) {
//        createTower(x, y, factionsManager.getRandomTemplateForTowerFromAllFaction(), 1);
//        rerouteForAllUnits();
//    } else if (field[x][y].getTower() != null) {
//        removeTower(x, y);
//    }
//}

bool GameField::createTower(int buildX, int buildY, TemplateForTower* templateForTower, int player) {
    if (gamerGold >= templateForTower->cost) {
        int towerSize = templateForTower->size;
        int startX = 0, startY = 0, finishX = 0, finishY = 0;
        if (towerSize != 1) {
            // Нижняя карта
            if (towerSize % 2 == 0) {
                startX = -(towerSize / 2);
                startY = -(towerSize / 2);
                finishX = (towerSize / 2)-1;
                finishY = (towerSize / 2)-1;
            } else {
                startX = -(towerSize / 2);
                startY = -(towerSize / 2);
                finishX = (towerSize / 2);
                finishY = (towerSize / 2);
            }
            // Правая карта
//                if (towerSize % 2 == 0) {
//                    startX = -(towerSize / 2);
//                    startY = -((towerSize / 2) - 1);
//                    finishX = ((towerSize / 2) - 1);
//                    finishY = (towerSize / 2);
//                } else {
//                    startX = -(towerSize / 2);
//                    startY = -(towerSize / 2);
//                    finishX = (towerSize / 2);
//                    finishY = (towerSize / 2);
//                }
        }
        for (int tmpX = startX; tmpX <= finishX; tmpX++)
            for (int tmpY = startY; tmpY <= finishY; tmpY++)
                if (!getCell(buildX + tmpX, buildY + tmpY)->isEmpty())
                    return false;

        // GOVNO CODE
        QPoint* position = new QPoint(buildX, buildY);
        Tower* tower = towersManager->createTower(position, templateForTower, player);
//        qDebug() << "GameField::createTower(); -- templateForTower.towerAttackType:" << templateForTower->towerAttackType;
//        if (templateForTower.towerAttackType != TowerAttackType.Pit) {
            for (int tmpX = startX; tmpX <= finishX; tmpX++) {
                for (int tmpY = startY; tmpY <= finishY; tmpY++) {
                    getCell(buildX + tmpX, buildY + tmpY)->setTower(tower);
//                    pathFinder.nodeMatrix[buildY + tmpY][buildX + tmpX].setKey('T');
                }
            }
//        }
        // GOVNO CODE

//            rerouteForAllUnits();
        gamerGold -= templateForTower->cost;
        qDebug() << "GameField::createTower(); -- Now gamerGold:" << gamerGold;
        return true;
    } else {
        return false;
    }
}

void GameField::removeLastTower() {
//        if(towersManager.amountTowers() > 0) {
        Tower* tower = towersManager->getTower();//towersManager->towers.size() - 1);
        QPoint* pos = tower->position;
        removeTower(pos->x(), pos->y());
//        }
}

void GameField::removeTower(int cellX, int cellY) {
    Tower* tower = getCell(cellX, cellY)->getTower();
    if (tower != NULL) {
        int x = tower->position->x();
        int y = tower->position->y();
        int towerSize = tower->templateForTower->size;
        int startX = 0, startY = 0, finishX = 0, finishY = 0;
        if (towerSize != 1) {
            if (towerSize % 2 == 0) {
                startX = -(towerSize / 2);
                startY = -(towerSize / 2);
                finishX = (towerSize / 2)-1;
                finishY = (towerSize / 2)-1;
            } else {
                startX = -(towerSize / 2);
                startY = -(towerSize / 2);
                finishX = towerSize / 2;
                finishY = towerSize / 2;
            }
        }

        for (int tmpX = startX; tmpX <= finishX; tmpX++) {
            for (int tmpY = startY; tmpY <= finishY; tmpY++) {
                getCell(x + tmpX, y + tmpY)->removeTower();
//                pathFinder.getNodeMatrix()[y + tmpY][x + tmpX].setKey('.');
            }
        }
        towersManager->removeTower(tower);
//        rerouteForAllUnits();
        updatePathFinderWalls();
        gamerGold += tower->templateForTower->cost;//*0.5;
    }
}

//bool GameField::deleteTower(int x, int y) {
//    Tower* tower = towersManager->getTower(x, y);
//    if (tower != NULL) {
//        int towerX = tower->position->x();
//        int towerY = tower->position->y();
//        int size = tower->templateForTower->size;
//        towersManager->deleteTower(towerX, towerY);
//        for (int tmpX = 0; tmpX < size; tmpX++) {
//            for (int tmpY = 0; tmpY < size; tmpY++) {
//                this->getCell(tmpX+towerX, tmpY+towerY)->removeTower();
//            }
//        }
//        return true;
//    }
//    return false;
//}
