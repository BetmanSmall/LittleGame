#include "gamewidget.h"
#include "ui_gamewidget.h"

GameWidget::GameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameWidget)
{
    ui->setupUi(this);

    ASSETS_PATH = "./assets/";

    srand(time(0));

    gameStart = true;
    gamePause = false;
    mapLoad = false;

    underConstruction = NULL;

    pixelsShiftMap = 32;

    zoomSizeCell = 8;
    zoomMaxSizeCell = 256;
    zoomMinSizeCell = 48;

    defaultNumCreateUnits = 30;

    unitsMove_TimerMilliSec = 100;
    towersAttack_TimerMilliSec = 1000;
    scanMouseMove_TimerMilliSec = 100;
    bulletsFly_TimerMilliSec = 100;

    bulletsFly_TimerId = 0;
    unitsMove_TimerId = 0;
    towersAttack_TimerId = 0;
    scanMouseMove_TimerId = 0;

    scanMouseMove_TimerId = startTimer(scanMouseMove_TimerMilliSec);

    // ПЕРЕПИСАТЬ!
    bulletsFly_TimerId = startTimer(bulletsFly_TimerMilliSec);
    // ----------

    test = 0;

//    setMouseTracking(true);

    ui->loadMaps->setHidden(true);
    ui->clearMap->setHidden(true);
    ui->goUnits->setHidden(true);
    ui->closeWidget->setHidden(true);

//    qDebug() << "GameWidget: X: " << width() << " Y: " << height();

//    setWindowState(Qt::WindowFullScreen);

//    int x = 65;//120
//    int y = 30;//60
//    field.createField(x, y);// 65, 30
//    this->setMaximumSize(spaceWidget*2 + x*sizeCell, spaceWidget*2 + y*sizeCell);
//    this->setMinimumSize(spaceWidget*2 + x*sizeCell, spaceWidget*2 + y*sizeCell);
//    this->move(0, 0);

//    for(int x = 0; x < field.getSizeX(); x++)
//        for(int y = 0; y < field.getSizeY(); y++)
//            if(rand()%101 <= 30)
//                field.setBusy(x, y, global_pixmap);

//    for(int x = field.getSizeX(); x >= 0; x--)
//        for(int y = field.getSizeY(); y >= 0; y--)
//            if(rand()%101 <= 10)
//                field.setTower(x, y);

//    int numUnitsK = 0;
//    for(int x = 0; x < field.getSizeX(); x++)
//        for(int y = 0; y < field.getSizeY(); y++)
//            if(rand()%101 <= 5)
//                if(numUnitsK++ < defaultNumCreateUnits)
//                    field.setUnit(x, y);

//    field.createSpawnPoint(defaultNumCreateUnits, 0, 0);
//    field.createExitPoint(field.getSizeX()-1, field.getSizeY()-1);

//    loadMap(ASSETS_PATH + "maps/swamp.tmx");
}

GameWidget::~GameWidget()
{
    field.deleteField();
    delete ui;
}

void GameWidget::timerEvent(QTimerEvent *event) {
//    if(test == 1)
//        qDebug() << "test";
    int timerId = event->timerId();

    if(timerId == unitsMove_TimerId) {
//        if(test == 0)
//            field.spawnHeroInSpawnPoint();
//        test = test<8 ? test+1 : 0;
//        qDebug() << "GameWidget::timerEvent(); -- unitsMove_TimerId test:" << test;
        if(int result = field.stepAllUnits()) {
            if(result == 4) {
                global_text = "Hero contact With Enemy!";
                signal_closeWidgetGameFinished(false);
            } else if(result == 3) {
                global_text = "Hero in ExitPoint!";
                signal_closeWidgetGameFinished(true);
            } else if(result == 2) {
                global_text = "You WIN!";
//                QMessageBox msg;
//                msg.setText("You WIN!");
//                msg.exec();
//                qDebug() << "You Win!";
//                stopTimer_UnitsMoveAndTowerAttack();
            } else if(result == 1) {
                global_text = "You LOSE!";
//                QMessageBox msg;
//                msg.setText("You LOSE!");
//                msg.exec();
//                qDebug() << "You Lose!";
//                stopTimer_UnitsMoveAndTowerAttack();
            } else if(result == -1) {
                if(field.deleteTower()) {
                    field.waveAlgorithm();
                } else {
                    stopTimer_UnitsMoveAndTowerAttack();
                }
            }
        } else {
//            field.setMainCoorMapAndSizeCell(mainCoorMapX, mainCoorMapY, sizeCell);
            field.towersAttack();
        }
    } else if(timerId == scanMouseMove_TimerId) {
        int curX = cursor().pos().x();
        int curY = cursor().pos().y();

//        qDebug() << "X: " << curX;
//        qDebug() << "Y: " << curY;

//        qDebug() << "width()" << width();
//        qDebug() << "height()" << height();

        if(curX == 0 || curY == 0 || curX == width()-1 || curY == height()-1) {
            int mainCoorMapX = field.getMainCoorMapX();
            int mainCoorMapY = field.getMainCoorMapY();
            int sizeCell = field.getSizeCell();

            int sizeX = field.getSizeX();
            int sizeY = field.getSizeY();

            if(curX == 0)
                if(mainCoorMapX < 0)
                    mainCoorMapX += pixelsShiftMap;
            if(curY == 0)
                if(mainCoorMapY < 0)
                    mainCoorMapY += pixelsShiftMap;
            if(curX == width()-1)
                if(mainCoorMapX+sizeX*sizeCell > width())
                    mainCoorMapX -= pixelsShiftMap;
            if(curY == height()-1)
                if(mainCoorMapY+sizeY*sizeCell > height())
                    mainCoorMapY -= pixelsShiftMap;

            mainCoorMapX = (mainCoorMapX > 0) ? 0 : mainCoorMapX;
            mainCoorMapY = (mainCoorMapY > 0) ? 0 : mainCoorMapY;

            mainCoorMapX = (mainCoorMapX + sizeCell*sizeX < width()) ? width()-sizeCell*sizeX : mainCoorMapX;
            mainCoorMapY = (mainCoorMapY + sizeCell*sizeY < height()) ? height()-sizeCell*sizeY : mainCoorMapY;

            field.setMainCoorMap(mainCoorMapX, mainCoorMapY);
        }
        if (whichCell(curX, curY)) {
            if (underConstruction) {
                underConstruction->setEndCoors(curX, curY);
            }
        }
    } else if (timerId == bulletsFly_TimerId) {
        // ПЕРЕПИСАТЬ!
        std::vector<Tower*> towers = field.getAllTowers();

        for(int k = 0; k < towers.size(); k++)
        {
            Tower* tmpTower = towers[k];

            for(int iBullet = 0; iBullet < tmpTower->bullets.size(); iBullet++) {
//                qDebug() << "k: " << k << " iBullet: " << iBullet;
                tmpTower->bullets[iBullet].move();
            }
        }
        // --------------------
    }
    update();
}

void GameWidget::keyPressEvent(QKeyEvent * event)
{
    int mainCoorMapX = field.getMainCoorMapX();
    int mainCoorMapY = field.getMainCoorMapY();
    int sizeCell = field.getSizeCell();

    int sizeX = field.getSizeX();
    int sizeY = field.getSizeY();

    int key = event->key();
//    qDebug() << "GameWidget::keyPressEvent() -- key: " << key;

    if(key == Qt::Key_0) {
//        qDebug
//        test = 1;
    } else if (event->key() == Qt::Key_Enter) {
        signal_closeWidgetGameFinished(true);
    } else if(key == Qt::Key_Left) {
        if(mainCoorMapX < 0) {
            mainCoorMapX += pixelsShiftMap;
        }
    } else if(key == Qt::Key_Up) {
        if(mainCoorMapY < 0) {
            mainCoorMapY += pixelsShiftMap;
        }
    }
    else if(key == Qt::Key_Right)
    {
        if(mainCoorMapX+sizeX*sizeCell > width()) {
            mainCoorMapX -= pixelsShiftMap;
        }
    }
    else if(key == Qt::Key_Down)
    {
        if(mainCoorMapY+sizeY*sizeCell > height()) {
            mainCoorMapY -= pixelsShiftMap;
        }
    }
    else if(key == Qt::Key_B) {
        buildTower();
    } else if(key == Qt::Key_N) {
        if(underConstruction) {
            delete underConstruction;
            underConstruction = NULL;
        }
    }
    field.setMainCoorMap(mainCoorMapX, mainCoorMapY);
//    if(key == Qt::Key_0)
//    {
//        if(waveAlgorithm(mouseX, mouseY) == 1)
//            global_text2 = "Yes!";
//        else
//            global_text2 = "No!";

//        field.createExitPoint(mouseX,mouseY);

//    }
//    else if(event->button() == Qt::XButton2)
//    {
//        field.createSpawnPoint(defaultNumCreateUnits, mouseX, mouseY);
//        field.setUnit(mouseX,mouseY);
//    }
    update();
}

void GameWidget::paintEvent(QPaintEvent *)
{
//    if(test == 1)
//        qDebug() << "test";
//    test = test<100 ? test+1 : 0;
//    qDebug() << "Paint" << test;
    p.begin(this);
    if(gameStart)
    {
//        if(!gamePause)
//        {
            drawFullField();
            if(ui->drawField_checkBox->isChecked())
                drawField();
            if(ui->drawRelief_checkBox->isChecked())
                drawRelief();
//            drawTowersByField();

            if(ui->drawUnits_checkBox->isChecked())
                drawUnits();

            if(ui->drawTowersByTowers_checkBox->isChecked())
                drawTowersByTowers();

            if(ui->drawGrid_checkBox->isChecked())
                drawGrid();
            if(ui->drawStepsAndMouse_checkBox->isChecked())
                drawStepsAndMouse();
            if(ui->drawTowerUnderConstruction_checkBox->isChecked())
                drawTowersUnderConstruction();

            p.setPen(QColor(255,0,0));
            p.drawLine(width()/2-5, height()/2-5, width()/2+5, height()/2+5);
            p.drawLine(width()/2+5, height()/2-5, width()/2-5, height()/2+5);

            p.setPen(QColor(255,0,0));
//            p.drawText(width()-width()/4, height()-height()/10, QString("%1").arg(mainCoorMapX));
//            p.drawText(width()-width()/4, height()-height()/10+20, QString("%1").arg(mainCoorMapY));
            p.drawText(10, 20, QString(global_text.c_str()));
            p.drawText(10, 40, QString(global_text2.c_str()));
            p.drawText(10, 60, QString("%1").arg(field.getMainCoorMapX()));
            p.drawText(10, 80, QString("%1").arg(field.getMainCoorMapY()));
//            p.drawText(10, 100, QString("%1").arg(underConstruction->startX));
//            p.drawText(10, 120, QString("%1").arg(underConstruction->startY));
//        }
    }
    p.end();
}

