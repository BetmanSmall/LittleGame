#-------------------------------------------------
#
# Project created by QtCreator 2013-03-26T18:48:39
#
#-------------------------------------------------

QT       += core gui multimedia # opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LittleGame
TEMPLATE = app


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
    towersmanager.cpp

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
    towersmanager.h

FORMS    += \
    mainmenu.ui \
    optionmenu.ui \
    choosemapmenu.ui \
    gamewidget.ui
