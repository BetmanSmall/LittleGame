#include "src/head/gamewidget.h"

GameWidget::GameWidget(QString mapFile, FactionsManager* factionsManager,
                       int enemyCount, int difficultyLevel, int towersCount,
                       QWidget *parent):
    QWidget(parent),
    ui(new Ui::GameWidget)
{
    ui->setupUi(this);

    field = new Field(mapFile, factionsManager, enemyCount, difficultyLevel, towersCount);
    cameraController = new CameraController(field->map->tileWidth, field->map->tileHeight);

    timeOfGame = 0;
    gameTimer = new QTimer(this);
    connect(gameTimer, SIGNAL(timeout()), this, SLOT(update()));
    gameTimer->start(0);

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
//    if (scanMouseMove_TimerId)
//        killTimer(scanMouseMove_TimerId);
    delete field;
    delete cameraController;
    delete gameTimer;
    delete ui;
}

void GameWidget::timerEvent(QTimerEvent *event) {
//    int timerId = event->timerId();
//    if(timerId == scanMouseMove_TimerId) {
//        int curX = cursor().pos().x();
//        int curY = cursor().pos().y();
//        if(curX == 0 || curY == 0 || curX == width()-1 || curY == height()-1) {
//            int mainCoorMapX = field->getMainCoorMapX();
//            int mainCoorMapY = field->getMainCoorMapY();
//            int sizeCell = field->getSizeCell();
//            int sizeFieldX = field->sizeFieldX;
//            int sizeFieldY = field->sizeFieldY;

//            if(curX == 0)
//                if(mainCoorMapX < 0)
//                    mainCoorMapX += pixelsShiftMap;
//            if(curY == 0)
//                if(mainCoorMapY < 0)
//                    mainCoorMapY += pixelsShiftMap;
//            if(curX == width()-1)
//                if(mainCoorMapX+sizeFieldX*sizeCell > width())
//                    mainCoorMapX -= pixelsShiftMap;
//            if(curY == height()-1)
//                if(mainCoorMapY+sizeFieldY*sizeCell > height())
//                    mainCoorMapY -= pixelsShiftMap;

//            mainCoorMapX = (mainCoorMapX > 0) ? 0 : mainCoorMapX;
//            mainCoorMapY = (mainCoorMapY > 0) ? 0 : mainCoorMapY;
//            mainCoorMapX = (mainCoorMapX + sizeCell*sizeFieldX < width()) ? width()-sizeCell*sizeFieldX : mainCoorMapX;
//            mainCoorMapY = (mainCoorMapY + sizeCell*sizeFieldY < height()) ? height()-sizeCell*sizeFieldY : mainCoorMapY;
//            field->setMainCoorMap(mainCoorMapX, mainCoorMapY);
//        }
//    }
//    update();
}

void GameWidget::paintEvent(QPaintEvent* event) {
    lastTime = currentTime;
    currentTime = QTime::currentTime();
    elapsedTime = (currentTime.second()*1000 + currentTime.msec()) - (lastTime.second()*1000 + lastTime.msec()); // /!\ max = 59 seconds 999 ms
    this->fps = 1000 / elapsedTime;

    cameraController->painter->begin(this);
    field->render(elapsedTime, cameraController);
    cameraController->update(elapsedTime);
    cameraController->painter->setPen(QColor(255, 0, 0));
    cameraController->painter->drawEllipse(QPoint(cameraController->cameraX, cameraController->cameraY), 5, 5);
    cameraController->painter->drawText(10, 10, "FPS:" + QString::number(fps));
    cameraController->painter->drawText(10, 20, "lastTime:" + QString::number(lastTime.msec()));
    cameraController->painter->drawText(10, 30, "currentTime:" + QString::number(currentTime.msec()));
    cameraController->painter->drawText(10, 40, "elapsedTime:" + QString::number(elapsedTime));
    cameraController->painter->drawText(10, 50, "cameraController->cameraX:" + QString::number(cameraController->cameraX));
    cameraController->painter->drawText(10, 60, "cameraController->cameraY:" + QString::number(cameraController->cameraY));
    cameraController->painter->drawText(10, 70, "cameraController->sizeCellX:" + QString::number(cameraController->sizeCellX));
    cameraController->painter->drawText(10, 80, "cameraController->sizeCellY:" + QString::number(cameraController->sizeCellY));
    cameraController->painter->drawText(10, 90, "cameraController->halfSizeCellX:" + QString::number(cameraController->halfSizeCellX));
    cameraController->painter->drawText(10, 100, "cameraController->halfSizeCellY:" + QString::number(cameraController->halfSizeCellY));
    cameraController->painter->drawText(10, 110, "cameraController->paning:" + QString::number(cameraController->paning));
    cameraController->painter->drawText(10, 120, "cameraController->flinging:" + QString::number(cameraController->flinging));
    cameraController->painter->drawText(10, 130, "field->drawOrder:" + QString::number(field->drawOrder));
    cameraController->painter->end();
}

