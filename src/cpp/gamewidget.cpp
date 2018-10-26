#include "src/head/gamewidget.h"

GameWidget::GameWidget(QString mapFile, int enemyCount, int towersCount, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameWidget)
{
    ui->setupUi(this);
//    gameStart = true;
//    gamePause = false;
//    mapLoad = false;

    // camera need
    pixelsShiftMap = 32;
    zoomSizeCell = 8;
    zoomMaxSizeCell = 256;
    zoomMinSizeCell = 48;

    timeOfGame = 0;
    unitsMove_TimerMilliSec = 100;
    towersAttack_TimerMilliSec = 50;
    scanMouseMove_TimerMilliSec = 100;
    unitsMove_TimerId = 0;
    towersAttack_TimerId = 0;
    scanMouseMove_TimerId = 0;
    scanMouseMove_TimerId = startTimer(scanMouseMove_TimerMilliSec);

    test = 0;
    field = new Field(mapFile, enemyCount, towersCount);

//    ui->loadMaps->setHidden(true);
//    ui->clearMap->setHidden(true);
//    ui->goUnits->setHidden(true);
//    ui->closeWidget->setHidden(true);
    qDebug() << "GameWidget::loadMap(); -- mapFile:" << mapFile;
    qDebug() << "GameWidget::loadMap(); -- enemyCount:" << enemyCount;
    qDebug() << "GameWidget::loadMap(); -- towersCount:" << towersCount;
    qDebug() << "GameWidget::loadMap(); -- field:" << field;
    qDebug() << "GameWidget::loadMap(); -- field->map:" << field->map;
    qDebug() << "GameWidget::GameWidget(); -END- -END-";
}

GameWidget::~GameWidget() {
    stopTimer_UnitsMoveAndTowerAttack();
    field->deleteField();
    if (scanMouseMove_TimerId)
        killTimer(scanMouseMove_TimerId);
    delete ui;
}

void GameWidget::timerEvent(QTimerEvent *event) {
    int timerId = event->timerId();
    if (timerId == unitsMove_TimerId) {
//        if (!gamePause) {
            timeOfGame += unitsMove_TimerMilliSec;
            if (int result = field->stepAllUnits()) {
                if(result == 4) {
                    global_text = "Hero contact With Enemy!";
                    signal_closeWidgetGameFinished(false, timeOfGame);
                    return;
                } else if(result == 3) {
                    global_text = "Hero in ExitPoint!";
                    signal_closeWidgetGameFinished(true, timeOfGame);
                    return;
                } else if(result == 2) {
                    global_text = "You WIN!";
                } else if(result == 1) {
                    global_text = "You LOSE!";
                } else if(result == -1) {
                    if(field->deleteTower()) {
                        field->waveAlgorithm();
                    } else {
                        stopTimer_UnitsMoveAndTowerAttack();
                    }
                }
            }
//        }
    } else if (timerId == towersAttack_TimerId) {
//        if (!gamePause) {
            field->towersAttack(towersAttack_TimerMilliSec);
//        }
    } else if(timerId == scanMouseMove_TimerId) {
        int curX = cursor().pos().x();
        int curY = cursor().pos().y();
        if(curX == 0 || curY == 0 || curX == width()-1 || curY == height()-1) {
            int mainCoorMapX = field->getMainCoorMapX();
            int mainCoorMapY = field->getMainCoorMapY();
            int sizeCell = field->getSizeCell();
            int sizeX = field->getSizeX();
            int sizeY = field->getSizeY();

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
            field->setMainCoorMap(mainCoorMapX, mainCoorMapY);
        }
    }
    update();
}

