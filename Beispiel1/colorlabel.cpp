#include <QColorDialog>

#include "colorlabel.h"

ColorLabel::ColorLabel(QWidget *parent) :
    QLabel(parent)
//    QPushButton(parent)
{
    setFrameShape(QFrame::StyledPanel);
}

void ColorLabel::setColor()
{
    setColor(QColorDialog::getColor(Qt::green, this));
}

void ColorLabel::setColor(QColor c)
{
    if (c.isValid()) {
        setText(c.name());
        setPalette(QPalette(c));
        setAutoFillBackground(true);

        color = c;
    }
}

QColor ColorLabel::getColor()
{
    return color;
}

void ColorLabel::mousePressEvent(QMouseEvent *e)
{
    setColor();
    emit editingFinished();
}
