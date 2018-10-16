#-------------------------------------------------
#
# Project created by QtCreator 2013-03-26T18:48:39
# new clear/Version 2018-10-10 22:22
#
#-------------------------------------------------

QT       += core gui multimedia xml # opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LittleGame
TEMPLATE = app
CONFIG += c++11
RC_FILE = myicon.rc
RESOURCES = resources.qrc

SOURCES += src/cpp/main.cpp\
    src/cpp/field.cpp \
    src/cpp/mainmenu.cpp \
    src/cpp/widgetcontroller.cpp \
    src/cpp/optionmenu.cpp \
    src/cpp/gamewidget.cpp \
    src/cpp/factionsmanager.cpp \
    src/cpp/faction.cpp \
    src/cpp/tower.cpp \
    src/cpp/defaultunit.cpp \
    src/cpp/defaulttower.cpp \
    src/cpp/bullet.cpp \
    src/cpp/underconstruction.cpp \
    src/cpp/unit.cpp \
    src/cpp/unitsmanager.cpp \
    src/cpp/towersmanager.cpp \
    src/cpp/cell.cpp \
    src/cpp/libs/astar.cpp \
    src/cpp/libs/tinyxml2.cpp

HEADERS  += \
    src/head/field.h \
    src/head/mainmenu.h \
    src/head/widgetcontroller.h \
    src/head/optionmenu.h \
    src/head/gamewidget.h \
    src/head/factionsmanager.h \
    src/head/faction.h \
    src/head/tower.h \
    src/head/defaultunit.h \
    src/head/defaulttower.h \
    src/head/bullet.h \
    src/head/underconstruction.h \
    src/head/unit.h \
    src/head/unitsmanager.h \
    src/head/towersmanager.h \
    src/head/cell.h \
    src/head/libs/astar.h \
    src/head/libs/tinyxml2.h

FORMS    += \
    src/uis/mainmenu.ui \
    src/uis/optionmenu.ui \
    src/uis/gamewidget.ui

DISTFILES += \
    .gitignore
