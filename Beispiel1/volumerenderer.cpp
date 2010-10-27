
#include "volumerenderer.h"

VolumeRenderer::VolumeRenderer(QWidget *parent)
//    : ArthurFrame(parent)
    : QGraphicsView(parent)
{
    transferLUT = QImage(4096, 1, QImage::Format_ARGB32);

    image = new QImage(100, 100, QImage::Format_RGB888);
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


void VolumeRenderer::mousePressEvent(QMouseEvent *)
{
    //setDescriptionEnabled(false);
}

void VolumeRenderer::paint(QPainter *p)
{
    //TODO replace code with raycasting


    for (int i = 0; i < 100; ++i)
    {

        image->setPixel(i, i, QColor(i, i, i).rgba());

        p->setBrush(QBrush(QColor(i,i,i)));
        p->drawPoint(i,i);
    }

    p->drawImage(100, 100, *image);
    return;

    QGradient g = QLinearGradient(QPoint(0, 0), QPoint(this->width(), this->height()));

    for (int i=0; i<m_stops.size(); ++i)
        g.setColorAt(m_stops.at(i).first, m_stops.at(i).second);

    g.setSpread(QGradient::PadSpread);

    p->setBrush(g);
    p->setPen(Qt::NoPen);

    p->drawRect(rect());
}
