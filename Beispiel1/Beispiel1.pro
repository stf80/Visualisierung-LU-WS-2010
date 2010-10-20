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
    gradients/gradients.cpp \
    volumerenderer.cpp

HEADERS  += mainwindow.h \
    gradients/gradients.h \
    volumerenderer.h

FORMS    += mainwindow.ui


SHARED_FOLDER = ../Beispiel1/gradients

include($$SHARED_FOLDER/shared.pri)

# RESOURCES += gradients.qrc
