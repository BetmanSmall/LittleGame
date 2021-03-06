#include "cameracontroller.h"

//CameraController::CameraController(int mapWidth, int mapHeight, float sizeCellX, float sizeCellY) {
CameraController::CameraController(GameField *gameField) {
    this->painter = new QPainter();

    this->gameField = gameField;
//    this->gameInterface = gameInterface;
//    this->camera = camera;

    this->mapWidth = gameField->map->width;
    this->mapHeight = gameField->map->height;
    this->sizeCellX = gameField->map->tileWidth;
    this->sizeCellY = gameField->map->tileHeight;
    this->halfSizeCellX = sizeCellX/2;
    this->halfSizeCellY = sizeCellY/2;

//    this->borderLeftX  = (0 - (halfSizeCellX * mapHeight));
//    this->borderRightX = (0 + (halfSizeCellX * mapWidth));
//    this->borderUpY    = (0);
//    this->borderDownY  = (0 - (sizeCellY * (mapWidth>mapHeight ? mapWidth : mapHeight)));
}

CameraController::~CameraController() {
    delete painter;
//    gameField = NULL;
}

//void CameraController::setBorders(float borderLeftX, float borderRightX, float borderUpY, float borderDownY) {
//}

//bool CameraController::tap(float x, float y, int count, int button) {
//    if (gameInterface.touchDown(x, y, count, button)) {
//        return false;
//    }
//    return false;
//}

bool CameraController::touchDown(int screenX, int screenY, int pointer, int button) {
    qDebug() << "CameraController::touchDown(); -- screenX:" << screenX << " screenY:" << screenY << " pointer:" << pointer << " button:" << button;
    this->touchDownX = screenX;
    this->touchDownY = screenY;
    this->prevMouseX = screenX;
    this->prevMouseY = screenY;
    whichPrevCell(screenX, screenY, 5);
    if ( ( (panLeftMouseButton && button == Qt::LeftButton) ||
           (panRightMouseButton && button == Qt::RightButton) ||
           (panMidMouseButton && button == Qt::MiddleButton) ) ) {
//            setCursor(Qt::ClosedHandCursor);
            this->paning = true;
    }
    this->flinging = false;
    initialScale = zoom;
//    if (!gameInterface.interfaceTouched) {
        UnderConstruction* underConstruction = gameField->getUnderConstruction();
        if (underConstruction != NULL) {
            if (button == Qt::LeftButton) {
                if (whichCell(screenX, screenY, isDrawableTowers)) {
                    underConstruction->setStartCoors((int) screenX, (int) screenY);
                }
//            } else if (button == Qt::RightButton) {
//                gameField->cancelUnderConstruction();
            }
        }
//    }
    return false;
}

bool CameraController::touchUp(int screenX, int screenY, int pointer, int button) {
    qDebug() << "CameraController::touchUp(); -- screenX:" << screenX << " screenY:" << screenY << " pointer:" << pointer << " button:" << button;
    if (paning) {
        if ( ( (panLeftMouseButton && button == Qt::LeftButton) ||
               (panRightMouseButton && button == Qt::RightButton) ||
               (panMidMouseButton && button == Qt::MiddleButton) ) ) {
//                setCursor(Qt::ArrowCursor);
                this->paning = false;
        }
        fling((float)(prevMouseX)-(screenX), (float)(prevMouseY)-(screenY), button);
    }
//    if (!gameInterface.interfaceTouched) {
        if (gameField->getUnderConstruction() != NULL) {
            if (button == Qt::LeftButton) {
                if (whichCell(screenX, screenY, isDrawableTowers)) {
                    gameField->buildTowersWithUnderConstruction((int) screenX, (int) screenY);
                }
            } else if (button == Qt::RightButton) {
                gameField->cancelUnderConstruction();
//                gameField.removeTower((int) touch.x, (int) touch.y);
            }
        } else {
//            int tmpCellX = screenX;
//            int tmpCellY = screenY;
//            whichCell(tmpCellX, tmpCellY, 5);
            if ( (touchDownX == screenX && touchDownY == screenY) /*|| (prevCellX == tmpCellX && prevCellY == tmpCellY)*/ ) {
                if (gameField->gameSettings->gameType == GameType::LittleGame) {
                    if (button == Qt::LeftButton) {
                        if (whichCell(screenX, screenY, isDrawableUnits)) {
                            gameField->rerouteHero((int) screenX, (int) screenY);
                        }
                    } else if (button == Qt::RightButton) {
                        if (whichCell(screenX, screenY, isDrawableGround)) {
                            Cell* cell = gameField->getCell((int) screenX, (int) screenY);
                            if (cell->isTerrain()) {
                                cell->removeTerrain((((int) (rand()%2)) == 0) ? true : false);
                                qDebug() << "CameraController::touchDown(); -- x:" << cell->cellX << " y:" << cell->cellY << " cell.isTerrain():" << cell->isTerrain();
                            } else if (cell->getTower() != NULL) {
                                Tower* tower = cell->getTower();
                                gameField->removeTower(tower->cell->cellX, tower->cell->cellY);
                            } else if (cell->isEmpty()) {
//                                gameField.towerActions(cell.cellX, cell.cellY);
                                gameField->createTower(cell->cellX, cell->cellY, gameField->factionsManager->getRandomTemplateForTowerFromAllFaction(), ((int) (rand()%3)));
                                if ((((int) (rand()%2)) == 0) ? true : false) {
                                    int randNumber = (125 + (int) (rand()%2));
                                    cell->setTerrain(gameField->map->getTileSets()->getTileSet(0)->getLocalTile(randNumber), true, true);
                                }
                            }
                        }
                    } else if (button == Qt::MiddleButton) {
                        if (whichCell(screenX, screenY, isDrawableUnits)) {
                            if (((int) (rand()%5) == 0)) {
                                gameField->spawnHero((int) screenX, (int) screenY);
                            } else {
                                gameField->spawnCompUnitToRandomExit((int) screenX, (int) screenY);
                            }
                        }
                    }
                } else if (gameField->gameSettings->gameType == GameType::TowerDefence) {
                    if (button == Qt::LeftButton || button == Qt::RightButton) {
                        if (whichCell(screenX, screenY, isDrawableTowers)) {
                            gameField->towerActions((int) screenX, (int) screenY);
                        }
                    } else if (button == Qt::MiddleButton) {
                        if (whichCell(screenX, screenY, isDrawableUnits)) {
                            gameField->spawnCompUnitToRandomExit((int) screenX, (int) screenY);
                        }
                    } else if (button == 4) {
                        if (whichCell(screenX, screenY, isDrawableUnits)) {
//                            gameField.setExitPoint((int) screenX, (int) screenY);
                        }
                    }
                }
            }
        }
//    }
//    gameInterface.interfaceTouched = false;
    return false;
}

