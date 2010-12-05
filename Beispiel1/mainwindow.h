#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "gradients.h"
#include "volumerenderer.h"
#include "colorlabel.h"
#include "slicingview.h"
#include "Volume.h"
#include "renderingoptions.h"

namespace Ui {
    class MainWindow;
}


/**
 * @brief The main window of the application. Also handles menu actions.
 *
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief The default constructor
     *
     * @param parent the parent widget
    */
    explicit MainWindow(QWidget *parent = 0);
    /**
     * @brief Default destructor.
     *
    */
    ~MainWindow();

public slots:
    /**
     * @brief Sets default values for gradient editor points.
     *
    */
    void setDefault();

private slots:
    /**
     * @brief Invoked when the value of the slicing view scrollbar changes. Redraws the slicing view.
     *
     * @param value
    */
   void on_actionBild_speichern_triggered();
   void on_slicingScrollBar_valueChanged(int value);
    /**
     * @brief Invoked when the direction of the slicing view changes. Redraws the slicing view and resets the slicing scrollbar.
     *
     * @param index
    */
    void on_slicingComboBox_currentIndexChanged(int index);
    /**
     * @brief Invoked when application is quit using the file menu. Closes the application.
     *
    */
    void on_actionBeenden_triggered();
    /**
     * @brief Invoked when a data set is loaded using the file menu. Opens a file dialog, loads the selected file and redraws the slicing view.
     *
    */
    void on_actionDatensatz_laden_triggered();

private:
    /**
     * @brief Sets maximum for slicing view scrollbar, depending on current view direction, and resets current value.
     *
    */
    void reset_slicing_scrollbar();
    /**
     * @brief Updates view direction, view plane and size of the slicing view and redraws it.
     *
    */
    void update_slicing_view();

    Ui::MainWindow *ui; /** the main window, contains all widgets created in the form editor */

    Volume* volume; /** the volume to render */
    RenderingOptions options;

    GradientEditor *m_gradient_editor; /** the gradient editor used to edit the transfer function */
    VolumeRenderer *m_volume_renderer; /** rendering view of the volume data */

    ColorLabel *m_lightColorLabel, *m_ambientColorLabel, *m_diffuseColorLabel, *m_specularColorLabel; /** ColorLabels used to select RGB rendering options */

private slots:
    void updateRenderingOptions();
    void updateTransferFunction();
};

#endif // MAINWINDOW_H