void GameWidget::keyPressEvent(QKeyEvent * event) {
    int mainCoorMapX = field->getMainCoorMapX();
    int mainCoorMapY = field->getMainCoorMapY();
    int sizeCell = field->getSizeCell();
    int sizeX = field->getSizeX();
    int sizeY = field->getSizeY();
    int key = event->key();
    qDebug() << "GameWidget::keyPressEvent(); -- key: " << key;
    if(key == Qt::Key_0) {
        test++;
        signal_changeWindowState();
        qDebug() << "GameWidget::keyPressEvent(); -- parentWidget()->windowState():" << parentWidget()->windowState();
    } else if(key == Qt::Key_1) {
        ui->drawGrid_checkBox->toggle();
    } else if(key == Qt::Key_2) {
        ui->drawBlackTiles_checkBox->toggle();
    } else if(key == Qt::Key_3) {
        ui->drawPaths_checkBox->toggle();
    } else if(key == Qt::Key_4) {
        ui->drawBackGround_checkBox->toggle();
    } else if(key == Qt::Key_5) {
        ui->drawGround_checkBox->toggle();
    } else if(key == Qt::Key_6) {
        ui->drawForeGround_checkBox->toggle();
    } else if(key == Qt::Key_7) {
        ui->drawUnits_checkBox->toggle();
    } else if(key == Qt::Key_8) {
        ui->drawTowersByTowers_checkBox->toggle();
    } else if(key == Qt::Key_Space || key == Qt::Key_Escape) {
//        gamePause = !gamePause;
//        qDebug() << "GameWidget::keyPressEvent(); -- gamePause: " << gamePause;
    } else if (event->key() == Qt::Key_Enter) {
        signal_closeWidget();
        return;
    } else if(key == Qt::Key_Left) {
        if(mainCoorMapX < 0) {
            mainCoorMapX += pixelsShiftMap;
        }
    } else if(key == Qt::Key_Up) {
        if(mainCoorMapY < 0) {
            mainCoorMapY += pixelsShiftMap;
        }
    } else if(key == Qt::Key_Right) {
        if(mainCoorMapX+sizeX*sizeCell > width()) {
            mainCoorMapX -= pixelsShiftMap;
        }
    } else if(key == Qt::Key_Down) {
        if(mainCoorMapY+sizeY*sizeCell > height()) {
            mainCoorMapY -= pixelsShiftMap;
        }
    }
    field->setMainCoorMap(mainCoorMapX, mainCoorMapY);
    update();
}

void GameWidget::paintEvent(QPaintEvent* event) {
    painter.begin(this);
//    if (gameStart) {
            drawFullField();
            if(ui->drawBackGround_checkBox->isChecked())
                drawBackGround();
            if(ui->drawUnits_checkBox->isChecked())
                drawUnits();
            if(ui->drawGround_checkBox->isChecked())
                drawGround();
            if(ui->drawTowersByTowers_checkBox->isChecked())
                drawTowersByTowers();
            if(ui->drawForeGround_checkBox->isChecked())
                drawForeGround();

            if(ui->drawBlackTiles_checkBox->isChecked())
                drawBlackTiles();
            if(ui->drawPaths_checkBox->isChecked())
                drawPaths();
            if(ui->drawGrid_checkBox->isChecked())
                drawGrid();
            painter.drawText(10, 20, QString("timeOfGame:%1").arg(timeOfGame));
//    }
    painter.end();
}

void GameWidget::drawFullField() {
    if(field->getIsometric()) {
        QPixmap pixmap = field->map->tileSets.getTile(85)->getPixmap(); // draw water2
        int sizeCellX = field->getSizeCell();
        int sizeCellY = sizeCellX/2;
        int sizeX = (width()/sizeCellX)+1;
        int sizeY = (height()/sizeCellY)*2+2;
        int isometricSpaceX = 0;
        int isometricSpaceY = -(sizeCellY/2);
        for (int y = 0; y <= sizeY; y++) {
            for (int x = 0; x <= sizeX; x++) {
                painter.drawPixmap(isometricSpaceX - sizeCellX/2 + x*sizeCellX, isometricSpaceY - sizeCellY, sizeCellX, sizeCellY*2, pixmap);
            }
            isometricSpaceY += sizeCellY/2;
            isometricSpaceX = isometricSpaceX != 0 ? 0 : sizeCellX/2;
        }
    }
}

void GameWidget::drawGrid() {
    int fieldX = field->getSizeX();
    int fieldY = field->getSizeY();
    int mainCoorMapX = field->getMainCoorMapX();
    int mainCoorMapY = field->getMainCoorMapY();
    int spaceWidget = field->getSpaceWidget();
    int sizeCell = field->getSizeCell();
    painter.setPen(QColor(100, 60, 21));
    if(field->getIsometric()) {
        int halfSizeCellX = field->getSizeCell()/2;
        int halfSizeCellY = halfSizeCellX/2;
        int isometricSpaceX = halfSizeCellX*fieldY;
        int isometricSpaceY = halfSizeCellY*fieldY;
        for(int x = 0; x < fieldX+1; x++) {
            int x1 = mainCoorMapX + isometricSpaceX+spaceWidget + x*halfSizeCellX;
            int y1 = mainCoorMapY + spaceWidget + x*halfSizeCellY;
            int x2 = mainCoorMapX + spaceWidget + x*halfSizeCellX;
            int y2 = mainCoorMapY + isometricSpaceY+spaceWidget + x*halfSizeCellY;
            painter.drawLine(x1, y1, x2, y2);
        }
        for(int y = 0; y < fieldY+1; y++) {
            int x1 = mainCoorMapX + isometricSpaceX+spaceWidget - y*halfSizeCellX;
            int y1 = mainCoorMapY + spaceWidget + y*halfSizeCellY;
            int x2 = mainCoorMapX + isometricSpaceX*2+spaceWidget - (halfSizeCellX*(fieldY-fieldX)) - y*halfSizeCellX;
            int y2 = mainCoorMapY + isometricSpaceY+spaceWidget - (halfSizeCellY*(fieldY-fieldX)) + y*halfSizeCellY;
            painter.drawLine(x1, y1, x2, y2);
        }
    } else {
        for(int x = 0; x < fieldX+1; x++) {
            painter.drawLine(mainCoorMapX + spaceWidget + x*sizeCell, mainCoorMapY + spaceWidget, mainCoorMapX + spaceWidget + x*sizeCell, mainCoorMapY + spaceWidget + sizeCell*fieldY);
        }
        for(int y = 0; y < fieldY+1; y++) {
            painter.drawLine(mainCoorMapX + spaceWidget, mainCoorMapY + spaceWidget + y*sizeCell, mainCoorMapX + spaceWidget + sizeCell*fieldX, mainCoorMapY + spaceWidget + y*sizeCell);
        }
    }
}

