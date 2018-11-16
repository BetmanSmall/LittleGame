#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <QDebug>
#include <QPainter>

#include "vector2.h"

class CameraController
{
public:
    QPainter* painter;
    float viewportWidth = 0;
    float viewportHeight = 0;
    int mapWidth, mapHeight;
    float cameraX = 800;
    float cameraY = 0;

    int isDrawableGrid = 3;
    int isDrawableUnits = 3;
    int isDrawableTowers = 3;
    int isDrawableBackground = 3;
    int isDrawableGround = 3;
    int isDrawableForeground = 3;
    int isDrawableGridNav = 3;
    int isDrawableRoutes = 3;
    int drawOrder = 8;

    bool flinging = false;
    float velX;
    float velY;

    float defSizeCellX, defSizeCellY;
    float sizeCellX, sizeCellY;
    float halfSizeCellX, halfSizeCellY;
//    static float _halfSizeCellX, _halfSizeCellY;
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
    CameraController(int mapWidth, int mapHeight, float sizeCellX, float sizeCellY);
//    void setBorders(float borderLeftX, float borderRightX, float borderUpY, float borderDownY);
    bool touchDown(int screenX, int screenY, int pointer, int button);
    bool touchUp(int screenX, int screenY, int pointer, int button);
    bool fling(float velocityX, float velocityY, int button);
    bool pan(float cameraX, float cameraY);
    bool scrolled(int amount);
    void update(float deltaTime);
    void unproject(int &screenX, int &screenY);
//    QPoint unproject(QPoint screenCoords);
//    QPoint unproject(QPoint screenCoords, float viewportX, float viewportY, float viewportWidth, float viewportHeight);
//    bool whichCell(int &mouseX, int &mouseY);
    bool whichCell(int &mouseX, int &mouseY, int map);
    bool getCorrectGraphicTowerCoord(Vector2 *towerPos, int towerSize, int map);
    Vector2* getCenterTowerGraphicCoord(int cellX, int cellY);
    QPointF* getCenterGraphicCoord(int cellX, int cellY, int map);
    QString toString();
};

#endif // CAMERACONTROLLER_H
