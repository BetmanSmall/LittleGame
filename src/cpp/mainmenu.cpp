#include "src/head/mainmenu.h"

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
}

MainMenu::~MainMenu() {
    delete ui;
}

void MainMenu::paintEvent(QPaintEvent *) {
    qDebug() << "MainMenu::paintEvent(); -- ";
}

void MainMenu::showEvent(QShowEvent*) {
    qDebug() << "MainMenu::showEvent(); -- ";
}

void MainMenu::updateGameStatus(bool win) {
    if (win) {
        ui->labelGameStatus->setPixmap(QPixmap(":/assets/images/statusWin.png"));
    } else {
        ui->labelGameStatus->setPixmap(QPixmap(":/assets/images/statusLose.png"));
    }
}

void MainMenu::updateRecords(QList<int> recordsList, int timeOfGame) {
    if (!recordsList.empty()) {
        if (timeOfGame != -1) {
            ui->labelYourTime->setText( QString("Твоё время: %1 секунд").arg(timeOfGame/1000) );
        }
        ui->labelGameRecord->setText("Лучшее время: " + QString("%1").arg(recordsList.first()/1000) + " секунд");
    }
}

void MainMenu::on_playButton_clicked() {
    qDebug() << "MainMenu::on_playButton_clicked(); -- ";
    signal_playNormalMap();
}

void MainMenu::on_optionsButton_clicked() {
    qDebug() << "MainMenu::on_optionsButton_clicked(); -- ";
    signal_openOptionMenu();
}

void MainMenu::on_exitButton_clicked() {
    qDebug() << "MainMenu::on_exitButton_clicked(); -- ";
    signal_exit();
}

void MainMenu::keyPressEvent(QKeyEvent* event) {
    int key = event->key();
    qDebug() << "MainMenu::keyPressEvent(); -- key:" << key;
    if (key == Qt::Key_Enter || key == Qt::Key_1) {
        signal_playNormalMap();
    } else if (key == Qt::Key_2) {
        signal_openOptionMenu();
    } else if (key == Qt::Key_3) {
        signal_exit();
    }
}
