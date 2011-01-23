/*! \mainpage LU Visualisierung 2010 - Beispiel 2

\section intro Introduction

This software visualizes static 2D flow data provided by the user. It offers various methods to display the data, including color coding, arrow plots and a variety of streamlines. The resulting visualization is arranged in layers, one for each method, which are blended to produce the final image. All parameters can be edited interactively, and the user can save an image of the current view.

\section download Download

The software is available here: <a href="../bin/Beispiel2.zip">Beispiel2.zip</a>.

\section installation Installation

No installation is required. Simply extract the downloaded archive and run "Beispiel2/Beispiel2.exe".

\section usage Usage

First, open a data set you want to view. From the menu, select "Datei / Datensatz laden..." and open an appropriate file. You should then be able to see a visualization of the data in the main view to the right.

The parameters controlling the visualization process are located to the left, and grouped according to visualization methods. Each of the image layers can be activated separately.

For color coding one can set the channel to display and select one of several color gradients. The current color gradient is displayed below.

The vector data displayed by arrow plots and streamlines can be selected in the "Vector Channel" group box. It allows to select X and Y channels of the vector data and shows the number of the channel which contains the vector length derived from the data.

Arrow plots are controlled by setting the grid size on which arrows are placed, and the maximum size of arrows relative to the grid size. Arrows are scaled so that their area is proportional to the normalized channel value.

Streamlines have a multitude of options. Integration is done using Euler or Runge-Kutta methods, and the time step and maximum number of steps can be set. The spacing of streamlines can be regular, meaning streamlines will start at points on a regular grid, or evenly spaced.

Additionally streamlines can use tapering to visualize vector lengths and glyph mapping to place arrows on streamlines in an equidistant fashion.

\section examples Examples

<img src="../examples/comparison-RK.png"></a>
<img src="../examples/evenly spaced streamlines 5.png"></a>

Created by Simon Parzer and Stefan Fiedler.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "FlowData.h"

namespace Ui {
    class MainWindow;
}

//#include "renderingview.h"

class RenderingView;
class GradientView;

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

private slots:
    /**
      @brief Saves the current image to a file.
      */
    void on_actionBild_speichern_triggered();
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
    Ui::MainWindow *ui; /** the main window, contains all widgets created in the form editor */

    FlowData flowData;

    RenderingView *renderingView;
    GradientView *gradientView;
};

#endif // MAINWINDOW_H

