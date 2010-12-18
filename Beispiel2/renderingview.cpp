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

    channelVectorLength = -1;
}

RenderingView::~RenderingView()
{

}

void RenderingView::setDataset(FlowData* dataset)
{
    flowData = dataset;

    channelVectorLength = -1;

    updateDerivedChannels();

    update();
}

void RenderingView::updateDerivedChannels()
{
    if (channelVectorLength >= 0)
        flowData->deleteChannel(channelVectorLength);

    channelVectorLength = flowData->createChannelVectorLength(
            ui->arrowPlotChannelX->value(), ui->arrowPlotChannelY->value(), -1);

    ui->arrowPlotChannelLength->setText(QString::number(channelVectorLength));

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

    float widgetAspectRatio = ((float) width()) / height();
    float dataAspectRatio = (flowData->getMaxX() - flowData->getMinX()) /
                            (flowData->getMaxY() - flowData->getMinY());

    int w, h;
    if ( widgetAspectRatio > dataAspectRatio)
    {
        h = height();
        w = h * dataAspectRatio;
    } else {
        w = width();
        h = w / dataAspectRatio;
    }

    QPainter painter(this);

    if (ui->colorCodingActive->isChecked())
    {
        FlowChannel *channel = flowData->getChannel(ui->colorCodingChannel->value());

        if (channel && channel->getRange() > 0) {

            QImage colorCodingImage(w, h, QImage::Format_ARGB32);

            colorCodingImage.fill(0x0000FF00);

            for (int y = 0; y < h; ++y)
            {
                for (int x = 0; x < w; ++x)
                {
                    float rawValue = channel->getValueNormPos(((float) x) / w, ((float) y) / h);
                    float normValue = channel->normalizeValue(rawValue);

                    colorCodingImage.setPixel(x, y, QColor(normValue * 255, normValue * 255, normValue * 255).rgba());
                }
            }

            painter.drawImage((width() - w) / 2, (height() - h) / 2, colorCodingImage);
        }
    }

    if (ui->arrowPlotActive->isChecked())
    {
        FlowChannel *channelX = flowData->getChannel(ui->arrowPlotChannelX->value());
        FlowChannel *channelY = flowData->getChannel(ui->arrowPlotChannelY->value());

        if (channelX && channelY) {
            // TODO: try QImage::Format_ARGB32_Premultiplied for better performance,
            // or painting on QGLWidget (with OpenGL backend) instead of QImage
            QImage arrowPlotImage(w, h, QImage::Format_ARGB32);
            arrowPlotImage.fill(0x00000000);

            QPainter arrowPlotPainter(&arrowPlotImage);
            arrowPlotPainter.setRenderHint(QPainter::Antialiasing);
            arrowPlotPainter.setBrush(Qt::black);

            int dist = ui->arrowPlotDistance->value();
            const QPointF arrowPoints[] = {
                QPointF(dist / 2.f, 0),
                QPointF(-dist / 2.f, -dist / 3.f),
                QPointF(-dist / 2.f,  dist / 3.f)
            };

            for (int y = dist / 2; y < h; y += dist)
            {
                float normPosY = ((float) y) / h;

                for (int x = dist / 2; x < w; x += dist)
                {
                    float normPosX = ((float) x) / w;

                    float rawValueX = channelX->getValueNormPos(normPosX, normPosY);
                    //float normValueX = channelX->normalizeValue(rawValueX);
                    float rawValueY = channelY->getValueNormPos(normPosX, normPosY);
                    //float normValueY = channelY->normalizeValue(rawValueY);

                    arrowPlotPainter.save();

                    arrowPlotPainter.translate(x, y);
                    arrowPlotPainter.rotate(atan2(rawValueY, rawValueX) * 180 / PI);

                    if (ui->arrowPlotScale->isChecked())
                    {
                        FlowChannel *channelLength = flowData->getChannel(channelVectorLength);

                        float rawValueLength = channelLength->getValueNormPos(normPosX, normPosY);
                        float normValueLength = channelLength->normalizeValue(rawValueLength);

                        // scale area, not length of vectors
                        float scale = sqrt(normValueLength);
                        arrowPlotPainter.scale(scale, scale);
                    }

                    arrowPlotPainter.drawPolygon(arrowPoints, 3);
                    arrowPlotPainter.restore();
                }
            }

            painter.drawImage((width() - w) / 2, (height() - h) / 2, arrowPlotImage);
        }
    }

    if (ui->streamlinesActive->isChecked())
    {
        FlowChannel *channelX = flowData->getChannel(ui->arrowPlotChannelX->value());
        FlowChannel *channelY = flowData->getChannel(ui->arrowPlotChannelY->value());

        if (channelX && channelY) {
            // TODO: try QImage::Format_ARGB32_Premultiplied for better performance,
            // or painting on QGLWidget (with OpenGL backend) instead of QImage
            QImage streamlinesImage(w, h, QImage::Format_ARGB32);
            streamlinesImage.fill(0x00000000);

            QPainter streamlinesPainter(&streamlinesImage);
            streamlinesPainter.setRenderHint(QPainter::Antialiasing);
            streamlinesPainter.setBrush(Qt::black);

            float dt = ui->streamlinesTimeStep->value();

            if (ui->streamlinesSpacing->currentIndex() == 0) // regular spacing
            {
                int dist = ui->streamlinesDistance->value();

                for (int y = dist / 2; y < h; y += dist)
                {
                    float normPosY = ((float) y) / h;

                    for (int x = dist / 2; x < w; x += dist)
                    {
                        float normPosX = ((float) x) / w;

                        // perform integration in geometry coordinates
                        vec3 pos = flowData->unNormalizeCoords(vec3(normPosX, normPosY, 0));

                        float t = 0, tMax = 100 * dt;

                        // TODO integrate in both directions
                        while (pos.v[0] >= flowData->getMinX() && pos.v[0] <= flowData->getMaxX()
                               && pos.v[1] >= flowData->getMinY() && pos.v[1] <= flowData->getMaxY()
                               && t <= tMax)
                        {

                                    float rawValueX = channelX->getValue(pos);
                                    float rawValueY = channelY->getValue(pos);

                                    vec3 newPos;
                                    if (ui->streamlinesIntegration->currentIndex() == 0) // Euler
                                    {
                                        vec3 v = vec3(rawValueX, rawValueY, 0) * dt;
                                        newPos = pos + v;
                                    } else { // TODO: Runge-Kutta
                                    }

                                    if (pos == newPos) // singularity
                                        break;

                                    // transform real geometrical coordinates to pixel coordinates
                                    vec3 v1 = flowData->normalizeCoords(pos),
                                        v2 = flowData->normalizeCoords(newPos);

                                    streamlinesPainter.drawLine(QPointF(v1.v[0] * w, v1.v[1] * h),
                                                                QPointF(v2.v[0] * w, v2.v[1] * h));

                                    t += dt;
                                    pos = newPos;
                        }
                    }
                }
            } else { // TODO: evenly-spaced streamlines
            }

            painter.drawImage((width() - w) / 2, (height() - h) / 2, streamlinesImage);
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
