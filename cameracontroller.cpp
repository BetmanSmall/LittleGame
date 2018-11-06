#include "cameracontroller.h"

CameraController::CameraController(int mapWidth, int mapHeight, float sizeCellX, float sizeCellY) {
    this->painter = new QPainter();
    this->mapWidth = mapWidth;
    this->mapHeight = mapHeight;
    this->defSizeCellX = sizeCellX;
    this->defSizeCellY = sizeCellY;
    this->sizeCellX = sizeCellX;
    this->sizeCellY = sizeCellY;
    this->halfSizeCellX = sizeCellX/2;
    this->halfSizeCellY = sizeCellY/2;
}

bool CameraController::touchDown(int screenX, int screenY, int pointer, int button) {
    qDebug() << "CameraController::touchDown(); -- screenX:" << screenX << " screenY:" << screenY << " pointer:" << pointer << " button:" << button;
    this->prevMouseX = screenX;
    this->prevMouseY = screenY;
    this->paning = true;
    this->flinging = false;
    qDebug() << "CameraController::touchDown(); -- cameraX:" << cameraX << " cameraY:" << cameraY;
}

bool CameraController::touchUp(int screenX, int screenY, int pointer, int button) {
    qDebug() << "CameraController::touchUp(); -- screenX:" << screenX << " screenY:" << screenY << " pointer:" << pointer << " button:" << button;
//    this->cameraX = screenX;
//    this->cameraY = screenY;
    if (paning) {
        this->paning = false;
//        fling((float)(screenX)-(prevMouseX), (float)(screenY)-(prevMouseY), button);
    }
    qDebug() << "CameraController::touchUp(); -- cameraX:" << cameraX << " cameraY:" << cameraY;
}

bool CameraController::fling(float velocityX, float velocityY, int button) {
    qDebug() << "CameraController::fling(); -- velocityX:" << velocityX << " velocityY:" << velocityY << " button:" << button;
//    if (!lastCircleTouched) {
        flinging = true;
        velX = zoom * velocityX * 0.5f;
        velY = zoom * velocityY * 0.5f;
//    }
    qDebug() << "CameraController::fling(); -- velX:" << velX << " velY:" << velY;
}

