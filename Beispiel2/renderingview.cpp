#define GLEW_STATIC
#include <GL/glew.h>
#include <QtGui>
#include <QtOpenGL>

#include <math.h>
#include <iostream>


#include "renderingview.h"
#include "ui_mainwindow.h"

RenderingView::RenderingView(Ui::MainWindow *ui, QWidget *parent)
//    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
    : QWidget(parent)
{
    this->ui = ui;
    flowData = 0;
    options = 0;
}

RenderingView::~RenderingView()
{

}

void RenderingView::setDataset(FlowData* dataset)
{
    flowData = dataset;

    update();
}

void RenderingView::setRenderingOptions(RenderingOptions *options)
{
    this->options = options;

    //updateGL();
    update();
}



 QSize RenderingView::minimumSizeHint() const
 {
     return QSize(200, 200);
 }

 QSize RenderingView::sizeHint() const
 {
     return QSize(400, 400);
 }

 void RenderingView::paintEvent(QPaintEvent *e)
 {
     if (! flowData)
         return;

     QPainter painter(this);

      if (ui->colorCodingActive->isEnabled())
      {

          QImage colorCodingImage(width(), height(), QImage::Format_ARGB32);

         colorCodingImage.fill(0x0000FF00);

         FlowChannel *channel = flowData->getChannel(ui->colorCodingChannel->value());

         if (channel && channel->getRange() > 0) {
             for (int y = 0; y < height(); ++y)
                 for (int x = 0; x < width(); ++x)
                 {
                     float rawValue = channel->getValueNormPos(((float) x) / width(), ((float) y) / height());
                     float normValue = channel->normalizeValue(rawValue);

                     colorCodingImage.setPixel(x, y, QColor(normValue * 255, normValue * 255, normValue * 255).rgba());
                 }

                painter.drawImage(0, 0, colorCodingImage);
        }
     }

 }

/*
 void RenderingView::initializeGL()
 {
     glewInit();
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
     glEnable(GL_TEXTURE_1D);
     glEnable(GL_TEXTURE_3D);
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



 }

 void RenderingView::paintGL()
 {
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 }

 void RenderingView::resizeGL(int width, int height)
 {

    glViewport(0, 0, width, height);

    this->width = width;
    this->height = height;

 }

*/
