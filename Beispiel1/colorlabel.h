#ifndef COLORLABEL_H
#define COLORLABEL_H

#include <QLabel>
#include <QPushButton>

class ColorLabel :
        public QLabel
//        public QPushButton
{
    Q_OBJECT
public:
    explicit ColorLabel(QWidget *parent = 0);

signals:

public slots:

private slots:
    // shows a color dialog and sets the background color of the given widget.
    void setColor();

protected:
    virtual void mousePressEvent(QMouseEvent *e);
};

#endif // COLORLABEL_H
