#ifndef RenderingView_H
#define RenderingView_H

#include <QtGui>
#include <QtOpenGL>
#include <QGLWidget>

#include "mainwindow.h"

#include "FlowData.h"

const double PI = 3.14159265358979323846;

/**
 * @brief Rendering view of flow data.
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
      @param ui the main window, containing pointers to UI objects
      * @param parent parent widget
     */
    RenderingView(Ui::MainWindow *ui, QWidget *parent = 0);

    /**
      * @brief Default destructor.
      *
     */
    ~RenderingView();

   /**
          * @brief Invoked when the widget should redraw itself. Redraws the widget using paint().
          *
          * @param e the event causing the widget to redraw itself
         */
    void paintEvent(QPaintEvent *e);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;



public:
    /**
      @brief Changes the data set that is to be displayed.
      @param dataset the data set to display
      */
    void setDataset(FlowData* dataset);

    /**
    @brief Maps a normalized value to an ARGB color. The gradient used depends on the value of the gradient combo box.
    @param normValue the value to map
    */
    QRgb normValueToRGB(float normValue);

public slots:
    /**
      @brief Recalculates derived channels (currently only vector length) from data set.
      */
    void updateDerivedChannels();
    /**
      @brief Causes color coding image plane to be redrawn.
      */
    void updateColorCoding();
    /**
      @brief Causes arrow plot image plane to be redrawn.
      */
    void updateArrowPlot();
    /**
      @brief Causes streamlines image plane to be redrawn.
      */
    void updateStreamlines();

protected:

private:
    Ui::MainWindow *ui;

    FlowData *flowData;

    int channelVectorLength;

    bool colorCodingNeedsUpdate, arrowPlotNeedsUpdate, streamlinesNeedsUpdate;

    QImage colorCodingImage, arrowPlotImage, streamlinesImage;

    typedef QList<vec3> Streamline;
    Streamline computeStreamline(vec3 p, int w, int h, bool lookup=true); // lookup: use lookup structure and check against dSep/dTest (yes/no)
    vec3 selectSeedPoint(Streamline streamLine, bool& valid);
    bool isPointValid(vec3 p, int testDistance);
    float distanceToOthers(vec3 p, const Streamline& streamline);
    void addPointToLookup(vec3 p);
    vec3 integratePoint(vec3 pos, FlowChannel* channelX, FlowChannel* channelY, float dt, float direction);
    void drawStreamline(const Streamline& streamline, QPainter& painter, int w, int h, bool lookup=true); //lookup: lookup structure available (for tapering)

    QList<vec3>* lookupGrid; // two-dimensional cartesian lookup structure. each cell stores a list of points (x,y); z is ignored
    int lookupW, lookupH; // dimensions of cartesian grid
    int dSep, dTest;
};

#endif // RenderingView_H
