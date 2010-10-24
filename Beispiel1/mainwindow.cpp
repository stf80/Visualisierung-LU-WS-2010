#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Volume.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(tr("Gradients"));

    m_gradient_editor = new GradientEditor(ui->transferFunctionGroupBox);

    QVBoxLayout *editorGroupLayout = new QVBoxLayout(ui->transferFunctionGroupBox);
    editorGroupLayout->addWidget(m_gradient_editor);

    m_volume_renderer = new VolumeRenderer(ui->rendererGroupBox);

    //QVBoxLayout *rendererGroupLayout = new QVBoxLayout(ui->rendererGroupBox);
    //rendererGroupLayout->addWidget(m_volume_renderer);

    //connect(m_gradient_editor, SIGNAL(gradientStopsChanged(QGradientStops)),
    //        m_volume_renderer, SLOT(setGradientStops(QGradientStops)));


    //m_volume_renderer->loadSourceFile(":res/gradients/gradients.cpp");
    //m_volume_renderer->loadDescription(":res/gradients/gradients.html");

    QTimer::singleShot(50, this, SLOT(setDefault()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionDatensatz_laden_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Datensatz laden"), ".", tr("DAT files (*.dat)"));
    Volume volume(fileName.toStdString());
    std::cout << "Volume (" << volume.GetWidth() << "x" << volume.GetHeight() << "x" << volume.GetDepth() << ") loaded." << std::endl;
}

void MainWindow::on_actionBeenden_triggered()
{
    QApplication::quit();
}