//bool GameWidget::whichCell(int &mouseX, int &mouseY) {
//    int mainCoorMapX = field->getMainCoorMapX();
//    int mainCoorMapY = field->getMainCoorMapY();
//    int sizeCell = field->getSizeCell();
//    int gameX, gameY;
//    if(!field->isometric) {
//        gameX = ( (mouseX+sizeCell - mainCoorMapX) / sizeCell);
//        gameY = ( (mouseY+sizeCell - mainCoorMapY) / sizeCell);
//    } else {
//        int fieldX = field->sizeFieldX;
//        int fieldY = field->sizeFieldY;
//        int sizeCellX = field->getSizeCell();
//        int sizeCellY = sizeCellX/2;
//        int isometricCoorX = (sizeCell/2) * fieldY;
//        int isometricCoorY = 0;
//        int localMouseX = -mainCoorMapX + mouseX - isometricCoorX;
//        int localMouseY = -mainCoorMapY + mouseY + sizeCellY;
//        gameX = (localMouseX/2 + localMouseY) / (sizeCell/2);
//        gameY = -(localMouseX/2 - localMouseY) / (sizeCell/2);
//    }
//    if(gameX > 0 && gameX < field->sizeFieldX+1) {
//        if(gameY > 0 && gameY < field->sizeFieldY+1) {
//            mouseX = gameX-1;
//            mouseY = gameY-1;
//            return true;
//        }
//    }
//    return false;
//}

//void GameWidget::startTimer_UnitsMoveAndTowerAttack() {
//    qDebug() << "GameWidget::startTimer_UnitsMoveAndTowerAttack(); -- ";
//    if(field->isSetSpawnPoint()) {
//        if(unitsMove_TimerId) {
//            killTimer(unitsMove_TimerId);
//        }
//        if(towersAttack_TimerId) {
//            killTimer(towersAttack_TimerId);
//        }
//        unitsMove_TimerId = startTimer(unitsMove_TimerMilliSec);
//        towersAttack_TimerId = startTimer(towersAttack_TimerMilliSec);
//    }
//}

//void GameWidget::buildTower(int x, int y) {
//    if (x == -1 && y == -1) {
//        qDebug() << "GameWidget:1:buildTower(" << x << ", " << y << "); -- ";
//        std::vector<TemplateForTower*> towers = field->factionsManager->getAllTemplateForTowers();
//        int size = towers.size();
//        QMessageBox msgBox;
//        msgBox.setText("Какую башню ты хочешь построить?");
//        for(int k = 0; k < size; k++) {
//            QPushButton* b1 = new QPushButton(QIcon(towers[k]->pixmap), QString());
//            msgBox.addButton(b1, QMessageBox::AcceptRole);
//        }
//    } else {
//        qDebug() << "GameWidget:2:buildTower(" << x << ", " << y << "); -- ";
//    }
//}

