#include "mainmenu.h"
#include "ui_mainmenu.h"

#include <QDebug>
//#include <QPainter>

MainMenu::MainMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainMenu)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    ASSETS_PATH = "../../LittleGame/assets/";
#else
    ASSETS_PATH = "./assets/";
#endif

    ui->playButton->setIcon(QIcon(ASSETS_PATH + "images/Play.png"));
    ui->optionsButton->setIcon(QIcon(ASSETS_PATH + "images/Options.png"));
    ui->exitButton->setIcon(QIcon(ASSETS_PATH + "images/Exit.png"));

//    qDebug() << "MainMenu: X: " << width() << " Y: " << height();
}

MainMenu::~MainMenu() {
    delete ui;
}

//void MainMenu::paintEvent(QPaintEvent *) {
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

void MainMenu::updateEnemyAndTowersCount() {
    signal_enemyCountChanged(ui->enemyCount->value());
    signal_towersCountChanged(ui->towersCount->value());
}

void MainMenu::on_playButton_clicked() {
    qDebug() << "MainMenu::on_playButton_clicked(); -- ";
    signal_openChooseMapMenu();
}

void MainMenu::on_optionsButton_clicked() {
    qDebug() << "MainMenu::on_optionsButton_clicked(); -- ";
    signal_openOptionMenu();
}

void MainMenu::on_exitButton_clicked() {
    qDebug() << "MainMenu::on_exitButton_clicked(); -- ";
    signal_exit();
}

void MainMenu::on_quickPlayButton_clicked() {
    qDebug() << "MainMenu::on_quickPlayButton_clicked(); -- ";
    signal_quickPlay();
}

void MainMenu::on_killButton_clicked() {
    qDebug() << "MainMenu::on_killButton_clicked(); -- ";
    ui->killButton->move(qrand()%width(), qrand()%height());
}

void MainMenu::keyPressEvent(QKeyEvent* event) {
    int key = event->key();
    qDebug() << "MainMenu::keyPressEvent(); -- key:" << key;
    if (key == Qt::Key_Enter || key == Qt::Key_1) {
        signal_quickPlay();
//    } else if (event->key() == Qt::Key_Space) {
//        QRect rec = QApplication::desktop()->screenGeometry();
//        move(0, 0);
//        this->setMaximumWidth(rec.width());
//        this->setMaximumHeight(rec.height());
    }
}

void MainMenu::on_sound_clicked() {
    qDebug() << "MainMenu::on_sound_clicked(); -- ";
    ui->sound->move(qrand()%width(), qrand()%height());
    signal_actionSoundRadioButton();
}

void MainMenu::on_enemyCount_valueChanged(int value) {
    qDebug() << "MainMenu::on_enemyCount_valueChanged(); -- value:" << value;
    signal_enemyCountChanged(value);
}

void MainMenu::on_towersCount_valueChanged(int value) {
    qDebug() << "MainMenu::on_towersCount_valueChanged(); -- value:" << value;
    signal_towersCountChanged(value);
}
