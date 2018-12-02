#ifndef WIDGETCONTROLLER_H
#define WIDGETCONTROLLER_H

#include <QDebug>
#include <QInputDialog>
#include <QDirIterator>
#include <QDesktopWidget>
#include <QWidget>
#include <QMediaPlayer>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QPaintEvent>
#include <QString>

#include "factionsmanager.h"
#include "mainmenu.h"
#include "optionmenu.h"
#include "gamescreengl.h"

class WidgetController : public QWidget
{
    Q_OBJECT

#ifdef QT_DEBUG
    QString ASSETS_PATH = "../../LittleGame/assets/";
#else
    QString ASSETS_PATH = "./assets/";
#endif

public:
    explicit WidgetController(QWidget *parent = 0);

private:
    QStackedWidget* stackedWidget;
    QMediaPlayer* mediaPlayer;

    FactionsManager* factionsManager;
    QList<int> gameRecords;
    QFile* gameRecordsFile;
    std::vector<QString> campaingMaps;

    MainMenu* mainMenu;
    QPixmap mainMenuBackground;

    int enemyCount = 16, difficultyLevel = 0, towersCount = 0;
    bool panMidMouseButtonBool = true;
    OptionMenu* optionMenu;

    GameScreenGL* gameWidget;
    QPainter painter;
    void paintEvent(QPaintEvent* );
    void loadMap(QString mapPath);

private slots:
    void showMainMenu();
    void loadRandomMap();
    void loadNormalMap();
    void showGameWidget(GameScreenGL *gameWidget);
    void changeWindowState();

    void showOptionMenu();
    void enemyCountChanged(int value);
    void difficultyLevelChanged(int value);
    void towersCountChanged(int value);
    void actionMainMenuSoundRadionButton(bool checked);
    void panMidMouseButton(bool checked);

    void closeWidget();
    void closeWidgetGameFinished(bool win, int timeOfGame);
    void loadNextCampaingMaps();
};

#endif // WIDGETCONTROLLER_H
