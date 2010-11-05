#ifndef VOLUMERENDERER_H
#define VOLUMERENDERER_H

#include <QtGui>

#include "arthurwidgets.h"

/**
 * @brief Rendering view of the volume data.
 *
*/
class VolumeRenderer :
//        public ArthurFrame
//        public QGraphicsView
        public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Default constructor
     *
     * @param parent parent widget
    */
    VolumeRenderer(QWidget *parent = 0);

    /**
     * @brief Default destructor.
     *
    */
    ~VolumeRenderer();

        /**
         * @brief Redraws the rendering view onto an arbitrary device using a QPainter.
         *
         * @param p the QPainter to use for drawing
        */
        void paint(QPainter *p);

        /**
         * @brief Invoked when the widget should redraw itself. Redraws the widget using paint().
         *
         * @param e the event causing the widget to redraw itself
        */
        void paintEvent(QPaintEvent *e);

        QSize sizeHint() const { return QSize(400, 400); }

    public slots:
        /**
         * @brief Sets the points defining the color and transparency gradient of the transfer function.
         *
         * @param stops list of points received from the gradient editor
        */
        void setGradientStops(const QGradientStops &stops);

    private:
        QGradientStops m_stops; /** gradient stops used for the transfer function */

        QImage *image; /** Stores the rendered image. */

        QImage transferLUT; /** Stores a look-up table for the transfer function. Contains RGBA values. The resolution is 1x4096 pixels. */

        /**
         * @brief Maps density values to RGBA colors using the transfer look-up table.
         *
         * @param value the density value to map; values are clamped to the interval [0; 1]
         * @return QRgb the RGBA color mapped to the density value
        */
        QRgb transfer(float value);

        ///
        /**
         * @brief Updates the transfer function look-up table.
         *
        */
        void updateTransfer();

};

#endif // VOLUMERENDERER_H
