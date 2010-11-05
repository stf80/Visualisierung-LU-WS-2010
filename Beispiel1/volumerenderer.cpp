
#include "volumerenderer.h"

VolumeRenderer::VolumeRenderer(QWidget *parent)
//    : ArthurFrame(parent)
//    : QGraphicsView(parent)
    : QWidget(parent)
{
    transferLUT = QImage(4096, 1, QImage::Format_ARGB32);

    image = new QImage(100, 100, QImage::Format_RGB32);
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