void GameWidget::drawBackGround() {
    int fieldX = field->getSizeX();
    int fieldY = field->getSizeY();
    int mainCoorMapX = field->getMainCoorMapX();
    int mainCoorMapY = field->getMainCoorMapY();
    int spaceWidget = field->getSpaceWidget();
    int sizeCell = field->getSizeCell();

    int isometricCoorX = (sizeCell/2) * fieldY;
    int isometricCoorY = 0;
    int sizeCellX = field->getSizeCell();
    int sizeCellY = sizeCellX/2;
    for(int y = 0; y < fieldY; y++) {
        for(int x = 0; x < fieldX; x++) {
            if(field->map != NULL) {
                Cell* cell = field->getCell(x, y);
                foreach (QPixmap pix, cell->backgroundTiles) {
                    if(!field->getIsometric()) {
                        int pxlsX = mainCoorMapX + spaceWidget + x*sizeCell;
                        int pxlsY = mainCoorMapY + spaceWidget + y*sizeCell;
                        int localSizeCell = sizeCell;
                        painter.drawPixmap(pxlsX, pxlsY, localSizeCell, localSizeCell, pix);
                    } else {
                        int x1 = mainCoorMapX + isometricCoorX - (sizeCellX/2) + x*(sizeCellX/2);
                        int y1 = mainCoorMapY + isometricCoorY - (sizeCellY) + x*(sizeCellY/2);
                        painter.drawPixmap(x1, y1, sizeCellX, sizeCellY*2, pix);
                    }
                }
            }
        }
        isometricCoorX = (field->getSizeCell()/2) * (fieldY - (y+1));
        isometricCoorY = (field->getSizeCell()/4) * (y+1);
    }
}

void GameWidget::drawGround() {
    int fieldX = field->getSizeX();
    int fieldY = field->getSizeY();
    int mainCoorMapX = field->getMainCoorMapX();
    int mainCoorMapY = field->getMainCoorMapY();
    int spaceWidget = field->getSpaceWidget();
    int sizeCell = field->getSizeCell();
    int sizeCellX = field->getSizeCell();
    int sizeCellY = sizeCellX/2;
    int isometricCoorX = (sizeCell/2) * fieldY;
    int isometricCoorY = 0;

    for(int y = 0; y < fieldY; y++) {
        for(int x = 0; x < fieldX; x++) {
            Cell* cell = field->getCell(x, y);
            if(cell != NULL && cell->isTerrain()) {
                foreach (QPixmap pix, cell->terrainTiles) {
                    if(!field->getIsometric()) {
                        int pxlsX = mainCoorMapX + spaceWidget + x*sizeCell;//+1;
                        int pxlsY = mainCoorMapY + spaceWidget + y*sizeCell;//+1;
                        int localSizeCell = sizeCell;//-1;
                            painter.fillRect(pxlsX+1, pxlsY+1, localSizeCell-1, localSizeCell-1, QColor(0, 0, 0));
                            painter.drawPixmap(pxlsX, pxlsY, localSizeCell, localSizeCell, pix);
                    } else {
                        int x1 = mainCoorMapX + isometricCoorX - (sizeCellX/2) + x*(sizeCellX/2);
                        int y1 = mainCoorMapY + isometricCoorY - (sizeCellY) + x*(sizeCellY/2);
                        painter.drawPixmap(x1, y1, sizeCellX, sizeCellY*2, pix);
                    }
                }
            }
        }
        isometricCoorX = (field->getSizeCell()/2) * (fieldY - (y+1));
        isometricCoorY = (field->getSizeCell()/4) * (y+1);
    }
}

