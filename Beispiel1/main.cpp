#include <QtGui/QApplication>
#include "mainwindow.h"

/**
 * @brief Main function; starts the application and displays the main window. Exits when the main windows is closed.
 *
 * @param argc number of command line arguments
 * @param argv[] array of command line arguments
 * @return int program return code, 0 for no error
*/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // Set default points for gradient editor
    QTimer::singleShot(50, &w, SLOT(setDefault()));

    w.show();

    return a.exec();
}
