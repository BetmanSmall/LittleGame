#include "gamefield.h"

GameField::GameField(QString mapPath, FactionsManager *factionsManager, GameSettings *gameSettings) {
    qDebug() << "GameField::GameField(); -- mapPath:" << mapPath;
    qDebug() << "GameField::GameField(); -- factionsManager:" << factionsManager;
    qDebug() << "GameField::GameField(); -- gameSettings:" << gameSettings;
    this->factionsManager = factionsManager;
    waveManager = new WaveManager();
    towersManager = new TowersManager();
    unitsManager = new UnitsManager();
    this->gameSettings = gameSettings;

    map = (new MapLoader())->load(mapPath);
    qDebug() << "GameField::GameField(); -- map:" << map;

    underConstruction = NULL;
    greenCheckmark = new QPixmap(ASSETS_PATH + "maps/textures/green_checkmark.png");
    redCross = new QPixmap(ASSETS_PATH + "maps/textures/red_cross.png");
    if (greenCheckmark == NULL || redCross == NULL) {
        qDebug() << "GameField::GameField(); -- Achtung! NOT FOUND 'maps/textures/green_checkmark.png' || 'maps/textures/red_cross.png'";
    }

    createField();
//    if (gameSettings->isometric) {
//        flipY();
//    }
    pathFinder = new AStar::PathFinder();
    pathFinder->setWorldSize({map->width, map->height});
    pathFinder->setHeuristic(AStar::Heuristic::euclidean);
    pathFinder->setDiagonalMovement(false);
    updatePathFinderWalls();
    qDebug() << "GameField::GameField(); -- pathFinder:" << pathFinder;

    qDebug() << "GameField::GameField(); -- gameSettings:" << gameSettings;
    if (gameSettings->gameType == GameType::LittleGame) {
        int randomEnemyCount = gameSettings->enemyCount;
        qDebug() << "GameField::GameField(); -- randomEnemyCount:" << randomEnemyCount;
        for (int k = 0; k < randomEnemyCount; k++) {
            int randomX = (int)(rand()%map->width);
            int randomY = (int)(rand()%map->height);
            qDebug() << "GameField::GameField(); -- k:" << k;
            qDebug() << "GameField::GameField(); -- randomX:" << randomX;
            qDebug() << "GameField::GameField(); -- randomY:" << randomY;
            if (getCell(randomX, randomY)->isEmpty()) {
                if (spawnCompUnitToRandomExit(randomX, randomY) == NULL) {
                    k--;
                }
            } else {
                k--;
            }
        }
        int randomTowerCount = gameSettings->towersCount;
        qDebug() << "GameField::GameField(); -- randomTowerCount:" << randomTowerCount;
        for (int k = 0; k < randomTowerCount; k++) {
            int randomX = (int)(rand()%map->width);
            int randomY = (int)(rand()%map->height);
            qDebug() << "GameField::GameField(); -- k:" << k;
            qDebug() << "GameField::GameField(); -- randomX:" << randomX;
            qDebug() << "GameField::GameField(); -- randomY:" << randomY;
            if (getCell(randomX, randomY)->isEmpty()) {
                if (createTower(randomX, randomY, factionsManager->getRandomTemplateForTowerFromAllFaction(), 0)) {
                    k--;
                }
            } else {
                k--;
            }
        }
        spawnHeroInSpawnPoint();
    } else if (gameSettings->gameType == GameType::TowerDefence) {
        waveManager->validationPoints(field, map->width, map->height);
        if (waveManager->waves.size() == 0) {
            for (int w = 0; w < 10; w++) {
                QPoint* spawnPoint = new QPoint((int) (rand()%map->width), (int) (rand()%map->height));
                QPoint* exitPoint = new QPoint((int) (rand()%map->width), (int) (rand()%map->height));
                Cell* spawnCell = getCell(spawnPoint->x(), spawnPoint->y());
                Cell* exitCell = getCell(exitPoint->x(), exitPoint->y());
                if (spawnCell != NULL && spawnCell->isEmpty()) {
                    if (exitCell != NULL && exitCell->isEmpty()) {
                        Wave* wave = new Wave(spawnPoint, exitPoint, 0.0);
                        for (int k = 0; k < 10; k++) {
                            wave->addAction("interval=" + QString::number(1));
                            wave->addAction(factionsManager->getRandomTemplateForUnitFromFirstFaction()->templateName);
                        }
                        waveManager->addWave(wave);
                    }
                }
            }

        }
        waveManager->checkRoutes(pathFinder);
        QMap<QString, QString> *mapProperties = map->getProperties();
        qDebug() << "GameField::GameField(); -- mapProperties:" << mapProperties;
        gamerGold = mapProperties->value("gamerGold", "10000").toInt();
        gameSettings->maxOfMissedUnitsForComputer0 = (mapProperties->value("maxOfMissedUnitsForComputer0").toInt());
        gameSettings->missedUnitsForComputer0 = 0;
        if (gameSettings->maxOfMissedUnitsForPlayer1 == 0) {
            gameSettings->maxOfMissedUnitsForPlayer1 = (mapProperties->value("maxOfMissedUnitsForPlayer1").toInt());
        }
        gameSettings->missedUnitsForPlayer1 = 0;
        qDebug() << "GameField::GameField(); -- gamerGold:" << gamerGold;
        qDebug() << "GameField::GameField(); -- gameSettings->maxOfMissedUnitsForComputer0:" << gameSettings->maxOfMissedUnitsForComputer0;
        qDebug() << "GameField::GameField(); -- gameSettings->maxOfMissedUnitsForPlayer1:" << gameSettings->maxOfMissedUnitsForPlayer1;
//    } else {
//        qDebug() << "GameField::GameField(); -- gameSettings->gameType:" << gameSettings->gameType;
    }

    timeOfGame = 0.0;
    gameSpeed = 1.0;
    gamePaused = false;
//    gamerGold = Integer.valueOf(mapProperties.get("gamerGold", "10000", String.class)); // HARD GAME | one gold = one unit for computer!!!
    gamerGold = 100000;

//    gameOverLimitUnits = 10;
//    currentFinishedUnits = 0;
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
}

void GameField::createField() {
    if (map->properties.contains("orientation")) {
        if (map->properties.value("orientation") == "isometric") {
            gameSettings->isometric = true;
        }
    }
//    qDebug() << "GameField::createField(); -1- field:" << field;
//    if(field == NULL) {
        field = new Cell[map->width*map->height];
        for (int y = 0; y < map->height; y++) {
            for (int x = 0; x < map->width; x++) {
                Cell* cell = getCell(x, y);
                cell->setGraphicCoordinates(x, y, map->tileWidth, map->tileHeight, gameSettings->isometric);
                for (Layer* layer : map->getMapLayers()->layers) {
//                    if (mapLayer instanceof TiledMapTileLayer) {
//                    TiledMapTileLayer layer = (TiledMapTileLayer) mapLayer;
                        TileLayerCell* tileLayerCell = layer->getCell(x, y);
                        if (tileLayerCell != NULL) {
                            Tile* tile = tileLayerCell->getTile();
                            if (tile != NULL) {
                                QString layerName = layer->getName();
                                if (layerName != NULL) {
                                    if (layerName == "background") {
                                        cell->backgroundTiles.push_back(tile);
                                    } else if (layerName == "ground" || layerName == "entity") {
                                        cell->setTerrain(tile, false, false);
                                    } else if (layerName == "towers") {
                                        cell->removeTerrain(true);
                                        this->createTower(x, y, factionsManager->getAllTemplateForTowers().front(), 0);
                                    } else {
                                        cell->foregroundTiles.push_back(tile);
                                    }
                                }
                                if (tile->getProperties()->contains("spawnPoint")) {
                                    gameSettings->cellSpawnHero = cell;
                                    gameSettings->cellSpawnHero->spawn = true;
                                    qDebug() << "GameField::createField(); -- Set gameSettings->cellSpawnHero:" << gameSettings->cellSpawnHero;
                                } else if (tile->getProperties()->contains("exitPoint")) {
                                    gameSettings->cellExitHero = cell;
                                    gameSettings->cellExitHero->exit = true;
                                    qDebug() << "GameField::createField(); -- Set gameSettings->cellExitHero:" << gameSettings->cellExitHero;
                                }
//                                qDebug() << "GameField::createField(); -7- ";
//                                // task 6. отрисовка деревьев полностью
//                                if(tile.getProperties().get("treeName") != null) {
//                                    String treeName = tile.getProperties().get("treeName");
//                                    int treeWidth = Integer.parseInt(tile.getProperties().get("treeWidth", "1"));
//                                    int treeHeight = Integer.parseInt(tile.getProperties().get("treeHeight", "1"));
//                                    qDebug() << "GameField::createField(); -- New Tree:" << treeName << "[" << treeWidth << "," << treeHeight << "]:{" << x << "," << y << "}");
//                                    float regionX = tile.getTextureRegion().getRegionX();
//                                    float regionY = tile.getTextureRegion().getRegionY();
//                                    float regionWidth = tile.getTextureRegion().getRegionWidth();
//                                    float regionHeight = tile.getTextureRegion().getRegionWidth();
//                                    qDebug() << "GameField::createField(); -- regionX:" << regionX << " regionY:" << regionY << " regionWidth:" << regionWidth << " regionHeight:" << regionHeight);
//                                    TextureRegion textureRegion = new TextureRegion(tile.getTextureRegion());
//                                    textureRegion.setRegion(regionX - ((treeWidth>2) ? (treeWidth-2)*regionWidth : 0), regionY - ((treeHeight>1) ? (treeHeight-1)*regionHeight : 0), treeWidth*regionWidth, treeHeight*regionHeight);
////                                        Cell::Tree tree = new Cell::Tree(textureRegion, treeWidth, treeHeight);
//                                }
                            }
                        }
//                    } else {
//                        qDebug() << "GameField::createField(); -- Не смог преобразовать MapLayer в TiledMapTileLayer");
//                    }
                }
            }
        }
//    }
    qDebug() << "GameField::createField(); -2- field:" << field;
}

