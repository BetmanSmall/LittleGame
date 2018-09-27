#include "widgetcontroller.h"

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
    setWindowState(Qt::WindowMaximized);

    gameRecordsFile = new QFile(ASSETS_PATH + "gameRecords.txt");
    if (!gameRecordsFile->exists()) {
        qDebug() << "WidgetController::WidgetController(); -- gameRecords->exists():" << gameRecordsFile->exists();
    } else {
        if (gameRecordsFile->open(QIODevice::ReadOnly)) {
           QTextStream in(gameRecordsFile);
           while (!in.atEnd()) {
              QString line = in.readLine();
              gameRecords.append(line.toInt());
           }
           gameRecordsFile->close();
        }
    }
    mediaPlayer = new QMediaPlayer();
    mediaPlayer->setMedia(QUrl::fromLocalFile(ASSETS_PATH + "music/mainmenu2.mp3"));
    mediaPlayer->setVolume(0);
    mediaPlayer->play();
//    mainMenuBackground = QPixmap(ASSETS_PATH + "images/mainmenu/mainmenu3.png");
//    mainMenuBackground = QPixmap(":/assets/images/mainmenu.jpg");
    std::vector<QString> menuImgs;
    QDirIterator it(":/assets/images/mainmenu/", QDirIterator::Subdirectories);
    while (it.hasNext()) {
        menuImgs.push_back(it.next());
    }
    mainMenuBackground = QPixmap(menuImgs[rand()%menuImgs.size()]);

    stackedWidget = new QStackedWidget;
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->addWidget(stackedWidget);
    setLayout(layout);

//    if (campaingMaps.empty()) {
//        loadNextCampaingMaps(); // pre-load
//    }
    enemyCount = 16, difficultyLevel = 0, towersCount = 0;
    panMidMouseButtonBool = true;
    showMainMenu();
}

void WidgetController::paintEvent(QPaintEvent *) {
    painter.begin(this);
    painter.drawPixmap(0, 0, width(), height(), mainMenuBackground);
    painter.end();
}

void WidgetController::showMainMenu() {
    qDebug() << "WidgetController::showMainMenu(); -- ";
    mainMenu = new MainMenu();
    connect(mainMenu, SIGNAL(signal_playNormalMap()), this, SLOT(loadNormalMap()));
    connect(mainMenu, SIGNAL(signal_openOptionMenu()), this, SLOT(showOptionMenu()));
    connect(mainMenu, SIGNAL(signal_exit()), this, SLOT(closeWidget()));

    mainMenu->updateRecords(gameRecords);
    stackedWidget->addWidget(mainMenu);
    stackedWidget->setCurrentWidget(mainMenu);
}

void WidgetController::loadRandomMap() {
    showGameWidget("maps/randomMap.tmx");
}

void WidgetController::loadNormalMap() {
    showGameWidget("maps/island.tmx");
}

//void WidgetController::showChooseMapMenu() {
//    qDebug() << "WidgetController::showChooseMapMenu(); -- ";

//    ChooseMapMenu* chooseMapMenu = new ChooseMapMenu();

////    connect(chooseMapMenu, SIGNAL(signal_loadMap1()), this, SLOT(loadMap1()));
////    connect(chooseMapMenu, SIGNAL(signal_loadMap2()), this, SLOT(loadMap2()));
////    connect(chooseMapMenu, SIGNAL(signal_loadMap3()), this, SLOT(loadMap3()));
////    connect(chooseMapMenu, SIGNAL(signal_loadMap4()), this, SLOT(loadMap4()));
////    connect(chooseMapMenu, SIGNAL(signal_loadMap5()), this, SLOT(loadMap5()));
////    connect(chooseMapMenu, SIGNAL(signal_loadMap6()), this, SLOT(loadMap6()));
//    connect(chooseMapMenu, SIGNAL(signal_closeWidget()), this, SLOT(closeWidget()));

//    stackedWidget->addWidget(chooseMapMenu);
//    stackedWidget->setCurrentWidget(chooseMapMenu);
//}

