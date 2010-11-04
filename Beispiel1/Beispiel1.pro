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
    Matrix.cpp \
    gradients/gradients.cpp \
    gradients/arthurwidgets.cpp \
    gradients/arthurstyle.cpp \
    gradients/hoverpoints.cpp \
    colorlabel.cpp

HEADERS  += mainwindow.h \
    volumerenderer.h \
    common.h \
    Volume.h \
    Vector.h \
    Quaternion.h \
    Matrix.h \
    gradients/gradients.h \
    gradients/arthurwidgets.h \
    gradients/arthurstyle.h \
    gradients/hoverpoints.h \
    colorlabel.h \
    gradients/private/qpixmapdata_p.h

FORMS    += mainwindow.ui

INCLUDEPATH += gradients

#SHARED_FOLDER = ../Beispiel1/gradients

#include($$SHARED_FOLDER/shared.pri)

# RESOURCES += gradients.qrc
