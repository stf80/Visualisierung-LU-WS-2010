#ifndef SLICINGVIEW_H
#define SLICINGVIEW_H

#include <QtGui>
#include "Volume.h"

/**
  * @brief Widget for displaying slicing view.
  */
class SlicingView : public QWidget
{
public:
    SlicingView();
    SlicingView(QGroupBox*& gb);
    virtual ~SlicingView();

    virtual void paintEvent(QPaintEvent* event);

    /**
      * @brief Set Volume to display.
      */
    inline void setVolume(Volume *volume) { this->volume = volume; }
    /**
      * @brief Set slicing direction.
      */
    inline void setDirection(int direction) { this->direction = direction; }
    /**
      * @brief Set slice position.
      *
      * E.g. for setDirection(DIRECTION_X), this specifies the slice's x position.
      */
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