void GameWidget::drawGrid()
{
    int mainCoorMapX = field.getMainCoorMapX();
    int mainCoorMapY = field.getMainCoorMapY();
    int spaceWidget = field.getSpaceWidget();
    int sizeCell = field.getSizeCell();

    int fieldX = field.getSizeX();
    int fieldY = field.getSizeY();

    p.setPen(QColor(100, 60, 21));

    if(field.getIsometric()) {
        int halfSizeCellX = field.getSizeCell()/2;
        int halfSizeCellY = halfSizeCellX/2;
//        int sizeCellX = field.getTileMapWidth()/2;
//        int sizeCellY = field.getTileMapHeight()/2;
        int isometricSpaceX = halfSizeCellX*fieldY;
        int isometricSpaceY = halfSizeCellY*fieldY;

//        qDebug() << "GameWidget::drawGrid(); -- isometricSpaceX: " << isometricSpaceX;
//        qDebug() << "GameWidget::drawGrid(); -- isometricSpaceY: " << isometricSpaceY;
//        qDebug() << "GameWidget::drawGrid(); -- fieldX: " << fieldX;
//        qDebug() << "GameWidget::drawGrid(); -- fieldY: " << fieldY;

        for(int x = 0; x < fieldX+1; x++) {
            int x1 = mainCoorMapX + isometricSpaceX+spaceWidget + x*halfSizeCellX;
            int y1 = mainCoorMapY + spaceWidget + x*halfSizeCellY;
            int x2 = mainCoorMapX + spaceWidget + x*halfSizeCellX;
            int y2 = mainCoorMapY + isometricSpaceY+spaceWidget + x*halfSizeCellY;
            p.drawLine(x1, y1, x2, y2);
//            qDebug() << "GameWidget::drawGrid(); -- x1: " << x1 << " y1: " << y1 << " x2: " << x2 << " y2: " << y2;
        }
        for(int y = 0; y < fieldY+1; y++) {
            int x1 = mainCoorMapX + isometricSpaceX+spaceWidget - y*halfSizeCellX;
            int y1 = mainCoorMapY + spaceWidget + y*halfSizeCellY;
            int x2 = mainCoorMapX + isometricSpaceX*2+spaceWidget - (halfSizeCellX*(fieldY-fieldX)) - y*halfSizeCellX;
            int y2 = mainCoorMapY + isometricSpaceY+spaceWidget - (halfSizeCellY*(fieldY-fieldX)) + y*halfSizeCellY;
            p.drawLine(x1, y1, x2, y2);
//            qDebug() << "GameWidget::drawGrid(); -- x1: " << x1 << " y1: " << y1 << " x2: " << x2 << " y2: " << y2;
        }
    } else {
        for(int x = 0; x < fieldX+1; x++) {
            p.drawLine(mainCoorMapX + spaceWidget + x*sizeCell, mainCoorMapY + spaceWidget, mainCoorMapX + spaceWidget + x*sizeCell, mainCoorMapY + spaceWidget + sizeCell*fieldY);
        }
        for(int y = 0; y < fieldY+1; y++) {
            p.drawLine(mainCoorMapX + spaceWidget, mainCoorMapY + spaceWidget + y*sizeCell, mainCoorMapX + spaceWidget + sizeCell*fieldX, mainCoorMapY + spaceWidget + y*sizeCell);
        }
    }
}

void GameWidget::drawField()
{
    int fieldX = field.getSizeX();
    int fieldY = field.getSizeY();

    int mainCoorMapX = field.getMainCoorMapX();
    int mainCoorMapY = field.getMainCoorMapY();
    int spaceWidget = field.getSpaceWidget();
    int sizeCell = field.getSizeCell();

    int isometricCoorX = (sizeCell/2) * fieldY;
    int isometricCoorY = 0;

//    int sizeCellX = field.getTileMapWidth()/2;
//    int sizeCellY = field.getTileMapHeight()/2;
    int sizeCellX = field.getSizeCell();
    int sizeCellY = sizeCellX/2;

    for(int y = 0; y < fieldY; y++)
    {
        for(int x = 0; x < fieldX; x++)
        {
//            qDebug() << "GameWidget::drawField(); -- x: (" << x << "/" << fieldX << ") -- y: (" << y << "/" << fieldY << ")";
            if(mapLoad) {
                QPixmap pix = field.getCell(x, y)->backgroundPixmap;//getPixmapOfCell(x, y);
                if(!field.getIsometric()) {
//                    qDebug() << "GameWidget::drawField(); -- NO Isometric!";
                    int pxlsX = mainCoorMapX + spaceWidget + x*sizeCell;
                    int pxlsY = mainCoorMapY + spaceWidget + y*sizeCell;
                    int localSizeCell = sizeCell;

                    p.drawPixmap(pxlsX, pxlsY, localSizeCell, localSizeCell, pix);
                } else {
//                    qDebug() << "GameWidget::drawField(); -- Isometric!";
                    int x1 = mainCoorMapX + isometricCoorX - (sizeCellX/2) + x*(sizeCellX/2);
                    int y1 = mainCoorMapY + isometricCoorY - (sizeCellY) + x*(sizeCellY/2);

//                    int x2 = mainCoorMapX + isometricSpaceX*2+spaceWidget - y*sizeCellX; << Android DNO
//                    int y2 = mainCoorMapY + isometricSpaceY+spaceWidget + y*sizeCellY; << Android DNO -- Dorabotat'

//                    qDebug() << "GameWidget::drawField(); -- sizeCellX: " << sizeCellX;
//                    qDebug() << "GameWidget::drawField(); -- sizeCellY: " << sizeCellY;
//                    qDebug() << "GameWidget::drawField(); -- mainCoorMapX: " << mainCoorMapX;
//                    qDebug() << "GameWidget::drawField(); -- mainCoorMapY: " << mainCoorMapY;
//                    qDebug() << "GameWidget::drawField(); -- isometricCoorX: " << isometricCoorX;
//                    qDebug() << "GameWidget::drawField(); -- isometricCoorY: " << isometricCoorY;
//                    qDebug() << "GameWidget::drawField(); -- x: " << x << " y: " << y << " pix: " << !pix.isNull();
//                    qDebug() << "GameWidget::drawField(); -- x: " << x;
//                    qDebug() << "GameWidget::drawField(); -- y: " << y;
//                    qDebug() << "GameWidget::drawField(); -- x1: " << x1;
//                    qDebug() << "GameWidget::drawField(); -- y1: " << y1;

                    p.drawPixmap(x1, y1, sizeCellX, sizeCellY*2, pix);
                }
            }
        }
        isometricCoorX = (field.getSizeCell()/2) * (fieldY - (y+1));
        isometricCoorY = (field.getSizeCell()/4) * (y+1);
    }
}

void GameWidget::drawFullField()
{
    if(field.getIsometric()) {
        QPixmap pix = field.getCell(0, 0)->backgroundPixmap;//.getPixmapOfCell(0, 0);

        int sizeCellX = field.getSizeCell();
        int sizeCellY = sizeCellX/2;

        int sizeX = (width()/sizeCellX)+1;
        int sizeY = (height()/sizeCellY)*2+2;

        int isometricSpaceX = 0;
        int isometricSpaceY = -(sizeCellY/2);

        for(int y = 0; y <= sizeY; y++) {
            for(int x = 0; x <= sizeX; x++) {
                p.drawPixmap(isometricSpaceX - sizeCellX/2 + x*sizeCellX, isometricSpaceY - sizeCellY, sizeCellX, sizeCellY*2, pix);
            }
            isometricSpaceY += sizeCellY/2;
            isometricSpaceX = isometricSpaceX != 0 ? 0 : sizeCellX/2;
        }
    }
}

void GameWidget::drawRelief()
{
//    qDebug() << "GameWidget::drawRelief();";
    int fieldX = field.getSizeX();
    int fieldY = field.getSizeY();

    int mainCoorMapX = field.getMainCoorMapX();
    int mainCoorMapY = field.getMainCoorMapY();
    int spaceWidget = field.getSpaceWidget();
    int sizeCell = field.getSizeCell();

    int isometricCoorX = (sizeCell/2) * fieldY;
    int isometricCoorY = 0;

//    int sizeCellX = field.getTileMapWidth()/2;
//    int sizeCellY = field.getTileMapHeight()/2;
    int sizeCellX = field.getSizeCell();
    int sizeCellY = sizeCellX/2;

    for(int y = 0; y < fieldY; y++)
    {
        for(int x = 0; x < fieldX; x++)
        {
            if(field.getCell(x, y)->isTerrain())
            {
//            qDebug() << "GameWidget::drawRelief(); -- x: (" << x << "/" << fieldX << ") -- y: (" << y << "/" << fieldY << ")";
//            if(mapLoad) {
                    QPixmap pix = field.getCell(x, y)->busyPixmap;//getBusyPixmapOfCell(x, y);
                    if(!field.getIsometric()) {
//                        qDebug() << "GameWidget::drawRelief(); -- NO Isometric!";
                        int pxlsX = mainCoorMapX + spaceWidget + x*sizeCell;//+1;
                        int pxlsY = mainCoorMapY + spaceWidget + y*sizeCell;//+1;
                        int localSizeCell = sizeCell;//-1;

//                        if(!mapLoad) {
                            p.fillRect(pxlsX+1, pxlsY+1, localSizeCell-1, localSizeCell-1, QColor(0, 0, 0));
//                        } else {
                            p.drawPixmap(pxlsX, pxlsY, localSizeCell, localSizeCell, pix);
//                        }
                    } else {
//                        qDebug() << "GameWidget::drawRelief(); -- Isometric!";
                        int x1 = mainCoorMapX + isometricCoorX - (sizeCellX/2) + x*(sizeCellX/2);
                        int y1 = mainCoorMapY + isometricCoorY - (sizeCellY) + x*(sizeCellY/2);
//                    qDebug() << "GameWidget::drawRelief(); -- x: " << x << " y: " << y << " pix: " << !pix.isNull();
//                    qDebug() << "GameWidget::drawRelief(); -- x1: " << x1;
//                    qDebug() << "GameWidget::drawRelief(); -- y1: " << y1;

                        p.drawPixmap(x1, y1, sizeCellX, sizeCellY*2, pix);
                    }
//            }
            }
        }
        isometricCoorX = (field.getSizeCell()/2) * (fieldY - (y+1));
        isometricCoorY = (field.getSizeCell()/4) * (y+1);
    }
}

void GameWidget::drawTowersByField()
{
    int fieldX = field.getSizeX();
    int fieldY = field.getSizeY();

    for(int y = 0; y < fieldY; y++)
    {
        for(int x = 0; x < fieldX; x++)
        {
            int mainCoorMapX = field.getMainCoorMapX();
            int mainCoorMapY = field.getMainCoorMapY();
            int spaceWidget = field.getSpaceWidget();
            int sizeCell = field.getSizeCell();

            int pxlsX = mainCoorMapX + spaceWidget + x*sizeCell;//+1;
            int pxlsY = mainCoorMapY + spaceWidget + y*sizeCell;// - sizeCell;//+1;
            int localSizeCell = sizeCell*2;//-1; // NOT GOOD WORK!!!!!!!!!!

            if(field.containTower(x, y))
            {
                if(!mapLoad)
                    p.fillRect(pxlsX+1, pxlsY+1, localSizeCell-1, localSizeCell-1, QColor(127, 255, 0));
                else
                    p.drawPixmap(pxlsX, pxlsY, localSizeCell/* + sizeCell*/, localSizeCell/* + sizeCell*/, field.towersManager.getTowerPixmap(x, y));//->getTowerPixmap(x, y));
            }
        }
    }
}

