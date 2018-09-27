#ifndef WIDGETCONTROLLER_H
#define WIDGETCONTROLLER_H

#include <QDebug>
#include <QInputDialog>
#include <QDirIterator>
#include <QDesktopWidget>
#include <QWidget>
//#include <QGLWidget>
//#include <QOpenGLWidget>
#include <QMediaPlayer>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QPaintEvent>
#include <QString>

#include "mainmenu.h"
#include "choosemapmenu.h"
#include "optionmenu.h"
#include "gamewidget.h"

class WidgetController : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetController(QWidget *parent = 0);

private:
    QString ASSETS_PATH;
    QFile* gameRecordsFile;
    QList<int> gameRecords;
    QMediaPlayer* mediaPlayer;
    QPixmap mainMenuBackground;
    QStackedWidget* stackedWidget;

//    std::vector<QString> campaingMaps;
    MainMenu* mainMenu;
    int enemyCount, difficultyLevel, towersCount;
    bool panMidMouseButtonBool;

    QPainter painter;
    void paintEvent(QPaintEvent* );
    void loadMap(GameWidget *gameWidget);

private slots:
    void showMainMenu();

    void loadRandomMap();
    void loadNormalMap();
//    void showChooseMapMenu();
    void showOptionMenu();
//    void loadNextCampaingMaps();
    void showGameWidget(QString mapPath);
    void changeWindowState();

//    void loadMap1();
//    void loadMap2();
//    void loadMap3();
//    void loadMap4();
//    void loadMap5();
//    void loadMap6();
    void enemyCountChanged(int value);
    void difficultyLevelChanged(int value);
    void towersCountChanged(int value);
    void actionMainMenuSoundRadionButton(bool checked);
    void panMidMouseButton(bool checked);
    void closeWidget();
    void closeWidgetGameFinished(bool win, int timeOfGame);
};

#endif // WIDGETCONTROLLER_H
