#ifndef COLORLABEL_H
#define COLORLABEL_H

#include <QLabel>
#include <QPushButton>

/**
 * @brief A label that opens a color selection widget when pressed and displays the selected color.
 *
*/
class ColorLabel :
        public QLabel
//        public QPushButton
{
    Q_OBJECT
public:
    /**
     * @brief The default constructor.
     *
     * @param parent the parent widget of this widget
    */
    explicit ColorLabel(QWidget *parent = 0);

    QColor getColor();
    void setColor(QColor color);

signals:
    void editingFinished();

public slots:

private:
    QColor color;

private slots:
    /**
     * @brief Shows a color selection dialog and sets the background color of this widget.
     *
    */
    void setColor();

protected:
    /**
     * @brief Callback invoked when the ColorLabel is pressed.
     *
     * @param e the mouse event causing the callback
    */
    virtual void mousePressEvent(QMouseEvent *e);
};

#endif // COLORLABEL_H
