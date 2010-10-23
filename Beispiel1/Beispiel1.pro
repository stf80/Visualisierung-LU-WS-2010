#-------------------------------------------------
#
# Project created by QtCreator 2010-10-18T15:30:04
#
#-------------------------------------------------

QT       += core gui

TARGET = Beispiel1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    volumerenderer.cpp \
    Volume.cpp \
    Vector.cpp \
    Quaternion.cpp \
    Matrix.cpp

HEADERS  += mainwindow.h \
    volumerenderer.h \
    common.h \
    Volume.h \
    Vector.h \
    Quaternion.h \
    Matrix.h

FORMS    += mainwindow.ui


SHARED_FOLDER = ../Beispiel1/gradients

include($$SHARED_FOLDER/shared.pri)

# RESOURCES += gradients.qrc