void GameWidget::drawTowersByTowers()
{
//    qDebug() << "GameWidget::drawTowersByTowers();";
    int mainCoorMapX = field.getMainCoorMapX();
    int mainCoorMapY = field.getMainCoorMapY();
    int spaceWidget = field.getSpaceWidget();
    int sizeCell = field.getSizeCell();

    std::vector<Tower*> towers = field.getAllTowers();

    int size = towers.size();
    for(int k = 0; k < size; k++)
    {
        int x = towers[k]->currX;
        int y = towers[k]->currY;
        int size = towers[k]->defTower->size;

        if(!field.getIsometric()) {
//            qDebug() << "GameWidget::drawTowersByTowers(); -- NO Isometric!";

            int pxlsX = mainCoorMapX + spaceWidget + x*sizeCell;
            int pxlsY = mainCoorMapY + spaceWidget + y*sizeCell;
            int localSizeCell = sizeCell*size;

            if(!mapLoad)
                p.fillRect(pxlsX+1, pxlsY+1, localSizeCell-1, localSizeCell-1, QColor(127, 255, 0));
            else
                p.drawPixmap(pxlsX, pxlsY, localSizeCell/* + sizeCell*/, localSizeCell/* + sizeCell*/, towers[k]->pixmap);

    //        int attackX = towers[k]->attackX;
    //        int attackY = towers[k]->attackY;
    //        if(attackX != -1 && attackY != -1)
    //        {
    //            attackX = mainCoorMapX + spaceWidget + attackX*sizeCell;
    //            attackY = mainCoorMapY + spaceWidget + attackY*sizeCell;
    //            p.drawLine(pxlsX+localSizeCell/2, pxlsY+localSizeCell/2, attackX, attackY);
    //        }

            for(int iBullet = 0; iBullet < towers[k]->bullets.size(); iBullet++) {
    //            int bulletX = mainCoorMapX + spaceWidget + towers[k]->bullets[iBullet].getCurrX()*sizeCell;
    //            int bulletY = mainCoorMapY + spaceWidget + towers[k]->bullets[iBullet].getCurrY()*sizeCell;
                int bulletX = towers[k]->bullets[iBullet].getCurrX() - mainCoorMapX;
                int bulletY = towers[k]->bullets[iBullet].getCurrY() - mainCoorMapY;

                p.drawPixmap(bulletX, bulletY, sizeCell, sizeCell, towers[k]->bullets[iBullet].getPixmap());
            }

    //        QPixmap bullet_fly_up = towers[k]->defTower->bullet_fly_up;
    //        QPixmap bullet_fly_up_right = towers[k]->defTower->bullet_fly_up_right;
    //        QPixmap bullet_fly_right = towers[k]->defTower->bullet_fly_right;
    //        QPixmap bullet_fly_down_right = towers[k]->defTower->bullet_fly_down_right;
    //        QPixmap bullet_fly_down = towers[k]->defTower->bullet_fly_down;
    //        QPixmap bullet_fly_down_left = towers[k]->defTower->bullet_fly_down_left;
    //        QPixmap bullet_fly_left = towers[k]->defTower->bullet_fly_left;
    //        QPixmap bullet_fly_up_left = towers[k]->defTower->bullet_fly_up_left;

    //        p.drawPixmap(pxlsX, pxlsY - localSizeCell, localSizeCell, localSizeCell, bullet_fly_up);
    //        p.drawPixmap(pxlsX + localSizeCell, pxlsY - localSizeCell, localSizeCell, localSizeCell, bullet_fly_up_right);
    //        p.drawPixmap(pxlsX + localSizeCell, pxlsY, localSizeCell, localSizeCell, bullet_fly_right);
    //        p.drawPixmap(pxlsX + localSizeCell, pxlsY + localSizeCell, localSizeCell, localSizeCell, bullet_fly_down_right);
    //        p.drawPixmap(pxlsX, pxlsY + localSizeCell, localSizeCell, localSizeCell, bullet_fly_down);
    //        p.drawPixmap(pxlsX - localSizeCell, pxlsY + localSizeCell, localSizeCell, localSizeCell, bullet_fly_down_left);
    //        p.drawPixmap(pxlsX - localSizeCell, pxlsY, localSizeCell, localSizeCell, bullet_fly_left);
    //        p.drawPixmap(pxlsX - localSizeCell, pxlsY - localSizeCell, localSizeCell, localSizeCell, bullet_fly_up_left);
        } else {
//            qDebug() << "GameWidget::drawTowersByTowers(); -- Isometric!";

            int sizeCellX = sizeCell;
            int sizeCellY = sizeCellX/2;
            int height = towers[k]->defTower->height;

            int isometricSpaceX = (field.getSizeY()-y)*(sizeCellX/2);
            int isometricSpaceY = y*(sizeCellY/2);

            int pxlsX = mainCoorMapX + isometricSpaceX+spaceWidget + x*(sizeCellX/2);
            int pxlsY = mainCoorMapY + isometricSpaceY+spaceWidget + x*(sizeCellY/2);

            if(!mapLoad) {
//                p.fillRect(pxlsX+1, pxlsY+1, localSizeCell-1, localSizeCell-1, QColor(127, 255, 0));
            } else {
//                sizeCellX = sizeCellX*2;
//                sizeCellY = sizeCellY*2;
                p.drawPixmap(pxlsX - sizeCellX/2, pxlsY + sizeCellY - (sizeCellY*2)*height, sizeCellX, (sizeCellY*2)*height, towers[k]->pixmap);
            }
        }
    }
}

