#ifndef GAMESCREENGL_H
#define GAMESCREENGL_H

#include <time.h>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QMouseEvent>
#include <QString>
#include <QDebug>

#include "astar.h"
#include "gamefield.h"
#include "underconstruction.h"
#include "cameracontroller.h"
#include "ui_gamescreengl.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)

namespace Ui {
    class GameScreenGL;
}

class GameScreenGL : public QOpenGLWidget, public QOpenGLFunctions {
    Q_OBJECT

#ifdef QT_DEBUG
    QString ASSETS_PATH = "../../LittleGame/assets/";
#else
    QString ASSETS_PATH = "./assets/";
#endif

public:
    GameField* gameField;
    CameraController* cameraController;

    QTime currentTime;
    QTime lastTime;
    float elapsedTime;
    float fps;
    QTimer* gameTimer;

    std::string global_text;
    std::string global_text2;

public:
    explicit GameScreenGL(QString mapPath, FactionsManager *factionsManager,
                        GameSettings *gameSettings,
                        QWidget *parent = 0);
    ~GameScreenGL();

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
//    void paintEvent(QPaintEvent* event);
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
//    void resizeEvent(QResizeEvent* event);

    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;

private:
    Ui::GameScreenGL *ui;

private slots:
    void on_closeWidget_clicked();

signals:
//    void clicked();
    void signal_changeWindowState();
    void signal_closeWidget();
    void signal_closeWidgetGameFinished(bool win, int timeOfGame);
};

#endif // GAMESCREENGL_H
