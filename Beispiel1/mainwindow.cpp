

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

    volume = NULL;

    ui->slicingScrollBar->setMinimum(0);
}

MainWindow::~MainWindow()
{
    delete ui;
    if (volume != NULL) {
        delete volume;
        volume = NULL;
    }
}

void MainWindow::reset_slicing_scrollbar()
{
    if (volume != NULL) {
        int idx = ui->slicingComboBox->currentIndex();
        if (idx == 0) { // X Axis
            ui->slicingScrollBar->setMaximum(volume->GetWidth() - 1);
        } else if (idx == 1) { // Y Axis
            ui->slicingScrollBar->setMaximum(volume->GetHeight() - 1);
        } else if (idx == 2) { // Z Axis
            ui->slicingScrollBar->setMaximum(volume->GetDepth() - 1);
        }
        ui->slicingScrollBar->setValue(0);
    }
}

void MainWindow::update_slicing_view()
{
    ui->slicingView->setDirection(ui->slicingComboBox->currentIndex());
    ui->slicingView->setPosition(ui->slicingScrollBar->value());
    ui->slicingView->repaint();
}

void MainWindow::on_actionDatensatz_laden_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Datensatz laden"), ".", tr("DAT files (*.dat)"));
    volume = new Volume(fileName.toStdString());

    ui->slicingView->setVolume(volume);
    ui->slicingView->setDirection(SlicingView::DIRECTION_X);
    ui->slicingView->setPosition(0);
    ui->slicingView->repaint();

    reset_slicing_scrollbar();
    std::cout << "Volume (" << volume->GetWidth() << "x" << volume->GetHeight() << "x" << volume->GetDepth() << ") loaded." << std::endl;
}

void MainWindow::on_actionBeenden_triggered()
{
    QApplication::quit();
}

void MainWindow::on_slicingComboBox_currentIndexChanged(int index)
{
    reset_slicing_scrollbar();
    update_slicing_view();
}

void MainWindow::on_slicingScrollBar_valueChanged(int value)
{
    update_slicing_view();
}
