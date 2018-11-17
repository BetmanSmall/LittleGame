#include "src/head/widgetcontroller.h"

WidgetController::WidgetController(QWidget *parent) :
    QWidget(parent)
{
//    setWindowState(Qt::WindowMaximized);

    stackedWidget = new QStackedWidget;
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->addWidget(stackedWidget);
    setLayout(layout);

    mediaPlayer = new QMediaPlayer();
    mediaPlayer->setMedia(QUrl::fromLocalFile(ASSETS_PATH + "music/mainmenu2.mp3"));
    mediaPlayer->setVolume(0);
    mediaPlayer->play();

    factionsManager = new FactionsManager();
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
    showMainMenu();
}

void WidgetController::paintEvent(QPaintEvent *) {
    painter.begin(this);
    painter.drawPixmap(0, 0, width(), height(), mainMenuBackground);
    painter.end();
}

void WidgetController::loadMap(QString mapPath) {
    qDebug() << "WidgetController::loadMap(); -- mapPath:" << mapPath;
    gameWidget = new GameScreenGL(ASSETS_PATH + mapPath, factionsManager, enemyCount, difficultyLevel, towersCount, this);

//    gameWidget->cameraController->panMidMouseButton = panMidMouseButtonBool;

    gameWidget->setMinimumWidth(1024);
    gameWidget->setMinimumHeight(768);
    qDebug() << "WidgetController::showGameWidget(); -- END";
    showGameWidget(gameWidget);
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

    QDirIterator it(":/assets/images/mainmenu/", QDirIterator::Subdirectories);
    std::vector<QString> menuImgs;
    while (it.hasNext()) { menuImgs.push_back(it.next()); }
    mainMenuBackground = QPixmap(menuImgs[rand()%menuImgs.size()]);
}

void WidgetController::loadRandomMap() {
    loadMap("maps/randomMap.tmx");
}

void WidgetController::loadNormalMap() {
    loadMap("maps/randomMap.tmx");
//    loadMap("maps/island.tmx");
//    loadMap("maps/desert.tmx");
//    loadMap("maps/winter.tmx");
//    loadMap("maps/summer.tmx");
}

void WidgetController::showGameWidget(GameScreenGL *gameWidget) {
    qDebug() << "WidgetController::showGameWidget(); -- gameWidget:" << gameWidget;
    connect(gameWidget, SIGNAL(signal_closeWidget()), this, SLOT(closeWidget()));
    connect(gameWidget, SIGNAL(signal_changeWindowState()), this, SLOT(changeWindowState()));
    connect(gameWidget, SIGNAL(signal_closeWidgetGameFinished(bool,int)), this, SLOT(closeWidgetGameFinished(bool,int)));
    stackedWidget->addWidget(gameWidget);
    stackedWidget->setCurrentWidget(gameWidget);
    qDebug() << "WidgetController::showGameWidget(); -- end";
}

void WidgetController::changeWindowState() {
    qDebug() << "WidgetController::changeWindowState(); -- ";
    setWindowState(windowState() ^ Qt::WindowFullScreen);
}

void WidgetController::showOptionMenu() {
    qDebug() << "WidgetController::showChooseMapMenu(); -- ";

    optionMenu = new OptionMenu();
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
//    mediaPlayer->setVolume( (mediaPlayer->volume()>0) ? 0 : 100);
    if (checked) {
        mediaPlayer->setVolume(100);
    } else {
        mediaPlayer->setVolume(0);
    }
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
    } else {
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
    }
    mainMenu->updateGameStatus(win);
    closeWidget();
    qDebug() << "WidgetController::closeWidgetGameFinished(); -- END";
}
