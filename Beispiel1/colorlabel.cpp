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
    QColor c = QColorDialog::getColor(Qt::green, this);
    if (c.isValid()) {
        setText(c.name());
        setPalette(QPalette(c));
        setAutoFillBackground(true);

        color = c;

        emit editingFinished();
    }
}

QColor ColorLabel::getColor()
{
    return color;
}

void ColorLabel::mousePressEvent(QMouseEvent *e)
{
    setColor();
}
