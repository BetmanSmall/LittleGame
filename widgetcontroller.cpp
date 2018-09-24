#include "widgetcontroller.h"

#include <QDebug>

WidgetController::WidgetController(QWidget *parent) :
    QWidget(parent)
{
    ASSETS_PATH = "./assets/";

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

    mediaPlayer = new QMediaPlayer();
    mediaPlayer->setMedia(QUrl::fromLocalFile(ASSETS_PATH + "music/mainmenu2.mp3"));
    mediaPlayer->setVolume(0);
    mediaPlayer->play();

    stackedWidget = new QStackedWidget;

    QVBoxLayout* layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->addWidget(stackedWidget);
    setLayout(layout);

//    show();
//    move(100,100);

//    setWindowState(Qt::WindowFullScreen);

    showMainMenu();
}

void WidgetController::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    QPixmap pix = QPixmap(ASSETS_PATH + "images/mainmenu.jpg");
//    qDebug() << pix;

    p.drawPixmap(0, 0, width(), height(), pix);
//    p.fillRect(0, 0, width(), height(), QColor(0, 0, 0));

    p.end();
}

void WidgetController::showMainMenu() {
    qDebug() << "WidgetController::showMainMenu(); -- ";
    mainMenu = new MainMenu();
    connect(mainMenu, SIGNAL(signal_quickPlay()), this, SLOT(quickPlay()));
    connect(mainMenu, SIGNAL(signal_openChooseMapMenu()), this, SLOT(showChooseMapMenu()));
    connect(mainMenu, SIGNAL(signal_openOptionMenu()), this, SLOT(showOptionMenu()));
    connect(mainMenu, SIGNAL(signal_exit()), this, SLOT(closeWidget()));
    connect(mainMenu, SIGNAL(signal_actionSoundRadioButton()), this, SLOT(actionMainMenuSoundRadionButton()));

    stackedWidget->addWidget(mainMenu);
    stackedWidget->setCurrentWidget(mainMenu);
}

void WidgetController::showChooseMapMenu() {
    qDebug() << "WidgetController::showChooseMapMenu(); -- ";

    ChooseMapMenu* chooseMapMenu = new ChooseMapMenu();

    connect(chooseMapMenu, SIGNAL(signal_loadMap1()), this, SLOT(loadMap1()));
    connect(chooseMapMenu, SIGNAL(signal_loadMap2()), this, SLOT(loadMap2()));
    connect(chooseMapMenu, SIGNAL(signal_loadMap3()), this, SLOT(loadMap3()));
    connect(chooseMapMenu, SIGNAL(signal_loadMap4()), this, SLOT(loadMap4()));
    connect(chooseMapMenu, SIGNAL(signal_loadMap5()), this, SLOT(loadMap5()));
    connect(chooseMapMenu, SIGNAL(signal_loadMap6()), this, SLOT(loadMap6()));
    connect(chooseMapMenu, SIGNAL(signal_closeWidget()), this, SLOT(closeWidget()));

    stackedWidget->addWidget(chooseMapMenu);
    stackedWidget->setCurrentWidget(chooseMapMenu);
}

void WidgetController::actionMainMenuSoundRadionButton() {
//    if (mediaPlayer->volume()) {
//        mediaPlayer->setVolume();
//    } else {
//        mediaPlayer->set
//    }
//    mediaPlayer->mutedChanged( true );
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
    connect(gameWidget, SIGNAL(signal_closeWidget()), this, SLOT(closeWidget()));
    connect(gameWidget, SIGNAL(signal_closeWidgetGameFinished(bool)), this, SLOT(closeWidgetGameFinished(bool)));
    stackedWidget->addWidget(gameWidget);
    stackedWidget->setCurrentWidget(gameWidget);
}

void WidgetController::quickPlay() {
    qDebug() << "WidgetController::quickPlay(); -- ";
    GameWidget* gameWidget = new GameWidget();
    gameWidget->setMinimumWidth(1024);
    gameWidget->setMinimumHeight(768);
//    gameWidget->setMaximumWidth(1024);
//    gameWidget->setMaximumHeight(768);
    loadMap(gameWidget);
    gameWidget->loadMap(ASSETS_PATH + "maps/randomMap.tmx");
    qDebug() << "WidgetController::quickPlay(); -- ";
}

void WidgetController::loadMap1()
{
    qDebug() << "loadMap1()";

    GameWidget* gameWidget = new GameWidget();

    loadMap(gameWidget);

    gameWidget->loadMap(ASSETS_PATH + "maps/arctic.tmx");
}

void WidgetController::loadMap2()
{
    qDebug() << "loadMap2()";

    GameWidget* gameWidget = new GameWidget();

    loadMap(gameWidget);

    gameWidget->loadMap(ASSETS_PATH + "maps/forest.tmx");
}

void WidgetController::loadMap3()
{
    qDebug() << "loadMap3()";

    GameWidget* gameWidget = new GameWidget();

    loadMap(gameWidget);

    gameWidget->loadMap(ASSETS_PATH + "maps/desert.tmx");
}

void WidgetController::loadMap4()
{
    qDebug() << "loadMap4()";

    GameWidget* gameWidget = new GameWidget();

    loadMap(gameWidget);

    gameWidget->loadMap(ASSETS_PATH + "maps/boloto.tmx");
}

void WidgetController::loadMap5()
{
    qDebug() << "loadMap5()";

    GameWidget* gameWidget = new GameWidget();

    loadMap(gameWidget);

    gameWidget->loadMap(ASSETS_PATH + "maps/isom_workMap.tmx");
}

void WidgetController::loadMap6()
{
    qDebug() << "loadMap6()";

    GameWidget* gameWidget = new GameWidget();

    loadMap(gameWidget);

    gameWidget->loadMap(ASSETS_PATH + "maps/isometric_testMap.tmx");
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
}

void WidgetController::closeWidgetGameFinished(bool win) {
    qDebug() << "WidgetController::closeWidgetGameFinished(); -- win:" << win;
    mainMenu->updateGameStatus(win);
    closeWidget();
}
