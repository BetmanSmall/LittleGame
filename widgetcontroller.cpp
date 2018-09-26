#include "widgetcontroller.h"

#include <QDebug>

WidgetController::WidgetController(QWidget *parent) :
    QWidget(parent)
{
#ifdef QT_DEBUG
    ASSETS_PATH = "../../LittleGame/assets/";
#else
    ASSETS_PATH = "./assets/";
#endif

//    this->setMaximumWidth(640);
//    this->setMaximumHeight(480);
//    this->setMinimumWidth(640);
//    this->setMinimumHeight(480);
//    this->setWindowTitle("xyu");
//    QPalette palette;
//    palette.setBrush(this->backgroundRole(), QBrush(QImage(ASSETS_PATH + "images/mainmenu.jpg")));
//    palette.setBrush(this->backgroundRole(), QBrush(QColor(0, 0, 0)));
//    this->setPalette(palette);
//    qDebug() << "WidgetController: X: " << width() << " Y: " << height();
//    show();
//    QRect rec = QApplication::desktop()->screenGeometry();
//    width = rec.width();
//    height = rec.height();
//    move(width()/2, 0);
//    setWindowState(Qt::WindowFullScreen);

    mediaPlayer = new QMediaPlayer();
    mediaPlayer->setMedia(QUrl::fromLocalFile(ASSETS_PATH + "music/mainmenu2.mp3"));
    mediaPlayer->setVolume(0);
    mediaPlayer->play();

    stackedWidget = new QStackedWidget;
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->addWidget(stackedWidget);
    setLayout(layout);

    if (campaingMaps.empty()) {
        loadNextCampaingMaps(); // pre-load
    }
    showMainMenu();
}

void WidgetController::paintEvent(QPaintEvent *) {
    QPainter p(this);
    QPixmap pix = QPixmap(ASSETS_PATH + "images/mainmenu.jpg");
    p.drawPixmap(0, 0, width(), height(), pix);
    p.end();
}

void WidgetController::showMainMenu() {
    qDebug() << "WidgetController::showMainMenu(); -- ";
    mainMenu = new MainMenu();
    connect(mainMenu, SIGNAL(signal_quickPlay()), this, SLOT(loadNextCampaingMaps()));
    connect(mainMenu, SIGNAL(signal_openChooseMapMenu()), this, SLOT(showChooseMapMenu()));
    connect(mainMenu, SIGNAL(signal_openOptionMenu()), this, SLOT(showOptionMenu()));
    connect(mainMenu, SIGNAL(signal_exit()), this, SLOT(closeWidget()));
    connect(mainMenu, SIGNAL(signal_enemyCountChanged(int)), this, SLOT(enemyCountChanged(int)));
    connect(mainMenu, SIGNAL(signal_difficultyLevelChanged(int)), this, SLOT(difficultyLevelChanged(int)));
    connect(mainMenu, SIGNAL(signal_towersCountChanged(int)), this, SLOT(towersCountChanged(int)));
    connect(mainMenu, SIGNAL(signal_actionSoundRadioButton()), this, SLOT(actionMainMenuSoundRadionButton()));

    mainMenu->updateSlidersValues();
    stackedWidget->addWidget(mainMenu);
    stackedWidget->setCurrentWidget(mainMenu);
}

void WidgetController::showChooseMapMenu() {
    qDebug() << "WidgetController::showChooseMapMenu(); -- ";

    ChooseMapMenu* chooseMapMenu = new ChooseMapMenu();

//    connect(chooseMapMenu, SIGNAL(signal_loadMap1()), this, SLOT(loadMap1()));
//    connect(chooseMapMenu, SIGNAL(signal_loadMap2()), this, SLOT(loadMap2()));
//    connect(chooseMapMenu, SIGNAL(signal_loadMap3()), this, SLOT(loadMap3()));
//    connect(chooseMapMenu, SIGNAL(signal_loadMap4()), this, SLOT(loadMap4()));
//    connect(chooseMapMenu, SIGNAL(signal_loadMap5()), this, SLOT(loadMap5()));
//    connect(chooseMapMenu, SIGNAL(signal_loadMap6()), this, SLOT(loadMap6()));
    connect(chooseMapMenu, SIGNAL(signal_closeWidget()), this, SLOT(closeWidget()));

    stackedWidget->addWidget(chooseMapMenu);
    stackedWidget->setCurrentWidget(chooseMapMenu);
}

void WidgetController::actionMainMenuSoundRadionButton() {
    qDebug() << "WidgetController::actionMainMenuSoundRadionButton(); -- volume:" << mediaPlayer->volume();
    mediaPlayer->setVolume( (mediaPlayer->volume()>0) ? 0 : 100); // (OptionMenu)(stackedWidget->currentWidget()).ui. );
}

void WidgetController::showOptionMenu() {
    qDebug() << "WidgetController::showChooseMapMenu(); -- ";

    OptionMenu* optionMenu = new OptionMenu();
    optionMenu->setMediaPlayer(mediaPlayer);

    connect(optionMenu, SIGNAL(signal_closeWidget()), this, SLOT(closeWidget()));

    stackedWidget->addWidget(optionMenu);
    stackedWidget->setCurrentWidget(optionMenu);
}