bool GameField::landscapeGenerator(QString mapPath) {
    qDebug() << "GameField::landscapeGenerator(); -- mapFile:" << mapPath;
//    int terrainType = rand()%2;
    if (mapPath.contains("randomMap")) {
        for (int x = 0; x < map->width; x++) {
            for (int y = 0; y < map->height; y++) {
                if( (rand()%100) < 30 ) {
                    int randNumber = ( 43+(rand()%4) );
                    Tile* tile = map->getTileSets()->getTileSet(1)->getLocalTile(randNumber);
                    getCell(x, y)->setTerrain(tile);
                }
            }
        }
    } else {
        for (int x = 0; x < map->width; x++) {
            for (int y = 0; y < map->height; y++) {
                if ( (rand()%100) < 10 ) {
                    if (getCell(x, y)->isEmpty()) {
                        int randNumber = ( 125+(rand()%2) );
                        Tile* tile = map->getTileSets()->getTileSet(0)->getLocalTile(randNumber);
                        getCell(x, y)->setTerrain(tile);
                    }
                }
            }
        }
    }
    return true;
}

Cell* GameField::getCell(int x, int y) {
    if (x >= 0 && x < map->width) {
        if (y >= 0 && y < map->height) {
            return &field[map->width*y + x];
        }
    }
    return NULL;
}

void GameField::updateCellsGraphicCoordinates(float sizeCellX, float sizeCellY) {
    for (int cellX = 0; cellX < map->width; cellX++) {
        for (int cellY = 0; cellY < map->height; cellY++) {
            field[map->width*cellY + cellX].setGraphicCoordinates(cellX, cellY, sizeCellX, sizeCellY, gameSettings->isometric);
        }
    }
}

void GameField::updateTowersGraphicCoordinates(CameraController *cameraController) {
    foreach (Tower* tower, towersManager->towers) {
        tower->updateGraphicCoordinates(cameraController);
    }
}

void GameField::render(float deltaTime, CameraController* cameraController) {
//    qDebug() << "GameField::render(); -- deltaTime:" << deltaTime;
    deltaTime = deltaTime * gameSpeed;
    if (!gamePaused) {
        timeOfGame += deltaTime;
        spawnUnits(deltaTime);
        stepAllUnits(deltaTime, cameraController);
        shotAllTowers(deltaTime, cameraController);
        moveAllShells(deltaTime, cameraController);
    }
//        drawFullField(cameraController);
        if (cameraController->isDrawableBackground) {
            drawBackGrounds(cameraController);
        }
        if (cameraController->isDrawableGround || cameraController->isDrawableUnits || cameraController->isDrawableTowers) {
            drawGroundsWithUnitsAndTowers(cameraController);
//            drawTowersUnderConstruction(cameraController);
        }
        if (cameraController->isDrawableForeground) {
            drawForeGrounds(cameraController);
        }

        cameraController->painter->save();
        drawUnitsBars(cameraController);
        cameraController->painter->restore();

        if (cameraController->isDrawableGrid)
            drawGrid(cameraController);
        if (cameraController->isDrawableGridNav) {
            drawGridNav(cameraController);
//            drawGridNavs(cameraController);
        }
        if (cameraController->isDrawableRoutes) {
            drawRoutes(cameraController);
//            drawWavesRoutes(camera);
        }
        drawBullets(cameraController);
        drawTowersUnderConstruction(cameraController);
}

//void GameField::drawFullField(CameraController* cameraController) {
////    qDebug() << "GameField::drawFullField(); -- map:" << map;
////    qDebug() << "GameField::drawFullField(); -- map->tileSets:size" << map->tileSets.size();
////    qDebug() << "GameField::drawFullField(); -- map->tileSets.getTile(85):" << map->tileSets.getTile(85);
////    qDebug() << "GameField::drawFullField(); -- map->tileSets.getTile(85)->getPixmap():" << map->tileSets.getTile(85)->getPixmap();
//    if(gameSettings->isometric) {
//        QPixmap pixmap = map->tileSets.getTile(85)->getPixmap(); // draw water2
//        int sizeX = 30;//width()/sizeCellX)+1;
//        int sizeY = 30;//(height()/sizeCellY)*2+2;
//        int isometricSpaceX = 0;
//        int isometricSpaceY = -(cameraController->sizeCellY/2);
//        for (int y = 0; y <= sizeY; y++) {
//            for (int x = 0; x <= sizeX; x++) {
//                cameraController->painter->drawPixmap(isometricSpaceX - cameraController->sizeCellX/2 + x*cameraController->sizeCellX, isometricSpaceY - cameraController->sizeCellY, sizeCellX, sizeCellY, pixmap);
//            }
//            isometricSpaceY += cameraController->sizeCellY/2;
//            isometricSpaceX = isometricSpaceX != 0 ? 0 : cameraController->sizeCellX/2;
//        }
//    }
//}

