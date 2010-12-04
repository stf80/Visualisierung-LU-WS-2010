#include <GL/glew.h>
#include <QtGui>
#include <QtOpenGL>

/*//#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#include "GL/wglext.h"
*/
//#include "GL/glext.h"

#include <math.h>
#include <iostream>


#include "volumerenderer.h"

VolumeRenderer::VolumeRenderer(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    transferLUT = QImage(4096, 1, QImage::Format_ARGB32);

    image = new QImage(100, 100, QImage::Format_RGB32);

        xRot = 0;
        yRot = 0;
        zRot = 0;

    volume = 0;
    options = 0;
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

    updateGL();
}

void VolumeRenderer::setVolume(Volume *volume)
{
    this->volume = volume;


    GLfloat *data = new GLfloat[volume->GetSize()];
    /*for (int i = 0; i < volume->GetWidth(); ++i)
    {
        for (int j = 0; j < volume->GetHeight(); ++j)
        {
            for (int i = 0; i < volume->GetDepth(); ++k)
            {
            }
        }
    }
    */
    for (int i = 0; i < volume->GetSize(); ++i)
    {
        data[i] = volume->Get(i).GetValue();
    }

    glBindTexture(GL_TEXTURE_3D, textureName);

    glTexImage3DEXT1(GL_TEXTURE_3D, 0, GL_ALPHA,
//    glTexImage3D(GL_TEXTURE_3D, 0, GL_ALPHA,
                 volume->GetWidth(), volume->GetHeight(), volume->GetDepth(),
                 0, GL_ALPHA, GL_FLOAT, data);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    delete[] data;

    updateGL();
}