bool CameraController::fling(float velocityX, float velocityY, int button) {
    qDebug() << "CameraController::fling(); -- velocityX:" << velocityX << " velocityY:" << velocityY << " button:" << button;
//    if (!gameInterface.interfaceTouched) {
        flinging = true;
        velX = ( (1/zoom) * velocityX) * 0.05f;
        velY = ( (1/zoom) * velocityY) * 0.05f;
//    }
    qDebug() << "CameraController::fling(); -- velX:" << velX << " velY:" << velY;
    return false;
}

bool CameraController::mouseMoved(int screenX, int screenY, int buttons) {
//    qDebug() << "CameraController::mouseMoved(); -- screenX:" << screenX << " screenY:" << screenY << " button:" << buttons;
    whichPrevCell(screenX, screenY, 5);
    float deltaX = prevMouseX - screenX;
    float deltaY = prevMouseY - screenY;
    pan(screenX, screenY, deltaX, deltaY, buttons);
    prevMouseX = screenX;
    prevMouseY = screenY;
    if (gameField != NULL && gameField->getUnderConstruction() != NULL) {
        if (whichCell(screenX, screenY, isDrawableTowers)) {
            gameField->getUnderConstruction()->setEndCoors((int)screenX, (int)screenY);
        }
    }
    return false;
}

//public boolean touchDragged(int screenX, int screenY, int pointer) {

bool CameraController::pan(float x, float y, float deltaX, float deltaY, int buttons) {
//    qDebug() << "CameraController::pan(); -- x:" << x << " x:" << x;
//    qDebug() << "CameraController::pan(); -- deltaX:" << deltaX << " deltaY:" << deltaY;
//    qDebug() << "CameraController::pan(); -- button:" << buttons;
//    if (paning && gameInterface.pan(x, y, deltaX, deltaY)) {
//        return true;
//    }
    if (gameField->getUnderConstruction() == NULL || buttons == Qt::MiddleButton) {
        if (paning) {
            float newCameraX = cameraX - (deltaX * (1/zoom) );
            float newCameraY = cameraY - (deltaY * (1/zoom) );
            if (borderLeftX != 0.0 || borderRightX != 0.0 || borderUpY != 0.0 || borderDownY != 0.0) {
                if (borderLeftX < newCameraX && newCameraX < borderRightX &&
                        borderUpY > newCameraY && newCameraY > borderDownY) {
                    this->cameraX = newCameraX;
                    this->cameraY = newCameraY;
                }
            } else {
                this->cameraX = newCameraX;
                this->cameraY = newCameraY;
            }
        }
    }
//    qDebug() << "CameraController::pan(); -- viewportWidth:" << viewportWidth << " viewportHeight:" << viewportHeight;
    float space = 50.0;
    float shiftCamera = 5.0;
    if (x < space) {
        cameraX += shiftCamera;
    }
    if (x > viewportWidth - space) {
        cameraX -= shiftCamera;
    }
    if (y < space) {
        cameraY += shiftCamera;
    }
    if (y > viewportHeight - space) {
        cameraY -= shiftCamera;
    }
    return false;
}

