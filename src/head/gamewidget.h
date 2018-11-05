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
#include "src/head/gamefield.h"
//#include "src/head/factionsmanager.h"
//#include "src/head/faction.h"
#include "src/head/underconstruction.h"
#include "cameracontroller.h"

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
    GameField* gameField;
    CameraController* cameraController;

    QTime currentTime;
    QTime lastTime;
    float elapsedTime;
    float fps; // current fps in the loop, depending on the "work time" required during the paintGL call
    QTimer* gameTimer;

// test screen ---1
    std::string global_text;
    std::string global_text2;
// test screen ---2
// option screen -3--
// option screen -4--

public:
    explicit GameWidget(QString mapPath, FactionsManager* factionsManager,
                        int enemyCount, int difficultyLevel, int towersCount,
                        QWidget *parent = 0);
    ~GameWidget();

//    void timerEvent(QTimerEvent* event);
    void paintEvent(QPaintEvent* event);
//    bool whichCell(int &mouseX, int &mouseY);

//    void startTimer_UnitsMoveAndTowerAttack();
//    void stopTimer_UnitsMoveAndTowerAttack();
//    void buildTower(int x = -1, int y = -1);

    void keyPressEvent(QKeyEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
    void resizeEvent(QResizeEvent* event);

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
