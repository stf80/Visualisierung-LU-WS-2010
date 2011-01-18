#ifndef RenderingView_H
#define RenderingView_H

#include <QtGui>
#include <QtOpenGL>
#include <QGLWidget>

#include "mainwindow.h"

#include "FlowData.h"

const double PI = 3.14159265358979323846;

/**
 * @brief Rendering view of the volume data.
 *
*/
class RenderingView
        //     : public QGLWidget
    : public QWidget
{
    Q_OBJECT

public:
    /**
      * @brief Default constructor
      *
      * @param parent parent widget
     */
    RenderingView(Ui::MainWindow *ui, QWidget *parent = 0);

    /**
      * @brief Default destructor.
      *
     */
    ~RenderingView();

    /**
          * @brief Redraws the rendering view onto an arbitrary device using a QPainter.
          *
          * @param p the QPainter to use for drawing
         */
    //void paint(QPainter *p);

    /**
          * @brief Invoked when the widget should redraw itself. Redraws the widget using paint().
          *
          * @param e the event causing the widget to redraw itself
         */
    void paintEvent(QPaintEvent *e);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;



public:
    void setDataset(FlowData* dataset);

    /// maps a normalized value to an ARGB color using a given gradient function
    QRgb normValueToRGB(float normValue);

public slots:
    void updateDerivedChannels();
    void updateColorCoding();
    void updateArrowPlot();
    void updateStreamlines();

protected:
    /*
     virtual void initializeGL();
     virtual void paintGL();
     virtual void resizeGL(int width, int height);
    */

private:
    Ui::MainWindow *ui;

    //QGLShaderProgram *program;

    // int width, height; // witdh and height of viewport

    FlowData *flowData;

    int channelVectorLength;

    bool colorCodingNeedsUpdate, arrowPlotNeedsUpdate, streamlinesNeedsUpdate;

    QImage colorCodingImage, arrowPlotImage, streamlinesImage;

    typedef QList<vec3> Streamline;
    Streamline computeStreamline(vec3 p, int w, int h);
    vec3 selectSeedPoint(Streamline streamLine, bool& valid);
    bool isPointValid(vec3 p, int testDistance);
    void addPointToLookup(vec3 p);
    vec3 integratePoint(vec3 pos, FlowChannel* channelX, FlowChannel* channelY, float dt, float direction);
    void drawStreamline(const Streamline& streamline, QPainter& painter);

    QList<vec3>* lookupGrid; // two-dimensional cartesian lookup structure. each cell stores a list of points (x,y); z is ignored
    int lookupW, lookupH; // dimensions of cartesian grid
    int dSep, dTest;
};

#endif // RenderingView_H
