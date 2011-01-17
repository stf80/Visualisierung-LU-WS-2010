#include "gradientview.h"

GradientView::GradientView(RenderingView *renderingView, QWidget *parent) :
    QWidget(parent)
{
    this->renderingView = renderingView;
}

void GradientView::paintEvent(QPaintEvent *e)
{
    QPainter gradientPainter(this);

    for (int x = 0; x < width(); ++x)
    {
        QPen pen(QColor(renderingView->normValueToRGB(((float) x) / width())));
        gradientPainter.setPen(pen);

        gradientPainter.drawLine(x, 0, x, height());
    }
}

QSize GradientView::minimumSizeHint() const
{
    return QSize(50, 20);
}

QSize GradientView::sizeHint() const
{
    return QSize(100, 40);
}
