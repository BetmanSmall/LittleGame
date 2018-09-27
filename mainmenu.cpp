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

//    ui->labelGameRecord->
//    ui->labelGameStatus->setTextFormat(Qt::RichText);
//    ui->labelGameStatus->setText("<img src=':/assets/images/best.png'>Hello!");
//    ui->playButton->setIcon(QIcon(ASSETS_PATH + "images/Play.png"));
//    ui->optionsButton->setIcon(QIcon(ASSETS_PATH + "images/Options.png"));
//    ui->exitButton->setIcon(QIcon(ASSETS_PATH + "images/Exit.png"));

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
//    ui->labelGameStatus->setVisible(true);
//    ui->labelGameRecord->setVisible(true);
    if (win) {
        ui->labelGameStatus->setPixmap(QPixmap(":/assets/images/statusWin.png"));
//        ui->labelGameStatus->setStyleSheet("QLabel { color : green; }");
//        ui->labelGameStatus->setText( "You WiN!" );
    } else {
        ui->labelGameStatus->setPixmap(QPixmap(":/assets/images/statusLose.png"));
//        ui->labelGameStatus->setStyleSheet("QLabel { color : red; }");
//        ui->labelGameStatus->setText( "You Lose!" );
    }
}

void MainMenu::updateRecords(QList<int> recordsList, int timeOfGame) {
//    ui->labelGameStatus->setVisible(false);
//    ui->labelGameRecord->setVisible(false);
    if (!recordsList.empty()) {
//        ui->gameRecordsListWidget->clear();
//        foreach (int record, recordsList) {
//            ui->gameRecordsListWidget->addItem(QString("%1").arg(record));
//        }
//        ui->gameRecordsListWidget->sortItems(Qt::SortOrder::);
        if (timeOfGame != -1) {
            ui->labelYourTime->setText( QString("Your time:%1ms").arg(timeOfGame) );
        }
        ui->labelGameRecord->setText("GameRecord:" + QString("%1").arg(recordsList.first()) + "ms");
//    } else {
    }
//    for (int w = 0; w < ui->gameRecordsVerticalLayout->count(); w++) {
//        QWidget* widget = ui->gameRecordsVerticalLayout->itemAt(w)->widget();
//        if (widget != NULL) {
//            widget->setVisible(!recordsList.empty());
//        }
//    }
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
//    } else if (event->key() == Qt::Key_Space) {
//        QRect rec = QApplication::desktop()->screenGeometry();
//        move(0, 0);
//        this->setMaximumWidth(rec.width());
//        this->setMaximumHeight(rec.height());
    }
}
