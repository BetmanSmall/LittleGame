#include "src/head/gamescreengl.h"

GameScreenGL::GameScreenGL(QString mapFile, FactionsManager* factionsManager,
                       int enemyCount, int difficultyLevel, int towersCount,
                       QWidget *parent):
    QOpenGLWidget(parent),
    ui(new Ui::GameScreenGL)
{
    ui->setupUi(this);

    gameField = new GameField(mapFile, factionsManager, enemyCount, difficultyLevel, towersCount);
    cameraController = new CameraController(gameField->map->width, gameField->map->height, gameField->map->tileWidth, gameField->map->tileHeight);

//    timeOfGame = 0;
    gameTimer = new QTimer(this);
    connect(gameTimer, SIGNAL(timeout()), this, SLOT(update()));
    gameTimer->start(0);

    setMouseTracking(true);
//    setFixedSize(200, 200);
//    setAutoFillBackground(false);

    qDebug() << "GameScreenGL::loadMap(); -- mapFile:" << mapFile;
    qDebug() << "GameScreenGL::loadMap(); -- enemyCount:" << enemyCount;
    qDebug() << "GameScreenGL::loadMap(); -- towersCount:" << towersCount;
    qDebug() << "GameScreenGL::loadMap(); -- field:" << gameField;
    qDebug() << "GameScreenGL::loadMap(); -- field->map:" << gameField->map;
    qDebug() << "GameScreenGL::GameScreenGL(); -END- -END-";
}

GameScreenGL::~GameScreenGL() {
    delete gameField;
    delete cameraController;
    delete gameTimer;
    delete ui;
}

//QSize GameScreenGL::minimumSizeHint() const
//{
//    return QSize(50, 50);
//}

//QSize GameScreenGL::sizeHint() const
//{
//    return QSize(200, 200);
//}

void GameScreenGL::initializeGL() {
    initializeOpenGLFunctions();

//    makeObject();

//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);

//#define PROGRAM_VERTEX_ATTRIBUTE 0
//#define PROGRAM_TEXCOORD_ATTRIBUTE 1

//    QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
//    const char *vsrc =
//        "attribute highp vec4 vertex;\n"
//        "attribute mediump vec4 texCoord;\n"
//        "varying mediump vec4 texc;\n"
//        "uniform mediump mat4 matrix;\n"
//        "void main(void)\n"
//        "{\n"
//        "    gl_Position = matrix * vertex;\n"
//        "    texc = texCoord;\n"
//        "}\n";
//    vshader->compileSourceCode(vsrc);

//    QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
//    const char *fsrc =
//        "uniform sampler2D texture;\n"
//        "varying mediump vec4 texc;\n"
//        "void main(void)\n"
//        "{\n"
//        "    gl_FragColor = texture2D(texture, texc.st);\n"
//        "}\n";
//    fshader->compileSourceCode(fsrc);

//    program = new QOpenGLShaderProgram;
//    program->addShader(vshader);
//    program->addShader(fshader);
//    program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
//    program->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
//    program->link();

//    program->bind();
//    program->setUniformValue("texture", 0);
}