void GameField::drawGrid(CameraController* cameraController) {
    cameraController->painter->setPen(QPen(QColor(100, 60, 21), 1));
    if(!gameSettings->isometric) {
        float sizeCellX = cameraController->sizeCellX;
//            float sizeCellY = cameraController->sizeCellY;
        if (cameraController->isDrawableGrid == 1 || cameraController->isDrawableGrid == 5) {
            for (int x = 0; x < map->width+1; x++)
                cameraController->painter->drawLine(-(x*sizeCellX), 0, -(x*sizeCellX), -(sizeCellX*map->height));
            for (int y = 0; y < map->height+1; y++)
                cameraController->painter->drawLine(0, -(y*sizeCellX), -(sizeCellX*map->width), -(y*sizeCellX));
        }
        if (cameraController->isDrawableGrid == 2 || cameraController->isDrawableGrid == 5) {
            for (int x = 0; x < map->width+1; x++)
                cameraController->painter->drawLine(x*sizeCellX, 0, x*sizeCellX, -(sizeCellX*map->height));
            for (int y = 0; y < map->height+1; y++)
                cameraController->painter->drawLine(0, -(y*sizeCellX), sizeCellX*map->width, -(y*sizeCellX));
        }
        if (cameraController->isDrawableGrid == 3 || cameraController->isDrawableGrid == 5) {
            for (int x = 0; x < map->width+1; x++)
                cameraController->painter->drawLine(x*sizeCellX, 0, x*sizeCellX, sizeCellX*map->height);
            for (int y = 0; y < map->height+1; y++)
                cameraController->painter->drawLine(0, y*sizeCellX, sizeCellX*map->width, y*sizeCellX);
        }
        if (cameraController->isDrawableGrid == 4 || cameraController->isDrawableGrid == 5) {
            for (int x = 0; x < map->width+1; x++)
                cameraController->painter->drawLine(-(x*sizeCellX), 0, -(x*sizeCellX), sizeCellX*map->height);
            for (int y = 0; y < map->height+1; y++)
                cameraController->painter->drawLine(0, y*sizeCellX, -(sizeCellX*map->width), y*sizeCellX);
        }
    } else {
        float halfSizeCellX = cameraController->halfSizeCellX;
        float halfSizeCellY = cameraController->halfSizeCellY;
        int widthForTop = map->height * halfSizeCellX; // A - B
        int heightForTop = map->height * halfSizeCellY; // B - Top
        int widthForBottom = map->width * halfSizeCellX; // A - C
        int heightForBottom = map->width * halfSizeCellY; // C - Bottom
//    qDebug() << "GameField::drawGrid(camera); -- widthForTop:" << widthForTop << " heightForTop:" << heightForTop << " widthForBottom:" << widthForBottom << " heightForBottom:" << heightForBottom;
        if(cameraController->isDrawableGrid == 1 || cameraController->isDrawableGrid == 5) {
            for (int x = 0; x <= map->width; x++)
                cameraController->painter->drawLine(+(halfSizeCellX*x),-(halfSizeCellY*x),-(widthForTop)+(halfSizeCellX*x),-(heightForTop)-(x*halfSizeCellY));
            for (int y = 0; y <= map->height; y++)
                cameraController->painter->drawLine(-(halfSizeCellX*y),-(halfSizeCellY*y),+(widthForBottom)-(halfSizeCellX*y),-(heightForBottom)-(halfSizeCellY*y));
        }
        if(cameraController->isDrawableGrid == 2 || cameraController->isDrawableGrid == 5) {
            for (int x = 0; x <= map->width; x++)
                cameraController->painter->drawLine(+(halfSizeCellX*x),-(halfSizeCellY*x),+(widthForTop)+(halfSizeCellX*x),+(heightForTop)-(x*halfSizeCellY));
            for (int y = 0; y <= map->height; y++)
                cameraController->painter->drawLine(+(halfSizeCellX*y),+(halfSizeCellY*y),+(widthForBottom)+(halfSizeCellX*y),-(heightForBottom)+(halfSizeCellY*y));
        }
        if(cameraController->isDrawableGrid == 3 || cameraController->isDrawableGrid == 5) {
            for (int x = 0; x <= map->height; x++) // WHT??? map->height check groundDraw
                cameraController->painter->drawLine(-(halfSizeCellX*x),+(halfSizeCellY*x),+(widthForBottom)-(halfSizeCellX*x),+(heightForBottom)+(x*halfSizeCellY));
            for (int y = 0; y <= map->width; y++) // WHT??? map->width check groundDraw
                cameraController->painter->drawLine(+(halfSizeCellX*y),+(halfSizeCellY*y),-(widthForTop)+(halfSizeCellX*y),+(heightForTop)+(halfSizeCellY*y));
        }
        if(cameraController->isDrawableGrid == 4 || cameraController->isDrawableGrid == 5) {
            for (int x = 0; x <= map->height; x++) // WHT??? map->height check groundDraw
                cameraController->painter->drawLine(-(halfSizeCellX*x),+(halfSizeCellY*x),-(widthForBottom)-(halfSizeCellX*x),-(heightForBottom)+(x*halfSizeCellY));
            for (int y = 0; y <= map->width; y++) // WHT??? map->width check groundDraw
                cameraController->painter->drawLine(-(halfSizeCellX*y),-(halfSizeCellY*y),-(widthForTop)-(halfSizeCellX*y),+(heightForTop)-(halfSizeCellY*y));
        }
    }
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
    float sizeCellX = cameraController->sizeCellX;
    float sizeCellY = cameraController->sizeCellY*2;
    float deltaX = cameraController->halfSizeCellX;
    float deltaY = cameraController->sizeCellY + cameraController->halfSizeCellY;
    if (!gameSettings->isometric) {
        sizeCellY = cameraController->sizeCellY;
        deltaY = cameraController->halfSizeCellY;
    }
    Cell* cell = getCell(cellX, cellY);
    foreach (Tile* tile, cell->backgroundTiles) {
        QPixmap textureRegion = tile->getPixmap();
        if (cameraController->isDrawableBackground == 1 || cameraController->isDrawableBackground == 5) {
            cameraController->painter->drawPixmap(cell->graphicCoordinates1->x-deltaX, cell->graphicCoordinates1->y-deltaY, sizeCellX, sizeCellY, textureRegion);
        }
        if (cameraController->isDrawableBackground == 2 || cameraController->isDrawableBackground == 5) {
            cameraController->painter->drawPixmap(cell->graphicCoordinates2->x-deltaX, cell->graphicCoordinates2->y-deltaY, sizeCellX, sizeCellY, textureRegion);
        }
        if (cameraController->isDrawableBackground == 3 || cameraController->isDrawableBackground == 5) {
            cameraController->painter->drawPixmap(cell->graphicCoordinates3->x-deltaX, cell->graphicCoordinates3->y-deltaY, sizeCellX, sizeCellY, textureRegion);
        }
        if (cameraController->isDrawableBackground == 4 || cameraController->isDrawableBackground == 5) {
            cameraController->painter->drawPixmap(cell->graphicCoordinates4->x-deltaX, cell->graphicCoordinates4->y-deltaY, sizeCellX, sizeCellY, textureRegion);
        }
    }
}

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
    float sizeCellX = cameraController->sizeCellX;
    float sizeCellY = cameraController->sizeCellY*2;
    float deltaX = cameraController->halfSizeCellX;
    float deltaY = cameraController->sizeCellY + cameraController->halfSizeCellY;
    if (!gameSettings->isometric) {
        sizeCellY = cameraController->sizeCellY;
        deltaY = cameraController->halfSizeCellY;
    }
    Cell* cell = getCell(cellX, cellY);
    foreach (Tile* tile, cell->groundTiles) {
        QPixmap textureRegion = tile->getPixmap();
        if(cameraController->isDrawableGround == 1 || cameraController->isDrawableGround == 5) {
            cameraController->painter->drawPixmap(cell->graphicCoordinates1->x-deltaX, cell->graphicCoordinates1->y-deltaY, sizeCellX, sizeCellY, textureRegion);
        }
        if(cameraController->isDrawableGround == 2 || cameraController->isDrawableGround == 5) {
            cameraController->painter->drawPixmap(cell->graphicCoordinates2->x-deltaX, cell->graphicCoordinates2->y-deltaY, sizeCellX, sizeCellY, textureRegion);
        }
        if(cameraController->isDrawableGround == 3 || cameraController->isDrawableGround == 5) {
            cameraController->painter->drawPixmap(cell->graphicCoordinates3->x-deltaX, cell->graphicCoordinates3->y-deltaY, sizeCellX, sizeCellY, textureRegion);
        }
        if(cameraController->isDrawableGround == 4 || cameraController->isDrawableGround == 5) {
            cameraController->painter->drawPixmap(cell->graphicCoordinates4->x-deltaX, cell->graphicCoordinates4->y-deltaY, sizeCellX, sizeCellY, textureRegion);
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
    float sizeCellX = cameraController->sizeCellX;
    float sizeCellY = cameraController->sizeCellY*2;
    float deltaX = cameraController->halfSizeCellX;
    float deltaY = cameraController->sizeCellY + cameraController->halfSizeCellY;
    if (!gameSettings->isometric) {
        sizeCellY = cameraController->sizeCellY;
        deltaY = cameraController->halfSizeCellY;
    }
    Cell* cell = getCell(cellX, cellY);
    foreach (Tile* tile, cell->foregroundTiles) {
        QPixmap textureRegion = tile->getPixmap();
        if (cameraController->isDrawableForeground == 1 || cameraController->isDrawableForeground == 5) {
            cameraController->painter->drawPixmap(cell->graphicCoordinates1->x-deltaX, cell->graphicCoordinates1->y-deltaY, sizeCellX, sizeCellY, textureRegion);
        }
        if (cameraController->isDrawableForeground == 2 || cameraController->isDrawableForeground == 5) {
            cameraController->painter->drawPixmap(cell->graphicCoordinates2->x-deltaX, cell->graphicCoordinates2->y-deltaY, sizeCellX, sizeCellY, textureRegion);
        }
        if (cameraController->isDrawableForeground == 3 || cameraController->isDrawableForeground == 5) {
            cameraController->painter->drawPixmap(cell->graphicCoordinates3->x-deltaX, cell->graphicCoordinates3->y-deltaY, sizeCellX, sizeCellY, textureRegion);
        }
        if (cameraController->isDrawableForeground == 4 || cameraController->isDrawableForeground == 5) {
            cameraController->painter->drawPixmap(cell->graphicCoordinates4->x-deltaX, cell->graphicCoordinates4->y-deltaY, sizeCellX, sizeCellY, textureRegion);
        }
    }
}

void GameField::drawUnit(CameraController *cameraController, Unit *unit) {
//    qDebug() << "GameField::drawUnit(); -- cameraController:" << cameraController << " unit:" << unit;
//    for (ShellEffectType shellAttackType : unit.shellEffectTypes) {
//        if(shellAttackType.shellEffectEnum == ShellEffectType.ShellEffectEnum.FreezeEffect) {
//            spriteBatch.setColor(0.0f, 0.0f, 1.0f, 0.9f);
//            // qDebug() << "GameField::drawUnit(" + unit + "," + spriteBatch + ")", "-- FreezeEffect!");
//        }
//        if(shellAttackType.shellEffectEnum == ShellEffectType.ShellEffectEnum.FireEffect) {
//            spriteBatch.setColor(1.0f, 0.0f, 0.0f, 0.9f);
//            // qDebug() << "GameField::drawUnit(" + unit + "," + spriteBatch + ")", "-- FireEffect!");
//        }
//    }
    QPixmap currentFrame;
    if (unit->isAlive()) {
        currentFrame = unit->getCurrentFrame();
    } else {
        currentFrame = unit->getCurrentDeathFrame();
    }

    float sizeCellX = cameraController->sizeCellX;
    float sizeCellY = cameraController->sizeCellY*2;
    float deltaX = cameraController->halfSizeCellX;
    float deltaY = cameraController->sizeCellY;
    if (!gameSettings->isometric) {
        sizeCellY = cameraController->sizeCellY;
        deltaY = cameraController->halfSizeCellY;
    }
    float fVx = 0, fVy = 0;
    if(cameraController->isDrawableUnits == 1 || cameraController->isDrawableUnits == 5) {
        fVx = unit->circle1->x - deltaX;
        fVy = unit->circle1->y - deltaY;
        cameraController->painter->drawPixmap(fVx, fVy, sizeCellX, sizeCellY, currentFrame);
    }
    if(cameraController->isDrawableUnits == 2 || cameraController->isDrawableUnits == 5) {
        fVx = unit->circle2->x - deltaX;
        fVy = unit->circle2->y - deltaY;
        cameraController->painter->drawPixmap(fVx, fVy, sizeCellX, sizeCellY, currentFrame);
    }
    if(cameraController->isDrawableUnits == 3 || cameraController->isDrawableUnits == 5) {
        fVx = unit->circle3->x - deltaX;
        fVy = unit->circle3->y - deltaY;
        cameraController->painter->drawPixmap(fVx, fVy, sizeCellX, sizeCellY, currentFrame);
    }
    if(cameraController->isDrawableUnits == 4 || cameraController->isDrawableUnits == 5) {
        fVx = unit->circle4->x - deltaX;
        fVy = unit->circle4->y - deltaY;
        cameraController->painter->drawPixmap(fVx, fVy, sizeCellX, sizeCellY, currentFrame);
    }
//    drawUnitBar(shapeRenderer, unit, currentFrame, fVx, fVy);
}

void GameField::drawUnitsBars(CameraController *cameraController) {
    foreach (Unit* unit, unitsManager->units) {
        if(cameraController->isDrawableUnits == 1 || cameraController->isDrawableUnits == 5) {
            drawUnitBar(cameraController, unit, unit->circle1->x, unit->circle1->y);
        }
        if(cameraController->isDrawableUnits == 2 || cameraController->isDrawableUnits == 5) {
            drawUnitBar(cameraController, unit, unit->circle2->x, unit->circle2->y);
        }
        if(cameraController->isDrawableUnits == 3 || cameraController->isDrawableUnits == 5) {
            drawUnitBar(cameraController, unit, unit->circle3->x, unit->circle3->y);
        }
        if(cameraController->isDrawableUnits == 4 || cameraController->isDrawableUnits == 5) {
            drawUnitBar(cameraController, unit, unit->circle4->x, unit->circle4->y);
        }
    }
}

void GameField::drawUnitBar(CameraController *cameraController, Unit *unit, float fVx, float fVy) {
    if (unit->isAlive()) {
        QPixmap currentFrame = unit->getCurrentFrame();
        fVx -= cameraController->sizeCellX/2;
        fVy -= cameraController->sizeCellY;
        float currentFrameWidth = currentFrame.width();
        float currentFrameHeight = currentFrame.height();
        float hpBarSpace = 0.8;
        float effectBarWidthSpace = hpBarSpace * 2;
        float effectBarHeightSpace = hpBarSpace * 2;
        float hpBarHPWidth = 30;
        float effectBarWidth = hpBarHPWidth - effectBarWidthSpace * 2;
        float hpBarHeight = 7;
        float effectBarHeight = hpBarHeight - (effectBarHeightSpace * 2);
        float hpBarWidthIndent = (currentFrameWidth - hpBarHPWidth) / 2;
        float hpBarTopIndent = hpBarHeight;

        cameraController->painter->setBrush(QColor(0, 0, 0));
        cameraController->painter->fillRect(fVx + hpBarWidthIndent, fVy + currentFrameHeight - hpBarTopIndent, hpBarHPWidth, hpBarHeight, cameraController->painter->brush());
        cameraController->painter->setBrush(QColor(0, 255, 0));
        float maxHP = unit->templateForUnit->healthPoints;
        float hp = unit->hp;
        hpBarHPWidth = hpBarHPWidth / maxHP * hp;
        cameraController->painter->fillRect(fVx + hpBarWidthIndent + hpBarSpace, fVy + currentFrameHeight - hpBarTopIndent + hpBarSpace, hpBarHPWidth - (hpBarSpace * 2), hpBarHeight - (hpBarSpace * 2), cameraController->painter->brush());

        float allTime = 0.0;
        foreach (TowerShellEffect* towerShellEffect, unit->shellEffectTypes) {
            allTime += towerShellEffect->time;
        }

        if (allTime != 0.0) {
            float effectWidth = effectBarWidth / allTime;
            float efX = fVx + hpBarWidthIndent + effectBarWidthSpace;
            float efY = fVy + currentFrameHeight - hpBarTopIndent + effectBarHeightSpace;
            float effectBlockWidth = effectBarWidth / unit->shellEffectTypes.size();
            for (int effectIndex = 0; effectIndex < unit->shellEffectTypes.size(); effectIndex++) {
                TowerShellEffect* towerShellEffect = unit->shellEffectTypes.at(effectIndex);
                if (towerShellEffect->shellEffectEnum == TowerShellEffect::ShellEffectEnum::FireEffect) {
                    cameraController->painter->setBrush(QColor(255, 0, 0));
                } else if (towerShellEffect->shellEffectEnum == TowerShellEffect::ShellEffectEnum::FreezeEffect) {
                    cameraController->painter->setBrush(QColor(0, 0, 255));
                }
                float efWidth = effectBlockWidth - (effectWidth * towerShellEffect->elapsedTime);
                cameraController->painter->fillRect(efX, efY, efWidth, effectBarHeight, cameraController->painter->brush().color());
                efX += effectBlockWidth;
    //            qDebug() << "GameField::drawUnit(); -- efX:" << efX << " efWidth:" << efWidth << ":" << effectIndex;
            }
        }
    }
}

void GameField::drawTower(CameraController* cameraController, Tower* tower) {
    Cell* cell = tower->cell;
    int towerSize = tower->templateForTower->size;
//    Vector2* towerPos = new Vector2(cell->getGraphicCoordinates(cameraController->isDrawableTowers));
//    cameraController->painter->drawEllipse(towerPos->getPointF(), 3, 3);
    Vector2 *towerPos = new Vector2();
    QPixmap currentFrame = tower->templateForTower->idleTile->getPixmap();
    float sizeCellX = cameraController->sizeCellX;
    float sizeCellY = cameraController->sizeCellY*2;
    if (!gameSettings->isometric) {
        sizeCellY = cameraController->sizeCellY;
    }
    if (cameraController->isDrawableTowers == 5) {
        for (int m = 1; m < cameraController->isDrawableTowers; m++) {
            towerPos->set(cell->getGraphicCoordinates(m));
            cameraController->getCorrectGraphicTowerCoord(towerPos, towerSize, m);
            cameraController->painter->drawPixmap(towerPos->x, towerPos->y, sizeCellX * towerSize, sizeCellY * towerSize, currentFrame);
//            cameraController->painter->drawEllipse(towerPos->getPointF(), tower->radiusDetectionCircle->radius, tower->radiusDetectionCircle->radius/2);
//            cameraController->painter->drawEllipse(tower->radiusDetectionCircle->getPosition()->getPointF(), tower->radiusDetectionCircle->radius, tower->radiusDetectionCircle->radius);
        }
    } else if (cameraController->isDrawableTowers != 0) {
        towerPos->set(cell->getGraphicCoordinates(cameraController->isDrawableTowers));
        cameraController->getCorrectGraphicTowerCoord(towerPos, towerSize, cameraController->isDrawableTowers);
        cameraController->painter->drawPixmap(towerPos->x, towerPos->y, sizeCellX * towerSize, sizeCellY * towerSize, currentFrame);
//        cameraController->painter->drawEllipse(towerPos->getPointF(), tower->radiusDetectionCircle->radius, tower->radiusDetectionCircle->radius/2);
//        cameraController->painter->drawEllipse(tower->radiusDetectionCircle->getPosition()->getPointF(), tower->radiusDetectionCircle->radius, tower->radiusDetectionCircle->radius);
    }
    cameraController->painter->setPen(QColor(255, 255, 255, 255));
    if (cameraController->prevCellX == cell->cellX && cameraController->prevCellY == cell->cellY) {
        cameraController->painter->drawEllipse(tower->radiusDetectionCircle->getPosition()->getPointF(), tower->radiusDetectionCircle->radius, tower->radiusDetectionCircle->radius);
    }
    delete towerPos; // towerPos = NULL;
}

void GameField::drawBullets(CameraController *cameraController) {
    for (Tower* tower : towersManager->towers) {
        for (Bullet* bullet : tower->bullets) {
            QPixmap textureRegion = bullet->pixmap;
            if (!textureRegion.isNull()) {
//                float width = textureRegion.getRegionWidth() * bullet.ammoSize;
//                float height = textureRegion.getRegionHeight() * bullet.ammoSize;
//                spriteBatch.draw(textureRegion, bullet.currentPoint.x, bullet.currentPoint.y, width, height);
                cameraController->painter->drawPixmap(bullet->currentPoint->x - bullet->currCircle->radius, bullet->currentPoint->y - bullet->currCircle->radius, bullet->currCircle->radius * 2, bullet->currCircle->radius * 2, textureRegion);
//                Gdx.app.log("GameField", "drawProjecTiles(); -- Draw bullet:" + bullet.currentPoint);
            }
        }
    }
}

void GameField::drawGridNav(CameraController *cameraController) {
    cameraController->painter->save();
    Vector2* pos = new Vector2();
    float gridNavRadius = cameraController->sizeCellX/20;
    for (int y = 0; y < cameraController->mapHeight; y++) {
        for (int x = 0; x < cameraController->mapWidth; x++) {
            Cell *cell = getCell(x, y);
            if (cell != NULL && !cell->isEmpty()) {
                if (cell->isTerrain()) {
                    cameraController->painter->setBrush(QColor(255, 0, 0));
                    if (cell->removableTerrain) {
                        cameraController->painter->setBrush(QColor(255, 0, 0, 100));
                    }
                } else if (cell->getUnit() != NULL) {
                    cameraController->painter->setBrush(QColor(0, 255, 0));
                } else if (cell->getTower() != NULL) {
                    cameraController->painter->setBrush(QColor(255, 255, 0));
                }
                if (cameraController->isDrawableGridNav == 1 || cameraController->isDrawableGridNav == 5) {
                    pos->set(cell->getGraphicCoordinates(1));
                    cameraController->painter->drawEllipse(pos->getPointF(), gridNavRadius, gridNavRadius);
                }
                if(cameraController->isDrawableGridNav == 2 || cameraController->isDrawableGridNav == 5) {
                    pos->set(cell->getGraphicCoordinates(2));
                    cameraController->painter->drawEllipse(pos->getPointF(), gridNavRadius, gridNavRadius);
                }
                if(cameraController->isDrawableGridNav == 3 || cameraController->isDrawableGridNav == 5) {
                    pos->set(cell->getGraphicCoordinates(3));
                    cameraController->painter->drawEllipse(pos->getPointF(), gridNavRadius, gridNavRadius);
                }
                if(cameraController->isDrawableGridNav == 4 || cameraController->isDrawableGridNav == 5) {
                    pos->set(cell->getGraphicCoordinates(4));
                    cameraController->painter->drawEllipse(pos->getPointF(), gridNavRadius, gridNavRadius);
                }
            }
        }
    }

    std::vector<QPoint*> spawnPoints = waveManager->getAllSpawnPoint();
    cameraController->painter->setBrush(QColor(0, 255, 204, 255));
    foreach (QPoint* spawnPoint, spawnPoints) {
        Cell* cell = getCell(spawnPoint->x(), spawnPoint->y());
        if(cameraController->isDrawableGridNav == 1 || cameraController->isDrawableGridNav == 5) {
            pos->set(cell->getGraphicCoordinates(1));
            cameraController->painter->drawEllipse(pos->x, pos->y, gridNavRadius, gridNavRadius);
        }
        if(cameraController->isDrawableGridNav == 2 || cameraController->isDrawableGridNav == 5) {
            pos->set(cell->getGraphicCoordinates(2));
            cameraController->painter->drawEllipse(pos->x, pos->y, gridNavRadius, gridNavRadius);
        }
        if(cameraController->isDrawableGridNav == 3 || cameraController->isDrawableGridNav == 5) {
            pos->set(cell->getGraphicCoordinates(3));
            cameraController->painter->drawEllipse(pos->x, pos->y, gridNavRadius, gridNavRadius);
        }
        if(cameraController->isDrawableGridNav == 4 || cameraController->isDrawableGridNav == 5) {
            pos->set(cell->getGraphicCoordinates(4));
            cameraController->painter->drawEllipse(pos->x, pos->y, gridNavRadius, gridNavRadius);
        }
    }

    std::vector<QPoint*> exitPoints = waveManager->getAllExitPoint();
    cameraController->painter->setBrush(QColor(255, 0, 102, 255));
    foreach (QPoint* exitPoint, exitPoints) {
        Cell* cell = getCell(exitPoint->x(), exitPoint->y());
        if(cameraController->isDrawableGridNav == 1 || cameraController->isDrawableGridNav == 5) {
            pos->set(cell->getGraphicCoordinates(1));
            cameraController->painter->drawEllipse(pos->x, pos->y, gridNavRadius, gridNavRadius);
        }
        if(cameraController->isDrawableGridNav == 2 || cameraController->isDrawableGridNav == 5) {
            pos->set(cell->getGraphicCoordinates(2));
            cameraController->painter->drawEllipse(pos->x, pos->y, gridNavRadius, gridNavRadius);
        }
        if(cameraController->isDrawableGridNav == 3 || cameraController->isDrawableGridNav == 5) {
            pos->set(cell->getGraphicCoordinates(3));
            cameraController->painter->drawEllipse(pos->x, pos->y, gridNavRadius, gridNavRadius);
        }
        if(cameraController->isDrawableGridNav == 4 || cameraController->isDrawableGridNav == 5) {
            pos->set(cell->getGraphicCoordinates(4));
            cameraController->painter->drawEllipse(pos->x, pos->y, gridNavRadius, gridNavRadius);
        }
    }

//    shapeRenderer.setColor(Color.ORANGE);
    cameraController->painter->setBrush(QColor(255, 165, 0));
    cameraController->painter->setPen(QColor(255, 165, 0));
    foreach (Tower* tower, towersManager->towers) {
        foreach (Bullet* bullet, tower->bullets) {
            cameraController->painter->drawLine(bullet->currentPoint->getPointF(), bullet->endPoint->getPointF());
            if (NULL != bullet->currCircle) {
                cameraController->painter->drawEllipse(bullet->currCircle->getPosition()->getPointF(), bullet->currCircle->radius, bullet->currCircle->radius);
            }
        }
    }

    cameraController->painter->restore();
    cameraController->painter->save();
    cameraController->painter->setPen(QColor(255, 0, 0));
    foreach (Unit* unit, unitsManager->units) {
        if( cameraController->isDrawableUnits == 1 ||  cameraController->isDrawableUnits == 5)
            cameraController->painter->drawEllipse(unit->circle1->getPosition()->getPointF(), unit->circle1->radius, unit->circle1->radius);
        if( cameraController->isDrawableUnits == 2 ||  cameraController->isDrawableUnits == 5)
            cameraController->painter->drawEllipse(unit->circle2->getPosition()->getPointF(), unit->circle2->radius, unit->circle2->radius);
        if( cameraController->isDrawableUnits == 3 ||  cameraController->isDrawableUnits == 5)
            cameraController->painter->drawEllipse(unit->circle3->getPosition()->getPointF(), unit->circle3->radius, unit->circle3->radius);
        if( cameraController->isDrawableUnits == 4 ||  cameraController->isDrawableUnits == 5)
            cameraController->painter->drawEllipse(unit->circle4->getPosition()->getPointF(), unit->circle4->radius, unit->circle4->radius);
    }

    cameraController->painter->setPen(QColor(255, 255, 255, 120));
//    Vector2* towerPos = new Vector2();
    foreach (Tower* tower, towersManager->towers) { // Draw white towers radius! -- radiusDetectionCircle
        if(cameraController->isDrawableGridNav == 5) {
            if(cameraController->isDrawableTowers == 5) {
                for (int m = 1; m < cameraController->isDrawableTowers; m++) {
                    cameraController->painter->drawEllipse(tower->centerGraphicCoord->getPointF(), tower->radiusDetectionCircle->radius, tower->radiusDetectionCircle->radius);
                }
            } else if(cameraController->isDrawableTowers != 0) {
                cameraController->painter->drawEllipse(tower->centerGraphicCoord->getPointF(), tower->radiusDetectionCircle->radius, tower->radiusDetectionCircle->radius);
            }
        } else /*if(cameraController->isDrawableGridNav != 0)*/ {
            if(cameraController->isDrawableGridNav == cameraController->isDrawableTowers) {
                cameraController->painter->drawEllipse(tower->centerGraphicCoord->getPointF(), tower->radiusDetectionCircle->radius, tower->radiusDetectionCircle->radius);
            }
        }
    }

    cameraController->painter->setPen(QColor(178, 34, 34));
    foreach (Tower* tower, towersManager->towers) { // Draw FIREBRICK towers radius! -- radiusFlyShellCircle
        if (tower->radiusFlyShellCircle != NULL) {
            if(cameraController->isDrawableGridNav == 5) {
                if(cameraController->isDrawableTowers == 5) {
                    for (int m = 1; m <= cameraController->isDrawableTowers; m++) {
                        cameraController->painter->drawEllipse(tower->centerGraphicCoord->getPointF(), tower->radiusFlyShellCircle->radius, tower->radiusFlyShellCircle->radius);
                    }
                } else {
                    cameraController->painter->drawEllipse(tower->centerGraphicCoord->getPointF(), tower->radiusFlyShellCircle->radius, tower->radiusFlyShellCircle->radius);
                }
            } else {
                if(cameraController->isDrawableGridNav == cameraController->isDrawableTowers) {
                    cameraController->painter->drawEllipse(tower->centerGraphicCoord->getPointF(), tower->radiusFlyShellCircle->radius, tower->radiusFlyShellCircle->radius);
                }
            }
        }
    }

//    cameraController->painter->setPen(QColor(255, 255, 0));
//    cameraController->painter->pen().setWidth(2);
//    bitmapFont.getData().setScale(0.7f);
    foreach (Tower* tower, towersManager->towers) { // Draw pit capacity value
        if (tower->templateForTower->towerAttackType == TowerAttackType::Pit) {
            cameraController->painter->setPen(QColor(255, 255, 0));
            if(cameraController->isDrawableGridNav == 5) {
                if(cameraController->isDrawableTowers == 5) {
                    for (int m = 1; m <= cameraController->isDrawableTowers; m++) {
                        cameraController->painter->drawText(tower->centerGraphicCoord->getPointF(), QString::number(tower->capacity));
                    }
                } else {
                    cameraController->painter->drawText(tower->centerGraphicCoord->getPointF(), QString::number(tower->capacity));
                }
            } else {
                if(cameraController->isDrawableGridNav == cameraController->isDrawableTowers) {
                    cameraController->painter->drawText(tower->centerGraphicCoord->getPointF(), QString::number(tower->capacity));
                }
            }
        }
//        cameraController->bitmapFont.getData().setScale(0.9f);
        if (tower->player == 0) {
            cameraController->painter->setPen(QColor(255, 0, 0));
        } else if (tower->player == 1) {
            cameraController->painter->setPen(QColor(0, 0, 255));
        }
        if(cameraController->isDrawableGridNav == 5) {
            if(cameraController->isDrawableTowers == 5) {
                for (int m = 1; m <= cameraController->isDrawableTowers; m++) {
                    cameraController->painter->drawText(tower->centerGraphicCoord->getPointF(), QString::number(tower->player));
                }
            } else {
                cameraController->painter->drawText(tower->centerGraphicCoord->getPointF(), QString::number(tower->player));
            }
        } else {
            if(cameraController->isDrawableGridNav == cameraController->isDrawableTowers) {
                cameraController->painter->drawText(tower->centerGraphicCoord->getPointF(), QString::number(tower->player));
            }
        }
    }
    cameraController->painter->restore();
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
    float sizeCellX = cameraController->sizeCellX;
    float sizeCellY = cameraController->sizeCellY*2;
    float deltaX = cameraController->halfSizeCellX;
    float deltaY = cameraController->sizeCellY + cameraController->halfSizeCellY;
    if (!gameSettings->isometric) {
        sizeCellY = cameraController->sizeCellY;
        deltaY = cameraController->halfSizeCellY;
    }
    Cell* cell = getCell(cellX, cellY);
    if (pathFinder->detectCollision({cellX, cellY})) {
//    foreach (QPixmap textureRegion, cell->backgroundTiles) {
        QPixmap textureRegion = map->tileSets.getTile(8)->getPixmap();
        if (cameraController->isDrawableGridNav == 1 || cameraController->isDrawableGridNav == 5) {
            cameraController->painter->drawPixmap(cell->graphicCoordinates1->x-deltaX, cell->graphicCoordinates1->y-deltaY, sizeCellX, sizeCellY, textureRegion);
        }
        if (cameraController->isDrawableGridNav == 2 || cameraController->isDrawableGridNav == 5) {
            cameraController->painter->drawPixmap(cell->graphicCoordinates2->x-deltaX, cell->graphicCoordinates2->y-deltaY, sizeCellX, sizeCellY, textureRegion);
        }
        if (cameraController->isDrawableGridNav == 3 || cameraController->isDrawableGridNav == 5) {
            cameraController->painter->drawPixmap(cell->graphicCoordinates3->x-deltaX, cell->graphicCoordinates3->y-deltaY, sizeCellX, sizeCellY, textureRegion);
        }
        if (cameraController->isDrawableGridNav == 4 || cameraController->isDrawableGridNav == 5) {
            cameraController->painter->drawPixmap(cell->graphicCoordinates4->x-deltaX, cell->graphicCoordinates4->y-deltaY, sizeCellX, sizeCellY, textureRegion);
        }
//    }
    }
}

void GameField::drawRoutes(CameraController *cameraController) {
    float radius = cameraController->sizeCellX/22;
    cameraController->painter->save();
    foreach (Unit* unit, unitsManager->units) {
        if (unit->player == 1) {
            cameraController->painter->setBrush(QColor(255, 255, 255));
        } else {
            cameraController->painter->setBrush(QColor(100, 60, 21));
        }
        AStar::CoordinateList unitRoute = unit->route;
        if (!unitRoute.empty()) {
            for (int k = 1; k < unitRoute.size(); k++) {
                AStar::Vec2i point = unitRoute[k];
                Cell* cell = getCell(point.x, point.y);
                if (cameraController->isDrawableRoutes == 1 || cameraController->isDrawableRoutes == 5) {
                    cameraController->painter->drawEllipse(cell->graphicCoordinates1->getPointF(), radius, radius);
                }
                if (cameraController->isDrawableRoutes == 2 || cameraController->isDrawableRoutes == 5) {
                    cameraController->painter->drawEllipse(cell->graphicCoordinates2->getPointF(), radius, radius);
                }
                if (cameraController->isDrawableRoutes == 3 || cameraController->isDrawableRoutes == 5) {
                    cameraController->painter->drawEllipse(cell->graphicCoordinates3->getPointF(), radius, radius);
                }
                if (cameraController->isDrawableRoutes == 4 || cameraController->isDrawableRoutes == 5) {
                    cameraController->painter->drawEllipse(cell->graphicCoordinates4->getPointF(), radius, radius);
                }
            }
            cameraController->painter->setBrush(QColor(193, 84, 193));
            AStar::Vec2i destinationPoint = unitRoute.front();
            Cell* cell = getCell(destinationPoint.x, destinationPoint.y);
            if (cameraController->isDrawableRoutes == 1 || cameraController->isDrawableRoutes == 5) {
                cameraController->painter->drawEllipse(cell->graphicCoordinates1->getPointF(), radius*0.7, radius*0.7);
            }
            if (cameraController->isDrawableRoutes == 2 || cameraController->isDrawableRoutes == 5) {
                cameraController->painter->drawEllipse(cell->graphicCoordinates2->getPointF(), radius*0.7, radius*0.7);
            }
            if (cameraController->isDrawableRoutes == 3 || cameraController->isDrawableRoutes == 5) {
                cameraController->painter->drawEllipse(cell->graphicCoordinates3->getPointF(), radius*0.7, radius*0.7);
            }
            if (cameraController->isDrawableRoutes == 4 || cameraController->isDrawableRoutes == 5) {
                cameraController->painter->drawEllipse(cell->graphicCoordinates4->getPointF(), radius*0.7, radius*0.7);
            }
        }
    }
    cameraController->painter->restore();
}

//void GameField::drawWavesRoutes(CameraController *cameraController) {
//    cameraController->shapeRenderer->setProjectionMatrix(cameraController->camera.combined);
//    cameraController->shapeRenderer->begin(ShapeRenderer.ShapeType.Filled);

//    cameraController->shapeRenderer->setColor(Color.BROWN);
//    for (Wave wave : waveManager.waves) {
//        drawWave(cameraController, wave);
//    }
//    cameraController->shapeRenderer->setColor(Color.BLUE);
//    for (Wave wave : waveManager.wavesForUser) {
//        drawWave(cameraController, wave);
//    }
//    cameraController->shapeRenderer->end();
//}

//void GameField::drawWave(CameraController *cameraController, Wave *wave) {
////        Gdx.app.log("GameField::drawWave(" + wave + ")", "--");
//    float linesWidth = cameraController->sizeCellX/15f;
//    ArrayDeque<Node> route = wave.route;
//    if (route != null && !route.isEmpty()) {
//        Iterator<Node> nodeIterator = route.iterator();
//        Node startNode = nodeIterator.next();
//        Node endNode = null;
//        while (nodeIterator.hasNext()) {
//            endNode = nodeIterator.next();
//            Cell startCell = field[startNode.getX()][startNode.getY()];
//            Cell endCell = field[endNode.getX()][endNode.getY()];
//            if(cameraController->isDrawableGridNav == 1 || cameraController->isDrawableGridNav == 5) {
//                cameraController->shapeRenderer->rectLine(startCell.graphicCoordinates1, endCell.graphicCoordinates1, linesWidth);
//            }
//            if(cameraController->isDrawableGridNav == 2 || cameraController->isDrawableGridNav == 5) {
//                cameraController->shapeRenderer->rectLine(startCell.graphicCoordinates2, endCell.graphicCoordinates2, linesWidth);
//            }
//            if(cameraController->isDrawableGridNav == 3 || cameraController->isDrawableGridNav == 5) {
//                cameraController->shapeRenderer->rectLine(startCell.graphicCoordinates3, endCell.graphicCoordinates3, linesWidth);
//            }
//            if(cameraController->isDrawableGridNav == 4 || cameraController->isDrawableGridNav == 5) {
//                cameraController->shapeRenderer->rectLine(startCell.graphicCoordinates4, endCell.graphicCoordinates4, linesWidth);
//            }
//            startNode = endNode;
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
    }
    QPoint startDrawCell(startX, startY);
    QPoint finishDrawCell(finishX, finishY);
    for (int x = startX; x <= finishX; x++) {
        for (int y = startY; y <= finishY; y++) {
            Cell* cell = getCell(buildX + x, buildY + y);
            if (cell == NULL || !cell->isEmpty()) {
                if (drawFull) {
                    canBuild = false;
                }
            }
        }
    }
    if (drawFull) {
        Cell* mainCell = getCell(buildX, buildY);
        if(mainCell != NULL) {
            cameraController->painter->save();
//            cameraController->painter->setOpacity(0.5);
//            cameraController->painter->setCompositionMode(QPainter::CompositionMode_SourceIn);
            if (enoughGold && canBuild) {
//                cameraController->painter->setBrush(QColor(0, 255, 80));
                cameraController->painter->setPen(QColor(0, 255, 0, 255));
            } else {
//                cameraController->painter->setBrush(QColor(255, 0, 80));
                cameraController->painter->setPen(QColor(255, 0, 0, 255));
            }
            if (cameraController->isDrawableTowers == 5) {
                for (int map = 1; map < cameraController->isDrawableTowers; map++) {
                    drawTowerUnderConstructionAndMarks(cameraController, map, templateForTower, mainCell, startDrawCell, finishDrawCell);
                }
            } else if (cameraController->isDrawableTowers != 0) {
                drawTowerUnderConstructionAndMarks(cameraController, cameraController->isDrawableTowers, templateForTower, mainCell, startDrawCell, finishDrawCell);
            }
            cameraController->painter->restore();
        }
    }
}