void WidgetController::loadMap(GameWidget* gameWidget) {
    qDebug() << "WidgetController::loadMap(GameWidget* gameWidget)";
//    connect(gameWidget, SIGNAL(signal_closeWidget()), this, SLOT(closeWidget()));
    connect(gameWidget, SIGNAL(signal_changeWindowState()), this, SLOT(changeWindowState()));
    connect(gameWidget, SIGNAL(signal_closeWidgetGameFinished(bool)), this, SLOT(closeWidgetGameFinished(bool)));
    stackedWidget->addWidget(gameWidget);
    stackedWidget->setCurrentWidget(gameWidget);
    qDebug() << "WidgetController::loadMap(); -- end";
}

void WidgetController::showGameWidget(QString mapPath) {
    qDebug() << "WidgetController::showGameWidget(); -- mapPath:" << mapPath;
//    mainMenu->setEnabled(false);
    GameWidget* gameWidget = new GameWidget(this);
    gameWidget->setMinimumWidth(1024);
    gameWidget->setMinimumHeight(768);
//    gameWidget->setMaximumWidth(1024);
//    gameWidget->setMaximumHeight(768);
//    gameWidget->parentWidget()->move(0, 0);
//    this->setWindowState(Qt::WindowFullScreen);
//    this->move(0, 0);
//    setWindowState(windowState() ^ Qt::WindowFullScreen);
//    gameWidget->resize();
    loadMap(gameWidget);
    gameWidget->loadMap(ASSETS_PATH + mapPath, enemyCount, towersCount);
    gameWidget->field.towersManager.difficultyLevel = difficultyLevel; // not good | unsafe
    qDebug() << "WidgetController::showGameWidget(); -- END";
}

void WidgetController::changeWindowState() {
    qDebug() << "WidgetController::changeWindowState(); -- ";
    setWindowState(windowState() ^ Qt::WindowFullScreen);
}

//void WidgetController::loadMap1() {
//    qDebug() << "loadMap1()";
//    GameWidget* gameWidget = new GameWidget();
//    loadMap(gameWidget);
//    gameWidget->loadMap(ASSETS_PATH + "maps/arctic.tmx");
//}

//void WidgetController::loadMap2() {
//    qDebug() << "loadMap2()";
//    GameWidget* gameWidget = new GameWidget();
//    loadMap(gameWidget);
//    gameWidget->loadMap(ASSETS_PATH + "maps/forest.tmx");
//}

//void WidgetController::loadMap3() {
//    qDebug() << "loadMap3()";
//    GameWidget* gameWidget = new GameWidget();
//    loadMap(gameWidget);
//    gameWidget->loadMap(ASSETS_PATH + "maps/desert.tmx");
//}

//void WidgetController::loadMap4() {
//    qDebug() << "loadMap4()";
//    GameWidget* gameWidget = new GameWidget();
//    loadMap(gameWidget);
//    gameWidget->loadMap(ASSETS_PATH + "maps/boloto.tmx");
//}

//void WidgetController::loadMap5() {
//    qDebug() << "loadMap5()";
//    GameWidget* gameWidget = new GameWidget();
//    loadMap(gameWidget);
//    gameWidget->loadMap(ASSETS_PATH + "maps/isom_workMap.tmx");
//}

//void WidgetController::loadMap6() {
//    qDebug() << "loadMap6()";
//    GameWidget* gameWidget = new GameWidget();
//    loadMap(gameWidget);
//    gameWidget->loadMap(ASSETS_PATH + "maps/isometric_testMap.tmx");
//}

void WidgetController::enemyCountChanged(int value) {
    qDebug() << "WidgetController::enemyCountChanged(); -- value:" << value;
    enemyCount = value;
}

void WidgetController::difficultyLevelChanged(int value) {
    qDebug() << "WidgetController::difficultyLevelChanged(); -- value:" << value;
    difficultyLevel = value;
}

void WidgetController::towersCountChanged(int value) {
    qDebug() << "WidgetController::towersCountChanged(); -- value:" << value;
    towersCount = value;
}

void WidgetController::closeWidget() {
    qDebug() << "WidgetController::closeWidget(); -- ";
    QWidget* currentWidget = stackedWidget->currentWidget();
    if(stackedWidget->currentIndex() == 0) {
        this->close();
    } else {// if((stackedWidget->currentWidget() == w) && (stackedWidget->currentIndex() != 0))
        stackedWidget->setCurrentIndex(stackedWidget->count() - 2);
        stackedWidget->removeWidget(currentWidget);
        disconnect(currentWidget);
        delete currentWidget;
    }
    qDebug() << "WidgetController::closeWidget(); -- END";
}

void WidgetController::closeWidgetGameFinished(bool win) {
    qDebug() << "WidgetController::closeWidgetGameFinished(); -- win:" << win;
    mainMenu->updateGameStatus(win);
    closeWidget();
    loadNextCampaingMaps();
    qDebug() << "WidgetController::closeWidgetGameFinished(); -- END";
}

void WidgetController::loadNextCampaingMaps() {
    qDebug() << "WidgetController::loadNextCampaingMaps(); -- ";
    if (!campaingMaps.empty()) {
        QString mapPath = campaingMaps.back();
        campaingMaps.pop_back();
//        closeWidget();
        showGameWidget(mapPath);
    } else {
        campaingMaps.push_back("maps/island.tmx");
        campaingMaps.push_back("maps/randomMap.tmx");
//        showMainMenu();
    }
    qDebug() << "WidgetController::loadNextCampaingMaps(); -- campaingMaps.size():" << campaingMaps.size();
}