void GameScreenGL::paintGL() {
    glClearColor(0, 0, 0, 255);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    lastTime = currentTime;
    currentTime = QTime::currentTime();
//    elapsedTime = (currentTime.second()*1000 + currentTime.msec()) - (lastTime.second()*1000 + lastTime.msec()); // /!\ max = 59 seconds 999 ms
//    this->fps = 1000 / elapsedTime;
    elapsedTime = (currentTime.msecsSinceStartOfDay()) - (lastTime.msecsSinceStartOfDay());
    this->fps = 1000 / elapsedTime;

    cameraController->painter->begin(this);
    cameraController->painter->scale(cameraController->zoom, cameraController->zoom);
//    cameraController->painter->setRenderHint(QPainter::Antialiasing);
//    cameraController->painter->fillRect(event->rect(), QColor(0, 0, 0));
    cameraController->painter->translate(cameraController->cameraX, cameraController->cameraY);
//    cameraController->painter->rotate(fps);

    gameField->render(elapsedTime/1000, cameraController);
    cameraController->painter->setPen(QColor(255, 0, 0));
    cameraController->painter->drawEllipse(QPoint(0, 0), 5, 5);
    cameraController->painter->end();

    cameraController->painter->begin(this);
//    cameraController->update(elapsedTime);
//    cameraController->painter->scale(cameraController->zoom, -cameraController->zoom);
//    cameraController->painter->translate(-cameraController->cameraX, -cameraController->cameraY);
    cameraController->painter->setPen(QColor(0, 255, 0));
    cameraController->painter->drawEllipse(QPoint(0, 0), 5, 5);
    cameraController->painter->drawText(10, 10, "FPS:" + QString::number(fps));
    cameraController->painter->drawText(10, 20, "lastTime:" + QString::number(lastTime.msecsSinceStartOfDay()));
    cameraController->painter->drawText(10, 30, "currentTime:" + QString::number(currentTime.msecsSinceStartOfDay()));
    cameraController->painter->drawText(10, 40, "elapsedTime:" + QString::number(elapsedTime));
    cameraController->painter->drawText(10, 50, "cameraController->cameraX:" + QString::number(cameraController->cameraX));
    cameraController->painter->drawText(10, 60, "cameraController->cameraY:" + QString::number(cameraController->cameraY));
    cameraController->painter->drawText(10, 70, "cameraController->zoom:" + QString::number(cameraController->zoom));
    cameraController->painter->drawText(10, 80, "cameraController->paning:" + QString::number(cameraController->paning));
    cameraController->painter->drawText(10, 90, "cameraController->flinging:" + QString::number(cameraController->flinging));
    cameraController->painter->drawText(10, 130, "cameraController->drawOrder:" + QString::number(cameraController->drawOrder));
    cameraController->painter->drawText(10, 140, "cameraController->isDrawableGrid:" + QString::number(cameraController->isDrawableGrid));
    cameraController->painter->drawText(10, 150, "cameraController->isDrawableUnits:" + QString::number(cameraController->isDrawableUnits));
    cameraController->painter->drawText(10, 160, "cameraController->isDrawableTowers:" + QString::number(cameraController->isDrawableTowers));
    cameraController->painter->drawText(10, 170, "cameraController->isDrawableBackground:" + QString::number(cameraController->isDrawableBackground));
    cameraController->painter->drawText(10, 180, "cameraController->isDrawableGround:" + QString::number(cameraController->isDrawableGround));
    cameraController->painter->drawText(10, 190, "cameraController->isDrawableForeground:" + QString::number(cameraController->isDrawableForeground));
    cameraController->painter->drawText(10, 200, "cameraController->isDrawableGridNav:" + QString::number(cameraController->isDrawableGridNav));
    cameraController->painter->drawText(10, 210, "cameraController->isDrawableRoutes:" + QString::number(cameraController->isDrawableRoutes));
    if (gameField->getUnderConstruction() != NULL) {
        cameraController->painter->drawText(10, 230, "gameField->underConstruction->endX:" + QString::number(gameField->underConstruction->endX) + " endY:" + QString::number(gameField->underConstruction->endY));
    }
    cameraController->painter->drawText(10, 240, "gameField->towersManager->towers.size():" + QString::number(gameField->towersManager->towers.size()));
    cameraController->painter->drawText(10, 250, "gameField->unitsManager->units.size():" + QString::number(gameField->unitsManager->units.size()));

    cameraController->painter->drawText(10, 260, "gameField->isometric:" + QString::number(gameField->isometric));
    cameraController->painter->drawText(10, 270, "gameField->mapName:" + gameField->map->properties.value("name"));
    cameraController->painter->end();
}


void GameScreenGL::resizeGL(int width, int height) {
    qDebug() << "GameScreenGL::resizeGL(); -- width:" << width << " height:" << height;
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
}