void GameField::drawTowerUnderConstructionAndMarks(CameraController* cameraController, int map, TemplateForTower* templateForTower, Cell* mainCell, QPoint startDrawCell, QPoint finishDrawCell) {
//    qDebug() << "GameField::drawTowerUnderConstructionAndMarks(); -- map:" << map << " templateForTower:" << templateForTower->toString().toStdString().c_str() << " mainCell:" << mainCell << " startDrawCell:" << startDrawCell << " finishDrawCell:" << finishDrawCell;
    float sizeCellX = cameraController->sizeCellX;
    float sizeCellY = cameraController->sizeCellY*2;
    if (!gameSettings->isometric) {
        sizeCellY = cameraController->sizeCellY;
    }
    QPixmap textureRegion = templateForTower->idleTile->getPixmap();
    int towerSize = templateForTower->size;
    Vector2 *towerPos = new Vector2(mainCell->getGraphicCoordinates(map));
    if (templateForTower->radiusDetection != 0.0) {
        cameraController->painter->drawEllipse(towerPos->getPointF(), templateForTower->radiusDetection, templateForTower->radiusDetection);
    }
    cameraController->getCorrectGraphicTowerCoord(towerPos, towerSize, map);
    cameraController->painter->drawPixmap(towerPos->x, towerPos->y, sizeCellX * towerSize, sizeCellY * towerSize, textureRegion);
//    cameraController->painter->drawEllipse(towerPos->getPointF(), 3, 3);//templateForTower->radiusDetection, templateForTower->radiusDetection);
//    cameraController->painter->fillRect(towerPos->x(), towerPos->y(), sizeCellX * towerSize, cameraController->sizeCellY * towerSize, cameraController->painter->pen().color());
    if (greenCheckmark != NULL && redCross != NULL) {
        Vector2 *markPos = new Vector2();
        for (int x = startDrawCell.x(); x <= finishDrawCell.x(); x++) {
            for (int y = startDrawCell.y(); y <= finishDrawCell.y(); y++) {
                Cell* markCell = getCell(mainCell->cellX + x, mainCell->cellY + y);
                if (markCell != NULL) {
                    markPos->set(markCell->getGraphicCoordinates(map));
                    if (gameSettings->isometric) {
                        markPos->add(-cameraController->halfSizeCellX, -cameraController->sizeCellY-cameraController->halfSizeCellY);
                    } else {
                        markPos->add(-cameraController->halfSizeCellX, -cameraController->halfSizeCellY);
                    }
                    if(markCell->isEmpty()) {
                        cameraController->painter->drawPixmap(markPos->x, markPos->y, sizeCellX, sizeCellY, *greenCheckmark);
                    } else {
                        cameraController->painter->drawPixmap(markPos->x, markPos->y, sizeCellX, sizeCellY, *redCross);
                    }
                }
            }
        }
        delete markPos; // markPos = NULL;
    }
    delete towerPos; // towerPos = NULL;
}

