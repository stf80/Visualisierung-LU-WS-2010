#include "slicingview.h"
#include "Volume.h"

SlicingView::SlicingView(void)
    :QWidget(), volume(NULL), image(NULL)
{
}

SlicingView::SlicingView(QGroupBox*& gb)
    :QWidget(gb), volume(NULL), image(NULL)
{
}

SlicingView::~SlicingView()
{
}

void SlicingView::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    if (volume == NULL) {
        painter.fillRect(0, 0, this->width(), this->height(), QColor(0, 0, 0));
        return;
    }
    if (image != NULL) {
        delete image;
        image = NULL;
    }
    int x,y,z;
    int value;
    switch (this->direction) {
    case DIRECTION_X:
        x = this->position;
        if (x < 0 || x >= volume->GetWidth()) break;
        image = new QImage(volume->GetDepth(), volume->GetHeight(), QImage::Format_RGB32);
        for (y=0; y<volume->GetHeight(); y++) {
            for (z=0; z<volume->GetDepth(); z++) {
                value = (int)(volume->Get(x, y, z).GetValue() * 255.f);
                image->setPixel(z, y, qRgb(value, value, value));
            }
        }
        break;
    case DIRECTION_Y:
        y = this->position;
        if (y < 0 || y >= volume->GetHeight()) break;
        image = new QImage(volume->GetWidth(), volume->GetDepth(), QImage::Format_RGB32);
        for (x=0; x<volume->GetWidth(); x++) {
            for (z=0; z<volume->GetDepth(); z++) {
                value = (int)(volume->Get(x, y, z).GetValue() * 255.f);
                image->setPixel(x, z, qRgb(value, value, value));
            }
        }
        break;
    case DIRECTION_Z:
        z = this->position;
        if (z < 0 || z >= volume->GetDepth()) break;
        image = new QImage(volume->GetWidth(), volume->GetHeight(), QImage::Format_RGB32);
        for (x=0; x<volume->GetWidth(); x++) {
            for (y=0; y<volume->GetHeight(); y++) {
                value = (int)(volume->Get(x, y, z).GetValue() * 255.f);
                image->setPixel(x, y, qRgb(value, value, value));
            }
        }
        break;
    default: break;
    }
    if (image != NULL) {
        painter.drawImage(QRect(0, 0, this->width(), this->height()), *image);
    }
}