void GameWidget::keyPressEvent(QKeyEvent * event) {
//    int mainCoorMapX = field->getMainCoorMapX();
//    int mainCoorMapY = field->getMainCoorMapY();
//    int sizeCell = field->getSizeCell();
//    int sizeFieldX = field->sizeFieldX;
//    int sizeFieldY = field->sizeFieldY;
    int key = event->key();
    qDebug() << "GameWidget::keyPressEvent(); -- key: " << key;
    if(key == Qt::Key_0) {
        signal_changeWindowState();
        qDebug() << "GameWidget::keyPressEvent(); -- parentWidget()->windowState():" << parentWidget()->windowState();
    } else if(key == Qt::Key_1) {
        ui->drawGrid_checkBox->toggle();
        qDebug() << "GameWidget::keyPressEvent(); -- isKeyJustPressed(Input.Keys.NUM_1 || Input.Keys.NUMPAD_1)";
        field->isDrawableGrid--;
        if (field->isDrawableGrid < 0) {
            field->isDrawableGrid = 5;
        }
//        gameInterface.addActionToHistory("-- field->isDrawableGrid:" + field->isDrawableGrid);
        qDebug() << "GameWidget::keyPressEvent(); -- field->isDrawableGrid: " << field->isDrawableGrid;
    } else if(key == Qt::Key_2) {
        ui->drawUnits_checkBox->toggle();
        qDebug() << "GameWidget::keyPressEvent(); -- isKeyJustPressed(Input.Keys.NUM_2 || Input.Keys.NUMPAD_2)";
        field->isDrawableUnits--;
        if (field->isDrawableUnits < 0) {
            field->isDrawableUnits = 5;
        }
//        gameInterface.addActionToHistory("-- field->isDrawableUnits:" + field->isDrawableUnits);
        qDebug() << "GameWidget::keyPressEvent(); -- field->isDrawableUnits: " << field->isDrawableUnits;
    } else if(key == Qt::Key_3) {
        ui->drawTowersByTowers_checkBox->toggle();
        qDebug() << "GameWidget::keyPressEvent(); -- isKeyJustPressed(Input.Keys.NUM_3 || Input.Keys.NUMPAD_3)";
        field->isDrawableTowers--;
        if (field->isDrawableTowers < 0) {
            field->isDrawableTowers = 5;
        }
//        gameInterface.addActionToHistory("-- field->isDrawableTowers:" + field->isDrawableTowers);
        qDebug() << "GameWidget::keyPressEvent(); -- field->isDrawableTowers: " << field->isDrawableTowers;
    } else if(key == Qt::Key_4) {
        ui->drawBackGround_checkBox->toggle();
        qDebug() << "GameWidget::keyPressEvent(); -- isKeyJustPressed(Input.Keys.NUM_4 || Input.Keys.NUMPAD_4)";
        field->isDrawableBackground--;
        if (field->isDrawableBackground < 0) {
            field->isDrawableBackground = 5;
        }
//        gameInterface.addActionToHistory("-- field->isDrawableBackground:" + field->isDrawableBackground);
        qDebug() << "GameWidget::keyPressEvent(); -- field->isDrawableBackground: " << field->isDrawableBackground;
    } else if(key == Qt::Key_5) {
        ui->drawGround_checkBox->toggle();
        qDebug() << "GameWidget::keyPressEvent(); -- isKeyJustPressed(Input.Keys.NUM_6 || Input.Keys.NUMPAD_6)";
        field->isDrawableGround--;
        if (field->isDrawableGround < 0) {
            field->isDrawableGround = 5;
        }
//        gameInterface.addActionToHistory("-- field->isDrawableGround:" + field->isDrawableGround);
        qDebug() << "GameWidget::keyPressEvent(); -- field->isDrawableGround: " << field->isDrawableGround;
    } else if(key == Qt::Key_6) {
        ui->drawForeGround_checkBox->toggle();
        qDebug() << "GameWidget::keyPressEvent(); -- isKeyJustPressed(Input.Keys.NUM_6 || Input.Keys.NUMPAD_6)";
        field->isDrawableForeground--;
        if (field->isDrawableForeground < 0) {
            field->isDrawableForeground = 5;
        }
//        gameInterface.addActionToHistory("-- field->isDrawableForeground:" + field->isDrawableForeground);
        qDebug() << "GameWidget::keyPressEvent(); -- field->isDrawableForeground: " << field->isDrawableForeground;
    } else if(key == Qt::Key_7) {
        ui->drawBlackTiles_checkBox->toggle();
        qDebug() << "GameWidget::keyPressEvent(); -- isKeyJustPressed(Input.Keys.NUM_6 || Input.Keys.NUMPAD_6)";
        field->isDrawableGridNav--;
        if (field->isDrawableGridNav < 0) {
            field->isDrawableGridNav = 5;
        }
//        gameInterface.addActionToHistory("-- field->isDrawableGridNav:" + field->isDrawableGridNav);
        qDebug() << "GameWidget::keyPressEvent(); -- field->isDrawableGridNav: " << field->isDrawableGridNav;
    } else if(key == Qt::Key_8) {
        ui->drawPaths_checkBox->toggle();
        qDebug() << "GameWidget::keyPressEvent(); -- isKeyJustPressed(Input.Keys.NUM_6 || Input.Keys.NUMPAD_6)";
        field->isDrawableRoutes--;
        if (field->isDrawableRoutes < 0) {
            field->isDrawableRoutes = 5;
        }
//        gameInterface.addActionToHistory("-- field->isDrawableRoutes:" + field->isDrawableRoutes);
        qDebug() << "GameWidget::keyPressEvent(); -- field->isDrawableRoutes: " << field->isDrawableRoutes;
    } else if(key == Qt::Key_9) {
        qDebug() << "GameWidget::keyPressEvent(); -- isKeyJustPressed(Input.Keys.NUM_9 || Input.Keys.NUMPAD_9)";
        field->drawOrder++;
        if (field->drawOrder > 8) {
            field->drawOrder = 0;
        }
//        gameInterface.addActionToHistory("-- field->drawOrder:" + field->drawOrder);
        qDebug() << "GameWidget::keyPressEvent(); -- field->drawOrder: " << field->drawOrder;
    } else if(key == Qt::Key_Space || key == Qt::Key_Escape) {
//        gamePause = !gamePause;
//        qDebug() << "GameWidget::keyPressEvent(); -- gamePause: " << gamePause;
    } else if (key == Qt::Key_Enter) {
        signal_closeWidget();
        return;
    } else if (key == Qt::Key_A) {
        qDebug() << "GameScreen::inputHandler(); -- isKeyJustPressed(Input.Keys.A)";
        field->turnLeft();
//        gameInterface.addActionToHistory("-- gameField.turnLeft()");
        qDebug() << "GameScreen::inputHandler(); -- gameField.turnLeft()";
    } else if (key == Qt::Key_S) {
        qDebug() << "GameScreen::inputHandler(); -- -- isKeyJustPressed(Input.Keys.S)";
        field->turnRight();
//        gameInterface.addActionToHistory("-- gameField.turnRight()");
        qDebug() << "GameScreen::inputHandler(); -- gameField.turnRight()";
    } else if (key == Qt::Key_Q) {
        qDebug() << "GameScreen::inputHandler(); -- isKeyJustPressed(Input.Keys.Q)";
        field->flipX();
//        gameInterface.addActionToHistory("-- gameField.flipX()");
        qDebug() << "GameScreen::inputHandler(); -- gameField.flipX()";
    } else if (key == Qt::Key_W) {
        qDebug() << "GameScreen::inputHandler(); -- isKeyJustPressed(Input.Keys.W)";
        field->flipY();
//        gameInterface.addActionToHistory("-- gameField.flipY()");
        qDebug() << "GameScreen::inputHandler(); -- gameField.flipY()";
//    } else if(key == Qt::Key_Left) {
//        if(mainCoorMapX < 0) {
//            mainCoorMapX += pixelsShiftMap;
//        }
//    } else if(key == Qt::Key_Up) {
//        if(mainCoorMapY < 0) {
//            mainCoorMapY += pixelsShiftMap;
//        }
//    } else if(key == Qt::Key_Right) {
//        if(mainCoorMapX+sizeFieldX*sizeCell > width()) {
//            mainCoorMapX -= pixelsShiftMap;
//        }
//    } else if(key == Qt::Key_Down) {
//        if(mainCoorMapY+sizeFieldY*sizeCell > height()) {
//            mainCoorMapY -= pixelsShiftMap;
//        }
    }
//    field->setMainCoorMap(mainCoorMapX, mainCoorMapY);
//    cameraController->x
}

