#include "src/head/gamewidget.h"

GameWidget::GameWidget(QString mapFile, FactionsManager* factionsManager,
                       int enemyCount, int difficultyLevel, int towersCount,
                       QWidget *parent):
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

    painter = new QPainter();
    test = 0;
    field = new Field(mapFile, factionsManager, enemyCount, difficultyLevel, towersCount);

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
//                        field->waveAlgorithm();
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
    painter->begin(this);
    field->render(10, painter);
    painter->end();
}

bool GameWidget::whichCell(int &mouseX, int &mouseY) {
    int mainCoorMapX = field->getMainCoorMapX();
    int mainCoorMapY = field->getMainCoorMapY();
    int sizeCell = field->getSizeCell();
    int gameX, gameY;
    if(!field->isometric) {
        gameX = ( (mouseX+sizeCell - mainCoorMapX) / sizeCell);
        gameY = ( (mouseY+sizeCell - mainCoorMapY) / sizeCell);
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
