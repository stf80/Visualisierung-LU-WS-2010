#ifndef SLICINGVIEW_H
#define SLICINGVIEW_H

#include <QtGui>
#include "Volume.h"

class SlicingView
{
public:
    SlicingView(const Volume& volume);
    virtual ~SlicingView();

    QImage* render(int direction, int depth);

    enum {
        DIRECTION_X,
        DIRECTION_Y,
        DIRECTION_Z
    };

private:
    const Volume& volume;
    int direction;
    int depth;
    QImage* image;
};

#endif
