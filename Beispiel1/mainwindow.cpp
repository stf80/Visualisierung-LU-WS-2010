

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Volume.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // transfer function editor
    setWindowTitle(tr("Gradients"));

    m_gradient_editor = new GradientEditor(ui->transferFunctionGroupBox);

    QVBoxLayout *editorGroupLayout = new QVBoxLayout(ui->transferFunctionGroupBox);
    editorGroupLayout->addWidget(m_gradient_editor);

    // volume renderer view
    m_volume_renderer = new VolumeRenderer(ui->rendererGroupBox);
    ui->renderingLayout->addWidget(m_volume_renderer);

    //connect(m_gradient_editor, SIGNAL(gradientStopsChanged(QGradientStops)),
    //        m_volume_renderer, SLOT(setGradientStops(QGradientStops)));

    // Light and reflection coefficients
    m_lightColorLabel = new ColorLabel(ui->optionsGroupBox);
    ui->optionsGridLayout->addWidget(m_lightColorLabel, 0, 1);

    m_ambientColorLabel = new ColorLabel(ui->optionsGroupBox);
    ui->optionsGridLayout->addWidget(m_ambientColorLabel, 0, 3);

    m_diffuseColorLabel = new ColorLabel(ui->optionsGroupBox);
    ui->optionsGridLayout->addWidget(m_diffuseColorLabel, 1, 3);

    m_specularColorLabel = new ColorLabel(ui->optionsGroupBox);
    ui->optionsGridLayout->addWidget(m_specularColorLabel, 2, 3);

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


