#include "cameracontroller.h"

CameraController::CameraController(float sizeCellX, float sizeCellY) {
    this->painter = new QPainter();
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
        fling((float)(screenX)-(prevMouseX), (float)(screenY)-(prevMouseY), button);
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
        if (velX == 0.0 || velY == 0.0) {
            flinging = false;
        }
        qDebug() << "CameraController::update(); -- newCameraX:" << newCameraX << " newCameraY:" << newCameraY;
    }
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

