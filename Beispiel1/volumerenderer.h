#ifndef VOLUMERENDERER_H
#define VOLUMERENDERER_H

#include <QtGui>

#include "arthurwidgets.h"

class VolumeRenderer :
//        public ArthurFrame
        public QGraphicsView
{
    Q_OBJECT

public:
    VolumeRenderer(QWidget *parent);

    ~VolumeRenderer();

        void paint(QPainter *p);

        QSize sizeHint() const { return QSize(400, 400); }

        void mousePressEvent(QMouseEvent *e);

    public slots:
        void setGradientStops(const QGradientStops &stops);

    private:
        QGradientStops m_stops;

        /// Stores the rendered image;
        QImage *image;

        /// Stores a look-up table for the transfer function.
        /// Contains RGBA values. The resolution is 1x4096 pixels.
        QImage transferLUT;

        /// maps density values to RGBA colors using the transfer look-up table.
        /// parameter values:   values are clamped to the interval [0; 1]
        /// returns the RGBA color mapped to the density value
        QRgb transfer(float value);

        /// updates the transfer function
        void updateTransfer();
};

#endif // VOLUMERENDERER_H
