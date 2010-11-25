

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

    m_volume_renderer->setRenderingOptions(&options);

    //connect(m_gradient_editor, SIGNAL(gradientStopsChanged(QGradientStops)),
    //        m_volume_renderer, SLOT(setGradientStops(QGradientStops)));

    connect(ui->samplesSpinBox, SIGNAL(editingFinished()),
            this, SLOT(updateRenderingOptions()));

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

void MainWindow::setDefault()
{
    // set default stops
    QGradientStops stops;

    stops << QGradientStop(0.00, QColor::fromRgba(0xff000000));
    stops << QGradientStop(1.00, QColor::fromRgba(0xffffffff));

    m_gradient_editor->setGradientStops(stops);
    m_volume_renderer->setGradientStops(stops);
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

    // set view size to natural size of volume so image isn't scaled
    if (volume)
    {
        int idx = ui->slicingComboBox->currentIndex();
        int scale = 1;
        if (idx == 0) { // X Axis
            ui->slicingView->setMinimumSize(volume->GetDepth()*scale, volume->GetHeight()*scale);
            ui->slicingView->setMaximumSize(volume->GetDepth()*scale, volume->GetHeight()*scale);
        } else if (idx == 1) { // Y Axis
            ui->slicingView->setMinimumSize(volume->GetWidth()*scale, volume->GetDepth()*scale);
            ui->slicingView->setMaximumSize(volume->GetWidth()*scale, volume->GetDepth()*scale);
        } else if (idx == 2) { // Z Axis
            ui->slicingView->setMinimumSize(volume->GetWidth()*scale, volume->GetHeight()*scale);
            ui->slicingView->setMaximumSize(volume->GetWidth()*scale, volume->GetHeight()*scale);
        }

    }
    ui->slicingView->repaint();
}

void MainWindow::on_actionDatensatz_laden_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Datensatz laden"), ".", tr("DAT files (*.dat)"));

    // user may abort the file dialog, in which case the file name will be empty
    if (!fileName.isEmpty())
    {
        // free the old volume, if any
        if (volume)
            delete volume;

        volume = new Volume(fileName.toStdString());

        ui->slicingView->setVolume(volume);
        ui->slicingView->setDirection(SlicingView::DIRECTION_X);
        ui->slicingView->setPosition(0);
        ui->slicingView->repaint();

        reset_slicing_scrollbar();
        update_slicing_view();

        m_volume_renderer->setVolume(volume);
        //std::cout << "Volume (" << volume->GetWidth() << "x" << volume->GetHeight() << "x" << volume->GetDepth() << ") loaded." << std::endl;
    }
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

void MainWindow::updateRenderingOptions()
{
    options.light = m_lightColorLabel->getColor();
    options.ambient = m_ambientColorLabel->getColor();
    options.diffuse = m_diffuseColorLabel->getColor();
    options.specular = m_specularColorLabel->getColor();
    options.k1 = ui->k1SpinBox->value();
    options.k2 = ui->k2SpinBox->value();
    options.exponent = ui->exponentSpinBox->value();
    options.N = ui->samplesSpinBox->value();

    m_volume_renderer->setRenderingOptions(&options);
}


