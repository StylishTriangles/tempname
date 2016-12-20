#-------------------------------------------------
#
# Project created by QtCreator 2016-04-26T07:53:38
#
#-------------------------------------------------

QT       += core gui
CONFIG   += c++17

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Aurora
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    options.cpp \
    game.cpp \
    geometryprovider.cpp \
    modelcontainer.cpp

HEADERS  += mainwindow.h \
    options.h \
    game.h \
    geometryprovider.h \
    modelcontainer.h

FORMS    += mainwindow.ui \
    options.ui

DISTFILES +=

RESOURCES += \
    shaders.qrc \
    textures.qrc
