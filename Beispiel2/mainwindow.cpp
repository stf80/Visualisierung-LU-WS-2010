

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "FlowData.h"
#include "renderingview.h"
#include "gradientview.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(tr("LU Visualisierung 2010, Beispiel 2"));

    renderingView = new RenderingView(ui, ui->rendererGroupBox);
    ui->renderingLayout->addWidget(renderingView);

    gradientView = new GradientView(renderingView, ui->gradientGroupBox);
    ui->gradientLayout->addWidget(gradientView);

    connect(ui->colorCodingActive, SIGNAL(toggled(bool)),
            renderingView, SLOT(updateColorCoding()));
    connect(ui->colorCodingChannel, SIGNAL(valueChanged(int)),
            renderingView, SLOT(updateColorCoding()));
    connect(ui->colorCodingGradient, SIGNAL(currentIndexChanged(int)),
            renderingView, SLOT(updateColorCoding()));

    connect(ui->colorCodingGradient, SIGNAL(currentIndexChanged(int)),
            gradientView, SLOT(update()));

    connect(ui->arrowPlotActive, SIGNAL(toggled(bool)),
            renderingView, SLOT(updateArrowPlot()));
    connect(ui->arrowPlotChannelX, SIGNAL(valueChanged(int)),
            renderingView, SLOT(updateDerivedChannels()));
    connect(ui->arrowPlotChannelY, SIGNAL(valueChanged(int)),
            renderingView, SLOT(updateDerivedChannels()));
    connect(ui->arrowPlotDistance, SIGNAL(valueChanged(int)),
            renderingView, SLOT(updateArrowPlot()));
    connect(ui->arrowPlotScale, SIGNAL(toggled(bool)),
            renderingView, SLOT(updateArrowPlot()));
    connect(ui->arrowPlotSize, SIGNAL(valueChanged(int)),
            renderingView, SLOT(updateArrowPlot()));

    connect(ui->streamlinesActive, SIGNAL(toggled(bool)),
            renderingView, SLOT(updateStreamlines()));
    connect(ui->streamlinesDistance, SIGNAL(valueChanged(int)),
            renderingView, SLOT(updateStreamlines()));
    connect(ui->streamlinesDSep, SIGNAL(valueChanged(double)),
            renderingView, SLOT(updateStreamlines()));
    connect(ui->streamlinesDTest, SIGNAL(valueChanged(double)),
            renderingView, SLOT(updateStreamlines()));
    connect(ui->streamlinesIntegration, SIGNAL(currentIndexChanged(int)),
            renderingView, SLOT(updateStreamlines()));
    connect(ui->streamlinesSpacing, SIGNAL(currentIndexChanged(int)),
            renderingView, SLOT(updateStreamlines()));
    connect(ui->streamlinesTimeStep, SIGNAL(valueChanged(double)),
            renderingView, SLOT(updateStreamlines()));
    connect(ui->streamlinesSteps, SIGNAL(valueChanged(int)),
            renderingView, SLOT(updateStreamlines()));
    connect(ui->streamlinesTapering, SIGNAL(toggled(bool)),
            renderingView, SLOT(updateStreamlines()));
    connect(ui->streamlinesMaximumWidth, SIGNAL(valueChanged(int)),
            renderingView, SLOT(updateStreamlines()));
    connect(ui->streamlinesGlyphMapping, SIGNAL(toggled(bool)),
            renderingView, SLOT(updateStreamlines()));
    connect(ui->streamlinesGlyphDistance, SIGNAL(valueChanged(int)),
            renderingView, SLOT(updateStreamlines()));
    connect(ui->streamlinesGlyphSize, SIGNAL(valueChanged(int)),
            renderingView, SLOT(updateStreamlines()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionDatensatz_laden_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Datensatz laden"), ".", tr("Grid files (*.gri)"));

    // user may abort the file dialog, in which case the file name will be empty
    if (!fileName.isEmpty())
    {
        // clear all channels first
        for (int i = 0; i < max_channels; ++i)
            if (flowData.getChannel(i))
                flowData.deleteChannel(i);

        bool success = flowData.loadDataset(fileName.toStdString().c_str(), false);
        std::cout << "File load " << (success ? "successful" : "unsucessful") << std::endl;

        if (success)
        {
            renderingView->setDataset(&flowData);
        }
        else
            QMessageBox::warning(this, "Datensatz laden", "Laden des Datensatzes fehlgeschlagen.");
    }
}

void MainWindow::on_actionBeenden_triggered()
{
    QApplication::quit();
}


void MainWindow::on_actionBild_speichern_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save image", QString(), "PNG Image (*.png)");

    if (filename.isEmpty())
        return;

    if (!filename.endsWith(".png")) {
        filename.append(".png");
    }
    //QPixmap image = m_volume_renderer->renderPixmap(1024, 768);
    QPixmap image = QPixmap::grabWidget(renderingView);
    //QImage image = renderingView->grabFrameBuffer();
    if (!image.save(filename, "PNG")) {
        QMessageBox::warning(this, "Save Image", "Error saving image.");
    }
}