Unit *GameField::spawnUnitFromUser(TemplateForUnit *templateForUnit) {
    qDebug() << "GameField::spawnUnitFromUser(); -- templateForUnit:" << templateForUnit;
    if (gamerGold >= templateForUnit->cost) {
        gamerGold -= templateForUnit->cost;
        for (Wave* wave : waveManager->wavesForUser) {
            Cell* spawnCell = getCell(wave->spawnPoint->x(), wave->spawnPoint->y());
            Cell* destExitCell = getCell(wave->exitPoint->x(), wave->exitPoint->y());
            return createUnit(spawnCell, destExitCell, templateForUnit, 1, destExitCell); // create Player1 Unit
        }
    }
    return NULL;
}

void GameField::spawnUnits(float delta) {
    std::vector<TemplateNameAndPoints*> allUnitsForSpawn = waveManager->getAllUnitsForSpawn(delta);
    foreach (TemplateNameAndPoints* templateNameAndPoints, allUnitsForSpawn) {
        spawnUnit(templateNameAndPoints);
    }
}

Unit *GameField::spawnUnit(TemplateNameAndPoints *templateNameAndPoints) {
    if (templateNameAndPoints != NULL) {
        TemplateForUnit* templateForUnit = factionsManager->getTemplateForUnitByName(templateNameAndPoints->templateName);
        if (templateForUnit != NULL) {
            Cell* spawnCell = getCell(templateNameAndPoints->spawnPoint->x(), templateNameAndPoints->spawnPoint->y());
            Cell* destExitCell = getCell(templateNameAndPoints->exitPoint->x(), templateNameAndPoints->exitPoint->y());
            return createUnit(spawnCell, destExitCell, templateForUnit, 0, destExitCell); // create Computer0 Unit
        } else {
            qDebug() << "GameField::spawnUnit(); -- templateForUnit == null | templateName:" << templateNameAndPoints->templateName;
        }
    }
    return NULL;
}