bool CameraController::panStop(float x, float y, int pointer, int button) {
    qDebug() << "CameraController::panStop(); -- x:" << x << " y:" << y << " pointer:" << pointer << " button:" << button;
//    if(gameInterface.panStop(x, y, pointer, button)) {
//        return true;
//    }
    return false;
}

bool CameraController::scrolled(int amount) {
    qDebug() << "CameraController::scrolled(); -- amount:" << amount;
//    if (gameInterface.scrolled(amount)) {
//        return false;
//    }
    if (amount > 0) {
        if (zoom <= zoomMax) {
            zoom += 0.1;
        }
    } else if (amount < 0) {
        if (zoom >= zoomMin) {
            zoom -= 0.1;
        }
    }
//    camera.update();
    qDebug() << "CameraController::scrolled(); -- zoom:" << zoom;
    return false;
}

// Android-libgGDX
//@Override
//public boolean zoom(float initialDistance, float distance) {
//public boolean longPress(float x, float y) {
//public boolean touchDown(float x, float y, int pointer, int button)
//public boolean tap(float x, float y, int count, int button) {
//public boolean pinch(Vector2 initialPointer1, Vector2 initialPointer2, Vector2 pointer1, Vector2 pointer2) {
//public boolean keyDown(int keycode) {
//public boolean keyUp(int keycode) {
//public boolean keyTyped(char character) {
// Android-libgGDX

void CameraController::update(float deltaTime) {
//    qDebug() << "CameraController::update(); -- deltaTime:" << deltaTime;
//    try {
        if (gameField->getUnderConstruction() == NULL) {
            if (flinging) {
                velX *= 0.98f;
                velY *= 0.98f;
                float newCameraX = cameraX - velX;
                float newCameraY = cameraY - velY;
//                float newCameraX = cameraX - (velX * deltaTime);
//                float newCameraY = cameraY - (velY * deltaTime);
                if (borderLeftX != 0.0 || borderRightX != 0.0 || borderUpY != 0.0 || borderDownY != 0.0) {
                    if (borderLeftX < newCameraX && newCameraX < borderRightX &&
                            borderUpY > newCameraY && newCameraY > borderDownY) {
                        this->cameraX = newCameraX;
                        this->cameraY = newCameraY;
                    }
                } else {
                    this->cameraX = newCameraX;
                    this->cameraY = newCameraY;
                }
                if (qAbs(velX) < 0.01) velX = 0.0;
                if (qAbs(velY) < 0.01) velY = 0.0;
                if (velX == 0.0 && velY == 0.0) {
                    flinging = false;
                }
//                qDebug() << "CameraController::update(); -- velX:" << velX << " velY:" << velY;
//                qDebug() << "CameraController::update(); -- newCameraX:" << newCameraX << " newCameraY:" << newCameraY;
            }
        }
//        camera.update();
//    } catch (Exception exp) {
//        Gdx.app.error("CameraController::update()", "-- Exception:" + exp);
//    }
}

void CameraController::unproject(int &screenX, int &screenY) {
//    qDebug() << "CameraController::unproject(); -- screenX:" << screenX << " screenY:" << screenY << " cameraX:" << cameraX << " cameraY:" << cameraY;
    screenX -= (cameraX*zoom);
    screenY -= (cameraY*zoom);
//    qDebug() << "CameraController::unproject(); -- screenX:" << screenX << " screenY:" << screenY << " cameraX:" << cameraX << " cameraY:" << cameraY;
}

bool CameraController::whichPrevCell(const int screenX, const int screenY, int map) {
    prevCellX = screenX;
    prevCellY = screenY;
    return whichCell(prevCellX, prevCellY, map);
}

