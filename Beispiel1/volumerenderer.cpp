
#include <QtGui>
#include <QtOpenGL>

#include <math.h>
#include <iostream>

#include "volumerenderer.h"

VolumeRenderer::VolumeRenderer(QWidget *parent)
//    : ArthurFrame(parent)
//    : QGraphicsView(parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    transferLUT = QImage(4096, 1, QImage::Format_ARGB32);

    image = new QImage(100, 100, QImage::Format_RGB32);

        xRot = 0;
        yRot = 0;
        zRot = 0;

        std::cerr << "Widget is valid: " << isValid() << std::endl;
        std::cerr << "has OpenGL shader: " << QGLShaderProgram::hasOpenGLShaderPrograms() << std::endl;


}

VolumeRenderer::~VolumeRenderer()
{

}

QRgb VolumeRenderer::transfer(float value)
{
    value = value < 0.f ? 0.f : value;
    value = value > 1.f ? 1.f : value;

    int index = value * (transferLUT.width() - 1);

    return transferLUT.pixel(index, 0);
}

void VolumeRenderer::updateTransfer()
{
    QGradient g = QLinearGradient(QPoint(0, 0), QPoint(transferLUT.width(),0));

    for (int i=0; i<m_stops.size(); ++i)
        g.setColorAt(m_stops.at(i).first, m_stops.at(i).second);

    g.setSpread(QGradient::PadSpread);

    QPainter *p = new QPainter(&transferLUT);

    p->setBrush(g);
    p->setPen(Qt::NoPen);

    p->drawRect(rect());

    delete p;
}

void VolumeRenderer::setGradientStops(const QGradientStops &stops)
{
    m_stops = stops;

    updateTransfer();

    update();
}

/*
void VolumeRenderer::paint(QPainter *p)
{
    //TODO replace code with raycasting

    image->fill(Qt::black);

    // Draw to QImage
    for (int i = 0; i < 100; ++i)
    {
        image->setPixel(i, i, qRgb(i, i, i));
    }

    // Draw Qimage to QPaintDevice
    p->drawImage(0, 0, *image);
}

void VolumeRenderer::paintEvent(QPaintEvent *e)
{
    // Use this widget as paint device
    QPainter painter(this);

    paint(&painter);
}
*/

/****************************************************************************
 **
 ** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 ** Contact: Nokia Corporation (qt-info@nokia.com)
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
 **     the names of its contributors may be used to endorse or promote
 **     products derived from this software without specific prior written
 **     permission.
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/



 //#include "VolumeRenderer.h"
 //#include "qtlogo.h"

 #ifndef GL_MULTISAMPLE
 #define GL_MULTISAMPLE  0x809D
 #endif


 QSize VolumeRenderer::minimumSizeHint() const
 {
     return QSize(200, 200);
 }

 QSize VolumeRenderer::sizeHint() const
 {
     return QSize(400, 400);
 }

 static void qNormalizeAngle(int &angle)
 {
     while (angle < 0)
         angle += 360 * 16;
     while (angle > 360 * 16)
         angle -= 360 * 16;
 }

 void VolumeRenderer::setXRotation(int angle)
 {
     qNormalizeAngle(angle);
     if (angle != xRot) {
         xRot = angle;
         emit xRotationChanged(angle);
         updateGL();
     }
 }

 void VolumeRenderer::setYRotation(int angle)
 {
     qNormalizeAngle(angle);
     if (angle != yRot) {
         yRot = angle;
         emit yRotationChanged(angle);
         updateGL();
     }
 }

 void VolumeRenderer::setZRotation(int angle)
 {
     qNormalizeAngle(angle);
     if (angle != zRot) {
         zRot = angle;
         emit zRotationChanged(angle);
         updateGL();
     }
 }

 void VolumeRenderer::initializeGL()
 {
     //qglClearColor(qtPurple.dark());
    glClearColor(0.0, 0.0, 0.0, 1.0);
    /*
     glEnable(GL_DEPTH_TEST);
     glEnable(GL_CULL_FACE);
     glShadeModel(GL_SMOOTH);
     glEnable(GL_LIGHTING);
     glEnable(GL_LIGHT0);
     //glEnable(GL_MULTISAMPLE);
     static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
     glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    */
     program = new QGLShaderProgram(context());

     program->addShaderFromSourceCode(QGLShader::Vertex,
         "attribute highp vec4 vertex;\n"
         "attribute mediump mat4 matrix;\n"
         "void main(void)\n"
         "{\n"
         "   gl_Position = matrix * vertex;\n"
         "}");
     std::cout << "Vertex shader log: " << program->log().toStdString().c_str() << std::endl;
     program->addShaderFromSourceCode(QGLShader::Fragment,
         "uniform mediump vec4 color;\n"
         "void main(void)\n"
         "{\n"
         "   gl_FragColor = color;\n"
         "}");
     std::cerr << "Fragment shader log: " << program->log().toStdString().c_str() << std::endl;
     program->link();
     std::cout << "Shader link log: " << program->log().toStdString().c_str() << std::endl;
     program->bind();

     vertexLocation = program->attributeLocation("vertex");
     matrixLocation = program->attributeLocation("matrix");
     colorLocation = program->uniformLocation("color");
 }

 void VolumeRenderer::paintGL()
 {

     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     //glLoadIdentity();

     /*
     glTranslatef(0.0, 0.0, -10.0);
     glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
     glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
     glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
    */

     std::cout << "paintGL called" << std::endl;

     static GLfloat const triangleVertices[] = {
         0.0f,  0.0f,  0.0f,
         100.0f, 0.0f, 0.0f,
         0.0f,  100.0f, 0.0f
     };


     QColor color(0, 255, 0, 255);

     QMatrix4x4 pmvMatrix;
     pmvMatrix.ortho(rect());

     program->enableAttributeArray(vertexLocation);
     program->setAttributeArray(vertexLocation, triangleVertices, 3);
     program->setUniformValue(matrixLocation, pmvMatrix);
     program->setUniformValue(colorLocation, color);

     glDrawArrays(GL_TRIANGLES, 0, 3);

     program->disableAttributeArray(vertexLocation);
 }

 void VolumeRenderer::resizeGL(int width, int height)
 {
     //int side = qMin(width, height);
     //glViewport((width - side) / 2, (height - side) / 2, side, side);
     /*
    glViewport(0, 0, width, height);


     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     glOrtho(-500.5, +500.5, -500.5, +500.5, 0.0, 150.0);

     glMatrixMode(GL_MODELVIEW);
     */
 }

 void VolumeRenderer::mousePressEvent(QMouseEvent *event)
 {
     lastPos = event->pos();
 }

 void VolumeRenderer::mouseMoveEvent(QMouseEvent *event)
 {
     int dx = event->x() - lastPos.x();
     int dy = event->y() - lastPos.y();

     if (event->buttons() & Qt::LeftButton) {
         setXRotation(xRot + 8 * dy);
         setYRotation(yRot + 8 * dx);
     } else if (event->buttons() & Qt::RightButton) {
         setXRotation(xRot + 8 * dy);
         setZRotation(zRot + 8 * dx);
     }
     lastPos = event->pos();
 }