Unit *GameField::spawnHeroInSpawnPoint() {
    qDebug() << "GameField::spawnHeroInSpawnPoint(); -- gameSettings->cellSpawnHero:" << gameSettings->cellSpawnHero << " gameSettings->cellExitHero:" << gameSettings->cellExitHero;
    if (gameSettings->cellSpawnHero != NULL && gameSettings->cellExitHero != NULL) {
        gameSettings->cellSpawnHero->removeTerrain(true);
        gameSettings->cellExitHero->removeTerrain(true);
        removeTower(gameSettings->cellSpawnHero->cellX, gameSettings->cellSpawnHero->cellY);
        removeTower(gameSettings->cellExitHero->cellX, gameSettings->cellExitHero->cellY);
        return createUnit(gameSettings->cellSpawnHero, gameSettings->cellExitHero, factionsManager->getTemplateForUnitByName("unit3_footman"), 1, gameSettings->cellExitHero); // player1 = hero
    }
    return NULL;
}

Unit *GameField::spawnHero(int cellX, int cellY) {
    qDebug() << "GameField::spawnHero(); -- cellX:" << cellX << " cellY:" << cellY;
    qDebug() << "GameField::spawnHero(); -- gameSettings.cellExitHero:" << gameSettings->cellExitHero;
    if (gameSettings->cellExitHero != NULL) {
        Cell* cell = getCell(cellX, cellY);
        if (cell != NULL) {
            cell->removeTerrain(true);
            removeTower(cell->cellX, cell->cellY);
            return createUnit(cell, cell, factionsManager->getTemplateForUnitByName("unit3_footman"), 1, gameSettings->cellExitHero); // player1 = hero
        }
    } else {
        int randomX = (int)(rand()%map->width);
        int randomY = (int)(rand()%map->height);
        gameSettings->cellExitHero = getCell(randomX, randomY);
        Unit* hero = spawnHero(cellX, cellY);
        if (hero == NULL) {
            gameSettings->cellExitHero = NULL;
        }
    }
    return NULL;
}

