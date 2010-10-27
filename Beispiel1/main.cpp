#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QTimer::singleShot(50, &w, SLOT(setDefault()));

    w.show();

    return a.exec();
}
