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
    colorlabel.cpp \
    framework/Vector.cpp \
    framework/Quaternion.cpp \
    framework/Matrix.cpp \
    framework/Volume.cpp \
    gradients/gradients.cpp \
    gradients/arthurwidgets.cpp \
    gradients/arthurstyle.cpp \
    gradients/hoverpoints.cpp

HEADERS  += mainwindow.h \
    volumerenderer.h \
    common.h \
    Volume.h \
    Vector.h \
    Quaternion.h \
    Matrix.h \
    colorlabel.h \
    framework/Vector.h \
    framework/Quaternion.h \
    framework/Matrix.h \
    framework/Volume.h \
    gradients/gradients.h \
    gradients/arthurwidgets.h \
    gradients/arthurstyle.h \
    gradients/hoverpoints.h

FORMS    += mainwindow.ui


SHARED_FOLDER = ../Beispiel1/gradients

include($$SHARED_FOLDER/shared.pri)

# RESOURCES += gradients.qrc
