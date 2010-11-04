

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Volume.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // transfer function editor
    setWindowTitle(tr("Visualisierung LU 1"));

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

    m_slicing_view = NULL;
    volume = NULL;

}

MainWindow::~MainWindow()
{
    delete ui;
    if (volume != NULL) {
        delete volume;
        volume = NULL;
    }
    if (m_slicing_view != NULL) {
        delete m_slicing_view;
        m_slicing_view = NULL;
    }
}

void MainWindow::refresh_slicing_view()
{
    if (m_slicing_view != NULL) {
        QGraphicsScene* scn = new QGraphicsScene(ui->slicingView);
        scn->setSceneRect(ui->slicingView->rect());

    }
}

void MainWindow::on_actionDatensatz_laden_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Datensatz laden"), ".", tr("DAT files (*.dat)"));
    volume = new Volume(fileName.toStdString());
    m_slicing_view = new SlicingView(*volume);
    std::cout << "Volume (" << volume->GetWidth() << "x" << volume->GetHeight() << "x" << volume->GetDepth() << ") loaded." << std::endl;
}

void MainWindow::on_actionBeenden_triggered()
{
    QApplication::quit();
}
