#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <QDebug>
#include <QPainter>

#include "src/head/gamefield.h"
#include "vector2.h"

class GameField;

class CameraController
{
public:
    QPainter* painter;

    GameField* gameField;
//    GameInterface gameInterface;

    float cameraX = 800;
    float cameraY = 0;
//    OrthographicCamera camera;
    int mapWidth, mapHeight;
    float viewportWidth = 0;
    float viewportHeight = 0;

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
    float initialScale = 2.0;
    float velX;
    float velY;
//    boolean lastCircleTouched = false;

    float sizeCellX, sizeCellY;
    float halfSizeCellX, halfSizeCellY;
    float zoomMax = 5.0;
    float zoomMin = 0.1;
    float zoom = 1;
    float borderLeftX = 0.0, borderRightX = 0.0;
    float borderUpY = 0.0, borderDownY = 0.0;

    bool panLeftMouseButton = true;
    bool panMidMouseButton = true;
    bool panRightMouseButton = true;
    bool paning = false;
    int touchDownX, touchDownY;
    int prevMouseX, prevMouseY;
    int prevCellX, prevCellY;

public:
//    CameraController(int mapWidth, int mapHeight, float sizeCellX, float sizeCellY);
    CameraController(GameField *gameField);
    ~CameraController();

//    void setBorders(float borderLeftX, float borderRightX, float borderUpY, float borderDownY);
//    bool tap(float x, float y, int count, int button);
    bool touchDown(int screenX, int screenY, int pointer, int button);
    bool touchUp(int screenX, int screenY, int pointer, int button);
    bool fling(float velocityX, float velocityY, int button);
    bool mouseMoved(int screenX, int screenY);
//    bool touchDragged(int screenX, int screenY, int pointer);
    bool pan(float x, float y, float deltaX, float deltaY);
    bool panStop(float x, float y, int pointer, int button);
    bool scrolled(int amount);
//    bool zoom(float initialDistance, float distance);
//    bool touchDown(float x, float y, int pointer, int button);
//    bool tap(float x, float y, int count, int button);
//    bool longPress(float x, float y);
//    bool pinch(Vector2 initialPointer1, Vector2 initialPointer2, Vector2 pointer1, Vector2 pointer2);
//    bool keyDown(int keycode);
//    bool keyUp(int keycode);
//    bool keyTyped(char character);
    void update(float deltaTime);
    void unproject(int &screenX, int &screenY);
//    QPoint unproject(QPoint screenCoords);
//    QPoint unproject(QPoint screenCoords, float viewportX, float viewportY, float viewportWidth, float viewportHeight);
    bool whichPrevCell(const int screenX, const int screenY, int map);
    bool whichCell(int &mouseX, int &mouseY, int map);
    bool getCorrectGraphicTowerCoord(Vector2 *towerPos, int towerSize, int map);
    Vector2* getCenterTowerGraphicCoord(int cellX, int cellY);
    QPointF* getCenterGraphicCoord(int cellX, int cellY, int map);
    QString toString();
};

#endif // CAMERACONTROLLER_H
