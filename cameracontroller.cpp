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
}

CameraController::~CameraController() {
    delete painter;

//    gameField = NULL;
}

bool CameraController::touchDown(int screenX, int screenY, int pointer, int button) {
    qDebug() << "CameraController::touchDown(); -- screenX:" << screenX << " screenY:" << screenY << " pointer:" << pointer << " button:" << button;
    this->prevMouseX = screenX;
    this->prevMouseY = screenY;
    this->paning = true;
//    this->flinging = false;
//    initialScale = camera.zoom;

//    if (!gameInterface.interfaceTouched) {
//        UnderConstruction underConstruction = gameField.getUnderConstruction();
//        Vector3 touch = new Vector3(screenX, screenY, 0.0f);
//        if (underConstruction != null) {
//            if (button == 0) {
//                if (whichCell(touch, isDrawableTowers)) {
//                    underConstruction.setStartCoors((int) touch.x, (int) touch.y);
//                }
//            } else if (button == 1) {
//                gameField.cancelUnderConstruction();
////                    gameField.removeTower((int) touch.x, (int) touch.y);
//            }
//        }
//    }
//    UnderConstruction underConstruction = gameField.getUnderConstruction();
//    Vector3 touch = new Vector3(screenX, screenY, 0.0f);
//    if (underConstruction == null) {
//        if (gameField.gameSettings.gameType == GameType.LittleGame) {
//            if (button == 0) {
//                if (whichCell(touch, isDrawableUnits)) {
//                    gameField.updateHeroDestinationPoint((int) touch.x, (int) touch.y);
//                }
//            } else if (button == 1) {
//                if (whichCell(touch, isDrawableGround)) {
//                    Cell cell = gameField.getCell((int) touch.x, (int) touch.y);
//                    if (cell.isTerrain()) {
//                        cell.removeTerrain( ( ((int)(Math.random()*2))==0 ) ? true : false );
//                        Gdx.app.log("CameraController::tap()", "-- x:" + cell.cellX + " y:" + cell.cellY + " cell.isTerrain():" + cell.isTerrain());
//                    } else if (cell.getTower() != null) {
//                        Tower tower = cell.getTower();
//                        gameField.removeTower(tower.cell.cellX, tower.cell.cellY);
//                    } else if (cell.isEmpty()) {
////                                gameField.towerActions(cell.cellX, cell.cellY);
//                        gameField.createTower(cell.cellX, cell.cellY, gameField.factionsManager.getRandomTemplateForTowerFromAllFaction(), ( (int)(Math.random()*3) ) );
//                        if ( ( ((int)(Math.random()*2))==0 ) ? true : false ) {
//                            int randNumber = (125 + (int) (Math.random() * 2));
//                            cell.setTerrain(gameField.map.getTileSets().getTileSet(0).getTile(randNumber), true, true);
//                        }
////                            gameField.updateHeroDestinationPoint();
//                    }
//                }
//            } else if (button == 2) {
//                if (whichCell(touch, isDrawableUnits)) {
//                    if ( ((int)(Math.random()*5) == 0) ) {
//                        gameField.spawnHero((int) touch.x, (int) touch.y);
//                    } else {
//                        gameField.spawnCompUnitToRandomExit((int) touch.x, (int) touch.y);
//                    }
//                }
//            }
//        } else if (gameField.gameSettings.gameType == GameType.TowerDefence) {
//            if (button == 0 || button == 1) {
//                if (whichCell(touch, isDrawableTowers)) {
//                    gameField.towerActions((int) touch.x, (int) touch.y);
//                }
//            } else if (button == 2) {
//                if (whichCell(touch, isDrawableUnits)) {
//                    gameField.spawnCompUnitToRandomExit((int) touch.x, (int) touch.y);
//                }
//            } else if (button == 4) {
//                if (whichCell(touch, isDrawableUnits)) {
////                            gameField.setExitPoint((int) touch.x, (int) touch.y);
//                }
//            }
//        }
//    }
    qDebug() << "CameraController::touchDown(); -- cameraX:" << cameraX << " cameraY:" << cameraY;
    return false;
}

