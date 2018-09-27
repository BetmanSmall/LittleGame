#-------------------------------------------------
#
# Project created by QtCreator 2013-03-26T18:48:39
#
#-------------------------------------------------

QT       += core gui multimedia # opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LittleGame
TEMPLATE = app
CONFIG += c++11
RESOURCES = application.qrc
RC_FILE = myapp.rc

SOURCES += main.cpp\
    field.cpp \
    mainmenu.cpp \
    widgetcontroller.cpp \
    optionmenu.cpp \
    choosemapmenu.cpp \
    gamewidget.cpp \
    faction.cpp \
    tower.cpp \
    defaultunit.cpp \
    defaulttower.cpp \
    bullet.cpp \
    underconstruction.cpp \
    unit.cpp \
    unitsmanager.cpp \
    towersmanager.cpp \
    astar.cpp \
    cell.cpp \
    gamescreen.cpp

HEADERS  += \
    field.h \
    mainmenu.h \
    widgetcontroller.h \
    optionmenu.h \
    choosemapmenu.h \
    gamewidget.h \
    faction.h \
    tower.h \
    defaultunit.h \
    defaulttower.h \
    bullet.h \
    underconstruction.h \
    unit.h \
    unitsmanager.h \
    towersmanager.h \
    astar.h \
    cell.h \
    gamescreen.h

FORMS    += \
    mainmenu.ui \
    optionmenu.ui \
    choosemapmenu.ui \
    gamewidget.ui

DISTFILES += \
    assets/images/button1.jpg \
    assets/images/button1v1.jpg \
    assets/images/button2.jpg \
    assets/images/button3.jpg \
    assets/images/desert.jpg \
    assets/images/forest.jpg \
    assets/images/mainmenu.jpg \
    assets/images/button1v2.png \
    assets/images/button2v2.png \
    assets/images/button3v2.png \
    assets/images/Exit.png \
    assets/images/littlegame.png \
    assets/images/Options.png \
    assets/images/Play.png \
    assets/images/favicon.ico