void GameScreenGL::mousePressEvent(QMouseEvent* event) {
    qDebug() << "GameScreenGL::mousePressEvent(); -- event:" << event;
    int mouseX = event->x();
    int mouseY = event->y();
    Qt::MouseButton button = event->button();
    if (gameField->getUnderConstruction() != NULL) {
        if (button == Qt::RightButton) {
            gameField->cancelUnderConstruction();
        }
    }
    if ( (!cameraController->panMidMouseButton && button == Qt::RightButton) ||
          (cameraController->panMidMouseButton && button == Qt::MidButton) ) {
        cameraController->touchDown(mouseX, mouseY, 0, button);
        setCursor(Qt::ClosedHandCursor);
    }
    if (button == Qt::LeftButton) {
        if (gameField->getUnderConstruction() != NULL) {
            if (cameraController->whichCell(mouseX, mouseY, cameraController->isDrawableTowers)) {
                gameField->getUnderConstruction()->setStartCoors(mouseX, mouseY);
            }
        }
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

void GameScreenGL::mouseReleaseEvent(QMouseEvent* event) {
    qDebug() << "GameScreenGL::mouseReleaseEvent(); -- event:" << event;
    int mouseX = event->x();
    int mouseY = event->y();
    Qt::MouseButton button = event->button();
    if ( (!cameraController->panMidMouseButton && button == Qt::RightButton) ||
          (cameraController->panMidMouseButton && button == Qt::MidButton) ) {
        cameraController->touchUp(mouseX, mouseY, 0, button);
        setCursor(Qt::ArrowCursor);
    }
    if (button == Qt::LeftButton) {
        if (gameField->getUnderConstruction() != NULL) {
            if (cameraController->whichCell(mouseX, mouseY, cameraController->isDrawableTowers)) {
                gameField->buildTowersWithUnderConstruction(mouseX, mouseY);
            }
        } else {
            if (cameraController->whichCell(mouseX, mouseY, cameraController->isDrawableUnits)) {
                if (gameField->getCell(mouseX, mouseY)->isEmpty()) {
                    gameField->updateHeroDestinationPoint(mouseX, mouseY);
                }
                qDebug() << "GameScreenGL::mouseReleaseEvent(); -cell- mouseX:" << mouseX << " mouseY:" << mouseY << " isEmpty:" << gameField->getCell(mouseX, mouseY)->isEmpty();
            }
        }
    } else if (button == Qt::RightButton && !cameraController->panMidMouseButton) {
        if (cameraController->whichCell(mouseX, mouseY, cameraController->isDrawableTowers) != NULL) {
            qDebug() << "GameScreenGL::mouseReleaseEvent(); -whichCell- mouseX:" << mouseX << " mouseY:" << mouseY;
            if ( cameraController->panMidMouseButton || (cameraController->prevMouseCellX == mouseX && cameraController->prevMouseCellY == mouseY && cameraController->prevGlobalMouseX == event->globalX() && cameraController->prevGlobalMouseY == event->globalY()) ) {
                Cell* cell = gameField->getCell(mouseX, mouseY);
                if (cell != NULL) {
                    if(cell->isEmpty()) {
                        gameField->createTower(mouseX, mouseY, gameField->factionsManager->getRandomTemplateForTowerFromAllFaction(), 0);
                        int randNumber = ( 124+(rand()%2) );
                        cell->setTerrain(gameField->map->tileSets.getTileSet(0)->tiles[randNumber]);
                    } else if (cell->isTerrain()) {
                        cell->removeTerrain();
                    } else if (cell->getTower() != NULL) {
                        cell->removeTower();
                    } else {
                        qDebug() << "GameScreenGL::mouseReleaseEvent(); -- RightButton! cell bad:" << cell;
                    }
                }
                gameField->updateHeroDestinationPoint();
            }
        }
    }
}

void GameScreenGL::mouseMoveEvent(QMouseEvent* event) {
//    qDebug() << "GameScreenGL::mouseMoveEvent(); -- event:" << event;
    int mouseX = event->x();
    int mouseY = event->y();
    Qt::MouseButton button = event->button();
    cameraController->pan(mouseX, mouseY);

    if ( cameraController->whichCell(mouseX, mouseY, cameraController->isDrawableGrid) ) {
        if (gameField->getUnderConstruction() != NULL) {
            gameField->getUnderConstruction()->setEndCoors(mouseX, mouseY);
        }
//        qDebug() << "GameScreenGL::mouseMoveEvent(); -cell- mouseX:" << mouseX << " mouseY:" << mouseY;
    }
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

void GameScreenGL::wheelEvent(QWheelEvent* event) {
    qDebug() << "GameScreenGL::resizeEvent(); -- event->pixelDelta():" << event->pixelDelta();
    qDebug() << "GameScreenGL::resizeEvent(); -- event->angleDelta():" << event->angleDelta();

    cameraController->scrolled(event->angleDelta().y());
    gameField->updateCellsGraphicCoordinates(cameraController->halfSizeCellX, cameraController->halfSizeCellY);

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
//        cameraController->isDrawableGridNav = cameraController->isDrawableGrid;
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
