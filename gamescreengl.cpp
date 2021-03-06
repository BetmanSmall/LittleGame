#include "gamescreengl.h"

GameScreenGL::GameScreenGL(QString mapPath, FactionsManager* factionsManager,
                       GameSettings *gameSettings,
                       QWidget *parent):
    QOpenGLWidget(parent),
    ui(new Ui::GameScreenGL)
{
    ui->setupUi(this);

    ui->loadMaps->setVisible(false);
    ui->clearMap->setVisible(false);
//    ui->loadMaps->setVisible(false);

    gameField = new GameField(mapPath, factionsManager, gameSettings);
//    gameInterface = new GameInterface(gameField, bitmapFont);
//    gameInterface.mapNameLabel.setText("MapName:" + mapName);
    cameraController = new CameraController(gameField);

    gameTimer = new QTimer(this);
    connect(gameTimer, SIGNAL(timeout()), this, SLOT(update()));
    gameTimer->start(0);

    setMouseTracking(true);
//    setFixedSize(200, 200);
//    setAutoFillBackground(false);

    qDebug() << "GameScreenGL::GameScreenGL(); -- mapPath:" << mapPath;
    qDebug() << "GameScreenGL::GameScreenGL(); -- factionsManager:" << factionsManager;
    qDebug() << "GameScreenGL::GameScreenGL(); -- gameSettings:" << gameSettings;
    qDebug() << "GameScreenGL::GameScreenGL(); -- field:" << gameField;
    qDebug() << "GameScreenGL::GameScreenGL(); -- field->map:" << gameField->map;
    qDebug() << "GameScreenGL::GameScreenGL(); -END- -END-";
}

GameScreenGL::~GameScreenGL() {
    delete gameField;
    delete cameraController;
    delete gameTimer;
    delete ui;
}

void GameScreenGL::initializeGL() {
    initializeOpenGLFunctions();
}

