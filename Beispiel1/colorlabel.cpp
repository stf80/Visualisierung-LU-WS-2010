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
    QColor color = QColorDialog::getColor(Qt::green, this);
    if (color.isValid()) {
        setText(color.name());
        setPalette(QPalette(color));
        setAutoFillBackground(true);
    }
}

void ColorLabel::mousePressEvent(QMouseEvent *e)
{
    setColor();
}