void GameWidget::mousePressEvent(QMouseEvent* event) {
    qDebug() << "GameWidget::mousePressEvent(); -- event:" << event;
    int mouseX = event->x();
    int mouseY = event->y();
    Qt::MouseButton button = event->button();
    if ( (!cameraController->panMidMouseButton && button == Qt::RightButton) ||
          (cameraController->panMidMouseButton && button == Qt::MidButton) ) {
        cameraController->touchDown(mouseX, mouseY, 0, button);
        setCursor(Qt::ClosedHandCursor);
    }
//    if(whichCell(mouseX,mouseY)) {
//        prevMouseX = event->x();
//        prevMouseY = event->y();
//        prevMouseCellX = mouseX;
//        prevMouseCellY = mouseY;
//        prevGlobalMouseX = event->globalX();
//        prevGlobalMouseY = event->globalY();
//    }
}

void GameWidget::mouseReleaseEvent(QMouseEvent* event) {
    qDebug() << "GameWidget::mouseReleaseEvent(); -- event:" << event;
    int mouseX = event->x();
    int mouseY = event->y();
    Qt::MouseButton button = event->button();
    if ( (!cameraController->panMidMouseButton && button == Qt::RightButton) ||
          (cameraController->panMidMouseButton && button == Qt::MidButton) ) {
        cameraController->touchUp(mouseX, mouseY, 0, button);
        setCursor(Qt::ArrowCursor);
    }
//    if (whichCell(mouseX, mouseY)) {
//        if (button == Qt::LeftButton) {
//            if (field->getCell(mouseX, mouseY)->isEmpty()) {
//                field->updateHeroDestinationPoint(mouseX, mouseY);
//            }
//        } else if (button == Qt::RightButton) {
//            if ( panMidMouseButton || (prevMouseCellX == mouseX && prevMouseCellY == mouseY && prevGlobalMouseX == event->globalX() && prevGlobalMouseY == event->globalY()) ) {
//                Cell* cell = field->getCell(mouseX, mouseY);
//                if (cell != NULL) {
//                    if(cell->isEmpty()) {
//                        int randNumber = ( 124+(rand()%2) );
//                        QPixmap pixmap = field->map->tileSets.getTileSet(0)->tiles[randNumber]->getPixmap();
//                        cell->setTerrain(pixmap);
//                    } else if (cell->isTerrain()) {
//                        cell->removeTerrain();
//                    } else {
//                        qDebug() << "GameWidget::mouseReleaseEvent(); -- RightButton! cell bad:" << cell;
//                    }
//                }
//                field->updateHeroDestinationPoint();
//            }
//        }
//    }
}

