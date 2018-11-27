QT += core opengl xml multimedia # gui # widgets quick

TARGET = LittleGame
TEMPLATE = app
CONFIG += c++14
RC_FILE = myicon.rc
RESOURCES = resources.qrc

SOURCES += src/cpp/main.cpp\
    src/cpp/mainmenu.cpp \
    src/cpp/widgetcontroller.cpp \
    src/cpp/optionmenu.cpp \
    src/cpp/gamescreengl.cpp \
    src/cpp/factionsmanager.cpp \
    src/cpp/faction.cpp \
    src/cpp/tower.cpp \
    src/cpp/templatefortower.cpp \
    src/cpp/templateforunit.cpp \
    src/cpp/bullet.cpp \
    src/cpp/underconstruction.cpp \
    src/cpp/unit.cpp \
    src/cpp/unitsmanager.cpp \
    src/cpp/towersmanager.cpp \
    src/cpp/cell.cpp \
    src/cpp/libs/astar.cpp \
    src/cpp/mapEditor/animatedtile.cpp \
    src/cpp/mapEditor/layer.cpp \
    src/cpp/mapEditor/map.cpp \
    src/cpp/mapEditor/maplayers.cpp \
    src/cpp/mapEditor/maploader.cpp \
    src/cpp/mapEditor/statictile.cpp \
    src/cpp/mapEditor/tile.cpp \
    src/cpp/mapEditor/tileset.cpp \
    src/cpp/mapEditor/tilesets.cpp \
    src/cpp/mapEditor/tilelayercell.cpp \
    template.cpp \
    simpletemplate.cpp \
    animation.cpp \
    wavealgorithm.cpp \
    wavemanager.cpp \
    wave.cpp \
    cameracontroller.cpp \
    src/cpp/gamefield.cpp \
    gamesettings.cpp

HEADERS  += \
    src/head/mainmenu.h \
    src/head/widgetcontroller.h \
    src/head/optionmenu.h \
    src/head/gamescreengl.h \
    src/head/factionsmanager.h \
    src/head/faction.h \
    src/head/tower.h \
    src/head/templatefortower.h \
    src/head/templateforunit.h \
    src/head/bullet.h \
    src/head/underconstruction.h \
    src/head/unit.h \
    src/head/unitsmanager.h \
    src/head/towersmanager.h \
    src/head/cell.h \
    src/head/libs/astar.h \
    src/head/direction.h \
    src/head/mapEditor/animatedtile.h \
    src/head/mapEditor/layer.h \
    src/head/mapEditor/map.h \
    src/head/mapEditor/maplayers.h \
    src/head/mapEditor/maploader.h \
    src/head/mapEditor/statictile.h \
    src/head/mapEditor/tile.h \
    src/head/mapEditor/tileset.h \
    src/head/mapEditor/tilesets.h \
    src/head/mapEditor/tilelayercell.h \
    template.h \
    simpletemplate.h \
    animation.h \
    wavealgorithm.h \
    wavemanager.h \
    wave.h \
    cameracontroller.h \
    circle.h \
    src/head/gamefield.h \
    towerattacktype.h \
    towershelleffect.h \
    towershelltype.h \
    vector2.h \
    gametype.h \
    gamesettings.h

FORMS    += \
    src/uis/mainmenu.ui \
    src/uis/optionmenu.ui \
    src/uis/gamescreengl.ui

DISTFILES += \
    .gitignore
