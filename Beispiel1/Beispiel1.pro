#-------------------------------------------------
#
# Project created by QtCreator 2010-10-18T15:30:04
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = Beispiel1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    volumerenderer.cpp \
    framework/Vector.cpp \
    framework/Quaternion.cpp \
    framework/Matrix.cpp \
    framework/Volume.cpp \
    gradients/gradients.cpp \
    gradients/arthurwidgets.cpp \
    gradients/arthurstyle.cpp \
    gradients/hoverpoints.cpp \
    colorlabel.cpp \
    slicingview.cpp \
    renderingoptions.cpp \
    glew.c

HEADERS  += mainwindow.h \
    volumerenderer.h \
    framework/Vector.h \
    framework/Quaternion.h \
    framework/Matrix.h \
    framework/Volume.h \
    gradients/gradients.h \
    gradients/arthurwidgets.h \
    gradients/arthurstyle.h \
    gradients/hoverpoints.h \
    colorlabel.h \
    slicingview.h \
    renderingoptions.h \
    GL/glew.h

FORMS    += mainwindow.ui

INCLUDEPATH += gradients

SHARED_FOLDER = ../Beispiel1/gradients

#include($$SHARED_FOLDER/shared.pri)

INCLUDEPATH += ../Beispiel1/framework

# RESOURCES += gradients.qrc

OTHER_FILES += \
    FragmentShader.glsl
