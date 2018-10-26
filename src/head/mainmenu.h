#ifndef MAINMENU_H
#define MAINMENU_H

#include <QDebug>
#include <QWidget>
#include <QKeyEvent>
#include <QPainter>

#include "ui_mainmenu.h"

namespace Ui {
class MainMenu;
}

class MainMenu : public QWidget
{
    Q_OBJECT
#ifdef QT_DEBUG
    QString ASSETS_PATH = "../../LittleGame/assets/";
#else
    QString ASSETS_PATH = "./assets/";
#endif

public:
    explicit MainMenu(QWidget *parent = 0);
    ~MainMenu();
    void paintEvent(QPaintEvent *);
    void showEvent(QShowEvent*);
    void updateGameStatus(bool win);
    void updateRecords(QList<int> recordsList, int timeOfGame = -1);

private:
    Ui::MainMenu *ui;

    QPainter p;

private slots:
    void on_playButton_clicked();
    void on_optionsButton_clicked();
    void on_exitButton_clicked();

protected:
    void keyPressEvent(QKeyEvent* event);

signals:
    void signal_playNormalMap();
    void signal_openOptionMenu();
    void signal_exit();
};

#endif // MAINMENU_H
