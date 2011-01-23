#ifndef GRADIENTVIEW_H
#define GRADIENTVIEW_H

#include <QWidget>

#include <renderingview.h>

/**
  @brief A widget displaying a color gradient.
  */
class GradientView : public QWidget
{
    Q_OBJECT
public:
    /**
      * @brief Default constructor
      *
      @param renderingView the view defining the color gradients
      * @param parent parent widget
     */
    explicit GradientView(RenderingView *renderingView, QWidget *parent = 0);

    /**
      * @brief Invoked when the widget should redraw itself. Redraws the widget using paint().
      *
      * @param e the event causing the widget to redraw itself
     */
    void paintEvent(QPaintEvent *e);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

signals:

public slots:

private:
    RenderingView *renderingView;

};

#endif // GRADIENTVIEW_H