bool CameraController::pan(float x, float y) {
//    qDebug() << "CameraController::pan(); -- paning:" << paning;
//    qDebug() << "CameraController::pan(); -- cameraX:" << cameraX << " cameraY:" << cameraY;
    if (paning) {
        float deltaX = x - prevMouseX;
        float deltaY = y - prevMouseY;
//        qDebug() << "CameraController::pan(); -- x:" << x << " y:" << y << " deltaX:" << deltaX << " deltaY:" << deltaY;
        float newCameraX = cameraX + (deltaX * zoom);
        float newCameraY = cameraY + (deltaY * zoom);
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
    if (amount > 0) {
        if (zoom <= zoomMax) {
            zoom += 0.1;
        }
    } else if (amount < 0) {
        if (zoom >= zoomMin) {
            zoom -= 0.1;
        }
    }
    sizeCellX = defSizeCellX*zoom;
    sizeCellY = defSizeCellY*zoom;
    halfSizeCellX = sizeCellX/2;
    halfSizeCellY = sizeCellY/2;
    qDebug() << "CameraController::scrolled(); -- zoom:" << zoom;
    qDebug() << "CameraController::scrolled(); -- sizeCellX:" << sizeCellX;
    qDebug() << "CameraController::scrolled(); -- sizeCellY:" << sizeCellY;
    qDebug() << "CameraController::scrolled(); -- halfSizeCellX:" << halfSizeCellX;
    qDebug() << "CameraController::scrolled(); -- halfSizeCellY:" << halfSizeCellY;
}

void CameraController::update(float deltaTime) {
//    qDebug() << "CameraController::update(); -- deltaTime:" << deltaTime;
//    qDebug() << "CameraController::update(); -- flinging:" << flinging;
//    qDebug() << "CameraController::update(); -- velX:" << velX;
//    qDebug() << "CameraController::update(); -- velY:" << velY;
    if (flinging) {
        velX *= 0.98f;
        velY *= 0.98f;
        float newCameraX = cameraX + (velX * deltaTime);
        float newCameraY = cameraY + (velY * deltaTime);
//        if (borderLeftX != null && borderRightX != null && borderUpY != null && borderDownY != null) {
//            if (borderLeftX < newCameraX && newCameraX < borderRightX &&
//                    borderUpY > newCameraY && newCameraY > borderDownY) {
//                this->cameraX = newCameraX;
//                this->cameraY = newCameraY;
//            }
//        } else {
            this->cameraX = newCameraX;
            this->cameraY = newCameraY;
//        }
        if (qAbs(velX) < 0.01) velX = 0.0;
        if (qAbs(velY) < 0.01) velY = 0.0;
//        if (velX == 0.0 && velY == 0.0) {
//            flinging = false;
//        }
        qDebug() << "CameraController::update(); -- newCameraX:" << newCameraX << " newCameraY:" << newCameraY;
    }
}

void CameraController::unproject(int &screenX, int &screenY) {
//    qDebug() << "CameraController::unproject(); -- screenX:" << screenX << " screenY:" << screenY << " cameraX:" << cameraX << " cameraY:" << cameraY;
    screenX -= cameraX;
    screenY -= cameraY;
//    qDebug() << "CameraController::unproject(); -- screenX:" << screenX << " screenY:" << screenY << " cameraX:" << cameraX << " cameraY:" << cameraY;
}

//QPoint CameraController::unproject(QPoint screenCoords) {
//    qDebug() << "CameraController::unproject(); -- screenCoords:" << screenCoords;
//    return unproject(screenCoords, 0, 0, this->viewportWidth, this->viewportHeight);
//}

//QPoint CameraController::unproject(QPoint screenCoords, float viewportX, float viewportY, float viewportWidth, float viewportHeight) {
//    qDebug() << "CameraController::update(); -- viewportX:" << viewportX << " viewportY:" << viewportY;
//    qDebug() << "CameraController::update(); -- viewportWidth:" << viewportWidth << " viewportHeight:" << viewportHeight;
//    float x = screenCoords.x, y = screenCoords.y;
//    x = x - viewportX;
//    y = Gdx.graphics.getHeight() - y - 1;
//    y = y - viewportY;
//    screenCoords.x = (2 * x) / viewportWidth - 1;
//    screenCoords.y = (2 * y) / viewportHeight - 1;
//    screenCoords.z = 2 * screenCoords.z - 1;
//    screenCoords.prj(invProjectionView);
//    return screenCoords;
//}

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
    float gameX = (mouseX / halfSizeCellX + mouseY / halfSizeCellY) / 2;
    float gameY = (mouseY / halfSizeCellY -(mouseX / halfSizeCellX))/ 2;
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

QPointF* CameraController::getCorrectGraphicTowerCoord(QPointF* towerPos, int towerSize, int map) {
    QPointF* retTowerPos = new QPointF();
    if(map == 1) {
        retTowerPos->setX( towerPos->x() + (-(halfSizeCellX * towerSize) ) );
        retTowerPos->setY( towerPos->y() + (-(sizeCellY * (towerSize - ((towerSize % 2 != 0) ? 0 : 1))) ) - sizeCellY );
    } else if(map == 2) {
        retTowerPos->setX( towerPos->x() + (-(halfSizeCellX * ((towerSize % 2 != 0) ? towerSize : towerSize+1)) ) );
        retTowerPos->setY( towerPos->y() + (-(sizeCellY * towerSize) ) - sizeCellY );
    } else if(map == 3) {
        retTowerPos->setX( towerPos->x() + (-(halfSizeCellX * towerSize)) );
        retTowerPos->setY( towerPos->y() + (-(sizeCellY * ((towerSize % 2 != 0) ? towerSize : towerSize+1)) ) - sizeCellY );
    } else if(map == 4) {
        retTowerPos->setX( towerPos->x() + (-(halfSizeCellX * (towerSize - ((towerSize % 2 != 0) ? 0 : 1))) ) );
        retTowerPos->setY( towerPos->y() + (-(sizeCellY * towerSize) ) - sizeCellY );
    } else {
        qDebug() << "CameraController::getCorrectGraphicTowerCoord(" << towerPos << ", " << towerSize << ", " << map << "); -- Bad map[1-4] value:" << map;
    }
    return retTowerPos;
}

QString CameraController::toString() {
    QString str = "Camera:[";
    str.append("x:" + QString::number(cameraX));
    str.append(",y:" + QString::number(cameraY));
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

