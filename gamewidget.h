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

#include "astar.h"
#include "field.h"
#include "faction.h"
#include "underconstruction.h"

using namespace std;

namespace Ui {
    class GameWidget;
}

/**
 * @brief Данная структура отвечает за набор Tilов
 */
struct TileSet {
    int firstTileID; /// jdfklsdjfsdjf
    QString name;
    int spacing;
    int margin;
    int tileWidth;
    int tileHeight;
    QPixmap img;
    vector<QRect> subRects;
    vector<QString> tilesNames; // fireball specific load
    vector<QPixmap> tiles;
};

/**
 * @brief Класс отвечает за игровую сессию
 * Вся игра проходит, полностью в этом классе
 */
class GameWidget : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Конструктор GameWidget
     * @param parent - родительский QWidget
     */
    explicit GameWidget(QWidget *parent = 0);
    ~GameWidget();
    /**
     * @brief Обработчик всех таймеров
     * @param Евент таймера
     */
    void timerEvent(QTimerEvent* event);
    /**
     * @brief Обработчик нажатия клавиш на клавиатуре
     * @param Евент нажатия клавиши
     */
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
    void drawTowerUnderConstruction(int buildX, int buildY, DefaultTower* tower);

    /**
     * @brief Переводит графические координаты в игровые
     * @param mouseX
     * @param mouseY
     * @return False - не смогли перевести. Возможно за пределами игрового поля.
     */
    bool whichCell(int &mouseX, int &mouseY);

    void startTimer_UnitsMoveAndTowerAttack();
    void stopTimer_UnitsMoveAndTowerAttack();

    void buildTower(int x = -1, int y = -1);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);


    /**
     * @brief Загрузка карты
     * @param mapName
     */
    void loadMap(QString mapName, int enemyCount, int towersCount);

    bool gameStart;
    bool gamePause;
    bool mapLoad;


    int pixelsShiftMap;

    int zoomSizeCell;
    int zoomMaxSizeCell;
    int zoomMinSizeCell;

    int defaultNumCreateUnits;

    int timeOfGame;

    int unitsMove_TimerMilliSec;
    int towersAttack_TimerMilliSec;
    int scanMouseMove_TimerMilliSec;


    int unitsMove_TimerId;
    int towersAttack_TimerId;
    int scanMouseMove_TimerId;


    int test;
    Field field;

    vector<TileSet> tileSets;

    QPixmap global_pixmap;
    QPixmap global_pixmap_PathPoint;
    QPixmap global_pixmap_EnemyPathPoint;
    QPixmap global_pixmap_DestinationPoint;
    QPixmap global_pixmap_ExitPoint;
    string global_text;
    string global_text2;

    bool panMidMouseButton;
    bool pan = false;
    int prevMouseX, prevMouseY;
    int prevMouseCellX, prevMouseCellY;
    int prevGlobalMouseX, prevGlobalMouseY;

private:
    Ui::GameWidget *ui;
    QString ASSETS_PATH;

private slots:
    void on_loadMaps_clicked();
    void on_clearMap_clicked();
    void on_closeWidget_clicked();
    void on_goUnits_clicked();

signals:
    void signal_changeWindowState();
    void signal_closeWidget();
    void signal_closeWidgetGameFinished(bool win, int timeOfGame);
};

#endif // GAMEWIDGET_H
