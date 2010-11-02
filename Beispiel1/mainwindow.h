#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "gradients.h"
#include "volumerenderer.h"
#include "colorlabel.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
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
    void on_actionBeenden_triggered();
    void on_actionDatensatz_laden_triggered();

private:
    Ui::MainWindow *ui;

    GradientEditor *m_gradient_editor;
    VolumeRenderer *m_volume_renderer;

    ColorLabel *m_lightColorLabel, *m_ambientColorLabel, *m_diffuseColorLabel, *m_specularColorLabel;
};

#endif // MAINWINDOW_H

