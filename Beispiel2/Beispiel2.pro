#-------------------------------------------------
#
# Project created by QtCreator 2010-10-18T15:30:04
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = Beispiel2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    renderingview.cpp \
    glew.c \
    framework/textfile.cpp \
    framework/FlowGeometry.cpp \
    framework/FlowData.cpp \
    framework/FlowChannel.cpp \
    framework/vec3.cpp

HEADERS  += mainwindow.h \
    renderingview.h \
    GL/glew.h \
    framework/FlowChannel.h \
    framework/textfile.h \
    framework/reverseBytes.h \
    framework/FlowGeometry.h \
    framework/FlowData.h \
    framework/vec3.h \
    renderingview.h

FORMS    += mainwindow.ui

INCLUDEPATH += gradients

SHARED_FOLDER = ../Beispiel2/gradients

#include($$SHARED_FOLDER/shared.pri)

INCLUDEPATH += ../Beispiel2/framework

# RESOURCES += gradients.qrc

OTHER_FILES += \
    FragmentShader.glsl
