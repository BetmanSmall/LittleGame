QT += core opengl xml multimedia # gui # widgets quick

TARGET = LittleGame
TEMPLATE = app
CONFIG += c++14
RC_FILE = myicon.rc
RESOURCES = resources.qrc

SOURCES += main.cpp\
    mainmenu.cpp \
    widgetcontroller.cpp \
    optionmenu.cpp \
    gamescreengl.cpp \
    factionsmanager.cpp \
    faction.cpp \
    tower.cpp \
    templatefortower.cpp \
    templateforunit.cpp \
    bullet.cpp \
    underconstruction.cpp \
    unit.cpp \
    unitsmanager.cpp \
    towersmanager.cpp \
    cell.cpp \
    astar.cpp \
    animatedtile.cpp \
    layer.cpp \
    map.cpp \
    maplayers.cpp \
    maploader.cpp \
    statictile.cpp \
    tile.cpp \
    tileset.cpp \
    tilesets.cpp \
    tilelayercell.cpp \
    template.cpp \
    simpletemplate.cpp \
    animation.cpp \
    wavealgorithm.cpp \
    wavemanager.cpp \
    wave.cpp \
    cameracontroller.cpp \
    gamefield.cpp \
    gamesettings.cpp

HEADERS  += \
    mainmenu.h \
    widgetcontroller.h \
    optionmenu.h \
    gamescreengl.h \
    factionsmanager.h \
    faction.h \
    tower.h \
    templatefortower.h \
    templateforunit.h \
    bullet.h \
    underconstruction.h \
    unit.h \
    unitsmanager.h \
    towersmanager.h \
    cell.h \
    astar.h \
    direction.h \
    animatedtile.h \
    layer.h \
    map.h \
    maplayers.h \
    maploader.h \
    statictile.h \
    tile.h \
    tileset.h \
    tilesets.h \
    tilelayercell.h \
    template.h \
    simpletemplate.h \
    animation.h \
    wavealgorithm.h \
    wavemanager.h \
    wave.h \
    cameracontroller.h \
    circle.h \
    gamefield.h \
    towerattacktype.h \
    towershelleffect.h \
    towershelltype.h \
    vector2.h \
    gametype.h \
    gamesettings.h

FORMS    += \
    mainmenu.ui \
    optionmenu.ui \
    gamescreengl.ui

DISTFILES += \
    .gitignore