void GameScreenGL::paintGL() {
    glClearColor(0, 0, 0, 255);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    lastTime = currentTime;
    currentTime = QTime::currentTime();
//    deltaTime = (currentTime.second()*1000 + currentTime.msec()) - (lastTime.second()*1000 + lastTime.msec()); // /!\ max = 59 seconds 999 ms
    deltaTime = (currentTime.msecsSinceStartOfDay()) - (lastTime.msecsSinceStartOfDay());
    this->fps = 1000 / deltaTime;
    deltaTime = deltaTime / 1000;

    cameraController->update(deltaTime);
    cameraController->painter->begin(this);
    cameraController->painter->scale(cameraController->zoom, cameraController->zoom);
//    cameraController->painter->setRenderHint(QPainter::Antialiasing);
//    cameraController->painter->fillRect(event->rect(), QColor(0, 0, 0));
    cameraController->painter->translate(cameraController->cameraX, cameraController->cameraY);
//    cameraController->painter->rotate(fps);

    gameField->render(deltaTime, cameraController);
    cameraController->painter->setPen(QColor(255, 0, 0));
    cameraController->painter->drawEllipse(QPoint(0, 0), 5, 5);
    cameraController->painter->end();

    cameraController->painter->begin(this);
//    cameraController->painter->scale(cameraController->zoom, -cameraController->zoom);
//    cameraController->painter->translate(-cameraController->cameraX, -cameraController->cameraY);
    cameraController->painter->setPen(QColor(0, 255, 0));
    cameraController->painter->drawEllipse(QPoint(0, 0), 5, 5);
    cameraController->painter->drawText(10, 10, "fps:" + QString::number(fps));
    cameraController->painter->drawText(10, 20, "deltaTime:" + QString::number(deltaTime));
    cameraController->painter->drawText(10, 30, "gameField->map->mapPath:" + gameField->map->mapPath);
    cameraController->painter->drawText(10, 40, "gameField->gameSettings->gameType:" + QString(GameType::to_string(gameField->gameSettings->gameType).c_str()));
    cameraController->painter->drawText(10, 50, "gameField->gameSettings->isometric:" + QString::number(gameField->gameSettings->isometric));
    UnderConstruction* underConstruction = gameField->getUnderConstruction();
    if (underConstruction != NULL) {
        cameraController->painter->drawText(10, 60, "underConstrEndCoord:(" + QString::number(underConstruction->endX) + "," + QString::number(underConstruction->endY) + ")");
        cameraController->painter->drawText(10, 70, "underConstrTemplateName:" + underConstruction->templateForTower->name);
    } else {
        cameraController->painter->drawText(10, 60, "underConstrEndCoord:(WTF,WTF)");
        cameraController->painter->drawText(10, 70, "underConstrTemplateName:NULL");
    }
    cameraController->painter->drawText(10, 80, "gameField->gamerGold:" + QString::number(gameField->gamerGold));
    cameraController->painter->drawText(10, 90, "gameField->unitsManager->units.size():" + QString::number(gameField->unitsManager->units.size()));
    cameraController->painter->drawText(10, 100, "gameField->towersManager->towers.size():" + QString::number(gameField->towersManager->towers.size()));
    cameraController->painter->drawText(10, 110, "UnitsLimitPL1:" + QString::number(gameField->gameSettings->missedUnitsForPlayer1) + "/" + QString::number(gameField->gameSettings->maxOfMissedUnitsForPlayer1));
    cameraController->painter->drawText(10, 120, "UnitsLimitComp0:" + QString::number(gameField->gameSettings->missedUnitsForComputer0) + "/" + QString::number(gameField->gameSettings->maxOfMissedUnitsForComputer0));
    cameraController->painter->drawText(10, 130, "NextUnitSpawnAfter:" + ((gameField->waveManager->waitForNextSpawnUnit > 0.0) ? QString::number(gameField->waveManager->waitForNextSpawnUnit) + "sec" : "PRESS_PLAY_BUTTON"));
    cameraController->painter->drawText(10, 140, "gameField->unitsSpawn:" + QString::number(gameField->unitsSpawn));
    cameraController->painter->drawText(10, 150, "gameField->gamePaused:" + QString::number(gameField->gamePaused));

    ui->goUnits->setText( (gameField->gamePaused) ? "PLAY" : (gameField->unitsSpawn) ? "PAUSE" : (gameField->unitsManager->units.size()>0) ? "PAUSE" : "START NEXT WAVE");

    cameraController->painter->end();
}


void GameScreenGL::resizeGL(int width, int height) {
    qDebug() << "GameScreenGL::resizeGL(); -- width:" << width << " height:" << height;
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
    cameraController->viewportWidth = width;
    cameraController->viewportHeight = height;
}

void GameScreenGL::mousePressEvent(QMouseEvent* event) {
    qDebug() << "GameScreenGL::mousePressEvent(); -- event:" << event;
    int mouseX = event->x();
    int mouseY = event->y();
    Qt::MouseButton button = event->button();
    cameraController->touchDown(mouseX, mouseY, 0, button);
}

void GameScreenGL::mouseReleaseEvent(QMouseEvent* event) {
    qDebug() << "GameScreenGL::mouseReleaseEvent(); -- event:" << event;
    int mouseX = event->x();
    int mouseY = event->y();
    Qt::MouseButton button = event->button();
    cameraController->touchUp(mouseX, mouseY, 0, button);
}

void GameScreenGL::mouseMoveEvent(QMouseEvent* event) {
//    qDebug() << "GameScreenGL::mouseMoveEvent(); -- event:" << event;
    int mouseX = event->x();
    int mouseY = event->y();
    Qt::MouseButtons buttons = event->buttons();
    cameraController->mouseMoved(mouseX, mouseY, buttons);
}