void GameWidget::drawUnits()
{
    int fieldX = field.getSizeX();
    int fieldY = field.getSizeY();

    int mainCoorMapX = field.getMainCoorMapX();
    int mainCoorMapY = field.getMainCoorMapY();
    int spaceWidget = field.getSpaceWidget();
    int sizeCell = field.getSizeCell();

    int isometricCoorX = (sizeCell/2) * fieldY;
    int isometricCoorY = 0;

//    int sizeCellX = field.getTileMapWidth()/2;
//    int sizeCellY = field.getTileMapHeight()/2;
    int sizeCellX = field.getSizeCell();
    int sizeCellY = sizeCellX/2;

    for(int y = 0; y < fieldY; y++) {
        for(int x = 0; x < fieldX; x++) {
            int num = field.containUnit(x, y);
            if(num) {
                std::vector<Unit*> units = field.getCell(x, y)->getUnits();
                int size = units.size();
                for(int k = 0; k < size; k++) {
                    if(units[k]->alive || units[k]->preDeath) {// fixed!!!
                        int lastX, lastY;
                        int animationCurrIter, animationMaxIter;
                        QPixmap pixmap = units[k]->getAnimationInformation(&lastX, &lastY, &animationCurrIter, &animationMaxIter);
                        if(!field.getIsometric()) {
//                            qDebug() << "GameWidget::drawUnits(); -- Not Isometric!";

                            int pxlsX = mainCoorMapX + spaceWidget + x*sizeCell;//+1;
                            int pxlsY = mainCoorMapY + spaceWidget + y*sizeCell;// - sizeCell/2;//+1;
                            int localSizeCell = sizeCell;//-1;
                            int localSpaceCell = sizeCell/3;

//                            QColor color = QColor(num*10, num*10, num*10);
//                            p.fillRect(pxlsX+1 + localSpaceCell, pxlsY+1 + localSpaceCell, localSizeCell-1 - 2*(localSpaceCell), localSizeCell-1 - 2*(localSpaceCell), color);

                            pxlsX = mainCoorMapX + spaceWidget + x*sizeCell - localSpaceCell;
                            pxlsY = mainCoorMapY + spaceWidget + y*sizeCell - localSpaceCell;

                            if(lastX < x)
                                pxlsX -= (sizeCell/animationMaxIter)*(animationMaxIter-animationCurrIter);
                            if(lastX > x)
                                pxlsX += (sizeCell/animationMaxIter)*(animationMaxIter-animationCurrIter);
                            if(lastY < y)
                                pxlsY -= (sizeCell/animationMaxIter)*(animationMaxIter-animationCurrIter);
                            if(lastY > y)
                                pxlsY += (sizeCell/animationMaxIter)*(animationMaxIter-animationCurrIter);

//                            qDebug() << "GameWidget::drawUnits(); -- Pix: " << !pixmap.isNull();
                            p.drawPixmap(pxlsX, pxlsY, localSizeCell + localSpaceCell*2, localSizeCell + localSpaceCell*2, pixmap);
//                            p.drawRect(pxlsX, pxlsY, localSizeCell + localSpaceCell*2, localSizeCell + localSpaceCell*2);

                            int maxHP = 100;
                            int hp = units[k]->hp;
                            float hpWidth = localSizeCell-5;
                            hpWidth = hpWidth/maxHP*hp;
//                            qDebug() << "hpWidth: " << hpWidth;

                            p.drawRect(pxlsX + localSpaceCell+2, pxlsY, localSizeCell-4, 10);
                            p.fillRect(pxlsX + localSpaceCell+3, pxlsY+1, hpWidth, 9, QColor(Qt::green));

                            // IT's NOT GOOD!!! Fixed!
                            units[k]->coorByMapX = pxlsX;
                            units[k]->coorByMapY = pxlsY;
                            // -----------------------
                        } else {
//                            qDebug() << "GameWidget::drawUnits(); -- Isometric!";

                            int mainX = mainCoorMapX + isometricCoorX + x*(sizeCellX/2);
                            int mainY = mainCoorMapY + isometricCoorY + x*(sizeCellY/2);

                            int pxlsX = mainX - sizeCellX/2;
                            int pxlsY = mainY - sizeCellY;
                            if(x > lastX && y > lastY) {
                                pxlsY -= (sizeCellY/animationMaxIter)*(animationMaxIter-animationCurrIter);
                            } else if(x == lastX && y > lastY) {
                                pxlsX += (sizeCellX/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
                                pxlsY -= (sizeCellY/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
                            } else if (x < lastX && y > lastY) {
                                pxlsX += (sizeCellX/animationMaxIter)*(animationMaxIter-animationCurrIter);
                            } else if (x < lastX && y == lastY) {
                                pxlsX += (sizeCellX/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
                                pxlsY += (sizeCellY/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
                            } else if (x < lastX && y < lastY) {
                                pxlsY += (sizeCellY/animationMaxIter)*(animationMaxIter-animationCurrIter);
                            } else if (x == lastX && y < lastY) {
                                pxlsX -= (sizeCellX/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
                                pxlsY += (sizeCellY/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
                            } else if (x > lastX && y < lastY) {
                                pxlsX -= (sizeCellX/animationMaxIter)*(animationMaxIter-animationCurrIter);
                            } else if (x > lastX && y == lastY) {
                                pxlsX -= (sizeCellX/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
                                pxlsY -= (sizeCellY/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
                            }

//                            QPointF points[4] = {
//                                QPointF(mainX, mainY + sizeCellY/4),
//                                QPointF(mainX + sizeCellX/2 - sizeCellX/4, mainY + sizeCellY/2),
//                                QPointF(mainX, mainY + sizeCellY - sizeCellY/4),
//                                QPointF(mainX - sizeCellX/2 + sizeCellX/4, mainY + sizeCellY/2)
//                            };
//                            p.drawPolygon(points, 4);

                            p.drawPixmap(pxlsX, pxlsY+sizeCellY/2, sizeCellX, sizeCellY*2, pixmap);
                        }
                    }
                }
            }
        }
        isometricCoorX = (field.getSizeCell()/2) * (fieldY - (y+1));
        isometricCoorY = (field.getSizeCell()/4) * (y+1);
    }
}

void GameWidget::drawStepsAndMouse() {
//    qDebug() << "GameWidget::drawStepsAndMouse();";
    int fieldX = field.getSizeX();
    int fieldY = field.getSizeY();
    int mainCoorMapX = field.getMainCoorMapX();
    int mainCoorMapY = field.getMainCoorMapY();
    int spaceWidget = field.getSpaceWidget();
    int sizeCell = field.getSizeCell();
    int isometricCoorX = (sizeCell/2) * fieldY;
    int isometricCoorY = 0;

//    int sizeCellX = field.getTileMapWidth()/2;
//    int sizeCellY = field.getTileMapHeight()/2;
    int sizeCellX = field.getSizeCell();
    int sizeCellY = sizeCellX/2;

    p.setPen(QColor(255,0,0));
    for (int y = 0; y < fieldY; y++) {
        for (int x = 0; x < fieldX; x++) {
            if (!field.getIsometric()) {
//                qDebug() << "GameWidget::drawStepsAndMouse(); -- NOT Isometric!";
                int pxlsX = mainCoorMapX + spaceWidget + x*sizeCell+1;
                int pxlsY = mainCoorMapY + spaceWidget + y*sizeCell+1;
                int localSizeCell = sizeCell-1;
                int localSpaceCell = sizeCell/4;

                p.drawPixmap(sizeCell, 0, global_pixmap.width(), global_pixmap.height(), global_pixmap);

                if(field.getStepCell(x, y))
                    p.drawText(pxlsX + sizeCell/2-5, pxlsY + sizeCell/2+5, QString("%1").arg(field.getStepCell(x, y)));

                if(field.isSetSpawnPoint(x,y))
                    p.fillRect(pxlsX + localSpaceCell, pxlsY + localSpaceCell, localSizeCell - 2*(localSpaceCell), localSizeCell - 2*(localSpaceCell), QColor(255, 162, 0));

                if(field.isSetExitPoint(x,y))
                    p.fillRect(pxlsX + localSpaceCell, pxlsY + localSpaceCell, localSizeCell - 2*(localSpaceCell), localSizeCell - 2*(localSpaceCell), QColor(0, 255, 0));
            } else {
//                qDebug() << "GameWidget::drawStepsAndMouse(); -- Isometric!";
//                p.drawPixmap(sizeCell, 0, global_pixmap.width(), global_pixmap.height(), global_pixmap);

                int x2 = mainCoorMapX + isometricCoorX - (sizeCellX/2) + x*(sizeCellX/2);
                int y2 = mainCoorMapY + isometricCoorY - (sizeCellY) + x*(sizeCellY/2);
                if (field.pathFinder.detectCollision({x, y})) {
                    p.drawPixmap(x2, y2, sizeCellX, sizeCellY*2, global_pixmap);
//                    p.drawEllipse(x1-sizeCell/4+7, y1-sizeCell/4+7, sizeCell/4, sizeCell/4);
//                    p.fillRect(x1, pxlsY + localSpaceCell, localSizeCell - 2*(localSpaceCell), localSizeCell - 2*(localSpaceCell), QColor(255, 162, 0));
                } else {
//                    p.fillRect(pxlsX + localSpaceCell, pxlsY + localSpaceCell, localSizeCell - 2*(localSpaceCell), localSizeCell - 2*(localSpaceCell), QColor(0, 255, 0));
                }
//                int x1 = mainCoorMapX + isometricCoorX + x*(sizeCellX/2);
//                int y1 = mainCoorMapY + isometricCoorY + (sizeCellY/2) + x*(sizeCellY/2);
//                if(field.getStepCell(x, y)) {
//                    p.drawText(x1 - 5, y1 + 5, QString("%1").arg(field.getStepCell(x, y)));
//                }
            }
        }
        isometricCoorX = (field.getSizeCell()/2) * (fieldY - (y+1));
        isometricCoorY = (field.getSizeCell()/4) * (y+1);
    }

//    p.setPen(QColor(255,255,255));
//    p.setBrush(Qt::red);
//    AStar::Vec2i heroPoint = heroPath.back();
//    isometricCoorX = (field.getSizeCell()/2) * (fieldY - (heroPoint.y+0));
//    isometricCoorY = (field.getSizeCell()/4) * (heroPoint.y+0);
//    int x3 = mainCoorMapX + isometricCoorX + heroPoint.x*(sizeCellX/2);
//    int y3 = mainCoorMapY + isometricCoorY + (sizeCellY/2) + heroPoint.x*(sizeCellY/2);
//    int x4 = mainCoorMapX + isometricCoorX - (sizeCellX/2) + heroPoint.x*(sizeCellX/2);
//    int y4 = mainCoorMapY + isometricCoorY - (sizeCellY) + heroPoint.x*(sizeCellY/2);
//    p.drawPixmap(x4, y4, sizeCellX, sizeCellY*2, global_pixmap_HeroPoint);

    for (int u = 0; u < field.unitsManager.getAmount(); u++) {
        Unit* tmpUnit = field.unitsManager.getUnit(u);
        if (tmpUnit->type == 0) {
            AStar::CoordinateList heroPath = tmpUnit->path;
            if (!heroPath.empty()) {
                for (int k = 1; k < heroPath.size(); k++) {
                    AStar::Vec2i point = heroPath[k];
                    isometricCoorX = (field.getSizeCell()/2) * (fieldY - (point.y+0));
                    isometricCoorY = (field.getSizeCell()/4) * (point.y+0);
                    int pPx = mainCoorMapX + isometricCoorX - (sizeCellX/2) + point.x*(sizeCellX/2);
                    int pPy = mainCoorMapY + isometricCoorY - (sizeCellY) + point.x*(sizeCellY/2);
                    p.drawPixmap(pPx, pPy, sizeCellX, sizeCellY*2, global_pixmap_PathPoint);
                }
                AStar::Vec2i destinationPoint = heroPath.front();
                isometricCoorX = (field.getSizeCell()/2) * (fieldY - (destinationPoint.y+0));
                isometricCoorY = (field.getSizeCell()/4) * (destinationPoint.y+0);
                int destinationPointX = mainCoorMapX + isometricCoorX - (sizeCellX/2) + destinationPoint.x*(sizeCellX/2);
                int destinationPointY = mainCoorMapY + isometricCoorY - (sizeCellY) + destinationPoint.x*(sizeCellY/2);
                p.drawPixmap(destinationPointX, destinationPointY, sizeCellX, sizeCellY*2, global_pixmap_DestinationPoint);
            }
        }
    }
}

void GameWidget::drawTowersUnderConstruction()
{
    if(underConstruction)
    {
        if(underConstruction->state == 0) {
            drawTowerUnderConstruction(underConstruction->endX, underConstruction->endY, underConstruction->tower);
        } else if(underConstruction->state == 1) {
            drawTowerUnderConstruction(underConstruction->startX, underConstruction->startY, underConstruction->tower);

            for(int k = 0; k < underConstruction->coorsX.size(); k++) {
                drawTowerUnderConstruction(underConstruction->coorsX[k], underConstruction->coorsY[k], underConstruction->tower);
            }
        }
//        if(whichCell(bEndX, bEndY)) {
//            qDebug() << "GameWidget::drawTowerUnderConstruction() -- whichCell";
//            if(bStartX == bEndX) {
//                if(bStartY > bEndY) {
//                    for(int bCurrY = bStartX; bCurrY >= bEndY-towerSize; bCurrY-=towerSize) {
//                        drawTowerUnderConstruction(bStartX, bCurrY, underConstruction->tower);
//                    }
//                } else if(bStartY < bEndY) {
//                    for(int bCurrY = bStartX; bCurrY <= bEndY-towerSize; bCurrY+=towerSize) {
//                        drawTowerUnderConstruction(bStartX, bCurrY, underConstruction->tower);
//                    }
//                }
//            } else if(bStartY == bEndY) {
//                if(bStartX > bEndX) {
//                    for(int bCurrX = bStartX; bCurrX >= bEndY-towerSize; bCurrX-=towerSize) {
//                        drawTowerUnderConstruction(bCurrX, bStartY, underConstruction->tower);
//                    }
//                } else if(bStartX < bEndX) {
//                    for(int bCurrX = bStartX; bCurrX <= bEndY-towerSize; bCurrX+=towerSize) {
//                        drawTowerUnderConstruction(bCurrX, bStartY, underConstruction->tower);
//                    }
//                }
//            }
//        }
    }
}

void GameWidget::drawTowerUnderConstruction(int buildX, int buildY, DefaultTower *tower) {
    qDebug() << "GameWidget::drawTowerUnderConstruction(" << buildX << "," << buildY << "," << tower << ");";
    int mainCoorMapX = field.getMainCoorMapX();
    int mainCoorMapY = field.getMainCoorMapY();
    int spaceWidget = field.getSpaceWidget();
    int sizeCell = field.getSizeCell();

    QPixmap towerPix = tower->pixmap;
    int towerSize = tower->size;
    int pixSizeCell = towerPix.width() / towerSize;

    QColor cGreen(0, 255, 0, 80);
    QColor cRed(255, 0, 0, 80);

/*    vector<QPixmap> pixmaps;
    qDebug() << "towerPix: " << towerPix;
    qDebug() << "towerSize: " << towerSize;
    qDebug() << "pixSizeCell: " << pixSizeCell;
    int columns = towerPix.width() / towerSize;
    int rows = towerPix.height() / towerSize;*/

    for(int x = 0; x < towerSize; x++)
    {
        for(int y = 0; y < towerSize; y++)
        {
            QPixmap pix = towerPix.copy(x*pixSizeCell, y*pixSizeCell, pixSizeCell, pixSizeCell);

            if(!field.getIsometric()) {
                qDebug() << "GameWidget::drawTowerUnderConstruction(); -- NOT Isometric!";
                int pxlsX = mainCoorMapX + spaceWidget + (buildX+x)*sizeCell;//+1;
                int pxlsY = mainCoorMapY + spaceWidget + (buildY+y)*sizeCell;//+1;

                p.drawPixmap(pxlsX, pxlsY, sizeCell, sizeCell, pix);

                if(field.getCell(buildX+x, buildY+y)->isEmpty()) {
                    p.fillRect(pxlsX, pxlsY, sizeCell, sizeCell, cGreen);
                } else {
                    p.fillRect(pxlsX, pxlsY, sizeCell, sizeCell, cRed);
                }
            } else {
                qDebug() << "GameWidget::drawTowerUnderConstruction(); -- Isometric!";

                int sizeCellX = sizeCell;
                int sizeCellY = sizeCellX/2;
                int height = tower->height;

                int isometricSpaceX = (field.getSizeY()-(buildY+y))*(sizeCellX/2);
                int isometricSpaceY = (buildY+y)*(sizeCellY/2);

                int pxlsX = mainCoorMapX + isometricSpaceX+spaceWidget + (buildX+x)*(sizeCellX/2);
                int pxlsY = mainCoorMapY + isometricSpaceY+spaceWidget + (buildX+x)*(sizeCellY/2);

//                sizeCellX = sizeCellX*2;
//                sizeCellY = sizeCellY*2;

                QPixmap pix = tower->pixmap;

                if(field.getCell(buildX+x, buildY+y)->isEmpty()) {
                    p.setOpacity(0.5);
                    p.drawPixmap(pxlsX - sizeCellX/2, pxlsY + sizeCellY - (sizeCellY*2)*height, sizeCellX, (sizeCellY*2)*height, pix);
//                    p.setOpacity(1);
                } else {
                    p.setOpacity(0.5);
                    p.drawPixmap(pxlsX - sizeCellX/2, pxlsY + sizeCellY - (sizeCellY*2)*height, sizeCellX, (sizeCellY*2)*height, pix);

                    QPainter painter(&pix);
                    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
                    painter.fillRect(pix.rect(), cRed);
                    painter.end();
                    p.drawPixmap(pxlsX - sizeCellX/2, pxlsY + sizeCellY - (sizeCellY*2)*height, sizeCellX, (sizeCellY*2)*height, pix);
                }
            }
        }
    }
}

bool GameWidget::whichCell(int &mouseX, int &mouseY)
{
//    qDebug() << "GameWidget::whichCell(1); -- Isometric!";
//    qDebug() << "GameWidget::whichCell(1); - mouseX:" << mouseX << ", mouseY:" << mouseY;
//    qDebug() << "GameWidget::whichCell(0); - sizeCellX:" << field.getSizeCell() << ", sizeCellY:" << field.getSizeCell()/2;
    int mainCoorMapX = field.getMainCoorMapX();
    int mainCoorMapY = field.getMainCoorMapY();
    int spaceWidget = field.getSpaceWidget();
    int sizeCell = field.getSizeCell();

    int gameX, gameY;
    if(!field.getIsometric()) {
        gameX = ( (mouseX+sizeCell - spaceWidget - mainCoorMapX) / sizeCell);
        gameY = ( (mouseY+sizeCell - spaceWidget - mainCoorMapY) / sizeCell);
//        qDebug() << "GameWidget::whichCell(0); - gameX:" << gameX << ", gameY:" << gameY;
    } else {
//        qDebug() << "GameWidget::whichCell(1); -- Isometric!";
        int fieldX = field.getSizeX();
        int fieldY = field.getSizeY();
        int sizeCellX = field.getSizeCell();
        int sizeCellY = sizeCellX/2;

        int isometricCoorX = (sizeCell/2) * fieldY;
        int isometricCoorY = 0;

//        qDebug() << "GameWidget::whichCell(2); - isometricCoorX:" << isometricCoorX << ", isometricCoorY:" << isometricCoorY;

        int localMouseX = -mainCoorMapX + mouseX - isometricCoorX;
        int localMouseY = -mainCoorMapY + mouseY + sizeCellY;
//        qDebug() << "GameWidget::whichCell(3); - localMouseX:" << localMouseX << ", localMouseY:" << localMouseY;

        gameX = (localMouseX/2 + localMouseY) / (sizeCell/2);
        gameY = -(localMouseX/2 - localMouseY) / (sizeCell/2);
//        qDebug() << "GameWidget::whichCell(4); - gameX:" << gameX << ", gameY:" << gameY;

//        QString text = QString("%1/%2").arg(gameX).arg(gameY);
//        global_text2 = text.toStdString().c_str();
    }

    if(gameX > 0 && gameX < field.getSizeX()+1)
        if(gameY > 0 && gameY < field.getSizeY()+1)
        {
            mouseX = gameX-1;
            mouseY = gameY-1;
//            qDebug() << "GameWidget::whichCell(5); - mouseX:" << mouseX << ", mouseY:" << mouseY;
            return true;
        }

    return false;
}

void GameWidget::startTimer_UnitsMoveAndTowerAttack()
{
    qDebug() << "GameWidget::startTimer_UnitsMoveAndTowerAttack();";
    if(field.isSetSpawnPoint())
    {
        if(unitsMove_TimerId)
            killTimer(unitsMove_TimerId);

        unitsMove_TimerId = startTimer(unitsMove_TimerMilliSec);
    }
}

void GameWidget::stopTimer_UnitsMoveAndTowerAttack()
{
    qDebug() << "stopTimer_UnitsMoveAndTowerAttack()";
    if(unitsMove_TimerId)
        killTimer(unitsMove_TimerId);

    unitsMove_TimerId = 0;
}

void GameWidget::buildTower(int x, int y)
{
    if(x == -1 && y == -1)
    {
        qDebug() << "buildTower1(" << x << "," << y << ");";

        vector<DefaultTower*> towers = faction.getFirstTowers();
        int size = towers.size();
//        qDebug() << "towers.size(): " << size;

        QMessageBox msgBox;
        msgBox.setText("Какую башню ты хочешь построить?");

        for(int k = 0; k < size; k++)
        {
            QPushButton* b1 = new QPushButton(QIcon(towers[k]->pixmap), QString());
            msgBox.addButton(b1, QMessageBox::AcceptRole);
        }
//        qDebug() << "buildTower(-1, -1) level 2";

        int ret = msgBox.exec();
//        qDebug() << "ret: " << ret;

        if(underConstruction)
            delete underConstruction;
        underConstruction = new UnderConstruction(towers[ret]);

//        int currX = cursor().pos().x();
//        int currY = cursor().pos().y();
//        if(whichCell(currX, currY)) {
//            underConstruction = new UnderConstruction(currX, currY, towers[ret]);
//        }

//        qDebug() << "buildTower(-1, -1) level 3";
//        field.setTower(mouseX, mouseY, towers[ret]);
    }
    else
    {
        qDebug() << "buildTower2(" << x << "," << y << ");";
        if(underConstruction) {
            underConstruction->setStartCoors(x, y);
        }
//        if(underConstruction)
//            delete underConstruction;

//        underConstruction = new UnderConstruction();
//        underConstruction->startX = x;
//        underConstruction->startY = y;
//        if(towerUnderConstruction == NULL)
//        {
//            qDebug() << "buildTower2(" << x << "," << y << ");";

//            vector<DefaultTower*> towers = faction.getFirstTowers();
//            int size = towers.size();
////            qDebug() << "towers.size(): " << size;

//            QMessageBox msgBox;
//            msgBox.setText("Какую башню ты хочешь построить?");

//            for(int k = 0; k < size; k++)
//            {
//                QPushButton* b1 = new QPushButton(QIcon(towers[k]->pixmap), QString());
//                msgBox.addButton(b1, QMessageBox::AcceptRole);
//            }
////            qDebug() << "buildTower(" << x << "," << y << "); level2";

//            int ret = msgBox.exec();
////            qDebug() << "ret: " << ret;

//            field.setTower(x, y, towers[ret]);
////            qDebug() << "buildTower(" << x << "," << y << "); level3";
//        }
//        else
//        {
//            if(field.setTower(x, y, towerUnderConstruction))
//                towerUnderConstruction = NULL;
//        }
    }
}

void GameWidget::mousePressEvent(QMouseEvent* event) {
    Qt::MouseButton button = event->button();
    int mouseX = event->x();
    int mouseY = event->y();
    QString text = QString("Press(%1/%2) -- %3").arg(mouseX).arg(mouseY).arg(button);
    global_text = text.toStdString().c_str();
    qDebug() << "GameWidget::mousePressEvent(); -- " << text;
    if(whichCell(mouseX,mouseY)) {
        text = QString("Press(%1/%2) -- %3").arg(mouseX).arg(mouseY).arg(button);
        global_text2 = text.toStdString().c_str();
        if(event->button() == Qt::LeftButton) {
            pan = true;
            prevMouseX = event->x();
            prevMouseY = event->y();
            prevMouseCellX = mouseX;
            prevMouseCellY = mouseY;
            prevGlobalMouseX = event->globalX();
            prevGlobalMouseY = event->globalY();
            setCursor(Qt::ClosedHandCursor);
        } else if(event->button() == Qt::RightButton) {
        }
    }
    update();
}

void GameWidget::mouseReleaseEvent(QMouseEvent* event) {
    Qt::MouseButton button = event->button();
    int mouseX = event->x();
    int mouseY = event->y();
    QString text = QString("Release(%1/%2) -- %3").arg(mouseX).arg(mouseY).arg(button);
    global_text = text.toStdString().c_str();
    qDebug() << "GameWidget::mouseReleaseEvent(); -- " << text;
    if(whichCell(mouseX, mouseY)) {
        text = QString("Release(%1/%2)-(%3/%4) -- %3").arg(mouseX).arg(mouseY).arg(prevMouseCellX).arg(prevMouseCellY).arg(button);
        global_text2 = text.toStdString().c_str();
        qDebug() << "GameWidget::mouseReleaseEvent(); -- " << text;
        if(button == Qt::LeftButton) {
            pan = false;
            setCursor(Qt::ArrowCursor);
            if ( (prevMouseCellX == mouseX && prevMouseCellY == mouseY) ) {
                if ( (prevGlobalMouseX == event->globalX() && prevGlobalMouseY == event->globalY()) ) {
                    field.updateHeroDestinationPoint(mouseX, mouseY);
                    if(field.isSetSpawnPoint()) {
                        startTimer_UnitsMoveAndTowerAttack();
                    }
                }
            }
//            event->accept();
//            if(field.containEmpty(mouseX, mouseY)) {
//                buildTower(mouseX, mouseY);
//            } else if(field.containTower(mouseX, mouseY)) {
//                field.deleteTower(mouseX, mouseY);
//            }
//            if(!field.containBusy(mouseX, mouseY))
//                field.waveAlgorithm();
        } else if (button == Qt::RightButton) {
            QPixmap pixmap;
            if(!field.getCell(mouseX, mouseY)->isTerrain()) {
                field.getCell(mouseX, mouseY)->setTerrain(pixmap);
            } else {
                field.getCell(mouseX,mouseY)->removeTerrain();
            }
            field.updateHeroDestinationPoint();
//            field.updatePathFinderWalls();
        } else if(event->button() == Qt::XButton1) {
            field.updateHeroDestinationPoint(mouseX,mouseY);
            if(field.isSetSpawnPoint()) {
                startTimer_UnitsMoveAndTowerAttack();
            }
//            if(waveAlgorithm(mouseX, mouseY) == 1)
//                global_text2 = "Yes!";
//            else
//                global_text2 = "No!";
//            test = 1;
//            field.setUnit(mouseX, mouseY);
        } else if(event->button() == Qt::XButton2) {
            field.setMousePress(mouseX,mouseY);
            field.createSpawnPoint(defaultNumCreateUnits, mouseX, mouseY);
            startTimer_UnitsMoveAndTowerAttack();
        } else if(event->button() == Qt::MidButton) {
            field.createUnit(mouseX, mouseY);
        }
    }
//    qDebug() << "GameWidget::mouseReleaseEvent() -- underConstruction: " << underConstruction;
//    if(underConstruction) {
//        field.setTower(underConstruction->startX, underConstruction->startY, underConstruction->tower);
//        for(int k = 0; k < underConstruction->coorsX.size(); k++) {
//            field.setTower(underConstruction->coorsX[k], underConstruction->coorsY[k], underConstruction->tower);
//        }
//        underConstruction->clearStartCoors();
//        field.waveAlgorithm();
//    }
    update();
}

void GameWidget::mouseMoveEvent(QMouseEvent * event) {
//    static int maxX = 0;
//    static int maxY = 0;

//    qDebug() << "GameWidget::mouseMoveEvent(); -- event->MouseButtonPress:" << event->MouseButtonPress;
    if (pan) {
//    if (event->MouseButtonPress == QEvent::MouseButtonPress) {
        int mainCoorMapX = field.getMainCoorMapX();
        int mainCoorMapY = field.getMainCoorMapY();
//        qDebug() << "GameWidget::mouseMoveEvent(); -- mainCoorMapX: " << mainCoorMapX;
//        qDebug() << "GameWidget::mouseMoveEvent(); -- mainCoorMapY: " << mainCoorMapY;
        int x = event->x();
        int y = event->y();
//        qDebug() << "GameWidget::mouseMoveEvent(); -- x: " << x;
//        qDebug() << "GameWidget::mouseMoveEvent(); -- y: " << y;
//        qDebug() << "GameWidget::mouseMoveEvent(); -- prevMouseX: " << prevMouseX;
//        qDebug() << "GameWidget::mouseMoveEvent(); -- prevMouseY: " << prevMouseY;
//        qDebug() << "GameWidget::mouseMoveEvent(); -- (x-prevMouseX): " << (x-prevMouseX);
//        qDebug() << "GameWidget::mouseMoveEvent(); -- (y-prevMouseY): " << (y-prevMouseY);
        mainCoorMapX = mainCoorMapX + ((x-prevMouseX)/1);
        mainCoorMapY = mainCoorMapY + ((y-prevMouseY)/1);
        field.setMainCoorMap(mainCoorMapX, mainCoorMapY);
//        qDebug() << "GameWidget::mouseMoveEvent(); -- mainCoorMapX: " << mainCoorMapX;
//        qDebug() << "GameWidget::mouseMoveEvent(); -- mainCoorMapY: " << mainCoorMapY;
//        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->x() - _panStartX));
//        verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->y() - _panStartY));
        prevMouseX = event->x();
        prevMouseY = event->y();
//        event->accept();
        update();
    }
//    event->ignore();
}

void GameWidget::wheelEvent(QWheelEvent* event) {
    int mainCoorMapX = field.getMainCoorMapX();
    int mainCoorMapY = field.getMainCoorMapY();
    int sizeCell = field.getSizeCell();

    QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta() / 8;

//    qDebug() << "x: " << event->x();
//    qDebug() << "y: " << event->y();

    if (!numPixels.isNull())
        qDebug() << "numPixels: " << numPixels;
    else if (!numDegrees.isNull())
    {
        QPoint numSteps = numDegrees / 15;
        qDebug() << "numSteps: " << numSteps; // QPoint(0,1) up : QPoint(0,-1) down

        int wheelX = numSteps.x();
        int wheelY = numSteps.y();

        if(wheelX == 0 && wheelY != 0)
        {
            if(wheelY > 0)
            {
                if(sizeCell < zoomMaxSizeCell)
                    for(int k = 0; k < wheelY; k++)
                    {
                        sizeCell += zoomSizeCell;
                        mainCoorMapX -= (zoomSizeCell*field.getSizeX()/2);
                        mainCoorMapY -= (zoomSizeCell*field.getSizeY()/2);

                        int mouseMapX = event->x();
                        int mouseMapY = event->y();
                        int mapCenterX = width()/2;
                        int mapCenterY = height()/2;
                        mainCoorMapX -= (mouseMapX-mapCenterX);
                        mainCoorMapY -= (mouseMapY-mapCenterY);
                    }
            }
            else
            {
//                if(sizeCell*field.getSizeX()-zoomSizeCell*sizeCell > width() && sizeCell*field.getSizeY()-zoomSizeCell*sizeCell > height())
//                if(sizeCell*field.getSizeX()-zoomSizeCell*sizeCell < width() )
                    if(sizeCell > zoomMinSizeCell)
                    {
                        wheelY = -wheelY;
                        for(int k = 0; k < wheelY; k++)
                        {
                            sizeCell -= zoomSizeCell;
                            mainCoorMapX += (zoomSizeCell*field.getSizeX()/2);
                            mainCoorMapY += (zoomSizeCell*field.getSizeY()/2);
                        }
                    }
            }
            mainCoorMapX = (mainCoorMapX > 0) ? 0 : mainCoorMapX;
            mainCoorMapY = (mainCoorMapY > 0) ? 0 : mainCoorMapY;

            mainCoorMapX = (mainCoorMapX + sizeCell*field.getSizeX() < width()) ? width()-sizeCell*field.getSizeX() : mainCoorMapX;
            mainCoorMapY = (mainCoorMapY + sizeCell*field.getSizeY() < height()) ? height()-sizeCell*field.getSizeY() : mainCoorMapY;

            field.setMainCoorMap(mainCoorMapX, mainCoorMapY);
            field.setSizeCell(sizeCell);
        }
    }
//    event->accept();
}

/*void GameWidget::leaveEvent(QEvent *event)
{
    qDebug() << "leaveEvent()";
}*/


//void GameWidget::keyReleaseEvent(QKeyEvent *event) {
//}

void GameWidget::loadMap(QString mapName)
{
    if(unitsMove_TimerId)
    {
        killTimer(unitsMove_TimerId);
        unitsMove_TimerId = 0;
    }

    if(mapLoad)
        mapLoad = false;

    QFile* file = new QFile(mapName);
    if(!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Can't load map: " << mapName;
        return;
    }

    QXmlStreamReader xmlReader(file);

    //map
    int mapSizeX, mapSizeY;
    int tileMapWidth, tileMapHeight;

    //tileset
    vector<TileSet> tileSets;
    TileSet tileSet;
//    DefaultUnit unit;
//    bool loadUnit = false;

    //layer
    bool layer = false;
    bool ground = false;
    int x = 0;
    int y = 0;

    while(!xmlReader.atEnd() && !xmlReader.hasError())
    {
        if(xmlReader.isStartElement())
        {
            QString nameElement = xmlReader.name().toString();
            //qDebug() << "StartElement: " << nameElement;

            if(nameElement == "map")
            {
//                QString version = xmlReader.attributes().value("version");
                QString orientation = xmlReader.attributes().value("orientation").toString();
                mapSizeX = xmlReader.attributes().value("width").toInt();
                mapSizeY = xmlReader.attributes().value("height").toInt();
                tileMapWidth = xmlReader.attributes().value("tilewidth").toInt();
                tileMapHeight = xmlReader.attributes().value("tileheight").toInt();
                qDebug() << "mapSizeX: " << mapSizeX;
                qDebug() << "mapSizeY: " << mapSizeY;
                qDebug() << "tileMapWidth: " << tileMapWidth;
                qDebug() << "tileMapHeight: " << tileMapHeight;

                field.createField(mapSizeX, mapSizeY);
//                setMaximumSize(spaceWidget*2 + mapSizeX*sizeCell, spaceWidget*2 + mapSizeY*sizeCell);
//                setMinimumSize(spaceWidget*2 + mapSizeX*sizeCell, spaceWidget*2 + mapSizeY*sizeCell);
                if(orientation == "isometric") {
                    qDebug() << "GameWidget::loadMap(); -- field.setIsometric(true);";
                    field.setIsometric(true);
                    field.setTileMapSize(tileMapWidth, tileMapHeight);
                    field.setSizeCell(tileMapWidth);
                }
            }
            else if(nameElement == "tileset")
            {
                tileSet.firstTileID = xmlReader.attributes().value("firstgid").toInt();
                tileSet.name = xmlReader.attributes().value("name").toString();
                tileSet.spacing = xmlReader.attributes().value("spacing").toInt();
                tileSet.margin = xmlReader.attributes().value("margin").toInt();
                tileSet.tileWidth = xmlReader.attributes().value("tilewidth").toInt();
                tileSet.tileHeight = xmlReader.attributes().value("tileheight").toInt();
                //qDebug() << "tileSet.firstTileID: " << tileSet.firstTileID;
                qDebug() << "tileSet.name: " << tileSet.name;
                //qDebug() << "tileSet.spacing: " << tileSet.spacing;
                //qDebug() << "tileSet.margin: " << tileSet.margin;
                qDebug() << "tileSet.tileWidth: " << tileSet.tileWidth;
                qDebug() << "tileSet.tileHeight: " << tileSet.tileHeight;

                if(tileSet.name.contains("tower"))
                {
//                    qDebug() << "tileSet.name.contains('unit')";

                    DefaultTower tower;

                    xmlReader.readNext(); // <tileset "empty">
                    xmlReader.readNext(); // <properties>
//                    qDebug() << xmlReader.name().toString() << " " << xmlReader.isStartElement();
                    xmlReader.readNext(); // <properties "empty">
                    xmlReader.readNext(); // <property>
//                    qDebug() << xmlReader.name().toString() << " " << xmlReader.isStartElement();

                    while(xmlReader.name().toString() == "property")
                    {
//                        qDebug() << xmlReader.name().toString() << " " << xmlReader.isStartElement();

                        if(xmlReader.attributes().value("name").toString() == "attack")
                            tower.attack = xmlReader.attributes().value("value").toInt();
                        else if(xmlReader.attributes().value("name").toString() == "name")
                            tower.name = xmlReader.attributes().value("value").toString();
                        else if(xmlReader.attributes().value("name").toString() == "radius")
                            tower.radius = xmlReader.attributes().value("value").toInt();
                        else if(xmlReader.attributes().value("name").toString() == "size")
                            tower.size = xmlReader.attributes().value("value").toInt();
                        else if(xmlReader.attributes().value("name").toString() == "height")
                            tower.height = xmlReader.attributes().value("value").toInt();
                        else if(xmlReader.attributes().value("name").toString() == "type")
                            tower.type = xmlReader.attributes().value("value").toInt();

                        xmlReader.readNext(); // </property>
//                        qDebug() << xmlReader.name().toString() << " " << xmlReader.isStartElement();
                        xmlReader.readNext(); // </property "empty">
//                        qDebug() << xmlReader.name().toString() << " " << xmlReader.isStartElement();
                        xmlReader.readNext(); // <property> - </properties>
//                        qDebug() << xmlReader.name().toString() << " " << xmlReader.isStartElement();
                    }

//                    qDebug() << "tower.attack" << tower.attack;

//                    qDebug() << "tower.type" << tower.type;

                    xmlReader.readNext(); // </properties "empty">
                    xmlReader.readNext(); // <image>

                    if(xmlReader.name().toString() == "image")
                    {
                        QString imagePath = xmlReader.attributes().value("source").toString();
//                        qDebug() << "imagepath: " << imagePath;

                        imagePath.prepend(ASSETS_PATH + "maps/");
                        //qDebug() << imagePath;
                        if(!tileSet.img.load(imagePath))
                        {
                            qDebug() << "Failed to load tile sheet.";
                            return;
                        }

                        int columns = tileSet.img.width() / tileSet.tileWidth;
                        int rows = tileSet.img.height() / tileSet.tileHeight;

//                        qDebug() << "columnsPixmap: " << columns;
//                        qDebug() << "rowsPixmap: " << rows;

                        for(int y = 0; y < rows; y++)
                            for(int x = 0; x < columns; x++)
                            {
                                QRect rect(tileSet.margin + (tileSet.spacing * x) + (x * tileSet.tileWidth), tileSet.margin + (tileSet.spacing * y) + (y * tileSet.tileHeight), tileSet.tileWidth, tileSet.tileHeight);
                                tileSet.subRects.push_back(rect);
//                                qDebug() << "QRect rect(tower) -- push_back:" << rect;
                            }
                    }

                    xmlReader.readNext(); // </image>
                    xmlReader.readNext(); // </image "empty">
                    xmlReader.readNext(); // <terraintypes>
                    xmlReader.readNext(); // <terraintypes "empty">
                    xmlReader.readNext(); // <terrain>

                    while(xmlReader.name().toString() == "terrain")
                    {
                        QString name = xmlReader.attributes().value("name").toString();
                        int tileGID = xmlReader.attributes().value("tile").toInt();
                        QPixmap pixmap = tileSet.img.copy(tileSet.subRects[tileGID]);

                        if(name == "idle_up")
                            tower.pixmap = pixmap;
                        else if(name == "bullet_fly_up")
                            tower.bullet_fly_up = pixmap;
                        else if(name == "bullet_fly_up_right") {
                            tower.bullet_fly_up_right = pixmap;
                            tower.bullet_fly_up_left = QPixmap::fromImage(pixmap.toImage().mirrored(true, false));
                        }
                        else if(name == "bullet_fly_right") {
                            tower.bullet_fly_right = pixmap;
                            tower.bullet_fly_left = QPixmap::fromImage(pixmap.toImage().mirrored(true, false));
                        }
                        else if(name == "bullet_fly_down_right") {
                            tower.bullet_fly_down_right = pixmap;
                            tower.bullet_fly_down_left = QPixmap::fromImage(pixmap.toImage().mirrored(true, false));
                        }
                        else if(name == "bullet_fly_down")
                            tower.bullet_fly_down = pixmap;

/*//                        IN DEVELOPING..
                            tower.idle_up = pixmap;
                        else if(name == "idle_up_right")
                        {
                            tower.idle_up_right = pixmap;
                            tower.idle_up_left = QPixmap::fromImage(pixmap.toImage().mirrored(true, false));
                        }
                        else if(name == "idle_right")
                        {
                            tower.idle_right = pixmap;
                            tower.idle_left = QPixmap::fromImage(pixmap.toImage().mirrored(true, false));
                        }
                        else if(name == "idle_down_right")
                        {
                            tower.idle_down_right = pixmap;
                            tower.idle_down_left = QPixmap::fromImage(pixmap.toImage().mirrored(true, false));
                        }
                        else if(name == "idle_down")
                            tower.idle_down = pixmap;
                        else if(name.contains("walk"))
                        {
                            if(name.contains("up"))
                            {
                                if(!name.contains("right"))
                                    tower.walk_up.push_back(pixmap);
                                else
                                {
                                    tower.walk_up_right.push_back(pixmap);
                                    tower.walk_up_left.push_back(QPixmap::fromImage(pixmap.toImage().mirrored(true, false)));
                                }
                            }
                            else if(name.contains("right"))
                            {
                                if(!name.contains("down"))
                                {
                                    tower.walk_right.push_back(pixmap);
                                    tower.walk_left.push_back(QPixmap::fromImage(pixmap.toImage().mirrored(true, false)));
                                }
                                else
                                {
                                    tower.walk_down_right.push_back(pixmap);
                                    tower.walk_down_left.push_back(QPixmap::fromImage(pixmap.toImage().mirrored(true, false)));
                                }
                            }
                            else if(name.contains("down"))
                            {
                                if(!name.contains("right"))
                                    tower.walk_down.push_back(pixmap);
                            }
                        }
                        else if(name.contains("attack"))
                        {
                            if(name.contains("up"))
                            {
                                if(!name.contains("right"))
                                    tower.attack_up.push_back(pixmap);
                                else
                                {
                                    tower.attack_up_right.push_back(pixmap);
                                    tower.attack_up_left.push_back(QPixmap::fromImage(pixmap.toImage().mirrored(true, false)));
                                }
                            }
                            else if(name.contains("right"))
                            {
                                if(!name.contains("down"))
                                {
                                    tower.attack_right.push_back(pixmap);
                                    tower.attack_left.push_back(QPixmap::fromImage(pixmap.toImage().mirrored(true, false)));
                                }
                                else
                                {
                                    tower.attack_down_right.push_back(pixmap);
                                    tower.attack_down_left.push_back(QPixmap::fromImage(pixmap.toImage().mirrored(true, false)));
                                }
                            }
                            else if(name.contains("down"))
                            {
                                if(!name.contains("right"))
                                    tower.attack_down.push_back(pixmap);
                            }
                        }
                        else if(name.contains("death"))
                        {
                            if(name.contains("up"))
                            {
                                if(!name.contains("right"))
                                    tower.death_up.push_back(pixmap);
                                else
                                {
                                    tower.death_up_right.push_back(pixmap);
                                    tower.death_up_left.push_back(QPixmap::fromImage(pixmap.toImage().mirrored(true, false)));
                                }
                            }
                            else if(name.contains("right"))
                            {
                                if(!name.contains("down"))
                                {
                                    tower.death_right.push_back(pixmap);
                                    tower.death_left.push_back(QPixmap::fromImage(pixmap.toImage().mirrored(true, false)));
                                }
                                else
                                {
                                    tower.death_down_right.push_back(pixmap);
                                    tower.death_down_left.push_back(QPixmap::fromImage(pixmap.toImage().mirrored(true, false)));
                                }
                            }
                            else if(name.contains("down"))
                            {
                                if(!name.contains("right"))
                                    tower.death_down.push_back(pixmap);
                            }
                        }*/

                        xmlReader.readNext(); // </terrain>
                        xmlReader.readNext(); // </terrain "empty">
                        xmlReader.readNext(); // <terrain> - </terraintypes>
                    }

//                    qDebug() << "tower.walk_down.size(): " << &tower << "->" << tower.walk_down.size();
                    qDebug() << "faction.creatyNewTower(tower);";
                    faction.createNewTower(tower);
                }
                else if(tileSet.name.contains("unit"))
                {
//                    qDebug() << "tileSet.name.contains('unit')";

                    DefaultUnit unit;
//                    loadUnit = true;

                    xmlReader.readNext(); // <tileset "empty">
                    xmlReader.readNext(); // <properties>
//                    qDebug() << xmlReader.name().toString() << " " << xmlReader.isStartElement();
                    xmlReader.readNext(); // <properties "empty">
                    xmlReader.readNext(); // <property>
//                    qDebug() << xmlReader.name().toString() << " " << xmlReader.isStartElement();

                    while(xmlReader.name().toString() == "property")
                    {
//                        qDebug() << xmlReader.name().toString() << " " << xmlReader.isStartElement();

                        if(xmlReader.attributes().value("name").toString() == "health_point")
                            unit.healtPoint = xmlReader.attributes().value("value").toInt();
                        else if(xmlReader.attributes().value("name").toString() == "name")
                            unit.name = xmlReader.attributes().value("value").toString();

                        xmlReader.readNext(); // </property>
//                        qDebug() << xmlReader.name().toString() << " " << xmlReader.isStartElement();
                        xmlReader.readNext(); // </property "empty">
//                        qDebug() << xmlReader.name().toString() << " " << xmlReader.isStartElement();
                        xmlReader.readNext(); // <property> - </properties>
//                        qDebug() << xmlReader.name().toString() << " " << xmlReader.isStartElement();
                    }

//                    qDebug() << "unit.healtPoint" << unit.healtPoint;
//                    qDebug() << "unit.name" << unit.name;

                    xmlReader.readNext(); // </properties "empty">
                    xmlReader.readNext(); // <image>

                    if(xmlReader.name().toString() == "image")
                    {
                        QString imagePath = xmlReader.attributes().value("source").toString();
//                        qDebug() << "imagepath: " << imagePath;

                        imagePath.prepend(ASSETS_PATH + "maps/");
                        //qDebug() << imagePath;
                        if(!tileSet.img.load(imagePath))
                        {
                            qDebug() << "Failed to load tile sheet.";
                            return;
                        }

                        int columns = tileSet.img.width() / tileSet.tileWidth;
                        int rows = tileSet.img.height() / tileSet.tileHeight;

                        qDebug() << "GameWidget::loadMap(); -- unit : columnsPixmap: " << columns;
                        qDebug() << "GameWidget::loadMap(); -- unit : rowsPixmap: " << rows;

                        for(int y = 0; y < rows; y++) {
                            for(int x = 0; x < columns; x++) {
                                QRect rect(tileSet.margin + (tileSet.spacing * x) + (x * tileSet.tileWidth), tileSet.margin + (tileSet.spacing * y) + (y * tileSet.tileHeight), tileSet.tileWidth, tileSet.tileHeight);
                                tileSet.subRects.push_back(rect);
//                                qDebug() << "QRect rect(Unit) -- push_back:" << rect;
                            }
                        }
                    }

                    xmlReader.readNext(); // </image>
                    xmlReader.readNext(); // </image "empty">
                    xmlReader.readNext(); // <terraintypes>
                    xmlReader.readNext(); // <terraintypes "empty">
                    xmlReader.readNext(); // <terrain>

                    while(xmlReader.name().toString() == "terrain") {
                        QString name = xmlReader.attributes().value("name").toString();
                        int tileGID = xmlReader.attributes().value("tile").toInt();
                        QPixmap pixmap = tileSet.img.copy(tileSet.subRects[tileGID]);

//                        qDebug() << "GameWidget::loadMap(); -- unitPixmaps: " << name << "tileGID: " << tileGID << " : " << !pixmap.isNull();

                        if(name == "idle_up") {
                            unit.idle_up = pixmap;
                            unit.idle.push_back(pixmap);
                        } else if(name == "idle_up_right") {
                            unit.idle_up_right = pixmap;
                            unit.idle.push_back(pixmap);
                            unit.idle_up_left = QPixmap::fromImage(pixmap.toImage().mirrored(true, false));
                            unit.idle.push_back(unit.idle_up_left);
                        } else if(name == "idle_right") {
                            unit.idle_right = pixmap;
                            unit.idle.push_back(pixmap);
                            unit.idle_left = QPixmap::fromImage(pixmap.toImage().mirrored(true, false));
                            unit.idle.push_back(unit.idle_left);
                        } else if(name == "idle_down_right") {
                            unit.idle_down_right = pixmap;
                            unit.idle.push_back(pixmap);
                            unit.idle_down_left = QPixmap::fromImage(pixmap.toImage().mirrored(true, false));
                            unit.idle.push_back(unit.idle_down_left);
                        } else if(name == "idle_down") {
                            unit.idle_down = pixmap;
                            unit.idle.push_back(pixmap);
                        } else if(name.contains("walk")) {
                            if(name.contains("up")) {
                                if(!name.contains("right")) {
                                    unit.walk_up.push_back(pixmap);
                                } else {
                                    unit.walk_up_right.push_back(pixmap);
                                    unit.walk_up_left.push_back(QPixmap::fromImage(pixmap.toImage().mirrored(true, false)));
                                }
                            }
                            else if(name.contains("right"))
                            {
                                if(!name.contains("down"))
                                {
                                    unit.walk_right.push_back(pixmap);
                                    unit.walk_left.push_back(QPixmap::fromImage(pixmap.toImage().mirrored(true, false)));
                                }
                                else
                                {
                                    unit.walk_down_right.push_back(pixmap);
                                    unit.walk_down_left.push_back(QPixmap::fromImage(pixmap.toImage().mirrored(true, false)));
                                }
                            }
                            else if(name.contains("down"))
                            {
                                if(!name.contains("right"))
                                    unit.walk_down.push_back(pixmap);
                            }
                        }
                        else if(name.contains("attack"))
                        {
                            if(name.contains("up"))
                            {
                                if(!name.contains("right"))
                                    unit.attack_up.push_back(pixmap);
                                else
                                {
                                    unit.attack_up_right.push_back(pixmap);
                                    unit.attack_up_left.push_back(QPixmap::fromImage(pixmap.toImage().mirrored(true, false)));
                                }
                            }
                            else if(name.contains("right"))
                            {
                                if(!name.contains("down"))
                                {
                                    unit.attack_right.push_back(pixmap);
                                    unit.attack_left.push_back(QPixmap::fromImage(pixmap.toImage().mirrored(true, false)));
                                }
                                else
                                {
                                    unit.attack_down_right.push_back(pixmap);
                                    unit.attack_down_left.push_back(QPixmap::fromImage(pixmap.toImage().mirrored(true, false)));
                                }
                            }
                            else if(name.contains("down"))
                            {
                                if(!name.contains("right"))
                                    unit.attack_down.push_back(pixmap);
                            }
                        }
                        else if(name.contains("death"))
                        {
                            if(name.contains("up"))
                            {
                                if(!name.contains("right"))
                                    unit.death_up.push_back(pixmap);
                                else
                                {
                                    unit.death_up_right.push_back(pixmap);
                                    unit.death_up_left.push_back(QPixmap::fromImage(pixmap.toImage().mirrored(true, false)));
                                }
                            }
                            else if(name.contains("right"))
                            {
                                if(!name.contains("down"))
                                {
                                    unit.death_right.push_back(pixmap);
                                    unit.death_left.push_back(QPixmap::fromImage(pixmap.toImage().mirrored(true, false)));
                                }
                                else
                                {
                                    unit.death_down_right.push_back(pixmap);
                                    unit.death_down_left.push_back(QPixmap::fromImage(pixmap.toImage().mirrored(true, false)));
                                }
                            }
                            else if(name.contains("down"))
                            {
                                if(!name.contains("right"))
                                    unit.death_down.push_back(pixmap);
                            }
                        }
                        xmlReader.readNext(); // </terrain>
                        xmlReader.readNext(); // </terrain "empty">
                        xmlReader.readNext(); // <terrain> - </terraintypes>
                    }

//                    qDebug() << "unit.walk" << unit.walk_down.size();
                    qDebug() << "unit.walk_down.size(): " << &unit << "->" << unit.walk_down.size();
                    qDebug() << "faction.creatyNewUnit(unit);";
                    faction.createNewUnit(unit);
//                    unit.clearVectors();
                }
            }
            else if(nameElement == "image")
            {
                QString imagePath = xmlReader.attributes().value("source").toString();
                //qDebug() << "imagepath: " << imagePath;

                imagePath.prepend(ASSETS_PATH + "maps/");
                //qDebug() << imagePath;
                if(!tileSet.img.load(imagePath))
                {
                    qDebug() << "Failed to load tile sheet.";
                    return;
                }

                int columns = tileSet.img.width() / tileSet.tileWidth;
                int rows = tileSet.img.height() / tileSet.tileHeight;

                qDebug() << "columnsPixmap: " << columns;
                qDebug() << "rowsPixmap: " << rows;

                for(int y = 0; y < rows; y++)
                    for(int x = 0; x < columns; x++)
                    {
//                        qDebug() << "x1: " << tileSet.margin + (tileSet.spacing * x) + (x * tileWidth);
//                        qDebug() << "y1: " << tileSet.margin + (tileSet.spacing * y) + (y * tileHeight);
//                        qDebug() << "tileWidth: " << tileWidth;
//                        qDebug() << "tileHeight: " << tileHeight;
                        QRect rect(tileSet.margin + (tileSet.spacing * x) + (x * tileSet.tileWidth), tileSet.margin + (tileSet.spacing * y) + (y * tileSet.tileHeight), tileSet.tileWidth, tileSet.tileHeight);
                        tileSet.subRects.push_back(rect);
                        tileSet.tiles.push_back(tileSet.img.copy(rect));
//                        qDebug() << "QRect rect(terrain) -- push_back:" << rect;
                    }
            }
            else if(nameElement == "terrain")
            {
                QString name = xmlReader.attributes().value("name").toString();
                int tileGID = xmlReader.attributes().value("tile").toInt();
//                    QPixmap pixmap = tileSet.img;
//                    pixmap = pixmap.copy(tileSet.subRects[tileGID]);
                QPixmap pixmap = tileSet.img.copy(tileSet.subRects[tileGID]);
//                QPixmap pixmapMirrored = QPixmap::fromImage(pixmap.toImage().mirrored(true, false));

                if(name == "Unit")
                {
                    global_pixmap = pixmap;
                    field.unitsManager.setDefaulPixmapForUnit(pixmap);//setPixmapForUnit(pixmap);
                    qDebug() << "GameWidget::loadMap(); -- Set default Pixmap for Unit!";
                }
                if(tileSet.name.contains("unit"))
                {
                    // GAVNO!!
                }
                else if(name == "Tower")
                {
//                    int towerAttack = -1;
//                    int towerRadius = -1;
//                    int towerType = -1;

//                    xmlReader.readNext(); // <properties>
//                    xmlReader.readNext(); // <property>

//                    while(xmlReader.name().toString() == "property")
//                    {
//                        if(xmlReader.attributes().value("name").toString() == "attack")
//                            towerAttack = xmlReader.attributes().value("value").toInt();
//                        else if(xmlReader.attributes().value("name").toString() == "radius")
//                            towerRadius = xmlReader.attributes().value("value").toInt();
//                        else if(xmlReader.attributes().value("name").toString() == "type")
//                            towerType = xmlReader.attributes().value("value").toInt();
//                        xmlReader.readNext();
//                    }

//                    global_pixmap = pixmap;
//                    field.setPixmapForTower(pixmap);
//                    faction.creatyNewTower(towerType, towerRadius, towerAttack, pixmap);
//                    qDebug() << "Tower Set!";
                }
            }
            else if(nameElement == "layer")
            {
                layer = true;
                if(xmlReader.attributes().value("name") == "ground")
                    ground = true;
                else
                    ground = false;

                x = 0;
                y = 0;
            }
//            if(nameElement == "data")
            else if(nameElement == "tile")
            {
                if(layer)
                {
                    int num = 0;
                    int tileGID = xmlReader.attributes().value("gid").toInt();

                    int size = tileSets.size();
                    for(int k = 0; k < size; k++)
                        if(tileGID >= tileSets[k].firstTileID)
                            num = k;

                    int subRectToUse = tileGID - tileSets[num].firstTileID;

                    // Устанавливаем TextureRect каждого тайла
                    if (subRectToUse >= 0)
                    {
                        QPixmap pixmap = tileSets[num].img;
                        pixmap = pixmap.copy(tileSets[num].subRects[subRectToUse]);
                        if(ground)
                            field.getCell(x, y)->setTerrain(pixmap);
                        else
                            field.getCell(x, y)->backgroundPixmap = pixmap;//setPixmapInCell(x, y, pixmap);
                    }
                    else
                    {
                        //qDebug() << "subRectToUse: " << subRectToUse;
                    }

                    x++;
                    if (x >= mapSizeX)
                    {
                        x = 0;
                        y++;
                        if(y >= mapSizeY)
                            y = 0;
                    }
                }
            }
//            if(nameElement == "objectgroup")
            else if(nameElement == "object")
            {
                QString name = xmlReader.attributes().value("name").toString();

                int num = 0;
                int tileGID = xmlReader.attributes().value("gid").toInt();

                int size = tileSets.size();
                for(int k = 0; k < size; k++)
                    if(tileGID >= tileSets[k].firstTileID)
                        num = k;

                int tileObjectID = tileGID - tileSets[num].firstTileID;
//                int tileObjectID = xmlReader.attributes().value("gid").toInt() - firstTileID;
                int x = xmlReader.attributes().value("x").toInt();
                int y = xmlReader.attributes().value("y").toInt();

                if(!field.getIsometric()) {
    //                qDebug() << "tileObjectID: " << tileObjectID;
    //                qDebug() << "x: " << x;
    //                qDebug() << "y: " << y;
                    x = x / tileMapWidth; // В файле кординаты графические. Поэтому преобразуем в игровые.
                    y = (y - tileMapWidth) / tileMapWidth;
                }

                QPixmap pixmap = tileSets[num].img;
                pixmap = pixmap.copy(tileSets[num].subRects[tileObjectID]);

                if(name == "spawnPoint") {
                    qDebug() << "GameWidget::loadMap(); -- createSpawnPoint(" << defaultNumCreateUnits << ", " << x << ", " << y << ");";
                    field.getCell(x, y)->backgroundPixmap = pixmap;//setPixmapInCell(x, y, pixmap); // need fix
                    field.setMousePress(x, y);
                    field.createSpawnPoint(defaultNumCreateUnits, x, y);
                } else if(name == "exitPoint") {
                    qDebug() << "GameWidget::loadMap(); -- createExitPoint(" << x << ", " << y << ");";
                    field.getCell(x, y)->backgroundPixmap = pixmap;//setPixmapInCell(x, y, pixmap); // need fix
                    field.createExitPoint(x, y);
//                    startUnitTimer();
                }
            }
        }
        else if(xmlReader.isEndElement())
        {
            QString nameElement = xmlReader.name().toString();
            //qDebug() << "EndElement: " << nameElement;

            if(nameElement == "tileset") {
                tileSets.push_back(tileSet);
                tileSet.subRects.clear();
//                if(loadUnit)
//                {
////                    qDebug() << "faction.creatyNewUnit(unit);";
////                    faction.creatyNewUnit(unit);
////                    unit.clearVectors();
//                    loadUnit = false;
//                }
            } else if(nameElement == "layer") {
                layer = false;
            }
        }
        xmlReader.readNext();
    }
    if(xmlReader.hasError()) {
        qDebug() << "Error: " << xmlReader.errorString();
    } else {
        qDebug() << "GameWidget::loadMap(); -- Completed load map.";
    }

    field.setFaction(&faction);
//    field.spawnHeroInSpawnPoint();

    int terrainType = rand()%2;
    if (mapName.contains("randomMap")) {
        for (int x = 0; x < mapSizeX; x++) {
            for (int y = 0; y < mapSizeY; y++) {
                if( (rand()%100) < 35 ) {
                    int randNumber = ( ( ((terrainType)?30:50) ) + (rand()%20) );
//                    qDebug() << "GameWidget::loadMap(); -- randNumber:" << randNumber;
                    field.getCell(x, y)->setTerrain(tileSet.tiles[randNumber]);
                }
            }
        }
    }
    field.pathFinder.setWorldSize({mapSizeX, mapSizeY});
    field.pathFinder.setHeuristic(AStar::Heuristic::euclidean);
    field.pathFinder.setDiagonalMovement(false);
    field.updatePathFinderWalls();
    global_pixmap = tileSet.tiles[7];
    global_pixmap_PathPoint = tileSet.tiles[8];
    global_pixmap_HeroPoint = tileSet.tiles[9];
    global_pixmap_ExitPoint = tileSet.tiles[18];
    global_pixmap_DestinationPoint = tileSet.tiles[19];

    mapLoad = true;
    tileSet.subRects.clear();
    tileSet.tiles.clear();
    tileSets.clear();
    file->close();
//    update();
    field.spawnHeroInSpawnPoint();
    int randomEnemyCount = 5+rand()%10;
    for (int k = 0; k < randomEnemyCount; k++) {
        int randomX = rand()%mapSizeX;
        int randomY = rand()%mapSizeY;
        field.createUnit(randomX, randomY); // magic numbers need fix
    }
    startTimer_UnitsMoveAndTowerAttack();
}

void GameWidget::on_loadMaps_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "../LittleGame/maps/", tr("Maps (*.xml)"));

//    qDebug() << "FileName: " << fileName;

    loadMap(fileName);
}

/*void Widget::on_loadMaps_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "./maps/", tr("Maps (*.tdm)"));

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QString str;
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setText("File is broken.");

    int newSizeX, newSizeY;

    if(!file.atEnd())
    {
        str = QString(file.readLine());
        if(str.indexOf("LittleGameMap") == -1)
        {
            msgBox.exec();
            return;
        }
    }

    if(!file.atEnd())
    {
        str = QString(file.readLine());

        newSizeX = str.toInt();

        if(!file.atEnd())
        {
            str = QString(file.readLine());

            newSizeY = str.toInt();
        }
    }

    field.createField(newSizeX, newSizeY);
    setMinimumSize(spaceWidget*2 + newSizeX*sizeCell, spaceWidget*2 + newSizeY*sizeCell);
    setMaximumSize(spaceWidget*2 + newSizeX*sizeCell, spaceWidget*2 + newSizeY*sizeCell);

    for(int x = 0; x < field.getSizeX(); x++)
    {
        for(int y = 0; y < field.getSizeY(); y++)
        {
            if(!file.atEnd())
            {
                str = QString(file.readLine());
                if(str.toInt())
                    field.setBusy(x, y);
            }
            else
            {
                msgBox.exec();
                return;
            }
        }
    }

    for(int x = 0; x < field.getSizeX(); x++)
    {
        for(int y = 0; y < field.getSizeY(); y++)
        {
            if(!file.atEnd())
            {
                str = QString(file.readLine());
                if(str.toInt())
                    field.setTower(x, y);
            }
            else
            {
                msgBox.exec();
                return;
            }
        }
    }

    file.close();
    update();
}*/

void GameWidget::on_clearMap_clicked()
{
    for(int x = 0; x < field.getSizeX(); x++)
    {
        for(int y = 0; y < field.getSizeY(); y++)
        {
            field.deleteTower(x, y);
            field.getCell(x, y)->removeTerrain();
//            field.clearStepCell(x, y);
        }
    }

//    int numUnitsK = 0;
//    for(int x = 0; x < field.getSizeX(); x++)
//    {
//        for(int y = 0; y < field.getSizeY(); y++)
//        {
//            int procent = 0;
    //            {
        //            {
//            if(rand()%101 <= procent)
//            {
    //            {
    //                if(numUnitsK++ < 30)
    //                {
//                {
//                    field.setUnit(x, y);
//                }
//            }
//        }
//    }

    update();
}

void GameWidget::on_closeWidget_clicked()
{
    signal_closeWidgetGameFinished(false);
}

void GameWidget::on_goUnits_clicked()
{
    field.waveAlgorithm();
    field.createSpawnPoint(defaultNumCreateUnits);
    startTimer_UnitsMoveAndTowerAttack();
}