void WidgetController::showOptionMenu() {
    qDebug() << "WidgetController::showChooseMapMenu(); -- ";

    OptionMenu* optionMenu = new OptionMenu();
    optionMenu->setMediaPlayer(mediaPlayer);

    connect(optionMenu, SIGNAL(signal_closeWidget()), this, SLOT(closeWidget()));
    connect(optionMenu, SIGNAL(signal_enemyCountChanged(int)), this, SLOT(enemyCountChanged(int)));
    connect(optionMenu, SIGNAL(signal_difficultyLevelChanged(int)), this, SLOT(difficultyLevelChanged(int)));
    connect(optionMenu, SIGNAL(signal_towersCountChanged(int)), this, SLOT(towersCountChanged(int)));
    connect(optionMenu, SIGNAL(signal_actionSoundRadioButton(bool)), this, SLOT(actionMainMenuSoundRadionButton(bool)));
    connect(optionMenu, SIGNAL(signal_panMidMouseButton(bool)), this, SLOT(panMidMouseButton(bool)));
    connect(optionMenu, SIGNAL(signal_playRandomMap()), this, SLOT(loadRandomMap()));

    optionMenu->updateSlidersAndValues();
    optionMenu->updateRecords(gameRecords);
    stackedWidget->addWidget(optionMenu);
    stackedWidget->setCurrentWidget(optionMenu);
}

void WidgetController::loadMap(GameWidget* gameWidget) {
    qDebug() << "WidgetController::loadMap(GameWidget* gameWidget)";
    connect(gameWidget, SIGNAL(signal_closeWidget()), this, SLOT(closeWidget()));
    connect(gameWidget, SIGNAL(signal_changeWindowState()), this, SLOT(changeWindowState()));
    connect(gameWidget, SIGNAL(signal_closeWidgetGameFinished(bool,int)), this, SLOT(closeWidgetGameFinished(bool,int)));
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
    gameWidget->panMidMouseButton = panMidMouseButtonBool;
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

void WidgetController::actionMainMenuSoundRadionButton(bool checked) {
    qDebug() << "WidgetController::actionMainMenuSoundRadionButton(); -- volume:" << mediaPlayer->volume();
    mediaPlayer->setVolume( (mediaPlayer->volume()>0) ? 0 : 100); // (OptionMenu)(stackedWidget->currentWidget()).ui. );
}

void WidgetController::panMidMouseButton(bool checked) {
    qDebug() << "WidgetController::panMidMouseButton(); -- checked:" << checked;
    panMidMouseButtonBool = checked;
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

void WidgetController::closeWidgetGameFinished(bool win, int timeOfGame) {
    qDebug() << "WidgetController::closeWidgetGameFinished(); -- win:" << win << " timeOfGame:" << timeOfGame;
    if (win) {
        gameRecords.push_back(timeOfGame);
        qSort(gameRecords.begin(), gameRecords.end());
        mainMenu->updateRecords(gameRecords, timeOfGame);
        if (gameRecordsFile->open(QIODevice::WriteOnly)) {
            QTextStream stream(gameRecordsFile);
            foreach (int record, gameRecords){
                stream << record << endl;
            }
            gameRecordsFile->close();
        }
//        int minRecord = gameRecords.first();
////        minRecord = (timeOfGame<minRecord) ? timeOfGame : minRecord;
//        for (int r = 1; r < gameRecords.size(); r++) {
//            int tmpRecord = gameRecords.at(r);
//            if (tmpRecord < minRecord) {
//                minRecord = tmpRecord;
//            }
//        }
//        if (timeOfGame < minRecord) {

//        }
    }
    mainMenu->updateGameStatus(win);
    closeWidget();
//    loadNextCampaingMaps();
    qDebug() << "WidgetController::closeWidgetGameFinished(); -- END";
}

//void WidgetController::loadNextCampaingMaps() {
//    qDebug() << "WidgetController::loadNextCampaingMaps(); -- ";
//    if (!campaingMaps.empty()) {
//        QString mapPath = campaingMaps.back();
//        campaingMaps.pop_back();
////        closeWidget();
//        showGameWidget(mapPath);
//    } else {
//        campaingMaps.push_back("maps/randomMap.tmx");
//        campaingMaps.push_back("maps/island.tmx");
////        showMainMenu();
//    }
//    qDebug() << "WidgetController::loadNextCampaingMaps(); -- campaingMaps.size():" << campaingMaps.size();
//}