bool CameraController::touchUp(int screenX, int screenY, int pointer, int button) {
    qDebug() << "CameraController::touchUp(); -- screenX:" << screenX << " screenY:" << screenY << " pointer:" << pointer << " button:" << button;
//    this->cameraX = screenX;
//    this->cameraY = screenY;
    if (paning) {
        this->paning = false;
//        fling((float)(screenX)-(prevMouseX), (float)(screenY)-(prevMouseY), button);
    }
//    if (!gameInterface.interfaceTouched) {
//        Vector3 touch = new Vector3(screenX, screenY, 0.0f);
//        if (gameField.getUnderConstruction() != null) {
//            if (button == 0) {
//                if (whichCell(touch, isDrawableTowers)) {
//                    gameField.buildTowersWithUnderConstruction((int) touch.x, (int) touch.y);
//                }
//            }
//        }
//    }
//    gameInterface.interfaceTouched = false;
    qDebug() << "CameraController::touchUp(); -- cameraX:" << cameraX << " cameraY:" << cameraY;
    return false;
}

bool CameraController::fling(float velocityX, float velocityY, int button) {
    qDebug() << "CameraController::fling(); -- velocityX:" << velocityX << " velocityY:" << velocityY << " button:" << button;
//    if (!lastCircleTouched) {
//        flinging = true;
//        velX = zoom * velocityX * 0.5f;
//        velY = zoom * velocityY * 0.5f;
//    }
//    qDebug() << "CameraController::fling(); -- velX:" << velX << " velY:" << velY;
    return false;
}