void GameWidget::mouseMoveEvent(QMouseEvent* event) {
//    qDebug() << "GameWidget::mouseMoveEvent(); -- event:" << event;
    int mouseX = event->x();
    int mouseY = event->y();
    Qt::MouseButton button = event->button();
    cameraController->pan(mouseX, mouseY);
//    if (gameInterface.pan(x, y, deltaX, deltaY)) {
//        lastCircleTouched = true;
//        return true;
//    }
//    lastCircleTouched = false;
//    if (gameField.getUnderConstruction() == null || Gdx.input.isButtonPressed(Input.Buttons.RIGHT)) {
//        float newCameraX = camera.position.x + (-deltaX * camera.zoom);
//        float newCameraY = camera.position.y + (deltaY * camera.zoom);
//                camera.position.set(newCameraX, newCameraY, 0.0f);
//            }
//        } else {
//            camera.position.set(newCameraX, newCameraY, 0.0f);
//        }
//    } else {
//        float space = 50f;
//        float shiftCamera = 5f;
//        if (x < space) {
//            camera.position.add(-shiftCamera, 0.0f, 0.0f);
//        }
//        if (x > Gdx.graphics.getWidth() - space) {
//            camera.position.add(shiftCamera, 0.0f, 0.0f);
//        }
//        if (y < space) {
//            camera.position.add(0.0f, shiftCamera, 0.0f);
//        }
//        if (y > Gdx.graphics.getHeight() - space) {
//            camera.position.add(0.0f, -shiftCamera, 0.0f);
//        }
//    }
//    return false;
//    if (pan) {
////        int mainCoorMapX = field->getMainCoorMapX();
////        int mainCoorMapY = field->getMainCoorMapY();
//        int x = event->x();
//        int y = event->y();

//        mainCoorMapX = mainCoorMapX + ((x-prevMouseX)/1);
//        mainCoorMapY = mainCoorMapY + ((y-prevMouseY)/1);
//        field->setMainCoorMap(mainCoorMapX, mainCoorMapY);
//        prevMouseX = event->x();
//        prevMouseY = event->y();
//        update();
//    }
}

