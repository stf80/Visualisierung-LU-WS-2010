#ifndef SLICINGVIEW_H
#define SLICINGVIEW_H

#include <QtGui>
#include "Volume.h"

class SlicingView : public QWidget
{
public:
    SlicingView();
    SlicingView(QGroupBox*& gb);
    virtual ~SlicingView();

    virtual void paintEvent(QPaintEvent* event);

    inline void setVolume(Volume *volume) { this->volume = volume; }
    inline void setDirection(int direction) { this->direction = direction; }
    inline void setPosition(int position) { this->position = position; }

    enum {
        DIRECTION_X,
        DIRECTION_Y,
        DIRECTION_Z
    };

private:
    Volume *volume;
    int direction;
    int position;
    QImage* image;
};

#endif