bool CameraController::whichCell(int &mouseX, int &mouseY, int map) {
//    qDebug() << "CameraController::whichCell(); -wind- mouseX:" << mouseX << " mouseY:" << mouseY;
    unproject(mouseX, mouseY);
//    qDebug() << "CameraController::whichCell(); -grph- mouseX:" << mouseX << " mouseY:" << mouseY;
    float gameX = ( (mouseX / (halfSizeCellX*zoom)) + (mouseY / (halfSizeCellY*zoom)) ) / 2;
    float gameY = ( (mouseY / (halfSizeCellY*zoom)) - (mouseX / (halfSizeCellX*zoom)) ) / 2;
    if (!gameField->gameSettings->isometric) {
        gameX = (mouseX / (sizeCellX*zoom));
        gameY = (mouseY / (sizeCellY*zoom));
    }
//    qDebug() << "CameraController::whichCell(); -graphics- mouseX:" << mouseX << " mouseY:" << mouseY << " map:" << map << " -new- gameX:" << gameX << " gameY:" << gameY;
    int cellX = qAbs((int) gameX);
    int cellY = qAbs((int) gameY);
    if(gameField->gameSettings->isometric && gameY < 0) {
        int tmpX = cellX;
        cellX = cellY;
        cellY = tmpX;
    } // Где то я накосячил. мб сделать подругому.
    // если это убирать то нужно будет править Cell::setGraphicCoordinates() для 3 и 4 карты-java // c++ ?? or ??
    mouseX = cellX;
    mouseY = cellY;
//    qDebug() << "CameraController::whichCell(); -cell- cellX:" << cellX << " cellY:" << cellY;
    if (cellX < mapWidth && cellY < mapHeight) {
        if (map == 5) {
            return true;
        } else {
            if ( (map == 2 && gameX > 0 && gameY < 0)
              || (map == 3 && gameX > 0 && gameY > 0) ) {
                return true;
            } else if ( (map == 4 && gameX < 0 && gameY > 0)
                     || (map == 1 && gameX < 0 && gameY < 0) ) {
                return true;
            }
        }
    }
    return false;
}

bool CameraController::getCorrectGraphicTowerCoord(Vector2 *towerPos, int towerSize, int map) {
    if(map == 1) {
        if (!gameField->gameSettings->isometric) {
            towerPos->x += (-(halfSizeCellX * (towerSize - ((towerSize % 2 != 0) ? 0 : 1))) );
            towerPos->y += (-(halfSizeCellY * (towerSize - ((towerSize % 2 != 0) ? 0 : 1))) );
        } else {
            towerPos->x += (-(halfSizeCellX * towerSize) );
            towerPos->y += (-(halfSizeCellY * (towerSize - ((towerSize % 2 != 0) ? 0 : 1))) - (sizeCellY * (towerSize - ((towerSize % 2 != 0) ? 0 : 0))) );
        }
    } else if(map == 2) {
        towerPos->x += (-(halfSizeCellX * ((towerSize % 2 != 0) ? towerSize : towerSize+1)) );
        if (!gameField->gameSettings->isometric) {
            towerPos->y += (-(halfSizeCellY * (towerSize - ((towerSize % 2 != 0) ? 0 : 1))));
        } else {
            towerPos->y += (-(halfSizeCellY * towerSize) - (sizeCellY * (towerSize /*- ((towerSize % 2 != 0) ? 0 : 1)*/)) );
        }
    } else if(map == 3) {
        if (!gameField->gameSettings->isometric) {
            towerPos->x += (-(halfSizeCellX * ((towerSize % 2 != 0) ? towerSize : towerSize+1)) );
            towerPos->y += (-(halfSizeCellY * ((towerSize % 2 != 0) ? towerSize : towerSize+1)) );
        } else {
            towerPos->x += (-(halfSizeCellX * towerSize) );
            towerPos->y += (-(halfSizeCellY * ((towerSize % 2 != 0) ? towerSize : towerSize+1)) - (sizeCellY * (towerSize /*- ((towerSize % 2 != 0) ? 0 : 1)*/)) );
        }
    } else if(map == 4) {
        towerPos->x += (-(halfSizeCellX * (towerSize - ((towerSize % 2 != 0) ? 0 : 1))) );
        if (!gameField->gameSettings->isometric) {
            towerPos->y += (-(halfSizeCellY * ((towerSize % 2 != 0) ? towerSize : towerSize+1)) );
        } else {
            towerPos->y += (-(halfSizeCellY * towerSize)  - (sizeCellY * (towerSize - ((towerSize % 2 != 0) ? 0 : 0))));
        }
    } else {
        qDebug() << "CameraController::getCorrectGraphicTowerCoord(" << towerPos << ", " << towerSize << ", " << map << "); -- Bad map[1-4] value:" << map;
        return false;
    }
    return true;
}

QString CameraController::toString() {
    QString str = "Camera:[";
    str.append("cameraX:" + QString::number(cameraX));
    str.append(",cameraY :" + QString::number(cameraY));
    str.append(",sizeCellX:" + QString::number(sizeCellX));
    str.append(",sizeCellY:" + QString::number(sizeCellY));
    str.append(",zoom:" + QString::number(zoom));
    str.append(",zoomMax:" + QString::number(zoomMax));
    str.append(",zoomMin:" + QString::number(zoomMin));
//    str.append(",borderLeftX:" + QString::number(borderLeftX));
//    str.append(",borderRightX:" + QString::number(borderRightX));
//    str.append(",borderUpY:" + QString::number(borderUpY));
//    str.append(",borderDownY:" + QString::number(borderDownY));
    str.append("]");
    return str;
}
