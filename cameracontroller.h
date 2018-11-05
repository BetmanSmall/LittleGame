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
    int mapWidth, mapHeight;
    int cameraX = 600, cameraY = 800;

    int isDrawableGrid = 1;
    int isDrawableUnits = 1;
    int isDrawableTowers = 1;
    int isDrawableBackground = 1;
    int isDrawableGround = 1;
    int isDrawableForeground = 1;
    int isDrawableGridNav = 0;
    int isDrawableRoutes = 1;
    int drawOrder = 8;

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
    QPointF* getCorrectGraphicTowerCoord(QPointF* towerPos, int towerSize, int map);
    QString toString();
};

#endif // CAMERACONTROLLER_H
