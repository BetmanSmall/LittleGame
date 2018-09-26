#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include <QKeyEvent>
//#include <QGLWidget>
//#include <QOpenGLWidget>

namespace Ui {
class MainMenu;
}

class MainMenu : public QWidget
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = 0);
    ~MainMenu();
//    void paintEvent(QPaintEvent *);
    void updateGameStatus(bool win);
    void updateSlidersValues();

private:
    Ui::MainMenu *ui;

    QString ASSETS_PATH;

private slots:
    void on_quickPlayButton_clicked();
    void on_playButton_clicked();
    void on_optionsButton_clicked();
    void on_exitButton_clicked();
    void on_killButton_clicked();
    void on_sound_clicked();
    void on_enemyCount_valueChanged(int value);
    void on_difficultyLevel_valueChanged(int value);
    void on_towersCount_valueChanged(int value);

protected:
    void keyPressEvent(QKeyEvent* event);

signals:
    void signal_quickPlay();
    void signal_openChooseMapMenu();
    void signal_openOptionMenu();
    void signal_exit();
    void signal_enemyCountChanged(int value);
    void signal_difficultyLevelChanged(int value);
    void signal_towersCountChanged(int value);
    void signal_actionSoundRadioButton();
};

#endif // MAINMENU_H