void GameWidget::wheelEvent(QWheelEvent* event) {
    qDebug() << "GameWidget::resizeEvent(); -- event->pixelDelta():" << event->pixelDelta();
    qDebug() << "GameWidget::resizeEvent(); -- event->angleDelta():" << event->angleDelta();

    cameraController->scrolled(event->angleDelta().y());
    field->updateCellsGraphicCoordinates(cameraController->halfSizeCellX, cameraController->halfSizeCellY);

//    int mainCoorMapX = field->getMainCoorMapX();
//    int mainCoorMapY = field->getMainCoorMapY();
//    int sizeCell = field->getSizeCell();
//    QPoint numPixels = event->pixelDelta();
//    QPoint numDegrees = event->angleDelta() / 8;
//    if (!numPixels.isNull()) {
//        qDebug() << "numPixels: " << numPixels;
//    } else if (!numDegrees.isNull()) {
//        QPoint numSteps = numDegrees / 15;
//        int wheelX = numSteps.x();
//        int wheelY = numSteps.y();
//        if(wheelX == 0 && wheelY != 0) {
//            if(wheelY > 0) {
//                if(sizeCell < zoomMaxSizeCell) {
//                    for(int k = 0; k < wheelY; k++) {
//                        sizeCell += zoomSizeCell;
//                        mainCoorMapX -= (zoomSizeCell*field->sizeFieldX/2);
//                        mainCoorMapY -= (zoomSizeCell*field->sizeFieldY/2);
//                        int mouseMapX = event->x();
//                        int mouseMapY = event->y();
//                        int mapCenterX = width()/2;
//                        int mapCenterY = height()/2;
//                        mainCoorMapX -= (mouseMapX-mapCenterX);
//                        mainCoorMapY -= (mouseMapY-mapCenterY);
//                    }
//                }
//            } else {
//                if(sizeCell > zoomMinSizeCell) {
//                    wheelY = -wheelY;
//                    for(int k = 0; k < wheelY; k++) {
//                        sizeCell -= zoomSizeCell;
//                        mainCoorMapX += (zoomSizeCell*field->sizeFieldX/2);
//                        mainCoorMapY += (zoomSizeCell*field->sizeFieldY/2);
//                    }
//                }
//            }
//            mainCoorMapX = (mainCoorMapX > 0) ? 0 : mainCoorMapX;
//            mainCoorMapY = (mainCoorMapY > 0) ? 0 : mainCoorMapY;
//            mainCoorMapX = (mainCoorMapX + sizeCell*field->sizeFieldX < width()) ? width()-sizeCell*field->sizeFieldX : mainCoorMapX;
//            mainCoorMapY = (mainCoorMapY + sizeCell*field->sizeFieldY < height()) ? height()-sizeCell*field->sizeFieldY : mainCoorMapY;
//            field->setMainCoorMap(mainCoorMapX, mainCoorMapY);
//            field->setSizeCell(sizeCell);
//        }
//    }
//    update();
}

void GameWidget::resizeEvent(QResizeEvent* event) {
    QSize newSize = event->size();
    qDebug() << "GameWidget::resizeEvent(); -- newSize" << newSize;
    cameraController->viewportWidth = newSize.width();
    cameraController->viewportHeight = newSize.height();
//    cameraController->update();
}

void GameWidget::on_closeWidget_clicked() {
    signal_closeWidget();
}