Unit *GameField::spawnCompUnitToRandomExit(int x, int y) {
    qDebug() << "GameField::spawnCompUnitToRandomExit(); -- x:" << x << " y:" << y;
    int randomX = (int)(rand()%map->width);
    int randomY = (int)(rand()%map->height);
    qDebug() << "GameField::spawnCompUnitToRandomExit(); -- randomX:" << randomX << " randomY:" << randomY;
    return createUnit(getCell(x, y), getCell(randomX, randomY), factionsManager->getRandomTemplateForUnitFromSecondFaction(), 0, NULL);
}

Unit *GameField::createUnit(Cell *spawnCell, Cell *destCell, TemplateForUnit* templateForUnit, int player, Cell *exitCell) {
//    qDebug() << "GameField::createUnit(); -- spawnPoint:" << spawnPoint;
//    qDebug() << "GameField::createUnit(); -- exitPoint:" << exitPoint;
//    qDebug() << "GameField::createUnit(); -- templateForUnit:" << templateForUnit->toString().toStdString().c_str();
//    qDebug() << "GameField::createUnit(); -- player:" << player;
//    qDebug() << "GameField::createUnit(); -- exitCell:" << exitCell;
//    if (destCell == NULL) {
//        destCell = waveManager.lastExitCell;
//    }
    Unit* unit = NULL;
    if (spawnCell != NULL && destCell != NULL && pathFinder != NULL) {
//        pathFinder.loadCharMatrix(getCharMatrix());
        AStar::CoordinateList path = pathFinder->findPath({spawnCell->cellX, spawnCell->cellY}, {destCell->cellX, destCell->cellY});
        if (!path.empty()) {
            unit = unitsManager->createUnit(path, templateForUnit, player, exitCell);
            spawnCell->setUnit(unit);
            qDebug() << "GameField::createUnit(); -- unit:" << unit;
        } else {
            qDebug() << "GameField::createUnit(); -- Not found route for createUnit!";
//            if(towersManager->towers.size() > 0) {
//                qDebug() << "GameField::createUnit(); -- Remove one last tower! And retry call createUnit()";
//                removeLastTower();
//                createUnit(spawnCell, destCell, templateForUnit, player, exitCell);
//            } else {
                return NULL;
//            }
        }
        return unit;
    } else {
        qDebug() << "GameField::createUnit(); -- Bad spawnCell:" << spawnCell << " || destCell:" << destCell << " || pathFinder:" << pathFinder;
        return NULL;
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

bool GameField::towerActions(int x, int y) {
    Cell* cell = getCell(x, y);
    if (cell != NULL) {
        if (getCell(x, y)->isEmpty()) {
            createTowerWithGoldCheck(x, y, factionsManager->getRandomTemplateForTowerFromAllFaction(), 1);
            rerouteAllUnits();
            return true;
        } else if (cell->getTower() != NULL) {
            removeTower(x, y);
            return true;
        }
    }
    return false;
}

Tower *GameField::createTowerWithGoldCheck(int buildX, int buildY, TemplateForTower *templateForTower, int player) {
    if (gamerGold >= templateForTower->cost) {
        Tower* tower = createTower(buildX, buildY, templateForTower, player);
//            rerouteForAllUnits();
        gamerGold -= templateForTower->cost;
        qDebug() << "GameField::createTowerWithGoldCheck(); -- Now gamerGold:" << gamerGold;
        return tower;
    } else {
        return NULL;
    }
}

Tower *GameField::createTower(int buildX, int buildY, TemplateForTower* templateForTower, int player) {
    if (templateForTower != NULL) {
        int towerSize = templateForTower->size;
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
                finishX = (towerSize / 2);
                finishY = (towerSize / 2);
            }
        }
        for (int tmpX = startX; tmpX <= finishX; tmpX++) {
            for (int tmpY = startY; tmpY <= finishY; tmpY++) {
                Cell* cell = getCell(buildX + tmpX, buildY + tmpY);
                if (cell == NULL || !cell->isEmpty()) {
                    return NULL;
                }
            }
        }

        Cell* cell = getCell(buildX, buildY);
        Tower* tower = towersManager->createTower(cell, templateForTower, player);
        if (templateForTower->towerAttackType != TowerAttackType::Pit) {
            for (int tmpX = startX; tmpX <= finishX; tmpX++) {
                for (int tmpY = startY; tmpY <= finishY; tmpY++) {
                    getCell(buildX + tmpX, buildY + tmpY)->setTower(tower);
//                    if (pathFinder != NULL) {
//                        pathFinder->walls[buildY + tmpY, buildX + tmpX]->setKey('T');
//                    }
                }
            }
        }
        qDebug() << "GameField::createTower(); -- tower:" << tower->toString(true).toStdString().c_str();
        return tower;
    }
    return NULL;
}

bool GameField::removeLastTower() {
    Tower* tower = towersManager->getTower();//towersManager->towers.size() - 1);
    if (tower != NULL) {
        Cell* cell = tower->cell;
        return ( (removeTower(cell->cellX, cell->cellY)==0)?false:true );
    }
    return false;
}

bool GameField::removeTowerWithGold(int cellX, int cellY) {
    int towerCost = removeTower(cellX, cellY);
    if (towerCost > 0) {
//        rerouteForAllUnits();
        gamerGold += towerCost; // *0.5;
        return true;
    }
    return false;
}

int GameField::removeTower(int cellX, int cellY) {
    Tower* tower = getCell(cellX, cellY)->getTower();
    if (tower != NULL) {
        int x = tower->cell->cellX;
        int y = tower->cell->cellY;
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
//                if (pathFinder != NULL) {
//                  pathFinder.getNodeMatrix()[y + tmpY][x + tmpX].setKey('.');
//                }
            }
        }
        int towerCost = tower->templateForTower->cost;
        towersManager->removeTower(tower);
        return towerCost;
    }
    return 0;
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

//public char[][] getCharMatrix() {

//void GameField::updateHeroDestinationPoint() {
//    qDebug() << "GameField::updateHeroDestinationPoint(); -- ";
////    Unit* hero = unitsManager->hero;
//    foreach (Unit* hero, unitsManager->hero) {
//        qDebug() << "GameField::updateHeroDestinationPoint(); -- hero:" << hero;
//        if (hero != NULL && !hero->route.empty()) {
//            updateHeroDestinationPoint(hero->route.front().x, hero->route.front().y);
//        } else {
//            updatePathFinderWalls();
//        }
//    }
//}

