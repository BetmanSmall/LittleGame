#include "mainmenu.h"
#include "ui_mainmenu.h"

#include <QDebug>
//#include <QPainter>

MainMenu::MainMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainMenu)
{
    ui->setupUi(this);

    ASSETS_PATH = "./assets/";

    ui->playButton->setIcon(QIcon(ASSETS_PATH + "images/Play.png"));
    ui->optionsButton->setIcon(QIcon(ASSETS_PATH + "images/Options.png"));
    ui->exitButton->setIcon(QIcon(ASSETS_PATH + "images/Exit.png"));

//    qDebug() << "MainMenu: X: " << width() << " Y: " << height();
}

MainMenu::~MainMenu()
{
    delete ui;
}

//void MainMenu::paintEvent(QPaintEvent *)
//{
//    QPainter p(this);
//    p.fillRect(0, 0, width(), height(), QColor(255, 0, 0));
//}

void MainMenu::updateGameStatus(bool win) {
//    ui->gameStatus->setBackgroundRole(QColor::red());
    if (win) {
        ui->gameStatus->setStyleSheet("QLabel { color : green; }");
        ui->gameStatus->setText( "You WiN!" );
    } else {
        ui->gameStatus->setStyleSheet("QLabel { color : red; }");
        ui->gameStatus->setText( "You Lose!" );
    }
}

void MainMenu::on_playButton_clicked()
{
    qDebug() << "on_PlayButton_clicked()";
    signal_openChooseMapMenu();
}

void MainMenu::on_optionsButton_clicked()
{
    qDebug() << "on_OptionsButton_clicked()";
    signal_openOptionMenu();
}

void MainMenu::on_exitButton_clicked()
{
    qDebug() << "on_ExitButton_clicked";
    signal_exit();
}

void MainMenu::on_quickPlayButton_clicked() {
    qDebug() << "on_quickPlayButton_clicked";
    signal_quickPlay();
}

void MainMenu::on_killButton_clicked() {
    ui->killButton->move(qrand()%width(), qrand()%height());
}

void MainMenu::keyPressEvent(QKeyEvent* event) {
    qDebug() << "MainMenu::keyPressEvent(); -- ";
    if (event->key() == Qt::Key_1) {
        signal_quickPlay();
    }
}

void MainMenu::on_sound_clicked() {
    ui->sound->move(qrand()%width(), qrand()%height());
    signal_actionSoundRadioButton();
}
