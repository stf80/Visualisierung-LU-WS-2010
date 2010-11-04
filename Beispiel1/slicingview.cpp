#include "slicingview.h"
#include "Volume.h"

SlicingView::SlicingView(const Volume &volume)
    :volume(volume)
{

}

SlicingView::~SlicingView()
{
}

QImage* SlicingView::render(int direction, int depth)
{
    if (image != NULL) {
        delete image;
        image = NULL;
    }
    int x,y,z;
    int value;
    switch (direction) {
    case DIRECTION_X:
        x = depth;
        if (x < 0 || x >= volume.GetWidth()) break;
        image = new QImage(volume.GetDepth(), volume.GetHeight(), QImage::Format_RGB32);
        for (y=0; y<volume.GetHeight(); y++) {
            for (z=0; z<volume.GetDepth(); z++) {
                value = (int)(volume.Get(x, y, z).GetValue() * 255.f);
                image->setPixel(z, y, qRgb(value, value, value));
            }
        }
        break;
    case DIRECTION_Y:
        y = depth;
        if (y < 0 || y >= volume.GetHeight()) break;
        image = new QImage(volume.GetWidth(), volume.GetDepth(), QImage::Format_RGB32);
        for (x=0; x<volume.GetWidth(); x++) {
            for (z=0; y<volume.GetDepth(); y++) {
                value = (int)(volume.Get(x, y, z).GetValue() * 255.f);
                image->setPixel(x, z, qRgb(value, value, value));
            }
        }
        break;
    case DIRECTION_Z:
        z = depth;
        if (z < 0 || z >= volume.GetDepth()) break;
        image = new QImage(volume.GetWidth(), volume.GetHeight(), QImage::Format_RGB32);
        for (x=0; x<volume.GetWidth(); x++) {
            for (y=0; y<volume.GetHeight(); y++) {
                value = (int)(volume.Get(x, y, z).GetValue() * 255.f);
                image->setPixel(x, y, qRgb(value, value, value));
            }
        }
        break;
    default: break;
    }
    return image;
}
