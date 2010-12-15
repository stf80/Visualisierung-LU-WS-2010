

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "FlowData.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // transfer function editor
    setWindowTitle(tr("Visualisierung LU 2"));

    renderingView = new RenderingView(ui->rendererGroupBox);
    ui->renderingLayout->addWidget(renderingView);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionDatensatz_laden_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Datensatz laden"), ".", tr("DAT files (*.dat)"));

    // user may abort the file dialog, in which case the file name will be empty
    if (!fileName.isEmpty())
    {
        bool success = flowData.loadDataset(fileName.toStdString().c_str(), false);
        std::cout << "File loaded " << (success ? "successfully" : "unsucessfully") << std::endl;

        renderingView->setDataset(&flowData);
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
    //QPixmap image = QPixmap::grabWidget(m_volume_renderer);
    QImage image = renderingView->grabFrameBuffer();
    if (!image.save(filename, "PNG")) {
        QMessageBox::warning(this, "Save Image", "Error saving image.");
    }
}