bool CameraController::pan(float x, float y) {
//    qDebug() << "CameraController::pan(); -- paning:" << paning;
//    qDebug() << "CameraController::pan(); -- cameraX:" << cameraX << " cameraY:" << cameraY;
    if (paning) {
        float deltaX = x - prevMouseX;
        float deltaY = y - prevMouseY;
//        qDebug() << "CameraController::pan(); -- x:" << x << " y:" << y << " deltaX:" << deltaX << " deltaY:" << deltaY;
        float newCameraX = cameraX + (deltaX * (1/zoom) );
        float newCameraY = cameraY + (deltaY * (1/zoom) );
//        if (borderLeftX != null && borderRightX != null && borderUpY != null && borderDownY != null) {
//            if (borderLeftX < newCameraX && newCameraX < borderRightX &&
//                    borderUpY > newCameraY && newCameraY > borderDownY) {
                    this->prevMouseX = x;
                    this->prevMouseY = y;
                    this->cameraX = newCameraX;
                    this->cameraY = newCameraY;
//        } else {
//        }
//        qDebug() << "CameraController::pan(); -- newCameraX:" << newCameraX << " newCameraY:" << newCameraY;
    }
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
//public boolean touchDown(float x, float y, int pointer, int button)
//public boolean tap(float x, float y, int count, int button) {
//public boolean longPress(float x, float y) {
//public boolean pinch(Vector2 initialPointer1, Vector2 initialPointer2, Vector2 pointer1, Vector2 pointer2) {
//public boolean keyDown(int keycode) {
//public boolean keyUp(int keycode) {
//public boolean keyTyped(char character) {
// Android-libgGDX

void CameraController::update(float deltaTime) {
//    qDebug() << "CameraController::update(); -- deltaTime:" << deltaTime;
//    qDebug() << "CameraController::update(); -- flinging:" << flinging;
//    qDebug() << "CameraController::update(); -- velX:" << velX;
//    qDebug() << "CameraController::update(); -- velY:" << velY;
//    if (flinging) {
//        velX *= 0.98f;
//        velY *= 0.98f;
//        float newCameraX = cameraX + (velX * deltaTime);
//        float newCameraY = cameraY + (velY * deltaTime);
////        if (borderLeftX != null && borderRightX != null && borderUpY != null && borderDownY != null) {
////            if (borderLeftX < newCameraX && newCameraX < borderRightX &&
////                    borderUpY > newCameraY && newCameraY > borderDownY) {
////                this->cameraX = newCameraX;
////                this->cameraY = newCameraY;
////            }
////        } else {
//            this->cameraX = newCameraX;
//            this->cameraY = newCameraY;
////        }
//        if (qAbs(velX) < 0.01) velX = 0.0;
//        if (qAbs(velY) < 0.01) velY = 0.0;
////        if (velX == 0.0 && velY == 0.0) {
////            flinging = false;
////        }
//        qDebug() << "CameraController::update(); -- newCameraX:" << newCameraX << " newCameraY:" << newCameraY;
//    }
//    camera.update();
}

void CameraController::unproject(int &screenX, int &screenY) {
//    qDebug() << "CameraController::unproject(); -- screenX:" << screenX << " screenY:" << screenY << " cameraX:" << cameraX << " cameraY:" << cameraY;
    screenX -= (cameraX*zoom);
    screenY -= (cameraY*zoom);
//    qDebug() << "CameraController::unproject(); -- screenX:" << screenX << " screenY:" << screenY << " cameraX:" << cameraX << " cameraY:" << cameraY;
}

//bool CameraController::whichCell(int &mouseX, int &mouseY) {
//    int mainCoorMapX = cameraX;
//    int mainCoorMapY = cameraY;
//    int gameX, gameY;
////    if(!field->isometric) {
////        gameX = ( (mouseX+sizeCell - mainCoorMapX) / sizeCell);
////        gameY = ( (mouseY+sizeCell - mainCoorMapY) / sizeCell);
////    } else {
//        int isometricCoorX = 0;//(sizeCellX/2) * mapHeight;
//        int isometricCoorY = 0;
//        int localMouseX = +mainCoorMapX + mouseX - isometricCoorX;
//        int localMouseY = +mainCoorMapY + mouseY + sizeCellY;
//        gameX = (localMouseX/2 + localMouseY) / (sizeCellX/2);
//        gameY = -(localMouseX/2 - localMouseY) / (sizeCellX/2);
////    }
//    if(gameX > 0 && gameX < mapWidth+1) {
//        if(gameY > 0 && gameY < mapHeight+1) {
//            qDebug() << "CameraController::whichCell(); -graphics- mouseX:" << mouseX << " mouseY:" << mouseY << " -new- gameX:" << gameX-1 << " gameY:" << gameY-1;
//            mouseX = gameX-1;
//            mouseY = gameY-1;
//            return true;
//        }
//    }
//    return false;
//}

bool CameraController::whichCell(int &mouseX, int &mouseY, int map) {
//    qDebug() << "CameraController::whichCell(); -wind- mouseX:" << mouseX << " mouseY:" << mouseY;
    unproject(mouseX, mouseY);
//    qDebug() << "CameraController::whichCell(); -grph- mouseX:" << mouseX << " mouseY:" << mouseY;
    float gameX = ( (mouseX / (halfSizeCellX*zoom)) + (mouseY / (halfSizeCellY*zoom)) ) / 2;
    float gameY = ( (mouseY / (halfSizeCellY*zoom)) - (mouseX / (halfSizeCellX*zoom)) ) / 2;
//    qDebug() << "CameraController::whichCell(); -graphics- mouseX:" << mouseX << " mouseY:" << mouseY << " map:" << map << " -new- gameX:" << gameX << " gameY:" << gameY;
    int cellX = qAbs((int) gameX);
    int cellY = qAbs((int) gameY);
    if(gameY < 0) {
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
//    qDebug() << "CameraController::getCorrectGraphicTowerCoord(); -- towerSize:" << towerSize << " qweqwe:" << (towerSize - ((towerSize % 2 != 0) ? 0 : 1));
    if(map == 1) {
        towerPos->x -= ( (halfSizeCellX * towerSize) );
        towerPos->y -= ( (halfSizeCellY * (towerSize - ((towerSize % 2 != 0) ? 0 : 1))) + (sizeCellY * (towerSize - ((towerSize % 2 != 0) ? 0 : 0))) );
    } else if(map == 2) {
        towerPos->x -= ( (halfSizeCellX * ((towerSize % 2 != 0) ? towerSize : towerSize+1)) );
        towerPos->y -= ( (halfSizeCellY * (towerSize - ((towerSize % 2 != 0) ? 0 : 1))) + (sizeCellY * (towerSize /*- ((towerSize % 2 != 0) ? 0 : 1)*/)) );
    } else if(map == 3) {
        towerPos->x -= ( (halfSizeCellX * towerSize) );
        towerPos->y -= ( (halfSizeCellY * (towerSize - ((towerSize % 2 != 0) ? 0 : 1))) + (sizeCellY * (towerSize /*- ((towerSize % 2 != 0) ? 0 : 1)*/)) );
    } else if(map == 4) {
        towerPos->x -= ( (halfSizeCellX * (towerSize - ((towerSize % 2 != 0) ? 0 : 1))) );
        towerPos->y -= ( (halfSizeCellY * (towerSize - ((towerSize % 2 != 0) ? 0 : 1))) + (sizeCellY * (towerSize - ((towerSize % 2 != 0) ? 0 : 0))) );
    } else {
        qDebug() << "CameraController::getCorrectGraphicTowerCoord(" << towerPos << ", " << towerSize << ", " << map << "); -- Bad map[1-4] value:" << map;
        return false;
    }
    return true;
}

Vector2* CameraController::getCenterTowerGraphicCoord(int cellX, int cellY) {
    QPointF* p = getCenterGraphicCoord(cellX, cellY, isDrawableTowers);
    return new Vector2(p->x(), p->y());
}

QPointF* CameraController::getCenterGraphicCoord(int cellX, int cellY, int map) {
    float pxlsX = 0.0, pxlsY = 0.0;
//        float offsetX = ((templateForTower.size%2 == 0) ? (templateForTower.size*halfSizeCellX) : ( (templateForTower.size == 1) ? 0 : (templateForTower.size-1)*halfSizeCellX));
//        float offsetY = ((templateForTower.size%2 == 0) ? (templateForTower.size*halfSizeCellY) : ( (templateForTower.size == 1) ? 0 : (templateForTower.size-1)*halfSizeCellY));
////        float offsetX = ((templateForTower.size%2 == 0) ? (templateForTower.size*halfSizeCellX) : (templateForTower.size-1)*halfSizeCellX);
////        float offsetY = ((templateForTower.size%2 == 0) ? (templateForTower.size*halfSizeCellY) : (templateForTower.size-1)*halfSizeCellY);
    if(map == 1) {
        pxlsX = (-(halfSizeCellX * cellY) + (cellX * halfSizeCellX));
        pxlsY = (-(halfSizeCellY * cellY) - (cellX * halfSizeCellY));
    } else if(map == 2) {
        pxlsX = ( (halfSizeCellX * cellY) + (cellX * halfSizeCellX)) + halfSizeCellX;
        pxlsY = ( (halfSizeCellY * cellY) - (cellX * halfSizeCellY)) + halfSizeCellY;
    } else if(map == 3) {
        pxlsX = (-(halfSizeCellX * cellY) + (cellX * halfSizeCellX));
        pxlsY = ( (halfSizeCellY * cellY) + (cellX * halfSizeCellY)) + halfSizeCellY*2;
    } else if(map == 4) {
        pxlsX = (-(halfSizeCellX * cellY) - (cellX * halfSizeCellX)) - halfSizeCellX;
        pxlsY = ( (halfSizeCellY * cellY) - (cellX * halfSizeCellY)) + halfSizeCellY;
    }
//        return new Vector2(pxlsX - halfSizeCellX, pxlsY + halfSizeCellY*templateForTower.size);
//    if (radiusDetectionCircle == NULL) {
//        radiusDetectionCircle = new Circle(pxlsX, pxlsY, templateForTower->radiusDetection);
//    }
    return new QPointF(pxlsX, pxlsY);
} // -------------------------------------------------------------- TODD It is analog GameField::getGraphicCoordinates() func!

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
