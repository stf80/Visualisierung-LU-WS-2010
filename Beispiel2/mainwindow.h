#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "gradients.h"
#include "FlowData.h"
#include "renderingoptions.h"

namespace Ui {
    class MainWindow;
}

//#include "renderingview.h"

class RenderingView;

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
};

#endif // MAINWINDOW_H

