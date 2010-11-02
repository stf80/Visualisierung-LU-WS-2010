#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "gradients.h"
#include "volumerenderer.h"
#include "colorlabel.h"

namespace Ui {
    class MainWindow;
}

/**
 * @brief
 *
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief
     *
     * @param parent
    */
    explicit MainWindow(QWidget *parent = 0);
    /**
     * @brief
     *
    */
    ~MainWindow();

public slots:
    /**
     * @brief
     *
    */
    void setDefault()
    {
        // set default stops
        //QGradientStops stops;

        //stops << QGradientStop(0.00, QColor::fromRgba(0xff000000));
        //stops << QGradientStop(1.00, QColor::fromRgba(0xffffffff));

        //m_gradient_editor->setGradientStops(stops);
        //m_volume_renderer->setGradientStops(stops);
    }

private slots:
    /**
     * @brief
     *
    */
    void on_actionBeenden_triggered();
    /**
     * @brief
     *
    */
    void on_actionDatensatz_laden_triggered();

private:
    Ui::MainWindow *ui; /**< TODO */

    GradientEditor *m_gradient_editor; /**< TODO */
    VolumeRenderer *m_volume_renderer; /**< TODO */

    ColorLabel *m_lightColorLabel, *m_ambientColorLabel, *m_diffuseColorLabel, *m_specularColorLabel; /**< TODO */
};

#endif // MAINWINDOW_H