void GameField::rerouteHero() {
    rerouteHero(-1, -1);
}

void GameField::rerouteHero(int x, int y) {
    if (x == -1 && y == -1) {
        foreach (Unit* unit, unitsManager->hero) {
            AStar::CoordinateList route = unit->route;
            if (/*route != null &&*/ !route.empty()) {
                AStar::Vec2i node = route.back();// front
                rerouteUnitPath(unit, node.x, node.y);
            }
        }
    } else {
        foreach (Unit* unit, unitsManager->hero) {
            rerouteUnitPath(unit, x, y);
        }
    }
}

void GameField::rerouteAllUnits() {
    rerouteAllUnits(-1, -1);
}

void GameField::rerouteAllUnits(int x, int y) {
    if (x == -1 && y == -1) {
        foreach (Unit* unit, unitsManager->units) {
            AStar::CoordinateList route = unit->route;
            if (/*route != null &&*/ !route.empty()) {
//                AStar::Vec2i node = route.back();// front
                AStar::Vec2i node = route.front();
                rerouteUnitPath(unit, node.x, node.y);
            }
        }
    } else {
        foreach (Unit* unit, unitsManager->units) {
            rerouteUnitPath(unit, x, y);
        }
    }
}

void GameField::rerouteUnitPath(Unit *unit, int x, int y) {
    AStar::CoordinateList route = pathFinder->findPath(unit->newPosition, {x, y});
    if (/*route != null &&*/ !route.empty()) {
//        if (route.front().equals({x, y})) {
//        route.erase(route.begin());
            route.pop_back();;
            unit->route = route;
//        } else {
//            unit->route.clear();
//        }
    }
}

//void GameField::rerouteForAllUnits() {
//    rerouteForAllUnits(NULL);
//}

//void GameField::rerouteForAllUnits(QPoint exitPoint) {
//    if (pathFinder != NULL) {
////        long start = System.nanoTime();
////        Gdx.app.log("GameField::rerouteForAllUnits()", "-- Start:" + start);
////            pathFinder.loadCharMatrix(getCharMatrix());
//        foreach (Unit* unit, unitsManager->units) {
//            AStar::CoordinateList route;
////            ArrayDeque<Node> route;
//            if (exitPoint) {
//                route = unit->route;
//                if(route != null && route.size() > 0) {
//                    Node node = unit.route.getLast();
//                    GridPoint2 localExitPoint = new GridPoint2(node.getX(), node.getY());
//                    route = pathFinder.route(unit.newPosition.getX(), unit.newPosition.getY(), localExitPoint.x, localExitPoint.y); // TODO BAGA!
//                }
//            } else {
//                route = pathFinder.route(unit.newPosition.getX(), unit.newPosition.getY(), exitPoint.x, exitPoint.y); // TODO BAGA!
//            }
//            if (route != null && route.size() > 0) {
//                route.removeFirst();
//                unit.route = route;
//            }
////                    long end2 = System.nanoTime();
////                    Gdx.app.log("GameField", "rerouteForAllUnits()', "-- Thread End:" + (end2-start2));
////                }
////            }.init(unit, outExitPoint)).start();
//        }
//        long end = System.nanoTime();
//        Gdx.app.log("GameField::rerouteForAllUnits()", "-- End:" + end + " Delta time:" + (end-start));
//    } else {
//        Gdx.app.log("GameField::rerouteForAllUnits(" + exitPoint + ")", "-- pathFinder:" + pathFinder);
//    }
//}

void GameField::stepAllUnits(float deltaTime, CameraController* cameraController) {
    for (Unit* unit : unitsManager->units) {
        AStar::Vec2i oldPosition = unit->newPosition;
        if (unit->isAlive()) {
            AStar::Vec2i* newPosition = unit->move(deltaTime, cameraController);
            if (newPosition != NULL) {
                if (!newPosition->operator ==(oldPosition)) {
                    getCell(oldPosition.x, oldPosition.y)->removeUnit(unit);
                    getCell(newPosition->x, newPosition->y)->setUnit(unit);
//                    qDebug() << "GameField::stepAllUnits(); -- Unit move to X:" << newPosition->x << " Y:" << newPosition->y;
                }
            } else {
//                Cell *cell = getCell(oldPosition.x, oldPosition.y);
                if (unit->player == 1) {
                    gameSettings->missedUnitsForComputer0++;
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
//                if(!gameSettings->isometric) {
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
    updateTowersGraphicCoordinates(cameraController);
    foreach (Tower* tower, towersManager->towers) {
//        qDebug() << "GameField::shotAllTowers(); -- tower:" << tower->toString(true).toStdString().c_str();
        TowerAttackType::type towerAttackType = tower->templateForTower->towerAttackType;
        if (towerAttackType == TowerAttackType::Pit) {
            chechPitTower(tower);
        } else if (towerAttackType == TowerAttackType::Melee) {
            shotMeleeTower(tower);
        } else if (towerAttackType == TowerAttackType::Range || towerAttackType == TowerAttackType::RangeFly) {
            if (tower->recharge(deltaTime)) {
                foreach (Unit* unit, unitsManager->units) {
                    if (unit != NULL && unit->isAlive() && unit->player != tower->player) {
                        if ( ( (unit->templateForUnit->type == "fly") && towerAttackType == TowerAttackType::RangeFly) ||
                                ((unit->templateForUnit->type != "fly") && towerAttackType == TowerAttackType::Range)) {
//                            qDebug() << "GameField::shotAllTowers(); -1- tower->radiusDetectionCircle:" << tower->radiusDetectionCircle;
                            if (tower->radiusDetectionCircle != NULL) {
                                if (tower->radiusDetectionCircle->overlaps(unit->circle3)) { // circle1 2 3 4?
//                                    qDebug() << "GameField::shotAllTowers(); -- Intersector.overlaps()";
                                    if (tower->shoot(unit, cameraController)) {
                                        if(tower->templateForTower->towerShellType != TowerShellType::MassAddEffect) {
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        } else if (towerAttackType == TowerAttackType::FireBall) {
            if (tower->recharge(deltaTime)) {
//              fireBallTowerAttack(delta, tower);
              tower->shotFireBall(cameraController);
            }
        }
    }
}

bool GameField::chechPitTower(Tower *tower) {
    Unit* unit = getCell(tower->cell->cellX, tower->cell->cellY)->getUnit();
    if (unit != NULL && (unit->templateForUnit->type != "fly") && unit->player != tower->player) {
        qDebug() << "GameField::chechPitTower(); -- tower.capacity:" << tower->capacity << " unit.getHp:" << unit->hp;
//        unit.die(unit.getHp());
        unitsManager->removeUnit(unit);
        getCell(tower->cell->cellX, tower->cell->cellY)->removeUnit(unit);
        tower->capacity--;
        if (tower->capacity <= 0) {
            towersManager->removeTower(tower);
        }
        return true;
    }
    return false;
}

bool GameField::shotMeleeTower(Tower *tower) {
    bool attack = false;
    Cell* towerCell = tower->cell;
    int radius = (int)tower->templateForTower->radiusDetection;
    for (int tmpX = -radius; tmpX <= radius; tmpX++) {
        for (int tmpY = -radius; tmpY <= radius; tmpY++) {
            Cell* cell = getCell(tmpX + towerCell->cellX, tmpY + towerCell->cellY);
            if (cell != NULL && cell->getUnit() != NULL) {
                attack = true;
                Unit* unit = cell->getUnit();
                if (unit != NULL && (unit->templateForUnit->type != "fly") && unit->player != tower->player) {
                    if (unit->die(tower->templateForTower->damage, tower->templateForTower->towerShellEffect)) {
                        gamerGold += unit->templateForUnit->bounty;
                    }
                    if (tower->templateForTower->towerShellType == TowerShellType::SingleTarget) {
                        return true;
                    }
                }
            }
        }
    }
    return attack;
}

void GameField::moveAllShells(float delta, CameraController *cameraController) {
    foreach (Tower* tower, towersManager->towers) {
        tower->moveAllShells(delta, cameraController);
    }
}

void GameField::turnRight() {
    if(map->width == map->height) {
        Cell* newCells = new Cell[map->width*map->height];
        for(int y = 0; y < map->height; y++) {
            for(int x = 0; x < map->width; x++) {
                newCells[map->width*(x)+(map->width-y-1)] = field[map->width*y+x];
                newCells[map->width*(x)+(map->width-y-1)].setGraphicCoordinates(map->width-y-1, x, map->tileWidth, map->tileHeight, gameSettings->isometric);
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
                newCells[map->width*(x)+(map->width-y-1)].setGraphicCoordinates(map->width-y-1, x, map->tileWidth, map->tileHeight, gameSettings->isometric);
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
                newCells[map->width*(map->height-x-1)+(y)].setGraphicCoordinates(y, map->height-x-1, map->tileWidth, map->tileHeight, gameSettings->isometric);
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
                newCells[map->width*(map->height-x-1)+(y)].setGraphicCoordinates(y, map->height-x-1, map->tileWidth, map->tileHeight, gameSettings->isometric);
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
            newCells[map->width*(y)+(map->width-x-1)].setGraphicCoordinates(map->width-x-1, y, map->tileWidth, map->tileHeight, gameSettings->isometric);
        }
    }
//    delete field;
    field = newCells;
}

void GameField::flipY() {
    Cell* newCells = new Cell[map->width*map->height];
    for(int y = 0; y < map->height; y++) {
        for(int x = 0; x < map->width; x++) {
            newCells[map->width*(map->height-y-1)+(x)] = field[map->width*y+x];
            newCells[map->width*(map->height-y-1)+(x)].setGraphicCoordinates(x, map->height-y-1, map->tileWidth, map->tileHeight, gameSettings->isometric);
        }
    }
//    delete field;
    field = newCells;
}
