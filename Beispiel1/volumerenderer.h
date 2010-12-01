#ifndef VOLUMERENDERER_H
#define VOLUMERENDERER_H

#include <QtGui>
#include <QtOpenGL>
#include <QGLWidget>

#include "GL/glext.h"

#include "arthurwidgets.h"

#include "Volume.h"
#include "renderingoptions.h"

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

/**
 * @brief Rendering view of the volume data.
 *
*/
 class VolumeRenderer : public QGLWidget
 {
     Q_OBJECT

 public:
     /**
      * @brief Default constructor
      *
      * @param parent parent widget
     */
     VolumeRenderer(QWidget *parent = 0);

     /**
      * @brief Default destructor.
      *
     */
     ~VolumeRenderer();

         /**
          * @brief Redraws the rendering view onto an arbitrary device using a QPainter.
          *
          * @param p the QPainter to use for drawing
         */
         //void paint(QPainter *p);

         /**
          * @brief Invoked when the widget should redraw itself. Redraws the widget using paint().
          *
          * @param e the event causing the widget to redraw itself
         */
         //void paintEvent(QPaintEvent *e);

         QSize minimumSizeHint() const;
         QSize sizeHint() const;

 public:
    void setVolume(Volume* volume);
    void setRenderingOptions(RenderingOptions *options);

     public slots:
         /**
          * @brief Sets the points defining the color and transparency gradient of the transfer function.
          *
          * @param stops list of points received from the gradient editor
         */
         void setGradientStops(const QGradientStops &stops);
 public slots:
     void setXRotation(int angle);
     void setYRotation(int angle);
     void setZRotation(int angle);

 signals:
     void xRotationChanged(int angle);
     void yRotationChanged(int angle);
     void zRotationChanged(int angle);

 protected:
     virtual void initializeGL();
     virtual void paintGL();
     virtual void resizeGL(int width, int height);
     void mousePressEvent(QMouseEvent *event);
     void mouseMoveEvent(QMouseEvent *event);

 private:
     QGradientStops m_stops; /** gradient stops used for the transfer function */

     QImage *image; /** Stores the rendered image. */

     QImage transferLUT; /** Stores a look-up table for the transfer function. Contains RGBA values. The resolution is 1x4096 pixels. */

     /**
      * @brief Maps density values to RGBA colors using the transfer look-up table.
      *
      * @param value the density value to map; values are clamped to the interval [0; 1]
      * @return QRgb the RGBA color mapped to the density value
     */
     QRgb transfer(float value);

     ///
     /**
      * @brief Updates the transfer function look-up table.
      *
     */
     void updateTransfer();
 private:
     int xRot;
     int yRot;
     int zRot;
     QPoint lastPos;
 private:
     QGLShaderProgram *program;
     int vertexLocation, texCoordLocation;
     int n0Location, uLocation, vLocation;
     int NLocation, volumeSizeLocation, volumeResolutionLocation;
     int samplerLocation;

     int width, height; // witdh and height of viewport

     Volume *volume;
     RenderingOptions *options;
     GLuint textureName;

     PFNGLTEXIMAGE3DEXTPROC glTexImage3DEXT1;

 };

#endif // VOLUMERENDERER_H
