#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <time.h>
#include <QDesktopWidget>
#include <QDebug>
#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QString>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QXmlStreamReader>
#include <QBitmap>

//#include "assets/libs/tinyxml2.h"
#include "src/head/libs/astar.h"
#include "src/head/field.h"
//#include "src/head/factionsmanager.h"
//#include "src/head/faction.h"
#include "src/head/underconstruction.h"

//#include "src/head/mapEditor/tileset.h"

#include "ui_gamewidget.h"

namespace Ui {
    class GameWidget;
}

class GameWidget : public QWidget {
    Q_OBJECT
#ifdef QT_DEBUG
    QString ASSETS_PATH = "../../LittleGame/assets/";
#else
    QString ASSETS_PATH = "./assets/";
#endif
public:
//    bool gameStart;
//    bool gamePause;
//    bool mapLoad;
    // camera need
    int pixelsShiftMap;
    int zoomSizeCell;
    int zoomMaxSizeCell;
    int zoomMinSizeCell;

    int timeOfGame;
    int unitsMove_TimerMilliSec;
    int towersAttack_TimerMilliSec;
    int scanMouseMove_TimerMilliSec;
    int unitsMove_TimerId;
    int towersAttack_TimerId;
    int scanMouseMove_TimerId;

    int test;
    Field* field;
//    std::vector<TileSet> tileSets;

// test screen ---1
    QPixmap global_pixmap;
    QPixmap global_pixmap_PathPoint;
    QPixmap global_pixmap_EnemyPathPoint;
    QPixmap global_pixmap_DestinationPoint;
    QPixmap global_pixmap_ExitPoint;
    std::string global_text;
    std::string global_text2;
// test screen ---2
// option screen -3--
    bool panMidMouseButton;
    bool pan = false;
    int prevMouseX, prevMouseY;
    int prevMouseCellX, prevMouseCellY;
    int prevGlobalMouseX, prevGlobalMouseY;
// option screen -4--

public:
    explicit GameWidget(QString mapPath, FactionsManager* factionsManager,
                        int enemyCount, int difficultyLevel, int towersCount,
                        QWidget *parent = 0);
    ~GameWidget();

    void timerEvent(QTimerEvent* event);
    void keyPressEvent(QKeyEvent* event);

    QPainter painter;
    void paintEvent(QPaintEvent* event);
    void drawFullField();
    void drawGrid();
    void drawBackGround();
    void drawGround();
    void drawForeGround();

    void drawTowersByTowers();
    void drawUnits();
    void drawBlackTiles();
    void drawPaths();
    void drawTowersUnderConstruction();
    void drawTowerUnderConstruction(int buildX, int buildY, TemplateForTower* tower);

    bool whichCell(int &mouseX, int &mouseY);

    void startTimer_UnitsMoveAndTowerAttack();
    void stopTimer_UnitsMoveAndTowerAttack();

    void buildTower(int x = -1, int y = -1);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);

private:
    Ui::GameWidget *ui;

private slots:
    void on_closeWidget_clicked();

signals:
    void signal_changeWindowState();
    void signal_closeWidget();
    void signal_closeWidgetGameFinished(bool win, int timeOfGame);
};

#endif // GAMEWIDGET_H
