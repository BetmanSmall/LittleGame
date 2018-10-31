#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <QDebug>
#include <QPainter>

class CameraController
{
public:
    QPainter* painter;
    float viewportWidth = 0;
    float viewportHeight = 0;
    int cameraX = 0, cameraY = 0;

    bool flinging = false;
    float velX;
    float velY;

    float defSizeCellX, defSizeCellY;
    float sizeCellX, sizeCellY;
    float halfSizeCellX, halfSizeCellY;
    float zoomMax = 50.0;
    float zoomMin = 0.2;
    float zoom = 1;
//    float borderLeftX, borderRightX;
//    float borderUpY, borderDownY;

    bool panMidMouseButton = false;
    bool paning = false;
    int prevMouseX, prevMouseY;
    int prevMouseCellX, prevMouseCellY;
    int prevGlobalMouseX, prevGlobalMouseY;

public:
    CameraController(float sizeCellX, float sizeCellY);
//    void setBorders(float borderLeftX, float borderRightX, float borderUpY, float borderDownY);
    bool touchDown(int screenX, int screenY, int pointer, int button);
    bool touchUp(int screenX, int screenY, int pointer, int button);
    bool fling(float velocityX, float velocityY, int button);
    bool pan(float cameraX, float cameraY);
    bool scrolled(int amount);
    void update(float deltaTime);
    QString toString();
};

#endif // CAMERACONTROLLER_H
