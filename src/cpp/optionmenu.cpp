#include "src/head/optionmenu.h"
#include "ui_optionmenu.h"

OptionMenu::OptionMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OptionMenu)
{
    ui->setupUi(this);
}

OptionMenu::~OptionMenu() {
    delete ui;
}

void OptionMenu::updateSlidersAndValues() {
    signal_enemyCountChanged(ui->enemyCount->value());
    signal_difficultyLevelChanged(ui->difficultyLevel->value());
    signal_towersCountChanged(ui->towersCount->value());
    int a = rand()%2;
    qDebug() << "OptionMenu::updateSlidersAndValues(); -- rand()%2:" << a;
    ui->panMidMouseButton->setChecked(a);
    signal_panMidMouseButton(ui->panMidMouseButton->isChecked());
}

void OptionMenu::setMediaPlayer(QMediaPlayer *mediaPlayer) {
    this->mediaPlayer = mediaPlayer;
    qDebug() << "OptionMenu::setMediaPlayer() -- mediaPlayer->volume(): " << mediaPlayer->volume();
    ui->soundSlider->setValue(mediaPlayer->volume());
}

void OptionMenu::updateRecords(QList<int> recordsList) {
    if (!recordsList.empty()) {
        ui->gameRecordsListWidget->clear();
        foreach (int record, recordsList) {
            ui->gameRecordsListWidget->addItem(QString("%1").arg(record));
        }
    }
}

void OptionMenu::on_soundSlider_sliderMoved(int position) {
    qDebug() << "OptionMenu::on_soundSlider_sliderMoved() -- position: " << position;
    mediaPlayer->setVolume(position);
}

void OptionMenu::on_enemyCount_valueChanged(int value) {
    qDebug() << "OptionMenu::on_enemyCount_valueChanged(); -- value:" << value;
    ui->labelEnemyCount->setText( QString("EnemyCount:%1").arg(value) );
    signal_enemyCountChanged(value);
}

void OptionMenu::on_difficultyLevel_valueChanged(int value) {
    qDebug() << "OptionMenu::on_difficultyLevel_valueChanged(); -- value:" << value;
    QString difficultyLevel = ( (value==0) ? "Easy" : ( (value==1) ? "Normal" : "Hard" ) );
    ui->labelDifficultyLevel->setText( QString("DifficultyLevel:%1").arg(difficultyLevel) );
    if (value == 0) {
        ui->towersCount->setValue(0);
        ui->towersCount->setEnabled(false);
    } else if (value == 1) {
        ui->towersCount->setValue(ui->towersCount->maximum()/2);
        ui->towersCount->setEnabled(true);
    } else if (value == 2) {
        ui->towersCount->setValue(ui->towersCount->maximum());
        ui->towersCount->setEnabled(true);
    }
    signal_difficultyLevelChanged(value);
}

void OptionMenu::on_towersCount_valueChanged(int value) {
    qDebug() << "OptionMenu::on_towersCount_valueChanged(); -- value:" << value;
    ui->labelTowerCount->setText( QString("TowersCount <= %1").arg(value) );
    signal_towersCountChanged(value);
}

void OptionMenu::on_randomMapButton_clicked() {
    qDebug() << "OptionMenu::on_randomMapButton_clicked(); -- ";
    signal_playRandomMap();
}

void OptionMenu::on_sound_toggled(bool checked) {
    qDebug() << "OptionMenu::on_sound_toggled(); -- checked:" << checked;
    signal_actionSoundRadioButton(checked);
}

void OptionMenu::on_panMidMouseButton_toggled(bool checked) {
    qDebug() << "OptionMenu::on_panMidMouseButton_toggled(); -- checked:" << checked;
    signal_panMidMouseButton(checked);
}

void OptionMenu::on_returnButton_clicked() {
    signal_closeWidget();
}