void GameWidget::drawForeGround() {
    int fieldX = field->getSizeX();
    int fieldY = field->getSizeY();
    int mainCoorMapX = field->getMainCoorMapX();
    int mainCoorMapY = field->getMainCoorMapY();
    int sizeCell = field->getSizeCell();
    int sizeCellX = field->getSizeCell();
    int sizeCellY = sizeCellX/2;
    int isometricCoorX = (sizeCell/2) * fieldY;
    int isometricCoorY = 0;
    for (int y = 0; y < fieldY; y++) {
        for (int x = 0; x < fieldX; x++) {
            Cell* cell = field->getCell(x, y);
            if (cell != NULL) {
                foreach (QPixmap pix, cell->foregroundTiles) {
                    if(!field->getIsometric()) {
                    } else {
                        int x1 = mainCoorMapX + isometricCoorX - (sizeCellX/2) + x*(sizeCellX/2);
                        int y1 = mainCoorMapY + isometricCoorY - (sizeCellY) + x*(sizeCellY/2);
                        painter.drawPixmap(x1, y1, sizeCellX, sizeCellY*2, pix);
                    }
                }
            }
        }
        isometricCoorX = (field->getSizeCell()/2) * (fieldY - (y+1));
        isometricCoorY = (field->getSizeCell()/4) * (y+1);
    }
}

