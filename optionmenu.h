#ifndef OPTIONMENU_H
#define OPTIONMENU_H

#include <QWidget>
#include <QMediaPlayer>

namespace Ui {
class OptionMenu;
}

class OptionMenu : public QWidget
{
    Q_OBJECT

public:
    explicit OptionMenu(QWidget *parent = 0);
    ~OptionMenu();

    void updateSlidersAndValues();
    void setMediaPlayer(QMediaPlayer* mediaPlayer);
    void updateRecords(QList<int> recordsList);

private:
    Ui::OptionMenu *ui;

    QMediaPlayer* mediaPlayer;

private slots:
    void on_soundSlider_sliderMoved(int position);
    void on_enemyCount_valueChanged(int value);
    void on_difficultyLevel_valueChanged(int value);
    void on_towersCount_valueChanged(int value);
    void on_randomMapButton_clicked();
    void on_sound_toggled(bool checked);
    void on_panMidMouseButton_toggled(bool checked);
    void on_returnButton_clicked();

signals:
    void signal_enemyCountChanged(int value);
    void signal_difficultyLevelChanged(int value);
    void signal_towersCountChanged(int value);
    void signal_playRandomMap();
    void signal_actionSoundRadioButton(bool checked);
    void signal_panMidMouseButton(bool checked);
    void signal_closeWidget();
};

#endif // OPTIONMENU_H
