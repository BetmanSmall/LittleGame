#include "src/head/widgetcontroller.h"

WidgetController::WidgetController(QWidget *parent) :
    QWidget(parent)
{
#ifdef QT_DEBUG
    ASSETS_PATH = "../../LittleGame/assets/";
#else
    ASSETS_PATH = "./assets/";
#endif

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
    GameWidget* gameWidget = new GameWidget(this);
    gameWidget->setMinimumWidth(1024);
    gameWidget->setMinimumHeight(768);

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
    mediaPlayer->setVolume( (mediaPlayer->volume()>0) ? 0 : 100);
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