void GameWidget::drawTowersByTowers() {
    int fieldX = field->getSizeX();
    int fieldY = field->getSizeY();
    int mainCoorMapX = field->getMainCoorMapX();
    int mainCoorMapY = field->getMainCoorMapY();
    int spaceWidget = field->getSpaceWidget();
    int sizeCell = field->getSizeCell();
    int sizeCellX = sizeCell;
    int sizeCellY = sizeCellX/2;
    std::vector<Tower*> towers = field->getAllTowers();
    int size = towers.size();
    for (int k = 0; k < size; k++) {
        Tower* tmpTower = towers[k];
        if (tmpTower != NULL) {
            int towerX = tmpTower->currX;
            int towerY = tmpTower->currY;
            int size = tmpTower->defTower->size;
            if(!field->getIsometric()) {
            } else {
                int height = tmpTower->defTower->height;
                int isometricSpaceX = (field->getSizeY()-towerY)*(sizeCellX/2);
                int isometricSpaceY = towerY*(sizeCellY/2);
                int pxlsX = mainCoorMapX + isometricSpaceX+spaceWidget + towerX*(sizeCellX/2);
                int pxlsY = mainCoorMapY + isometricSpaceY+spaceWidget + towerX*(sizeCellY/2);
                if (!field->map) {
                    painter.fillRect(pxlsX+1, pxlsY+1, sizeCell-1, sizeCell-1, QColor(127, 255, 0));
                } else {
                    painter.drawPixmap(pxlsX - sizeCellX/2, pxlsY + sizeCellY - (sizeCellY*2)*height, sizeCellX, (sizeCellY*2)*height, tmpTower->pixmap);
                }
                for (int iBullet = 0; iBullet < tmpTower->bullets.size(); iBullet++) {
                    Bullet* tmpBullet = tmpTower->bullets[iBullet];
                    int bulletCellX = tmpBullet->currCellX;
                    int bulletCellY = tmpBullet->currCellY;
                    int lastX, lastY;
                    int animationCurrIter, animationMaxIter;
                    QPixmap pixmap = tmpBullet->getAnimationInformation(&lastX, &lastY, &animationCurrIter, &animationMaxIter);
                    int isometricCoorX = (sizeCell/2) * fieldY;
                    int isometricCoorY = 0;
                    isometricCoorX = (field->getSizeCell()/2) * (fieldY - (bulletCellY));
                    isometricCoorY = (field->getSizeCell()/4) * (bulletCellY);
                    int mainX = mainCoorMapX + isometricCoorX + bulletCellX*(sizeCellX/2);
                    int mainY = mainCoorMapY + isometricCoorY + bulletCellX*(sizeCellY/2);
                    int pxlsX = mainX - sizeCellX/2;
                    int pxlsY = mainY - sizeCellY;
                    if(bulletCellX > lastX && bulletCellY > lastY) {
                        pxlsY -= (sizeCellY/animationMaxIter)*(animationMaxIter-animationCurrIter);
                    } else if(bulletCellX == lastX && bulletCellY > lastY) {
                        pxlsX += (sizeCellX/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
                        pxlsY -= (sizeCellY/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
                    } else if (bulletCellX < lastX && bulletCellY > lastY) {
                        pxlsX += (sizeCellX/animationMaxIter)*(animationMaxIter-animationCurrIter);
                    } else if (bulletCellX < lastX && bulletCellY == lastY) {
                        pxlsX += (sizeCellX/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
                        pxlsY += (sizeCellY/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
                    } else if (bulletCellX < lastX && bulletCellY < lastY) {
                        pxlsY += (sizeCellY/animationMaxIter)*(animationMaxIter-animationCurrIter);
                    } else if (bulletCellX == lastX && bulletCellY < lastY) {
                        pxlsX -= (sizeCellX/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
                        pxlsY += (sizeCellY/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
                    } else if (bulletCellX > lastX && bulletCellY < lastY) {
                        pxlsX -= (sizeCellX/animationMaxIter)*(animationMaxIter-animationCurrIter);
                    } else if (bulletCellX > lastX && bulletCellY == lastY) {
                        pxlsX -= (sizeCellX/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
                        pxlsY -= (sizeCellY/2/animationMaxIter)*(animationMaxIter-animationCurrIter);
                    }
                    painter.drawPixmap(pxlsX, pxlsY+sizeCellY/2, sizeCellX, sizeCellY*2, pixmap);
                }
            }
        }
    }
}

void GameWidget::drawUnits() {
    int fieldX = field->getSizeX();
    int fieldY = field->getSizeY();
    int mainCoorMapX = field->getMainCoorMapX();
    int mainCoorMapY = field->getMainCoorMapY();
    int spaceWidget = field->getSpaceWidget();
    int sizeCell = field->getSizeCell();
    int isometricCoorX = (sizeCell/2) * fieldY;
    int isometricCoorY = 0;
    int sizeCellX = field->getSizeCell();
    int sizeCellY = sizeCellX/2;
    for(int y = 0; y < fieldY; y++) {
        for(int x = 0; x < fieldX; x++) {
            int num = field->containUnit(x, y);
            if(num) {
                std::vector<Unit*> units = field->getCell(x, y)->getUnits();
                int size = units.size();
                for(int k = 0; k < size; k++) {
                    if(units[k]->alive || units[k]->preDeath) {// fixed!!!
                        int lastX, lastY;
                        int animationCurrIter, animationMaxIter;
                        QPixmap pixmap = units[k]->getAnimationInformation(&lastX, &lastY, &animationCurrIter, &animationMaxIter);
                        if(!field->getIsometric()) {
                            int pxlsX = mainCoorMapX + spaceWidget + x*sizeCell;//+1;
                            int pxlsY = mainCoorMapY + spaceWidget + y*sizeCell;// - sizeCell/2;//+1;
                            int localSizeCell = sizeCell;//-1;
                            int localSpaceCell = sizeCell/3;
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
                            painter.drawPixmap(pxlsX, pxlsY, localSizeCell + localSpaceCell*2, localSizeCell + localSpaceCell*2, pixmap);
                            int maxHP = 100;
                            int hp = units[k]->hp;
                            float hpWidth = localSizeCell-5;
                            hpWidth = hpWidth/maxHP*hp;
                            painter.drawRect(pxlsX + localSpaceCell+2, pxlsY, localSizeCell-4, 10);
                            painter.fillRect(pxlsX + localSpaceCell+3, pxlsY+1, hpWidth, 9, QColor(Qt::green));
                            units[k]->coorByMapX = pxlsX;
                            units[k]->coorByMapY = pxlsY;
                        } else {
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
                            painter.drawPixmap(pxlsX, pxlsY+sizeCellY/2, sizeCellX, sizeCellY*2, pixmap);
                        }
                    }
                }
            }
        }
        isometricCoorX = (field->getSizeCell()/2) * (fieldY - (y+1));
        isometricCoorY = (field->getSizeCell()/4) * (y+1);
    }
}

void GameWidget::drawBlackTiles() {
    int fieldX = field->getSizeX();
    int fieldY = field->getSizeY();
    int mainCoorMapX = field->getMainCoorMapX();
    int mainCoorMapY = field->getMainCoorMapY();
    int spaceWidget = field->getSpaceWidget();
    int sizeCell = field->getSizeCell();
    int sizeCellX = field->getSizeCell();
    int sizeCellY = sizeCellX/2;
    int isometricCoorX = (sizeCell/2) * fieldY;
    int isometricCoorY = 0;
    painter.setPen(QColor(255,0,0));
    for (int y = 0; y < fieldY; y++) {
        for (int x = 0; x < fieldX; x++) {
            if (!field->getIsometric()) {
                int pxlsX = mainCoorMapX + spaceWidget + x*sizeCell+1;
                int pxlsY = mainCoorMapY + spaceWidget + y*sizeCell+1;
                int localSizeCell = sizeCell-1;
                int localSpaceCell = sizeCell/4;
                painter.drawPixmap(sizeCell, 0, global_pixmap.width(), global_pixmap.height(), global_pixmap);
                if(field->getStepCell(x, y))
                    painter.drawText(pxlsX + sizeCell/2-5, pxlsY + sizeCell/2+5, QString("%1").arg(field->getStepCell(x, y)));
                if(field->isSetSpawnPoint(x,y))
                    painter.fillRect(pxlsX + localSpaceCell, pxlsY + localSpaceCell, localSizeCell - 2*(localSpaceCell), localSizeCell - 2*(localSpaceCell), QColor(255, 162, 0));
                if(field->isSetExitPoint(x,y))
                    painter.fillRect(pxlsX + localSpaceCell, pxlsY + localSpaceCell, localSizeCell - 2*(localSpaceCell), localSizeCell - 2*(localSpaceCell), QColor(0, 255, 0));
            } else {
                int x2 = mainCoorMapX + isometricCoorX - (sizeCellX/2) + x*(sizeCellX/2);
                int y2 = mainCoorMapY + isometricCoorY - (sizeCellY) + x*(sizeCellY/2);
                if (field->pathFinder.detectCollision({x, y})) {
                    painter.drawPixmap(x2, y2, sizeCellX, sizeCellY*2, global_pixmap);
                }
            }
        }
        isometricCoorX = (field->getSizeCell()/2) * (fieldY - (y+1));
        isometricCoorY = (field->getSizeCell()/4) * (y+1);
    }
}

void GameWidget::drawPaths() {
    int fieldX = field->getSizeX();
    int fieldY = field->getSizeY();
    int mainCoorMapX = field->getMainCoorMapX();
    int mainCoorMapY = field->getMainCoorMapY();
    int sizeCell = field->getSizeCell();
    int sizeCellX = field->getSizeCell();
    int sizeCellY = sizeCellX/2;
    int isometricCoorX = (sizeCell/2) * fieldY;
    int isometricCoorY = 0;
    painter.setPen(QColor(255,0,0));

    QPixmap pixmapPathPoint;
    for (int u = 0; u < field->unitsManager->getAmount(); u++) {
        Unit* tmpUnit = field->unitsManager->getUnit(u);
        if (tmpUnit->type == 0) {
            pixmapPathPoint = global_pixmap_PathPoint;
        } else {
            pixmapPathPoint = global_pixmap_EnemyPathPoint;
        }
        AStar::CoordinateList unitPath = tmpUnit->path;
        if (!unitPath.empty()) {
            for (int k = 1; k < unitPath.size(); k++) {
                AStar::Vec2i point = unitPath[k];
                isometricCoorX = (field->getSizeCell()/2) * (fieldY - (point.y+0));
                isometricCoorY = (field->getSizeCell()/4) * (point.y+0);
                int pPx = mainCoorMapX + isometricCoorX - (sizeCellX/2) + point.x*(sizeCellX/2);
                int pPy = mainCoorMapY + isometricCoorY - (sizeCellY) + point.x*(sizeCellY/2);
                painter.drawPixmap(pPx, pPy, sizeCellX, sizeCellY*2, pixmapPathPoint);
            }
            AStar::Vec2i destinationPoint = unitPath.front();
            isometricCoorX = (field->getSizeCell()/2) * (fieldY - (destinationPoint.y+0));
            isometricCoorY = (field->getSizeCell()/4) * (destinationPoint.y+0);
            int destinationPointX = mainCoorMapX + isometricCoorX - (sizeCellX/2) + destinationPoint.x*(sizeCellX/2);
            int destinationPointY = mainCoorMapY + isometricCoorY - (sizeCellY) + destinationPoint.x*(sizeCellY/2);
            painter.drawPixmap(destinationPointX, destinationPointY, sizeCellX, sizeCellY*2, global_pixmap_DestinationPoint);
        }
    }
}


void GameWidget::drawTowerUnderConstruction(int buildX, int buildY, TemplateForTower *tower) {
    int mainCoorMapX = field->getMainCoorMapX();
    int mainCoorMapY = field->getMainCoorMapY();
    int spaceWidget = field->getSpaceWidget();
    int sizeCell = field->getSizeCell();

    QPixmap towerPix = tower->pixmap;
    int towerSize = tower->size;
    int pixSizeCell = towerPix.width() / towerSize;
    QColor cGreen(0, 255, 0, 80);
    QColor cRed(255, 0, 0, 80);
    for(int x = 0; x < towerSize; x++) {
        for(int y = 0; y < towerSize; y++) {
            QPixmap pix = towerPix.copy(x*pixSizeCell, y*pixSizeCell, pixSizeCell, pixSizeCell);
            if(!field->getIsometric()) {
                int pxlsX = mainCoorMapX + spaceWidget + (buildX+x)*sizeCell;//+1;
                int pxlsY = mainCoorMapY + spaceWidget + (buildY+y)*sizeCell;//+1;
                painter.drawPixmap(pxlsX, pxlsY, sizeCell, sizeCell, pix);
                if(field->getCell(buildX+x, buildY+y)->isEmpty()) {
                    painter.fillRect(pxlsX, pxlsY, sizeCell, sizeCell, cGreen);
                } else {
                    painter.fillRect(pxlsX, pxlsY, sizeCell, sizeCell, cRed);
                }
            } else {
                int sizeCellX = sizeCell;
                int sizeCellY = sizeCellX/2;
                int height = tower->height;
                int isometricSpaceX = (field->getSizeY()-(buildY+y))*(sizeCellX/2);
                int isometricSpaceY = (buildY+y)*(sizeCellY/2);
                int pxlsX = mainCoorMapX + isometricSpaceX+spaceWidget + (buildX+x)*(sizeCellX/2);
                int pxlsY = mainCoorMapY + isometricSpaceY+spaceWidget + (buildX+x)*(sizeCellY/2);
                QPixmap pix = tower->pixmap;
                if(field->getCell(buildX+x, buildY+y)->isEmpty()) {
                    painter.setOpacity(0.5);
                    painter.drawPixmap(pxlsX - sizeCellX/2, pxlsY + sizeCellY - (sizeCellY*2)*height, sizeCellX, (sizeCellY*2)*height, pix);
                } else {
                    painter.setOpacity(0.5);
                    painter.drawPixmap(pxlsX - sizeCellX/2, pxlsY + sizeCellY - (sizeCellY*2)*height, sizeCellX, (sizeCellY*2)*height, pix);
                    QPainter painter(&pix);
                    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
                    painter.fillRect(pix.rect(), cRed);
                    painter.end();
                    painter.drawPixmap(pxlsX - sizeCellX/2, pxlsY + sizeCellY - (sizeCellY*2)*height, sizeCellX, (sizeCellY*2)*height, pix);
                }
            }
        }
    }
}

bool GameWidget::whichCell(int &mouseX, int &mouseY) {
    int mainCoorMapX = field->getMainCoorMapX();
    int mainCoorMapY = field->getMainCoorMapY();
    int spaceWidget = field->getSpaceWidget();
    int sizeCell = field->getSizeCell();
    int gameX, gameY;
    if(!field->getIsometric()) {
        gameX = ( (mouseX+sizeCell - spaceWidget - mainCoorMapX) / sizeCell);
        gameY = ( (mouseY+sizeCell - spaceWidget - mainCoorMapY) / sizeCell);
    } else {
        int fieldX = field->getSizeX();
        int fieldY = field->getSizeY();
        int sizeCellX = field->getSizeCell();
        int sizeCellY = sizeCellX/2;
        int isometricCoorX = (sizeCell/2) * fieldY;
        int isometricCoorY = 0;
        int localMouseX = -mainCoorMapX + mouseX - isometricCoorX;
        int localMouseY = -mainCoorMapY + mouseY + sizeCellY;
        gameX = (localMouseX/2 + localMouseY) / (sizeCell/2);
        gameY = -(localMouseX/2 - localMouseY) / (sizeCell/2);
    }
    if(gameX > 0 && gameX < field->getSizeX()+1) {
        if(gameY > 0 && gameY < field->getSizeY()+1) {
            mouseX = gameX-1;
            mouseY = gameY-1;
            return true;
        }
    }
    return false;
}

void GameWidget::startTimer_UnitsMoveAndTowerAttack() {
    qDebug() << "GameWidget::startTimer_UnitsMoveAndTowerAttack(); -- ";
    if(field->isSetSpawnPoint()) {
        if(unitsMove_TimerId) {
            killTimer(unitsMove_TimerId);
        }
        if(towersAttack_TimerId) {
            killTimer(towersAttack_TimerId);
        }
        unitsMove_TimerId = startTimer(unitsMove_TimerMilliSec);
        towersAttack_TimerId = startTimer(towersAttack_TimerMilliSec);
    }
}

void GameWidget::stopTimer_UnitsMoveAndTowerAttack() {
    qDebug() << "GameWidget::stopTimer_UnitsMoveAndTowerAttack(); -- ";
    if (unitsMove_TimerId) {
        killTimer(unitsMove_TimerId);
        unitsMove_TimerId = 0;
    }
    if (towersAttack_TimerId) {
        killTimer(towersAttack_TimerId);
        towersAttack_TimerId = 0;
    }
}

void GameWidget::buildTower(int x, int y) {
    if (x == -1 && y == -1) {
        qDebug() << "GameWidget:1:buildTower(" << x << ", " << y << "); -- ";
        std::vector<TemplateForTower*> towers = field->factionsManager->getAllTemplateForTowers();
        int size = towers.size();
        QMessageBox msgBox;
        msgBox.setText("Какую башню ты хочешь построить?");
        for(int k = 0; k < size; k++) {
            QPushButton* b1 = new QPushButton(QIcon(towers[k]->pixmap), QString());
            msgBox.addButton(b1, QMessageBox::AcceptRole);
        }
    } else {
        qDebug() << "GameWidget:2:buildTower(" << x << ", " << y << "); -- ";
    }
}

void GameWidget::mousePressEvent(QMouseEvent* event) {
    Qt::MouseButton button = event->button();
    int mouseX = event->x();
    int mouseY = event->y();

    if(whichCell(mouseX,mouseY)) {
        if ( (!panMidMouseButton && button == Qt::RightButton) || (panMidMouseButton && button == Qt::MidButton) ) {
            pan = true;
            setCursor(Qt::ClosedHandCursor);
        }
        prevMouseX = event->x();
        prevMouseY = event->y();
        prevMouseCellX = mouseX;
        prevMouseCellY = mouseY;
        prevGlobalMouseX = event->globalX();
        prevGlobalMouseY = event->globalY();
    }
    update();
}

void GameWidget::mouseReleaseEvent(QMouseEvent* event) {
    Qt::MouseButton button = event->button();
    int mouseX = event->x();
    int mouseY = event->y();

    if (whichCell(mouseX, mouseY)) {
        if ( (!panMidMouseButton && button == Qt::RightButton) || (panMidMouseButton && button == Qt::MidButton)) {
            pan = false;
            setCursor(Qt::ArrowCursor);
        }
        if (button == Qt::LeftButton) {
            if (field->getCell(mouseX, mouseY)->isEmpty()) {
                field->updateHeroDestinationPoint(mouseX, mouseY);
            }
        } else if (button == Qt::RightButton) {
            if ( panMidMouseButton || (prevMouseCellX == mouseX && prevMouseCellY == mouseY && prevGlobalMouseX == event->globalX() && prevGlobalMouseY == event->globalY()) ) {
                Cell* cell = field->getCell(mouseX, mouseY);
                if (cell != NULL) {
                    if(cell->isEmpty()) {
                        int randNumber = ( 124+(rand()%2) );
                        QPixmap pixmap = field->map->tileSets.getTileSet(0)->tiles[randNumber]->getPixmap();
                        cell->setTerrain(pixmap);
                    } else if (cell->isTerrain()) {
                        cell->removeTerrain();
                    } else {
                        qDebug() << "GameWidget::mouseReleaseEvent(); -- RightButton! cell bad:" << cell;
                    }
                }
                field->updateHeroDestinationPoint();
            }
        }
    }
    update();
}

void GameWidget::mouseMoveEvent(QMouseEvent * event) {
    if (pan) {
        int mainCoorMapX = field->getMainCoorMapX();
        int mainCoorMapY = field->getMainCoorMapY();
        int x = event->x();
        int y = event->y();

        mainCoorMapX = mainCoorMapX + ((x-prevMouseX)/1);
        mainCoorMapY = mainCoorMapY + ((y-prevMouseY)/1);
        field->setMainCoorMap(mainCoorMapX, mainCoorMapY);
        prevMouseX = event->x();
        prevMouseY = event->y();
        update();
    }
}

void GameWidget::wheelEvent(QWheelEvent* event) {
    int mainCoorMapX = field->getMainCoorMapX();
    int mainCoorMapY = field->getMainCoorMapY();
    int sizeCell = field->getSizeCell();

    QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta() / 8;
    if (!numPixels.isNull()) {
        qDebug() << "numPixels: " << numPixels;
    } else if (!numDegrees.isNull()) {
        QPoint numSteps = numDegrees / 15;
        int wheelX = numSteps.x();
        int wheelY = numSteps.y();
        if(wheelX == 0 && wheelY != 0) {
            if(wheelY > 0) {
                if(sizeCell < zoomMaxSizeCell) {
                    for(int k = 0; k < wheelY; k++) {
                        sizeCell += zoomSizeCell;
                        mainCoorMapX -= (zoomSizeCell*field->getSizeX()/2);
                        mainCoorMapY -= (zoomSizeCell*field->getSizeY()/2);
                        int mouseMapX = event->x();
                        int mouseMapY = event->y();
                        int mapCenterX = width()/2;
                        int mapCenterY = height()/2;
                        mainCoorMapX -= (mouseMapX-mapCenterX);
                        mainCoorMapY -= (mouseMapY-mapCenterY);
                    }
                }
            } else {
                if(sizeCell > zoomMinSizeCell) {
                    wheelY = -wheelY;
                    for(int k = 0; k < wheelY; k++) {
                        sizeCell -= zoomSizeCell;
                        mainCoorMapX += (zoomSizeCell*field->getSizeX()/2);
                        mainCoorMapY += (zoomSizeCell*field->getSizeY()/2);
                    }
                }
            }
            mainCoorMapX = (mainCoorMapX > 0) ? 0 : mainCoorMapX;
            mainCoorMapY = (mainCoorMapY > 0) ? 0 : mainCoorMapY;
            mainCoorMapX = (mainCoorMapX + sizeCell*field->getSizeX() < width()) ? width()-sizeCell*field->getSizeX() : mainCoorMapX;
            mainCoorMapY = (mainCoorMapY + sizeCell*field->getSizeY() < height()) ? height()-sizeCell*field->getSizeY() : mainCoorMapY;
            field->setMainCoorMap(mainCoorMapX, mainCoorMapY);
            field->setSizeCell(sizeCell);
        }
    }
}

void GameWidget::on_closeWidget_clicked() {
    signal_closeWidget();
}
