#define GLEW_STATIC
#include <GL/glew.h>
#include <QtGui>
#include <QtOpenGL>

#include <math.h>
#include <iostream>


#include "RenderingView.h"

RenderingView::RenderingView(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    flowData = 0;
    options = 0;
}

RenderingView::~RenderingView()
{

}

void RenderingView::setDataset(FlowData* dataset)
{
    flowData = dataset;
}

void RenderingView::setRenderingOptions(RenderingOptions *options)
{
    this->options = options;

    updateGL();
}



 QSize RenderingView::minimumSizeHint() const
 {
     return QSize(200, 200);
 }

 QSize RenderingView::sizeHint() const
 {
     return QSize(400, 400);
 }


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

