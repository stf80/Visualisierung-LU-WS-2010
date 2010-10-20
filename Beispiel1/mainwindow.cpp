#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_gradient_editor = new GradientEditor(ui->transferFunctionGroupBox);

    QVBoxLayout *editorGroupLayout = new QVBoxLayout(ui->transferFunctionGroupBox);
    editorGroupLayout->addWidget(m_gradient_editor);

    m_volume_renderer = new VolumeRenderer(ui->rendererGroupBox);

    QVBoxLayout *rendererGroupLayout = new QVBoxLayout(ui->rendererGroupBox);
    rendererGroupLayout->addWidget(m_volume_renderer);

    // set default stops
    QGradientStops stops;

    stops << QGradientStop(0.00, QColor::fromRgba(0xff000000));
    stops << QGradientStop(1.00, QColor::fromRgba(0xffffffff));

    m_gradient_editor->setGradientStops(stops);
    m_volume_renderer->setGradientStops(stops);

    connect(m_gradient_editor, SIGNAL(gradientStopsChanged(QGradientStops)),
            m_volume_renderer, SLOT(setGradientStops(QGradientStops)));


}

MainWindow::~MainWindow()
{
    delete ui;
}