void GameScreenGL::wheelEvent(QWheelEvent* event) {
//    qDebug() << "GameScreenGL::wheelEvent(); -- event->pixelDelta():" << event->pixelDelta();
//    qDebug() << "GameScreenGL::wheelEvent(); -- event->angleDelta():" << event->angleDelta();

    cameraController->scrolled(event->angleDelta().y());
//    gameField->updateCellsGraphicCoordinates(cameraController->halfSizeCellX, cameraController->halfSizeCellY);

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

void GameScreenGL::keyPressEvent(QKeyEvent * event) {
//    int mainCoorMapX = field->getMainCoorMapX();
//    int mainCoorMapY = field->getMainCoorMapY();
//    int sizeCell = field->getSizeCell();
//    int sizeFieldX = field->sizeFieldX;
//    int sizeFieldY = field->sizeFieldY;
    int key = event->key();
    int modifiers = event->modifiers();
    qDebug() << "GameScreenGL::keyPressEvent(); -- Key_" << QKeySequence(key).toString().toStdString().c_str();
    qDebug() << "GameScreenGL::keyPressEvent(); -- Modifiers_" << QKeySequence(modifiers).toString().toStdString().c_str();
    if(key == Qt::Key_0) {
        signal_changeWindowState();
        qDebug() << "GameScreenGL::keyPressEvent(); -- parentWidget()->windowState():" << parentWidget()->windowState();
        cameraController->cameraX = 0;
        cameraController->cameraY = 0;
    } else if(key == Qt::Key_Control) {
        qDebug() << "GameScreenGL::keyPressEvent(); -- isKeyJustPressed(Qt::Key_Control)";
        for (int x = 0; x < gameField->map->width; x++) {
            for (int y = 0; y < gameField->map->height; y++) {
                gameField->getCell(x, y)->removeUnit();
            }
        }
    } else if(key == Qt::Key_Alt) {
        qDebug() << "GameScreenGL::keyPressEvent(); -- isKeyJustPressed(Qt::Key_Alt)";
        gameField->gamePaused = !gameField->gamePaused;
//        gameInterface.addActionToHistory("-- gameField->gamePaused:" + gameField->gamePaused);
        qDebug() << "GameScreenGL::keyPressEvent(); -- gameField->gamePaused: " << gameField->gamePaused;
    } else if(key == Qt::Key_F1) {
        qDebug() << "GameScreenGL::keyPressEvent(); -- isKeyJustPressed(Qt::Key_F1)";
        if (modifiers == Qt::ShiftModifier) {
            cameraController->isDrawableGrid++;
            if (cameraController->isDrawableGrid > 5) {
                cameraController->isDrawableGrid = 0;
            }
        } else {
            cameraController->isDrawableGrid--;
            if (cameraController->isDrawableGrid < 0) {
                cameraController->isDrawableGrid = 5;
            }
        }
        cameraController->isDrawableUnits = cameraController->isDrawableGrid;
        cameraController->isDrawableTowers = cameraController->isDrawableGrid;
        cameraController->isDrawableBackground = cameraController->isDrawableGrid;
        cameraController->isDrawableGround = cameraController->isDrawableGrid;
        cameraController->isDrawableForeground = cameraController->isDrawableGrid;
        cameraController->isDrawableGridNav = cameraController->isDrawableGrid;
        cameraController->isDrawableRoutes = cameraController->isDrawableGrid;
        qDebug() << "GameScreenGL::keyPressEvent(); -and other- cameraController->isDrawableGrid: " << cameraController->isDrawableGrid;
    } else if(key == Qt::Key_1) {
        ui->drawGrid_checkBox->toggle();
        qDebug() << "GameScreenGL::keyPressEvent(); -- isKeyJustPressed(Input.Keys.NUM_1 || Input.Keys.NUMPAD_1)";
        cameraController->isDrawableGrid--;
        if (cameraController->isDrawableGrid < 0) {
            cameraController->isDrawableGrid = 5;
        }
//        gameInterface.addActionToHistory("-- cameraController->isDrawableGrid:" + cameraController->isDrawableGrid);
        qDebug() << "GameScreenGL::keyPressEvent(); -- cameraController->isDrawableGrid: " << cameraController->isDrawableGrid;
    } else if(key == Qt::Key_2) {
        ui->drawUnits_checkBox->toggle();
        qDebug() << "GameScreenGL::keyPressEvent(); -- isKeyJustPressed(Input.Keys.NUM_2 || Input.Keys.NUMPAD_2)";
        cameraController->isDrawableUnits--;
        if (cameraController->isDrawableUnits < 0) {
            cameraController->isDrawableUnits = 5;
        }
//        gameInterface.addActionToHistory("-- cameraController->isDrawableUnits:" + cameraController->isDrawableUnits);
        qDebug() << "GameScreenGL::keyPressEvent(); -- cameraController->isDrawableUnits: " << cameraController->isDrawableUnits;
    } else if(key == Qt::Key_3) {
        ui->drawTowersByTowers_checkBox->toggle();
        qDebug() << "GameScreenGL::keyPressEvent(); -- isKeyJustPressed(Input.Keys.NUM_3 || Input.Keys.NUMPAD_3)";
        cameraController->isDrawableTowers--;
        if (cameraController->isDrawableTowers < 0) {
            cameraController->isDrawableTowers = 5;
        }
//        gameInterface.addActionToHistory("-- cameraController->isDrawableTowers:" + cameraController->isDrawableTowers);
        qDebug() << "GameScreenGL::keyPressEvent(); -- cameraController->isDrawableTowers: " << cameraController->isDrawableTowers;
    } else if(key == Qt::Key_4) {
        ui->drawBackGround_checkBox->toggle();
        qDebug() << "GameScreenGL::keyPressEvent(); -- isKeyJustPressed(Input.Keys.NUM_4 || Input.Keys.NUMPAD_4)";
        cameraController->isDrawableBackground--;
        if (cameraController->isDrawableBackground < 0) {
            cameraController->isDrawableBackground = 5;
        }
//        gameInterface.addActionToHistory("-- cameraController->isDrawableBackground:" + cameraController->isDrawableBackground);
        qDebug() << "GameScreenGL::keyPressEvent(); -- cameraController->isDrawableBackground: " << cameraController->isDrawableBackground;
    } else if(key == Qt::Key_5) {
        ui->drawGround_checkBox->toggle();
        qDebug() << "GameScreenGL::keyPressEvent(); -- isKeyJustPressed(Input.Keys.NUM_6 || Input.Keys.NUMPAD_6)";
        cameraController->isDrawableGround--;
        if (cameraController->isDrawableGround < 0) {
            cameraController->isDrawableGround = 5;
        }
//        gameInterface.addActionToHistory("-- cameraController->isDrawableGround:" + cameraController->isDrawableGround);
        qDebug() << "GameScreenGL::keyPressEvent(); -- cameraController->isDrawableGround: " << cameraController->isDrawableGround;
    } else if(key == Qt::Key_6) {
        ui->drawForeGround_checkBox->toggle();
        qDebug() << "GameScreenGL::keyPressEvent(); -- isKeyJustPressed(Input.Keys.NUM_6 || Input.Keys.NUMPAD_6)";
        cameraController->isDrawableForeground--;
        if (cameraController->isDrawableForeground < 0) {
            cameraController->isDrawableForeground = 5;
        }
//        gameInterface.addActionToHistory("-- cameraController->isDrawableForeground:" + cameraController->isDrawableForeground);
        qDebug() << "GameScreenGL::keyPressEvent(); -- cameraController->isDrawableForeground: " << cameraController->isDrawableForeground;
    } else if(key == Qt::Key_7) {
        ui->drawBlackTiles_checkBox->toggle();
        qDebug() << "GameScreenGL::keyPressEvent(); -- isKeyJustPressed(Input.Keys.NUM_6 || Input.Keys.NUMPAD_6)";
        cameraController->isDrawableGridNav--;
        if (cameraController->isDrawableGridNav < 0) {
            cameraController->isDrawableGridNav = 5;
        }
//        gameInterface.addActionToHistory("-- cameraController->isDrawableGridNav:" + cameraController->isDrawableGridNav);
        qDebug() << "GameScreenGL::keyPressEvent(); -- cameraController->isDrawableGridNav: " << cameraController->isDrawableGridNav;
    } else if(key == Qt::Key_8) {
        ui->drawPaths_checkBox->toggle();
        qDebug() << "GameScreenGL::keyPressEvent(); -- isKeyJustPressed(Input.Keys.NUM_6 || Input.Keys.NUMPAD_6)";
        cameraController->isDrawableRoutes--;
        if (cameraController->isDrawableRoutes < 0) {
            cameraController->isDrawableRoutes = 5;
        }
//        gameInterface.addActionToHistory("-- cameraController->isDrawableRoutes:" + cameraController->isDrawableRoutes);
        qDebug() << "GameScreenGL::keyPressEvent(); -- cameraController->isDrawableRoutes: " << cameraController->isDrawableRoutes;
    } else if(key == Qt::Key_9) {
        qDebug() << "GameScreenGL::keyPressEvent(); -- isKeyJustPressed(Input.Keys.NUM_9 || Input.Keys.NUMPAD_9)";
        cameraController->drawOrder++;
        if (cameraController->drawOrder > 8) {
            cameraController->drawOrder = 0;
        }
//        gameInterface.addActionToHistory("-- cameraController->drawOrder:" + cameraController->drawOrder);
        qDebug() << "GameScreenGL::keyPressEvent(); -- cameraController->drawOrder: " << cameraController->drawOrder;
    } else if(key == Qt::Key_B) {
        gameField->createdRandomUnderConstruction();
    } else if(key == Qt::Key_Escape || key == Qt::Key_N) {
        gameField->cancelUnderConstruction();
    } else if (key == Qt::Key_Enter) {
        signal_closeWidget();
        return;
    } else if (key == Qt::Key_Plus) {
        qDebug() << "GameScreen::inputHandler(); -- isKeyJustPressed(Key_Plus)";
        gameField->gameSpeed += 0.1;
        //        gameInterface.addActionToHistory("-- gameField->gameSpeed:" + gameField->gameSpeed);
        qDebug() << "GameScreen::inputHandler(); -- gameField->gameSpeed:" << gameField->gameSpeed;
    } else if (key == Qt::Key_Minus) {
        qDebug() << "GameScreen::inputHandler(); -- isKeyJustPressed(Key_Minus)";
        gameField->gameSpeed -= 0.1;
        //        gameInterface.addActionToHistory("-- gameField->gameSpeed:" + gameField->gameSpeed);
        qDebug() << "GameScreen::inputHandler(); -- gameField->gameSpeed:" << gameField->gameSpeed;
    } else if (key == Qt::Key_A) {
        qDebug() << "GameScreen::inputHandler(); -- isKeyJustPressed(Input.Keys.A)";
        gameField->turnLeft();
//        gameInterface.addActionToHistory("-- gameField.turnLeft()");
        qDebug() << "GameScreen::inputHandler(); -- gameField.turnLeft()";
    } else if (key == Qt::Key_S) {
        qDebug() << "GameScreen::inputHandler(); -- -- isKeyJustPressed(Input.Keys.S)";
        gameField->turnRight();
//        gameInterface.addActionToHistory("-- gameField.turnRight()");
        qDebug() << "GameScreen::inputHandler(); -- gameField.turnRight()";
    } else if (key == Qt::Key_Q) {
        qDebug() << "GameScreen::inputHandler(); -- isKeyJustPressed(Input.Keys.Q)";
        gameField->flipX();
//        gameInterface.addActionToHistory("-- gameField.flipX()");
        qDebug() << "GameScreen::inputHandler(); -- gameField.flipX()";
    } else if (key == Qt::Key_W) {
        qDebug() << "GameScreen::inputHandler(); -- isKeyJustPressed(Input.Keys.W)";
        gameField->flipY();
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
}

void GameScreenGL::on_closeWidget_clicked() {
    signal_closeWidget();
}

void GameScreenGL::on_goUnits_clicked() {
    gameField->gamePaused = !gameField->gamePaused;
    if (!gameField->unitsSpawn && gameField->unitsManager->units.size() == 0) {
        gameField->unitsSpawn = true;
        gameField->gamePaused = false;
    }
}
