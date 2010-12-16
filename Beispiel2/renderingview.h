#ifndef RenderingView_H
#define RenderingView_H

#include <QtGui>
#include <QtOpenGL>
#include <QGLWidget>

#include "mainwindow.h"
#include "arthurwidgets.h"

#include "FlowData.h"
#include "renderingoptions.h"


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
         void paint(QPainter *p);

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
    void setRenderingOptions(RenderingOptions *options);

     public slots:

 protected:
    /*
     virtual void initializeGL();
     virtual void paintGL();
     virtual void resizeGL(int width, int height);
    */

 private:
     Ui::MainWindow *ui;

     QGLShaderProgram *program;

     // int width, height; // witdh and height of viewport

     FlowData *flowData;
     RenderingOptions *options;

 };

#endif // RenderingView_H
