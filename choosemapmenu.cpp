#include "choosemapmenu.h"
#include "ui_choosemapmenu.h"

#include <QDebug>

ChooseMapMenu::ChooseMapMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChooseMapMenu)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    ASSETS_PATH = "../../LittleGame/assets/";
#else
    ASSETS_PATH = "./assets/";
#endif

    ui->loadMap1Button->setIcon(QIcon(ASSETS_PATH + "images/button1v2.png"));
    ui->loadMap2Button->setIcon(QIcon(ASSETS_PATH + "images/button2v2.png"));
    ui->loadMap3Button->setIcon(QIcon(ASSETS_PATH + "images/button3v2.png"));

}

ChooseMapMenu::~ChooseMapMenu() {
    delete ui;
}

void ChooseMapMenu::on_returnButton_clicked() {
    signal_closeWidget();
}

void ChooseMapMenu::on_loadMap1Button_clicked() {

}

void ChooseMapMenu::on_loadMap2Button_clicked() {

}

void ChooseMapMenu::on_loadMap3Button_clicked() {

}

void ChooseMapMenu::on_loadMap4Button_clicked() {

}

void ChooseMapMenu::on_loadMap5Button_clicked() {

}

void ChooseMapMenu::on_loadMap6Button_clicked() {

}