void VolumeRenderer::setRenderingOptions(RenderingOptions *options)
{
    this->options = options;

    updateGL();
}


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
     //glewInit();
     std::cerr << "Widget is valid: " << isValid() << std::endl;
     std::cerr << "context " << QGLContext::currentContext() << std::endl;
     std::cerr << "has OpenGL shader: " << QGLShaderProgram::hasOpenGLShaderPrograms() << std::endl;
     std::cerr << "context " << context() << " is valid " << context()->isValid() << std::endl;
      program = new QGLShaderProgram(context());



     //qglClearColor(qtPurple.dark());
    glClearColor(0.0, 0.0, 0.0, 1.0);

     glEnable(GL_DEPTH_TEST);
     glEnable(GL_CULL_FACE);
     glShadeModel(GL_SMOOTH);
     glEnable(GL_LIGHTING);
     glEnable(GL_LIGHT0);
     glEnable(GL_MULTISAMPLE);
     static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
     glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

     program->addShaderFromSourceCode(QGLShader::Vertex,
         "attribute highp vec4 vertex;\n"
         "attribute vec4 texCoord;"
         "void main(void)\n"
         "{\n"
         "   gl_Position = vertex;\n"
         "  gl_TexCoord[0] = texCoord;"
         "}");
     std::cout << "Vertex shader log: " << program->log().toStdString().c_str() << std::endl;

     program->addShaderFromSourceFile(QGLShader::Fragment, "../Beispiel1/FragmentShader.glsl");

     std::cerr << "Fragment shader log: " << program->log().toStdString().c_str() << std::endl;
     program->link();
     std::cout << "Shader link log: " << program->log().toStdString().c_str() << std::endl;
     program->bind();

     vertexLocation = program->attributeLocation("vertex");
     texCoordLocation = program->attributeLocation("texCoord");

     n0Location = program->uniformLocation("n0");
     uLocation = program->uniformLocation("u");
     vLocation = program->uniformLocation("v");
     volumeSizeLocation = program->uniformLocation("volumeSize");
     volumeResolutionLocation = program->uniformLocation("volumeResolution");
     NLocation = program->uniformLocation("N");
     samplerLocation = program->uniformLocation("sampler");
     ambientColorLocation = program->uniformLocation("c_ambient");
     diffuseColorLocation = program->uniformLocation("c_diffuse");
     specularColorLocation = program->uniformLocation("c_specular");
     specularExponentLocation = program->uniformLocation("c_exponent");

     // 3D texture
     glGenTextures(1, &textureName);

     //glTexImage3D = (PFNGLTEXIMAGE3DPROC) wglGetProcAddress("glTexImage3D");
    //glTexImage3DEXT1 = (PFNGLTEXIMAGE3DEXTPROC)wglGetProcAddress("glTexImage3DEXT");


 }

 void VolumeRenderer::paintGL()
 {
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

     if (! volume || ! options)
         return;

     float x = width/4, y = height/4;
    x = 1.f; y = 1.f;

     static GLfloat const quadVertices[] = {
         x,  y, 0.0f,
        -x,  y, 0.0f,
        -x, -y, 0.0f,
         x, -y, 0.0f
    };

     static GLfloat const texCoords[] = {
         width / 2,  height / 2,
        -width / 2,  height / 2,
        -width / 2, -height / 2,
         width / 2, -height / 2,
     };

     QMatrix4x4 pmvMatrix;
     pmvMatrix.rotate(xRot, QVector3D(1.f, 0, 0));
     pmvMatrix.rotate(yRot, QVector3D(0, 1.f, 0));
     pmvMatrix.rotate(zRot, QVector3D(0, 0, 1.f));

     QVector3D n0 = pmvMatrix * QVector3D(0, 0, -1.f);
     // FIXME: keep aspect ratio
     QVector3D u = pmvMatrix * QVector3D(1.f / width, 0, 0) * 2;
     QVector3D v = pmvMatrix * QVector3D(0, 1.f / height, 0) * 2;

     QVector3D volumeResolution(volume->GetWidth(), volume->GetHeight(), volume->GetDepth());

     QVector3D volumeSize(volumeResolution);
     float max = volumeSize.x();
     if (volumeSize.y() > max)
         max = volumeSize.y();
     if (volumeSize.z() > max)
         max = volumeSize.z();
     volumeSize *= 1 / max;

     program->enableAttributeArray(vertexLocation);
     program->setAttributeArray(vertexLocation, quadVertices, 3);
     program->enableAttributeArray(texCoordLocation);
     program->setAttributeArray(texCoordLocation, texCoords, 2);

     program->setUniformValue(n0Location, n0);
     program->setUniformValue(uLocation, u);
     program->setUniformValue(vLocation, v);
     program->setUniformValue(NLocation, options->N);
     program->setUniformValue(volumeSizeLocation, volumeSize);
     program->setUniformValue(volumeResolutionLocation, volumeResolution);
     program->setUniformValue(samplerLocation, 0);
     program->setUniformValue(ambientColorLocation, options->ambient);
     program->setUniformValue(diffuseColorLocation, options->diffuse);
     program->setUniformValue(specularColorLocation, options->specular);
     program->setUniformValue(specularExponentLocation, options->exponent);

     glBindTexture(GL_TEXTURE_3D, textureName);

     glDrawArrays(GL_QUADS, 0, 4);

     program->disableAttributeArray(vertexLocation);
     program->disableAttributeArray(texCoordLocation);
 }

 void VolumeRenderer::resizeGL(int width, int height)
 {
    // int side = qMin(width, height);
    // glViewport((width - side) / 2, (height - side) / 2, side, side);

    glViewport(0, 0, width, height);

    this->width = width;
    this->height = height;

    /*
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     glOrtho(-500.5, +500.5, -500.5, +500.5, -10.0, 150.0);

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

     static const float s = 1.f;
     if (event->buttons() & Qt::LeftButton) {
         setXRotation(xRot + s * dy);
         setYRotation(yRot + s * dx);
     } else if (event->buttons() & Qt::RightButton) {
         setXRotation(xRot + s * dy);
         setZRotation(zRot + s * dx);
     }
     lastPos = event->pos();
 }
