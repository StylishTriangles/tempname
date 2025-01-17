#-------------------------------------------------
#
# Project created by QtCreator 2016-04-26T07:53:38
#
#-------------------------------------------------

QT       += core gui
QT       += xml
CONFIG   += c++17
QMAKE_LFLAGS += -Wl,--large-address-aware
QMAKE_LFLAGS_RELEASE = --static

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Aurora
TEMPLATE = app


SOURCES += main.cpp\
        interface/mainwindow.cpp \
    interface/options.cpp \
    game.cpp \
    geometryprovider.cpp \
    modelcontainer.cpp \
    interface/hud.cpp \
    src/ray_intersect.cpp \
    src/fileops.cpp \
    src/neural.cpp \
    levels/arkanoid.cpp \
    src/mapgenerator.cpp \
    src/helper_classes.cpp \
    details.cpp \
    player.cpp \
    interface/osd.cpp

HEADERS  += interface/mainwindow.h \
    interface/options.h \
    game.h \
    geometryprovider.h \
    modelcontainer.h \
    interface/hud.h \
    include/ray_intersect.h \
    include/fileops.h \
    include/neural.h \
    levels/arkanoid.h \
    include/mapgenerator.h \
    include/helper_classes.h \
    details.h \
    player.h \
    interface/osd.h

FORMS    += interface/mainwindow.ui \
    interface/options.ui \
    interface/hud.ui \
    interface/osd.ui

RESOURCES += \
    shaders.qrc \
    textures.qrc \
    icons.qrc
