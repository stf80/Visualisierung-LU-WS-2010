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

    colorCodingNeedsUpdate = true;
    arrowPlotNeedsUpdate = true;
    streamlinesNeedsUpdate = true;
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

    colorCodingNeedsUpdate = true;
    arrowPlotNeedsUpdate = true;
    streamlinesNeedsUpdate = true;

    update();
}

void RenderingView::updateColorCoding()
{
    colorCodingNeedsUpdate = true;
    update();
}

void RenderingView::updateArrowPlot()
{
    arrowPlotNeedsUpdate = true;
    update();
}

void RenderingView::updateStreamlines()
{
    streamlinesNeedsUpdate = true;
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

QRgb RenderingView::normValueToRGB(float normValue)
{
    int gradient = ui->colorCodingGradient->currentIndex();

    switch (gradient)
    {
    default:
    case 0: // grayscale
        return QColor(normValue * 255, normValue * 255, normValue * 255).rgba();
        break;
    case 1: // heat
        return QColor(normValue * 255, 255 - normValue * 255, 0).rgba();
        break;
    case 2: // HSV, with varying hue
        QColor color;
        color.setHsvF(normValue, 1.f, 1.f);

        return color.rgba();
        break;
    // TODO: other gradients
    }
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

    // check if images need resizing
    static int prevW = 0, prevH = 0;
    if (w != prevW || h != prevH)
    {
        colorCodingImage = QImage(w, h, QImage::Format_ARGB32_Premultiplied);
        arrowPlotImage = QImage(w, h, QImage::Format_ARGB32_Premultiplied);
        streamlinesImage = QImage(w, h, QImage::Format_ARGB32_Premultiplied);

        colorCodingNeedsUpdate = true;
        arrowPlotNeedsUpdate = true;
        streamlinesNeedsUpdate = true;

        prevW = w;
        prevH = h;
    }

    QPainter painter(this);

    if (ui->colorCodingActive->isChecked())
    {
        FlowChannel *channel = flowData->getChannel(ui->colorCodingChannel->value());

        if (colorCodingNeedsUpdate && channel && channel->getRange() > 0)
        {
            colorCodingImage.fill(0x0000FF00);

            for (int y = 0; y < h; ++y)
            {
                for (int x = 0; x < w; ++x)
                {
                    float rawValue = channel->getValueNormPos(((float) x) / w, ((float) y) / h);
                    float normValue = channel->normalizeValue(rawValue);

                    colorCodingImage.setPixel(x, y, normValueToRGB(normValue));
                }
            }

            colorCodingNeedsUpdate = false;
        }

        painter.drawImage((width() - w) / 2, (height() - h) / 2, colorCodingImage);
    }

    if (ui->arrowPlotActive->isChecked())
    {
        FlowChannel *channelX = flowData->getChannel(ui->arrowPlotChannelX->value());
        FlowChannel *channelY = flowData->getChannel(ui->arrowPlotChannelY->value());

        if (arrowPlotNeedsUpdate && channelX && channelY) {
            // TODO: try QImage::Format_ARGB32_Premultiplied for better performance,
            // or painting on QGLWidget (with OpenGL backend) instead of QImage
            arrowPlotImage.fill(0x00000000);

            QPainter arrowPlotPainter(&arrowPlotImage);
            arrowPlotPainter.setRenderHint(QPainter::Antialiasing);
            arrowPlotPainter.setBrush(Qt::black);

            int dist = ui->arrowPlotDistance->value();
            int size = dist * ui->arrowPlotSize->value() / 100;
            const QPointF arrowPoints[] = {
                QPointF(size / 2.f, 0),
                QPointF(-size / 2.f, -size / 3.f),
                QPointF(-size / 2.f,  size / 3.f)
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

            arrowPlotNeedsUpdate = false;
        }

        painter.drawImage((width() - w) / 2, (height() - h) / 2, arrowPlotImage);
    }

    if (ui->streamlinesActive->isChecked())
    {
        FlowChannel *channelX = flowData->getChannel(ui->arrowPlotChannelX->value());
        FlowChannel *channelY = flowData->getChannel(ui->arrowPlotChannelY->value());

        if (streamlinesNeedsUpdate && channelX && channelY) {
            streamlinesImage.fill(0x00000000);

            QPainter streamlinesPainter(&streamlinesImage);
            streamlinesPainter.setRenderHint(QPainter::Antialiasing);
            QBrush brush = Qt::black;
            streamlinesPainter.setBrush(brush);

            //float dt = ui->streamlinesTimeStep->value();
            //bool tapering = ui->streamlinesTapering->isChecked(),
            //    glyphMapping = ui->streamlinesGlyphMapping->isChecked();
            //int glyphDistance = ui->streamlinesGlyphDistance->value();

            if (ui->streamlinesSpacing->currentIndex() == 0) // regular spacing
            {
                int dist = ui->streamlinesDistance->value();

                for (int y = dist / 2; y < h; y += dist)
                {
                    //float normPosY = ((float) y) / h;

                    for (int x = dist / 2; x < w; x += dist)
                    {
                        Streamline sl = computeStreamline(vec3(x, y), w, h, false);
                        drawStreamline(sl, streamlinesPainter, w, h);
                    }
                }
            } else {
                dSep = ui->streamlinesDSep->value();
                dTest = ui->streamlinesDTest->value();
                lookupW = w / dSep;
                lookupH = h / dSep;
                lookupGrid = new QList<vec3>[lookupW * lookupH];
                QQueue<Streamline> streamlineQueue;
                Streamline currentStreamline = computeStreamline(vec3(w/2,h/2), w, h);
                drawStreamline(currentStreamline, streamlinesPainter, w, h);
                bool finished = false;
                do {
                    bool valid;
                    vec3 seedPoint = selectSeedPoint(currentStreamline, valid);
                    if (valid) {
                        qDebug() << "Valid seedpoint: (" << seedPoint.v[0] << "," << seedPoint.v[1] << ")";
                        streamlineQueue.append(computeStreamline(seedPoint, w, h));
                    } else {
                        qDebug() << "No valid seedpoint, next streamline (queue length: " << streamlineQueue.size() << ")";
                        if (streamlineQueue.empty()) {
                            finished = true;
                        } else {
                            currentStreamline = streamlineQueue.dequeue();
                            drawStreamline(currentStreamline, streamlinesPainter, w, h);
                        }
                    }
                } while (!finished);
                delete[] lookupGrid;
            }

            streamlinesNeedsUpdate = false;
        }

        painter.drawImage((width() - w) / 2, (height() - h) / 2, streamlinesImage);
    }
}

void RenderingView::drawStreamline(const Streamline& streamline, QPainter& painter, int w, int h)
{
    bool tapering = ui->streamlinesTapering->isChecked();
    bool glyphMapping = ui->streamlinesGlyphMapping->isChecked();
    int  glyphDistance = ui->streamlinesGlyphDistance->value();

    // length of path along streamline from last glyph to current position (in pixels)
    float pathLength = glyphDistance; // draw glyph at start of path

    for (int i=0; i<streamline.size()-1; i++) {
        const vec3& p0 = streamline[i];
        const vec3& p1 = streamline[i+1];

        if (tapering)
        {
            FlowChannel *channelLength = flowData->getChannel(channelVectorLength);

            float rawValueLength = channelLength->getValueNormPos(p0.v[0] / w, p0.v[1] / h);
            float normValueLength = channelLength->normalizeValue(rawValueLength);

            float width = 0.1f + normValueLength *
                          (ui->streamlinesMaximumWidth->value() - 0.1f);
            QPen pen(painter.brush(), width);
            painter.setPen(pen);
        }

        painter.drawLine(QPointF(p0.v[0], p0.v[1]), QPointF(p1.v[0], p1.v[1]));

        if (glyphMapping)
        {
            vec3 d = p1 - p0;
            float newPathLength = pathLength + d.length();

            if (newPathLength > glyphDistance)
            {
                int size = ui->streamlinesGlyphDistance->value() *
                           ui->streamlinesGlyphSize->value() / 100;
                const QPointF arrowPoints[] = {
                    QPointF(size / 2.f, 0),
                    QPointF(-size / 2.f, -size / 3.f),
                    QPointF(-size / 2.f,  size / 3.f)
                };

                float f = (newPathLength - glyphDistance) / (newPathLength - pathLength);
                vec3 glyphPos = p0 * f + p1 * (1 - f);

                painter.save();

                painter.translate(glyphPos.v[0], glyphPos.v[1]);
                painter.rotate(atan2(d.v[1], d.v[0]) * 180 / PI);

                FlowChannel *channelLength = flowData->getChannel(channelVectorLength);

                float rawValueLength = channelLength->getValueNormPos(p0.v[0] / w, p0.v[1] / h);
                float normValueLength = channelLength->normalizeValue(rawValueLength);

                // scale area, not length of vectors
                float scale = sqrt(normValueLength);
                painter.scale(scale, scale);

                painter.drawPolygon(arrowPoints, 3);
                painter.restore();

                newPathLength -= glyphDistance;
            }
            pathLength = newPathLength;
        }
    }
}

vec3 RenderingView::integratePoint(vec3 pos, FlowChannel* channelX, FlowChannel* channelY, float dt, float direction) // direction is either -1.0f or 1.0f
{
    vec3 v = vec3(channelX->getValue(pos), channelY->getValue(pos)) * dt;
    vec3 newPos;
    if (ui->streamlinesIntegration->currentIndex() == 0) // Euler
    {
        newPos = pos + v * direction;
    }
    else
    { // 2nd order Runge-Kutta
        vec3 midPoint = pos + v * 0.5f;

        if (midPoint.v[0] >= flowData->getMinX() && midPoint.v[0] <= flowData->getMaxX()
            && midPoint.v[1] >= flowData->getMinY() && midPoint.v[1] <= flowData->getMaxY())
        {
            float newRawValueX = channelX->getValue(midPoint);
            float newRawValueY = channelY->getValue(midPoint);

            v = vec3(newRawValueX, newRawValueY, 0) * dt;
            newPos = pos + v * direction;
        } else
            newPos = pos;
    }
    return newPos;
}

RenderingView::Streamline RenderingView::computeStreamline(vec3 p, int w, int h, bool lookup)
{
    int steps;
    vec3 pos;
    vec3 posPixel;
    FlowChannel *channelX = flowData->getChannel(ui->arrowPlotChannelX->value());
    FlowChannel *channelY = flowData->getChannel(ui->arrowPlotChannelY->value());
    float dt = ui->streamlinesTimeStep->value();
    vec3 normPos(p.v[0] / w, p.v[1] / h);
    // perform integration in geometry coordinates
    vec3 initialPos = flowData->unNormalizeCoords(normPos);
    Streamline streamlinePlus;
    streamlinePlus.append(p);
    if (lookup) addPointToLookup(p);
    steps = ui->streamlinesSteps->value();
    pos = initialPos;
    posPixel = p;
    while (pos.v[0] >= flowData->getMinX() && pos.v[0] <= flowData->getMaxX()
        && pos.v[1] >= flowData->getMinY() && pos.v[1] <= flowData->getMaxY()
        && steps > 0)
    {
        steps--;
        vec3 newPos = integratePoint(pos, channelX, channelY, dt, 1.0f);

        if (pos == newPos) // singularity
            break;

        vec3 newPosNormalized = flowData->normalizeCoords(newPos);
        vec3 newPosPixel(newPosNormalized.v[0] * w, newPosNormalized.v[1] * h);
        if (lookup && newPosPixel.dist(posPixel) < dTest) {
            pos = newPos;
            continue;
        }

        if (lookup && !isPointValid(newPosPixel, dTest)) break;
        streamlinePlus.append(newPosPixel);
        if (lookup) addPointToLookup(newPosPixel);
        pos = newPos;
        posPixel = newPosPixel;
    }
    Streamline streamlineMinus;
    steps = ui->streamlinesSteps->value();
    pos = initialPos;
    posPixel = p;
    while (pos.v[0] >= flowData->getMinX() && pos.v[0] <= flowData->getMaxX()
        && pos.v[1] >= flowData->getMinY() && pos.v[1] <= flowData->getMaxY()
        && steps > 0)
    {
        steps--;
        vec3 newPos = integratePoint(pos, channelX, channelY, dt, -1.0f);

        if (pos == newPos) // singularity
            break;

        vec3 newPosNormalized = flowData->normalizeCoords(newPos);
        vec3 newPosPixel(newPosNormalized.v[0] * w, newPosNormalized.v[1] * h);
        if (lookup && newPosPixel.dist(posPixel) < dTest) {
            pos = newPos;
            continue;
        }

        if (lookup && !isPointValid(newPosPixel, dTest)) break;
        streamlineMinus.append(newPosPixel);
        if (lookup) addPointToLookup(newPosPixel);
        pos = newPos;
        posPixel = newPosPixel;
    }

    Streamline streamline;
    for (int i=streamlineMinus.size() - 1; i>=0; i--) {
        streamline.append(streamlineMinus[i]);
    }
    for (int i=0; i<streamlinePlus.size(); i++) {
        streamline.append(streamlinePlus[i]);
    }

    return streamline;
}

vec3 RenderingView::selectSeedPoint(Streamline streamLine, bool& valid)
{
    if (streamLine.size() < 2) {
        valid = false;
        return vec3();
    }
    valid = true;
    for (int i=1; i<streamLine.size(); i++) {
        vec3& p0 = streamLine[i-1];
        vec3& p1 = streamLine[i];
        vec3 p0p1 = p1 - p0;
        vec3 mid = p0 + p0p1 / 2.0f;
        vec3 normal1(-p0p1.v[1], p0p1.v[0]);
        normal1 /= normal1.length();
        vec3 normal2(p0p1.v[1], -p0p1.v[0]);
        normal2 /= normal2.length();
        vec3 candidate1 = mid + normal1 * dSep;
        if (isPointValid(candidate1, dSep)) return candidate1;
        vec3 candidate2 = mid + normal2 * dSep;
        if (isPointValid(candidate2, dSep)) return candidate2;
    }
    valid = false;
    return vec3();
}

bool RenderingView::isPointValid(vec3 p, int testDistance)
{
    int x = p.v[0] / dSep;
    int y = p.v[1] / dSep;
    int idx = y * lookupW + x;
    if (idx < 0 || idx >= lookupW * lookupH) return false;
    const int indices[] = {
        idx, idx - 1, idx + 1,
        idx + lookupW, idx + lookupW - 1, idx + lookupW + 1,
        idx - lookupW, idx - lookupW - 1, idx - lookupW + 1,
    };
    for (int j = 0; j < 9; j++) {
        if (indices[j] < 0 || indices[j] >= lookupW * lookupH) continue;
        QList<vec3>& cell = lookupGrid[indices[j]];
        for (int i=0; i<cell.size(); i++) {
            vec3& cellPoint = cell[i];
            vec3 dv = cellPoint - p;
            float dist = sqrt(dv.v[0] * dv.v[0] + dv.v[1] * dv.v[1]);
            if (dist < (float)testDistance) return false;
        }
    }
    return true;
}

void RenderingView::addPointToLookup(vec3 p)
{
    int x = p.v[0] / dSep;
    int y = p.v[1] / dSep;
    int idx = y * lookupW + x;
    QList<vec3>& cell = lookupGrid[idx];
    cell.append(p);
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
